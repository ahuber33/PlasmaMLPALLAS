/**
 * @file PlasmaMLPALLASEventAction.cc
 * @brief Implementation of the EventAction class for the PALLAS PlasmaMLP simulation.
 *
 * This file defines the PlasmaMLPALLASEventAction class, which handles per-event
 * actions in the Geant4 simulation of the PALLAS PlasmaMLP setup.
 * 
 * The responsibilities of this class include:
 *  - Resetting all per-event statistics at the beginning of an event.
 *  - Collecting input, collimator, beam stop, YAG detector, and quadrupole statistics during the event.
 *  - Passing per-event statistics to the run-level action (PlasmaMLPALLASRunAction) at the end of the event.
 *
 * The class works in conjunction with:
 *  - PlasmaMLPALLASSteppingAction: for per-step updates within an event.
 *  - PlasmaMLPALLASRunAction: to accumulate run-level statistics.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @author Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */


#include "PlasmaMLPALLASSteppingAction.hh"    ///< Stepping action header (per-step updates)
#include "PlasmaMLPALLASEventAction.hh"       ///< Event action header
#include "PlasmaMLPALLASRunAction.hh"         ///< Run action header (for statistics accumulation)

/**
 * @brief Constructor for PlasmaMLPALLASEventAction
 * @param suff Suffix used for naming ROOT branches or output files
 *
 * Initializes the event action and stores the provided suffix.
 */
PlasmaMLPALLASEventAction::PlasmaMLPALLASEventAction(const char *suff) 
    : suffixe(suff) 
{}

/**
 * @brief Destructor for PlasmaMLPALLASEventAction
 *
 * Cleans up any resources used by the event action.
 */
PlasmaMLPALLASEventAction::~PlasmaMLPALLASEventAction() {}

/**
 * @brief Called at the beginning of each event
 * @param evt Pointer to the current G4Event
 *
 * Resets all per-event statistics and counters to initial empty states.
 * This includes:
 * - Input-related counters
 * - Horizontal and vertical collimator statistics
 * - Beam Stop (BS) and BSPEC YAG detector statistics
 * - Quadrupole statistics
 */
void PlasmaMLPALLASEventAction::BeginOfEventAction(const G4Event *evt)
{
    /** Reset input statistics */
    StatsInput = {};

    /** Reset horizontal and vertical collimator statistics */
    StatsHorizontalColl = {};
    StatsVerticalColl = {};

    /** Reset Beam Stop (BS) and BSPEC YAG detector statistics */
    StatsBSYAG = {};
    StatsBSPECYAG = {};

    /** Reset Quadrupole statistics */
    StatsQuadrupoles = {};
}

/**
 * @brief Called at the end of each event
 * @param evt Pointer to the current G4Event
 *
 * Updates run-level statistics by passing the per-event data to the
 * PlasmaMLPALLASRunAction. Only non-empty or relevant data are updated
 * for input, BS YAG, and BSPEC YAG statistics, while quadrupole and
 * collimator statistics are always updated.
 */
void PlasmaMLPALLASEventAction::EndOfEventAction(const G4Event *evt)
{
    /** Get pointer to current run action */
    PlasmaMLPALLASRunAction *runac = 
        (PlasmaMLPALLASRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());

    /** Update input energy statistics if valid */
    if (StatsInput.energy > 0) 
        runac->UpdateStatisticsInput(StatsInput);

    /** Update Beam Stop YAG statistics if not empty */
    if (!StatsBSYAG.energy.empty()) 
        runac->UpdateStatisticsBSYAG(StatsBSYAG);

    /** Update Beam Stop SPEC YAG statistics if not empty */
    if (!StatsBSPECYAG.energy.empty()) 
        runac->UpdateStatisticsBSPECYAG(StatsBSPECYAG);

    /** Always update quadrupole and collimator statistics */
    runac->UpdateStatisticsQuadrupoles(StatsQuadrupoles);

    if(StatsHorizontalColl.GetFlag()==true)
    {
      runac->UpdateStatisticsHorizontalColl(StatsHorizontalColl);
      StatsHorizontalColl.ResetFlag();
    }

    if(StatsVerticalColl.GetFlag()==true)
    {
      runac->UpdateStatisticsVerticalColl(StatsVerticalColl);
      StatsVerticalColl.ResetFlag();
    }
}
