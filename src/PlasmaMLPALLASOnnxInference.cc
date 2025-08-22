/**
 * @file PlasmaMLPALLASOnnxInference.cc
 * @brief Implementation of ONNX model inference for PALLAS plasma beam generation.
 *
 * This file implements the PlasmaMLPALLASOnnxInference class, which provides
 * a mechanism to generate plasma beam parameters by performing inference
 * on a pre-trained ONNX model. The class handles:
 *  - Initialization of the ONNX runtime environment
 *  - Loading the specified ONNX model file
 *  - Preparing input tensors from physical simulation parameters
 *  - Running inference and converting outputs to physical units
 *  - Sampling transverse beam coordinates (x, z) and momenta (xp, zp)
 *
 * The main purpose is to generate physically realistic beam parameters
 * for use in Geant4 simulations of the PALLAS plasma beamline.
 *
 * Input parameters for inference include:
 *  - Laser focal position (fXof)
 *  - Laser normalized amplitude (fA0)
 *  - Plasma density (fCN2)
 *  - Gas pressure (fPressure)
 *
 * Output parameters encapsulated in the BeamParameters structure include:
 *  - Kinetic energy (Ekin) and spread (dEkin)
 *  - Beam charge (Q)
 *  - Normalized emittance (epsb)
 *  - Transverse positions (x, z) and angles (xp, zp)
 *
 * The class uses CLHEP units for all physical quantities.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @author Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */


#include "PlasmaMLPALLASOnnxInference.hh"
#include "Randomize.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include <cmath>

/**
 * @brief Constructor
 * @param modelPath Path to the ONNX model file
 *
 * Initializes the ONNX runtime environment, sets session options, 
 * and loads the specified ONNX model. Also initializes input and output node names.
 */
PlasmaMLPALLASOnnxInference::PlasmaMLPALLASOnnxInference(const std::string& modelPath)
    : env(ORT_LOGGING_LEVEL_WARNING, "plasma"),
      sessionOptions(),
      memoryInfo(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault))
{
    sessionOptions.SetIntraOpNumThreads(1);
    session = std::make_unique<Ort::Session>(env, modelPath.c_str(), sessionOptions);

    // TODO: fill input/output node names according to the ONNX model
    inputNodeNames = {"input"}; 
    outputNodeNames = {"output"};
}

/**
 * @brief Generates beam parameters using the ONNX model
 * @param fXof Laser focal position
 * @param fA0 Laser normalized amplitude
 * @param fCN2 Plasma density
 * @param fPressure Gas pressure
 * @return BeamParameters structure with the generated beam values
 *
 * This method rescales inputs, runs inference on the ONNX model,
 * converts outputs to physical units, and samples transverse positions/momenta.
 */
BeamParameters PlasmaMLPALLASOnnxInference::GenerateBeam(G4double fXof, G4double fA0, G4double fCN2, G4double fPressure) {
    // Lambda to normalize input values between 0 and 1
    auto rescale = [](G4double val, G4double min, G4double max) {
        return (val - min) / (max - min);
    };

    G4double fXof_rescaled = rescale(fXof, -399.824698, 1798.325132);
    G4double fA0_rescaled  = rescale(fA0, 1.100516, 1.849792);
    G4double fCN2_rescaled = rescale(fCN2, 0.002064, 0.119983);
    G4double fPressure_rescaled = rescale(fPressure, 10.094508, 99.957409);

    // Prepare input tensor for ONNX runtime
    std::vector<double> inputValues = {fXof_rescaled, fA0_rescaled, fCN2_rescaled, fPressure_rescaled};
    std::vector<int64_t> inputDims = {1, (long long)inputValues.size()};
    Ort::Value inputTensor = Ort::Value::CreateTensor<double>(
        memoryInfo, inputValues.data(), inputValues.size(),
        inputDims.data(), inputDims.size()
    );

    // Run inference
    auto outputTensors = session->Run(Ort::RunOptions{nullptr},
                                      inputNodeNames.data(),
                                      &inputTensor, 1,
                                      outputNodeNames.data(),
                                      outputNodeNames.size());

    G4float* outVals = outputTensors.front().GetTensorMutableData<G4float>();

    BeamParameters params;

    // Convert model outputs to physical units
    params.Ekin  = outVals[0] * (368.2576*CLHEP::MeV - 43.88486*CLHEP::MeV) + 43.88486*CLHEP::MeV;
    params.dEkin = outVals[1] * (5.622887e-1 - 8.269990e-4) + 8.269990e-4;
    params.Q     = outVals[2] * (8.376833e-10 - 6.498496e-17) + 6.498496e-17;
    params.epsb  = outVals[3] * (7.615750e-5 - 2.504754e-9) + 2.504754e-9;

    // Twiss parameters (assume alpha=0, beta=1)
    G4double alphax = 0, betax = 1, alphay = 0, betay = 1;

    // Sample X transverse position and momentum
    G4double Ax = std::sqrt(params.epsb * betax) * std::sqrt(-2 * std::log(G4UniformRand()));
    G4double psix = CLHEP::twopi * G4UniformRand();
    params.x  = Ax * std::cos(psix);
    params.xp = -params.x * alphax / betax - Ax / std::sqrt(betax) * std::sin(psix);

    // Sample Z transverse position and momentum
    G4double Ay = std::sqrt(params.epsb * betay) * std::sqrt(-2 * std::log(G4UniformRand()));
    G4double psiy = CLHEP::twopi * G4UniformRand();
    params.z  = Ay * std::cos(psiy);
    params.zp = -params.z * alphay / betay - Ay / std::sqrt(betay) * std::sin(psiy);

    return params;
}
