#ifndef PlasmaMLPALLASOnnxInference_H
#define PlasmaMLPALLASOnnxInference_H

/**
 * @class PlasmaMLPALLASOnnxInference
 * @brief Interface for ONNX model inference for PALLAS plasma simulation beam generation
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright (C) Projet PALLAS
 */

#include <onnxruntime_cxx_api.h>
#include "globals.hh"

/**
 * @struct BeamParameters
 * @brief Structure storing the physical parameters of a generated beam.
 */
struct BeamParameters {
    G4double Ekin;   ///< Kinetic energy of the beam particle
    G4double dEkin;  ///< Energy spread
    G4double Q;      ///< Beam charge
    G4double epsb;   ///< Beam emittance
    G4double x;      ///< X position
    G4double xp;     ///< X momentum component
    G4double z;      ///< Z position
    G4double zp;     ///< Z momentum component
};

/**
 * @class PlasmaMLPALLASOnnxInference
 * @brief Class for performing ONNX model inference to generate plasma beam parameters.
 *
 * This class wraps an ONNX runtime session and provides a convenient
 * interface for generating beam parameters given laser and plasma conditions.
 */
class PlasmaMLPALLASOnnxInference {
public:
    /**
     * @brief Constructor
     * @param modelPath Path to the ONNX model file
     *
     * Initializes the ONNX runtime environment and loads the specified model.
     */
    PlasmaMLPALLASOnnxInference(const std::string& modelPath);

    /**
     * @brief Generate beam parameters using the ONNX model
     * @param fXof Laser focal position
     * @param fA0 Laser normalized amplitude
     * @param fCN2 Plasma density
     * @param fPressure Gas pressure
     * @return BeamParameters structure with the generated beam values
     */
    BeamParameters GenerateBeam(G4double fXof, G4double fA0, G4double fCN2, G4double fPressure);

private:
    Ort::Env env;                           ///< ONNX runtime environment
    Ort::SessionOptions sessionOptions;     ///< Session options for ONNX runtime
    std::unique_ptr<Ort::Session> session;  ///< ONNX runtime session for inference
    Ort::MemoryInfo memoryInfo;             ///< Memory information for input/output
    std::vector<const char*> inputNodeNames;///< Names of input nodes in the ONNX model
    std::vector<const char*> outputNodeNames;///< Names of output nodes in the ONNX model
};

#endif
