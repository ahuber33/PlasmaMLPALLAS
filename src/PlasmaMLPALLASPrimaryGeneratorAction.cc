/**
 * @file PlasmaMLPALLASPrimaryGeneratorAction.cc
 * @brief Implementation of the primary generator action for the PALLAS simulation.
 *
 * This file implements the `PlasmaMLPALLASPrimaryGeneratorAction` class, which handles
 * the generation of primary particles for Geant4 events in the PALLAS project.
 * 
 * Two generation modes are supported:
 *  1. **ONNX model inference**: Beam parameters (energy, charge, emittance, transverse 
 *     positions, and momenta) are predicted by a neural network model and used to 
 *     configure the particle gun.
 *  2. **Geant4 GeneralParticleSource (GPS)**: Standard Geant4 particle generation.
 *
 * Features:
 *  - Thread-safe generation using atomic counters and per-thread UI handling.
 *  - Progress display with estimated remaining simulation time.
 *  - Energy smearing with Gaussian fluctuations.
 *
 * Usage:
 *  - Instantiate the class and assign it to the Geant4 run manager via `SetUserAction`.
 *  - Configure the ONNX model or GPS via the associated messenger.
 *
 * @note Multithreading is supported; thread 0 handles progress display.
 *
 * @authors Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @authors Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */


#include "PlasmaMLPALLASPrimaryGeneratorAction.hh"

/// Global counter of generated particles (atomic to support multithreading).
std::atomic<size_t> currentParticleNumber{0};

/// Global pointer to the Geant4 UI manager.
G4UImanager *UI = G4UImanager::GetUIpointer();

/**
 * @brief Constructor for the primary generator action.
 *
 * Initializes the particle gun, particle source, ONNX inference engine, and
 * the associated UI messenger.
 *
 * @param N Total number of events to generate.
 * @param numThreads Number of threads used for multithreaded execution.
 * @param pMT Flag indicating whether multithreading is enabled.
 */
PlasmaMLPALLASPrimaryGeneratorAction::PlasmaMLPALLASPrimaryGeneratorAction(size_t N, size_t numThreads, G4bool pMT)
    : NEventsGenerated(N),
      numThreads(numThreads),
      G4VUserPrimaryGeneratorAction(),
      particleGun(nullptr),
      flag_MT(pMT),
      currentEvent(0)
{
  fPrimaryGeneratorMessenger = new PlasmaMLPALLASPrimaryGeneratorMessenger(this);
  particleGun = new G4ParticleGun(1);
  particleSource = new G4GeneralParticleSource();
  onnxInference = std::make_unique<PlasmaMLPALLASOnnxInference>("model2.onnx");
}

/**
 * @brief Destructor.
 *
 * Cleans up allocated resources such as the particle gun and particle source.
 */
PlasmaMLPALLASPrimaryGeneratorAction::~PlasmaMLPALLASPrimaryGeneratorAction()
{
  delete particleGun;
  delete particleSource;
}

void PlasmaMLPALLASPrimaryGeneratorAction::UpdateParticleDefinition()
{
  particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(OnnxParameters::Instance().GetParticleName());

  if (!particleDefinition)
  {
    G4cerr << "Particle doesn't exist : RUN ABORT" << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }
}

/**
 * @brief Display the progress of event generation.
 *
 * Prints a progress bar along with an estimate of the remaining simulation time.
 *
 * @param progress Fraction of events completed (between 0 and 1).
 * @param startTime Start time of the simulation.
 */
void PlasmaMLPALLASPrimaryGeneratorAction::ShowProgress(
    double progress,
    std::chrono::high_resolution_clock::time_point startTime)
{
    int barWidth = 70;
    std::ostringstream oss;

    oss << "\r[";  // retour chariot pour écraser la ligne
    int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) oss << "=";
        else if (i == pos) oss << ">";
        else oss << " ";
    }
    oss << "] " << int(progress * 100.0) << " %";

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

    double averageTimePerProgress = (progress > 0.0) ? (elapsedTime / progress) : 0.0;
    double estimatedRemainingTime = (1.0 - progress) * averageTimePerProgress;

    oss << " | ETA = " << std::fixed << std::setprecision(1)
        << estimatedRemainingTime / 60.0 << " min";

    if (progress >0.01) std::cerr << oss.str() << std::flush;
}



/**
 * @brief Generate primary particles for a simulation event.
 *
 * Depending on the ONNX status, this function either:
 *  - Uses the ONNX model inference to generate beam parameters and configure
 *    the particle gun.
 *  - Uses the Geant4 GeneralParticleSource (GPS) to generate the particle.
 *
 * @param anEvent Pointer to the Geant4 event where primary particles are generated.
 */
void PlasmaMLPALLASPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  if (!isStartTimeInitialized)
  {
    startTime = std::chrono::high_resolution_clock::now();
    isStartTimeInitialized = true;
  }

  // Récupération thread-safe des paramètres définis par le messenger
  OnnxParameters &params = OnnxParameters::Instance();

  UpdateParticleDefinition();

  // Copie des autres paramètres atomiques
  int status = params.GetStatusONNX();
  double xoff = params.GetXoff();
  double a0 = params.GetA0();
  double cn2 = params.GetCN2();
  double pressure = params.GetPressure();

  // ####################### CASE 1 : GENERATION FROM ONNX MODEL ##########################
  if (status == 1)
  {
    if (!onnxInference)
    {
      G4cerr << "Error : onnxInference is not initialized !" << G4endl;
      return;
    }

    BeamParameters params = onnxInference->GenerateBeam(xoff, a0, cn2, pressure);

    Ekin = params.Ekin;
    dEkin = params.dEkin;
    Q = params.Q;
    epsb = params.epsb;

    // Add Gaussian fluctuation to kinetic energy
    params.Ekin = G4RandGauss::shoot(params.Ekin, params.dEkin * params.Ekin);

    // Set particle parameters
    particleGun->SetParticleDefinition(particleDefinition);
    particleGun->SetParticleEnergy(params.Ekin);
    particleGun->SetParticlePosition(G4ThreeVector(params.x, 0, params.z));

    G4double MomentumDirectionY = 1. / (1. + std::pow(std::tan(params.xp), 2) + std::pow(std::tan(params.zp), 2));
    particleGun->SetParticleMomentumDirection(
        G4ThreeVector(MomentumDirectionY * std::tan(params.xp),
                      MomentumDirectionY,
                      MomentumDirectionY * std::tan(params.zp)));

    particleGun->GeneratePrimaryVertex(anEvent);
    currentParticleNumber++;
  }

  // ############################ CASE 2 : GENERATION FROM GPS ############################
  else if (status == 0)
  {
    particleSource->GeneratePrimaryVertex(anEvent);
    currentParticleNumber++;
  }

  else
  {
    G4Exception("PrimaryGeneratorAction", "PGA0001", FatalException,
                "Incorrect ONNX Status.");
  }

  
  if (threadID == 0)
    {
      ShowProgress(double(currentParticleNumber) / double(NEventsGenerated), startTime);
    }
    
}
