/**
 * @file PlasmaMLPALLASSteppingAction.cc
 * @brief Implements step-level tracking, data extraction, and event updates for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This file contains the method definitions for the `PlasmaMLPALLASSteppingAction` class
 * declared in `PlasmaMLPALLASSteppingAction.hh`. It manages:
 *  - Extraction of particle positions, momentum, energy, and track metadata at each Geant4 step
 *  - Handling of quadrupole crossing information for beamline statistics
 *  - Detection of interactions with collimators and updating of corresponding tallies
 *  - Updating of YAG screen statistics including energy deposition and exit positions
 *  - Optional termination of particles when leaving specific volumes or reaching collimators
 *
 * The stepping action workflow:
 *  - **UserSteppingAction**:
 *      - Extracts pre-step and post-step positions and momenta
 *      - Computes kinetic energy and deposited energy per step
 *      - Stores initial beam parameters at first step of primary particles
 *      - Updates quadrupole statistics at entry/exit
 *      - Updates collimator tallies and stops tracks if necessary
 *      - Updates YAG screen tallies and deposited energies
 *      - Kills particles leaving the world volume
 *
 *
 * @note This class relies on Geant4's step action mechanism to record fine-grained
 *       tracking information suitable for later beamline analysis.
 */


#include "PlasmaMLPALLASSteppingAction.hh"

/**
 * @brief Constructor.
 *
 * Initializes the Geant4 generic messenger and declares user commands
 * for controlling tracking status (global and for collimators).
 */
PlasmaMLPALLASSteppingAction::PlasmaMLPALLASSteppingAction()
{
    sMessenger = new G4GenericMessenger(this, "/PlasmaMLPALLAS/step/", "Control commands for my application");

    sMessenger->DeclareProperty("SetTrackingStatus", TrackingStatus)
        .SetGuidance("Enable or disable general tracking.")
        .SetParameterName("TrackingStatus", false)
        .SetDefaultValue("true");

    sMessenger->DeclareProperty("SetTrackingStatusCollimators", TrackingStatusCollimators)
        .SetGuidance("Enable or disable collimator tracking.")
        .SetParameterName("TrackingStatusCollimators", false)
        .SetDefaultValue("true");
}

/**
 * @brief Destructor.
 *
 * Cleans up the allocated messenger.
 */
PlasmaMLPALLASSteppingAction::~PlasmaMLPALLASSteppingAction()
{
    delete sMessenger;
}

/**
 * @brief Store the initial beam parameters at the beginning of the event.
 *
 * @param evtac Pointer to the event action where the values are stored.
 */
void PlasmaMLPALLASSteppingAction::SetInputInformations(PlasmaMLPALLASEventAction *evtac) const
{
    evtac->SetXStart(preStep.x);
    evtac->SetXpStart(preStep.px);
    evtac->SetYStart(preStep.y);
    evtac->SetYpStart(preStep.py);
    evtac->SetZStart(preStep.z);
    evtac->SetZpStart(preStep.pz);
    evtac->SetEnergyStart(energy);
}

/**
 * @brief Store quadrupole-related information in the event statistics.
 *
 * @param evtac Pointer to the event action.
 * @param quad Identifier of the quadrupole.
 * @param posType Position type (begin or end of quadrupole).
 */
void PlasmaMLPALLASSteppingAction::SetQuadrupoleInformation(
    PlasmaMLPALLASEventAction *evtac,
    QuadID quad,
    PositionType posType) const
{
    // Store energy only at the beginning of the first quadrupole
    if (quad == QuadID::Q1 && posType == PositionType::Begin)
    {
        evtac->SetEnergy(energy);
    }

    auto &stats = evtac->GetStatsQuadrupoles();

    // Position
    SetQuadrupoleValue(stats, quad, posType, VectorType::Position, Axis::X, postStep.x);
    SetQuadrupoleValue(stats, quad, posType, VectorType::Position, Axis::Y, postStep.y);
    SetQuadrupoleValue(stats, quad, posType, VectorType::Position, Axis::Z, postStep.z);

    // Momentum
    SetQuadrupoleValue(stats, quad, posType, VectorType::Momentum, Axis::X, postStep.px);
    SetQuadrupoleValue(stats, quad, posType, VectorType::Momentum, Axis::Y, postStep.py);
    SetQuadrupoleValue(stats, quad, posType, VectorType::Momentum, Axis::Z, postStep.pz);
}

/**
 * @brief Update collimator interaction information in the tally.
 *
 * @param tally Collimator tally to update.
 * @param x Interaction X position [mm].
 * @param y Interaction Y position [mm].
 * @param z Interaction Z position [mm].
 * @param energy Kinetic energy at interaction [MeV].
 */
void UpdateCollimators(RunTallyCollimators &tally, G4float x, G4float y, G4float z, G4float energy)
{
    tally.SetXInteraction(x);
    tally.SetYInteraction(y);
    tally.SetZInteraction(z);
    tally.SetEnergy(energy);
    tally.ActiveFlag();
}

/**
 * @brief Update YAG (Yttrium Aluminum Garnet screen) tally with particle data.
 *
 * @param tally YAG tally to update.
 * @param x Exit X position [mm].
 * @param y Exit Y position [mm].
 * @param z Exit Z position [mm].
 * @param energy Particle kinetic energy [MeV].
 * @param energyDeposited Deposited energy [keV].
 * @param parentID ID of the parent track.
 * @param particleID PDG encoding of the particle.
 * @param volumeNamePostStep Name of the post-step volume.
 * @param trackingStatus Whether particle tracking is active.
 * @param track Pointer to the current Geant4 track.
 */
void UpdateYAG(RunTallyYAG &tally, G4float x, G4float y, G4float z,
                 G4float energy, G4float energyDeposited,
                 G4float parentID, G4int particleID,
                 const G4String &volumeNamePostStep, G4bool trackingStatus,
                 G4Track *track)
{
    // If this is the first step for this particle in this tally
    if (!tally.flag)
    {
        tally.AddXExit(x);
        tally.AddYExit(y);
        tally.AddZExit(z);
        tally.AddParentID(parentID);
        tally.AddParticleID(particleID);
        tally.AddEnergy(energy);
        tally.ActivateFlag();
    }

    // Add energy deposited for this step
    tally.AddDepositedEnergy(energyDeposited);

    // If particle reached holder volume or lost all energy
    if (volumeNamePostStep == "Holder" || (energy - energyDeposited) == 0)
    {
        tally.AddTotalDepositedEnergy(tally.GetDepositedEnergy());
        tally.ResetDepositedEnergy();
        tally.ResetFlag();
    }
}

/**
 * @brief Main Geant4 stepping action executed at each simulation step.
 *
 * Collects track and step information (positions, momenta, energies, volumes).
 * Manages input beam initialization, quadrupole crossing, collimator detection,
 * YAG screens, and particle termination.
 *
 * @param aStep Pointer to the current Geant4 step.
 */
void PlasmaMLPALLASSteppingAction::UserSteppingAction(const G4Step *aStep)
{
    // --- Preparation of variables ---
    auto evtac  = static_cast<PlasmaMLPALLASEventAction *>(G4EventManager::GetEventManager()->GetUserEventAction());
    theTrack    = aStep->GetTrack();
    auto pre    = aStep->GetPreStepPoint();
    auto post   = aStep->GetPostStepPoint();

    // General info
    particleName   = theTrack->GetDefinition()->GetParticleName();
    particleID     = theTrack->GetDefinition()->GetPDGEncoding();
    endproc        = post->GetProcessDefinedStep()->GetProcessName();
    parentID       = theTrack->GetParentID();
    trackID        = theTrack->GetTrackID();
    stepNo         = theTrack->GetCurrentStepNumber();
    energy         = pre->GetKineticEnergy() / MeV;
    energyDeposited= aStep->GetTotalEnergyDeposit() / CLHEP::keV;

    // Positions
    auto prePos  = pre->GetPosition()  / CLHEP::mm;
    auto postPos = post->GetPosition() / CLHEP::mm;
    preStep.x = prePos.x();  preStep.y = prePos.y();  preStep.z = prePos.z();
    postStep.x = postPos.x(); postStep.y = postPos.y(); postStep.z = postPos.z();

    // Directions
    auto preMom  = pre->GetMomentumDirection();
    auto postMom = post->GetMomentumDirection();
    preStep.px = preMom.x();  preStep.py = preMom.y();  preStep.pz = preMom.z();
    postStep.px = postMom.x(); postStep.py = postMom.y(); postStep.pz = postMom.z();

    // Volumes
    volumeNamePreStep  = pre->GetPhysicalVolume()->GetName();
    volumeNamePostStep = post->GetPhysicalVolume()->GetName();

    // --- Begin main logic ---

    // Initial beam info (step 1, primary particle only)
    if (parentID == 0 && stepNo == 1)
        SetInputInformations(evtac);

    // Quadrupole crossings
    struct QuadTransition { const char* from; const char* to; QuadID id; };
    static const std::vector<QuadTransition> quadTransitions = {
        {"Holder", "Q1Volume", QuadID::Q1},
        {"Q1Volume", "Holder", QuadID::Q1},
        {"Holder", "Q2Volume", QuadID::Q2},
        {"Q2Volume", "Holder", QuadID::Q2},
        {"Holder", "Q3Volume", QuadID::Q3},
        {"Q3Volume", "Holder", QuadID::Q3},
        {"Holder", "Q4Volume", QuadID::Q4},
        {"Q4Volume", "Holder", QuadID::Q4},
    };

    for (const auto &qt : quadTransitions)
    {
        if (volumeNamePreStep == qt.from && volumeNamePostStep == qt.to)
        {
            PositionType posType = (std::string(qt.from) == "Holder") ? PositionType::Begin : PositionType::End;
            SetQuadrupoleInformation(evtac, qt.id, posType);
            break; // No need to check further
        }
    }

    // Collimators
    if (parentID == 0)
    {
        if (volumeNamePostStep == "HorizontalCollimator" && !evtac->GetHorizontalCollimators().GetFlag())
        {
            UpdateCollimators(evtac->GetHorizontalCollimators(), postStep.x, postStep.y, postStep.z, energy);
            if (!TrackingStatusCollimators) theTrack->SetTrackStatus(fStopAndKill);
        }

        if (volumeNamePreStep == "VerticalCollimator"
            && !evtac->GetHorizontalCollimators().GetFlag()
            && !evtac->GetVerticalCollimators().GetFlag())
        {
            UpdateCollimators(evtac->GetVerticalCollimators(), postStep.x, postStep.y, postStep.z, energy);
            if (!TrackingStatusCollimators) theTrack->SetTrackStatus(fStopAndKill);
        }
    }

    // YAG screens
    static const std::map<std::string, RunTallyYAG&(PlasmaMLPALLASEventAction::*)()> yagMap = {
        {"BS1_YAG",    &PlasmaMLPALLASEventAction::GetBSYAG},
        {"BSPEC1_YAG", &PlasmaMLPALLASEventAction::GetBSPECYAG},
    };

    auto it = yagMap.find(volumeNamePreStep);
    if (it != yagMap.end())
    {
        RunTallyYAG &yag = (evtac->*(it->second))();
        UpdateYAG(yag, postStep.x, postStep.y, postStep.z, energy, energyDeposited,
                  parentID, particleID, volumeNamePostStep, TrackingStatus, theTrack);
    }

    // Kill particles leaving the world
    if (volumeNamePostStep == "World")
        theTrack->SetTrackStatus(fStopAndKill);
}
