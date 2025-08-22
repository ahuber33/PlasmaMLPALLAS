#ifndef PlasmaMLPALLASPrimaryGeneratorAction_h
#define PlasmaMLPALLASPrimaryGeneratorAction_h 1

/**
 * @class PlasmaMLPALLASPrimaryGeneratorAction
 * @brief Defines the primary generator action for the simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This class controls the generation of primary particles in the Geant4 simulation.  
 * It integrates ONNX inference for beam parameter prediction and provides methods 
 * to configure laser and beam properties in a multithreaded environment.
 */

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PlasmaMLPALLASRunAction.hh"
#include "PlasmaMLPALLASPrimaryGeneratorMessenger.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "PlasmaMLPALLASOnnxInference.hh"
#include "PlasmaMLPALLASOnnxParameters.hh"

// Forward declarations
class G4ParticleGun;
class G4Event;

class PlasmaMLPALLASPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    /**
     * @brief Constructor.
     * @param N Number of events to generate.
     * @param numThreads Number of threads in multithreaded mode.
     * @param pMT Flag to enable multithreading.
     */
    PlasmaMLPALLASPrimaryGeneratorAction(size_t N, size_t numThreads, bool pMT);

    /**
     * @brief Destructor.
     */
    ~PlasmaMLPALLASPrimaryGeneratorAction();

    /**
     * @brief Generate primary particles for the event.
     * @param anEvent Pointer to the current G4Event.
     */
    void GeneratePrimaries(G4Event *anEvent) override;

    /**
     * @brief Update the particle definition based on current parameters.
     */
    void UpdateParticleDefinition();

    /// @name Laser & beam parameters (retrieved from OnnxParameters)
    ///@{
    double GetML_Xoff() const { return OnnxParameters::Instance().GetXoff(); }       /**< Get X-offset from ONNX parameters */
    double GetML_A0() const { return OnnxParameters::Instance().GetA0(); }           /**< Get amplitude from ONNX parameters */
    double GetML_CN2() const { return OnnxParameters::Instance().GetCN2(); }         /**< Get CN2 turbulence parameter */
    double GetML_Pressure() const { return OnnxParameters::Instance().GetPressure(); } /**< Get pressure value */
    ///@}

    /// @name Beam properties
    ///@{
    double GetEkin() const { return Ekin; }                 /**< Get kinetic energy (MeV) */
    double GetdEkin() const { return dEkin * 100.; }        /**< Get energy spread (%) */
    double GetQ() const { return Q * 1e12; }                /**< Get charge (pC) */
    double GetEPSB() const { return epsb * 1e6; }           /**< Get beam emittance (µm) */
    ///@}

private:
    PlasmaMLPALLASPrimaryGeneratorMessenger* fPrimaryGeneratorMessenger = nullptr; /**< Messenger for user interface commands */

    std::unique_ptr<PlasmaMLPALLASOnnxInference> onnxInference; /**< ONNX inference engine */
    G4ParticleGun* particleGun = nullptr;                       /**< Particle gun for primary generation */
    G4GeneralParticleSource* particleSource = nullptr;          /**< General particle source */
    G4ParticleDefinition* particleDefinition = nullptr;         /**< Definition of the particle to generate */

    /**
     * @brief Display progress of event generation.
     * @param progress Current progress (0.0–1.0).
     * @param startTime Time when event generation started.
     */
    void ShowProgress(double progress, std::chrono::high_resolution_clock::time_point startTime);

    size_t NEventsGenerated = 0;   /**< Number of events already generated */
    size_t currentEvent = 0;       /**< Current event index */
    size_t numThreads = 0;         /**< Number of threads */
    bool flag_MT = false;          /**< Flag indicating multithreading mode */
    int threadID = 0;              /**< Thread ID */
    int eventID = 0;               /**< Event ID */
    int nEvent = 0;                /**< Number of events processed */
    std::chrono::duration<double> elapsedTime; /**< Elapsed time since start */
    bool isStartTimeInitialized = false;       /**< Flag indicating start time initialized */
    std::chrono::high_resolution_clock::time_point startTime; /**< Start time */
    double progressRemaining = 0.;             /**< Remaining progress fraction */
    double averageTimePerProgress = 0.;        /**< Average time per progress step */
    double estimatedRemainingTime = 0.;        /**< Estimated remaining time (s) */

    // Beam physical parameters
    double Ekin = 1.;   /**< Kinetic energy (MeV) */
    double dEkin = 1.;  /**< Energy spread (%) */
    double Q = 1.;      /**< Beam charge (pC) */
    double epsb = 1.;   /**< Beam emittance (µm) */

    // ONNX session
    std::unique_ptr<Ort::Session> fSession;          /**< ONNX Runtime session */
    const OrtMemoryInfo* fMemory_info = nullptr;     /**< Memory information for ONNX session */
    std::vector<const char*> fInput_node_names = {"input"};   /**< Input node names for ONNX */
    std::vector<const char*> fOutput_node_names = {"output"}; /**< Output node names for ONNX */
};

#endif
