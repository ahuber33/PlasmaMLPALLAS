#ifndef PlasmaMLPALLASSteppingAction_h
#define PlasmaMLPALLASSteppingAction_h

/**
 * @class PlasmaMLPALLASSteppingAction
 * @brief Implements the Geant4 stepping action for the PALLAS project.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This class records tracking information at each step of a particle inside
 * the simulation. It extracts position, momentum, deposited energy and
 * other metadata (volume, process, etc.) for later analysis.
 *
 * It also handles quadrupole-related information and collimator updates.
 */

#include "G4UserSteppingAction.hh"
#include "PlasmaMLPALLASRunAction.hh"
#include "PlasmaMLPALLASQuadrupoleUtils.hh"
#include "G4GenericMessenger.hh"

class PlasmaMLPALLASSteppingAction : public G4UserSteppingAction
{
public:
    /**
     * @brief Constructor.
     *
     * Initializes the stepping action and messenger.
     */
    PlasmaMLPALLASSteppingAction();

    /**
     * @brief Destructor.
     */
    ~PlasmaMLPALLASSteppingAction() override;

    /**
     * @brief Store the initial particle information at the beginning of an event.
     *
     * @param evtac Pointer to the event action where data will be stored.
     */
    void SetInputInformations(PlasmaMLPALLASEventAction* evtac) const;

    /**
     * @brief Store quadrupole crossing information.
     *
     * @param evtac Pointer to the event action.
     * @param quad Identifier of the quadrupole.
     * @param posType Position type (pre-step or post-step).
     */
    void SetQuadrupoleInformation(PlasmaMLPALLASEventAction* evtac, QuadID quad, PositionType posType) const;

    /**
     * @brief Stepping action executed at each Geant4 step.
     *
     * Extracts step information (positions, momenta, energy deposition, etc.)
     * and stores it for later analysis. Also manages collimator updates.
     *
     * @param step Current Geant4 step.
     */
    void UserSteppingAction(const G4Step* step) override;

private:
    /**
     * @struct StepPoint
     * @brief Helper structure to store spatial and momentum coordinates of a step.
     */
    struct StepPoint {
        G4double x = 0.0;  ///< X position [mm]
        G4double y = 0.0;  ///< Y position [mm]
        G4double z = 0.0;  ///< Z position [mm]
        G4double px = 0.0; ///< X component of momentum direction
        G4double py = 0.0; ///< Y component of momentum direction
        G4double pz = 0.0; ///< Z component of momentum direction
    };

    // --- Configuration & control ---
    G4GenericMessenger* sMessenger = nullptr; ///< Command messenger for UI interaction
    G4bool TrackingStatus = true;             ///< Enable/disable general tracking
    G4bool TrackingStatusCollimators = true;  ///< Enable/disable collimator tracking

    // --- Track information ---
    G4Track* theTrack = nullptr;   ///< Pointer to the current track
    G4String particleName;         ///< Particle name
    G4int particleID = 0;          ///< PDG particle ID
    G4int parentID = 0;            ///< Parent track ID
    G4int trackID = 0;             ///< Current track ID
    G4int stepNo = 0;              ///< Step number in the current track
    G4String endproc;              ///< Process at track end
    G4String volumeNamePreStep;    ///< Volume name at pre-step point
    G4String volumeNamePostStep;   ///< Volume name at post-step point

    // --- Energy ---
    G4double energy = 0.0;           ///< Kinetic energy [MeV]
    G4double energyDeposited = 0.0;  ///< Deposited energy [keV]

    // --- Step coordinates ---
    StepPoint preStep;   ///< Pre-step point (position & momentum)
    StepPoint postStep;  ///< Post-step point (position & momentum)

};

#endif // PlasmaMLPALLASSteppingAction_h
