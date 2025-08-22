/**
 * @file PlasmaMLPALLASActionInitialization.cc
 * @brief Implementation of the ActionInitialization class for the PALLAS PlasmaMLP simulation.
 *
 * This file defines the PlasmaMLPALLASActionInitialization class, which is responsible
 * for setting up all user actions required for a Geant4 simulation of the PALLAS PlasmaMLP.
 * It manages the creation and assignment of the following actions:
 *   - Primary generator action
 *   - Run action
 *   - Event action
 *   - Stepping action
 *
 * In multithreaded mode, this class also defines master-thread-specific actions such as RunAction.
 * It stores configuration parameters such as the number of events, number of threads, and output suffix,
 * and provides access to the simulation geometry.
 *
 * This class inherits from G4VUserActionInitialization and overrides Build() and BuildForMaster()
 * to provide thread-aware initialization of user actions.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @author Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */


#include "PlasmaMLPALLASActionInitialization.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Constructor for ActionInitialization class
 * @param suff Suffix string to append to output files
 * @param N Number of events to generate
 * @param Ncores Number of threads to use in multithreaded mode
 * @param pMT Flag indicating whether multithreading is enabled
 * @param geometry Pointer to the simulation geometry
 */
PlasmaMLPALLASActionInitialization::PlasmaMLPALLASActionInitialization(
    const char *suff, 
    size_t N, 
    size_t Ncores, 
    G4bool pMT, 
    PlasmaMLPALLASGeometryConstruction* geometry)
    : G4VUserActionInitialization(), 
      suffixe(suff),
      NEventsGenerated(N),
      numThreads(Ncores),
      flag_MT(pMT),
      fGeometry(geometry)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Destructor for ActionInitialization class
 */
PlasmaMLPALLASActionInitialization::~PlasmaMLPALLASActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Build actions for the master thread
 *
 * This function is called in multithreaded mode to define actions
 * that are executed only in the master thread, such as RunAction.
 */
void PlasmaMLPALLASActionInitialization::BuildForMaster() const
{
    SetUserAction(new PlasmaMLPALLASRunAction(suffixe, NEventsGenerated, flag_MT));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Build actions for worker threads or single-threaded runs
 *
 * This function creates and assigns all user actions required for the simulation:
 * - PrimaryGeneratorAction
 * - RunAction
 * - EventAction
 * - SteppingAction
 */
void PlasmaMLPALLASActionInitialization::Build() const
{
    // Create primary generator action
    auto *generator = new PlasmaMLPALLASPrimaryGeneratorAction(NEventsGenerated, numThreads, flag_MT);
    
    // Create run action
    auto *runAction = new PlasmaMLPALLASRunAction(suffixe, NEventsGenerated, flag_MT);
    
    // Create event action
    auto *eventAction = new PlasmaMLPALLASEventAction(suffixe);

    // Provide the run action with pointers to the primary generator and geometry
    runAction->SetPrimaryGenerator(generator);
    runAction->SetGeometry(fGeometry);
    
    // Assign user actions to the simulation
    SetUserAction(generator);
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new PlasmaMLPALLASSteppingAction());
}
