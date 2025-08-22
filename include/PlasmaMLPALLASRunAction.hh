#ifndef PlasmaMLPALLASRunAction_h
#define PlasmaMLPALLASRunAction_h 1

/**
 * @class PlasmaMLPALLASRunAction
 * @brief Defines the run-level actions and statistics handling for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This header declares the `PlasmaMLPALLASRunAction` class, which inherits from 
 * Geant4's `G4UserRunAction` to manage actions at the beginning and end of each run. 
 * It handles:
 *  - Collection and storage of run-wide statistics
 *  - ROOT file and tree creation for data output
 *  - Synchronization in multithreaded runs
 *  - Coordination with primary generator and geometry configuration
 *
 * The associated `RunTallyGlobalInput` struct stores simulation configuration
 * parameters and beamline element settings for the entire run.
 *
 * Data recorded here typically includes:
 *  - Initial beam parameters
 *  - Magnet gradients and lengths
 *  - Detector and collimator configurations
 *  - Collected measurements from various beamline elements
 *
 * @note This class interacts closely with:
 *       - `PlasmaMLPALLASPrimaryGeneratorAction`
 *       - `PlasmaMLPALLASGeometryConstruction`
 *       - Event-level statistics classes such as `RunTallyQuadrupoles`
 */

// Include base classes and Geant4 utilities
#include "G4UserRunAction.hh"       // Base class for user-defined run actions
#include "G4Run.hh"                 // Run object for event accumulation
#include "G4UImanager.hh"           // UI manager (for commands)
#include "G4VVisManager.hh"         // Visualization manager
#include "G4RunManager.hh"
#include "TFile.h"                  // ROOT file I/O
#include "TTree.h"
#include "TBranch.h"
#include <mutex>
#include "PlasmaMLPALLASPrimaryGeneratorAction.hh"
#include "PlasmaMLPALLASGeometryConstruction.hh"
#include "PlasmaMLPALLASEventAction.hh" 


// Forward declarations
class PlasmaMLPALLASPrimaryGeneratorAction;
class PlasmaMLPALLASGeometryConstruction;

/**
 * @struct RunTallyGlobalInput
 * @brief Stores configuration parameters and beamline settings for a simulation run.
 */
struct RunTallyGlobalInput {
  // --- General run settings ---
  int NEvents = 0;                 
  int Display_Geometry = 0;             
  int Display_Collimators = 0;     
  int Display_Quadrupoles = 0;     

  // --- Physical parameters ---
  float Xoff = 0.0;               
  float p = 0.0;                  
  float cN2 = 0.0;                
  float A0 = 0.0;                 
  float Ekin = 0.0;               
  float dEkin = 0.0;              
  float Q = 0.0;                  
  float epsb = 0.0;               
  float Q1_Grad = 0.0;            
  float Q2_Grad = 0.0;            
  float Q3_Grad = 0.0;            
  float Q4_Grad = 0.0;            
  float Q1_Length = 0.0;          
  float Q2_Length = 0.0;          
  float Q3_Length = 0.0;          
  float Q4_Length = 0.0;          
  float SourceQ1Distance = 0.0;   
  float Q1Q2Distance = 0.0;       
  float Q2Q3Distance = 0.0;       
  float Q3Q4Distance = 0.0;       
  float B_Dipole = 0.0;           
  int B_Dipole_Map = 0;            

  /**
   * @brief Populate structure from generator and geometry settings.
   * @param gen Pointer to primary generator
   * @param geo Pointer to geometry construction
   * @param nEvents Number of events to simulate
   */
  void FillFrom(const PlasmaMLPALLASPrimaryGeneratorAction* gen, 
                const PlasmaMLPALLASGeometryConstruction* geo,
                int nEvents);
};

/**
 * @class PlasmaMLPALLASRunAction
 * @brief Manages run-level actions, statistics, and ROOT output.
 */
class PlasmaMLPALLASRunAction : public G4UserRunAction
{
public:
  /**
   * @brief Constructor
   * @param fileName Base name for ROOT output file
   * @param nEventsGenerated Number of events to process
   * @param multithreaded True if running with multithreading
   */
  PlasmaMLPALLASRunAction(const char* fileName, size_t nEventsGenerated, G4bool multithreaded);

  /// Destructor
  ~PlasmaMLPALLASRunAction();

  /// Called at the start of each run
  void BeginOfRunAction(const G4Run* run) override;

  /// Called at the end of each run
  void EndOfRunAction(const G4Run* run) override;

  /// Generic template to update cumulative statistics in a ROOT tree
  template<typename T>
  void UpdateStatistics(T& stats, const T& newStats, TTree* tree);

  // --- Specific statistics update methods ---
  void UpdateStatisticsGlobalInput(RunTallyGlobalInput);
  void UpdateStatisticsInput(RunTallyInput);
  void UpdateStatisticsQuadrupoles(RunTallyQuadrupoles);
  void UpdateStatisticsHorizontalColl(RunTallyCollimators);
  void UpdateStatisticsVerticalColl(RunTallyCollimators);
  void UpdateStatisticsBSYAG(RunTallyYAG);
  void UpdateStatisticsBSPECYAG(RunTallyYAG);

  /// Set the primary generator reference
  void SetPrimaryGenerator(PlasmaMLPALLASPrimaryGeneratorAction* gen);

  /// Set the geometry reference
  void SetGeometry(PlasmaMLPALLASGeometryConstruction* geom);

private:
  // --- Output configuration ---
  G4String suffixe;     ///< File suffix for ROOT outputs
  G4String fileName;    ///< Base file name for ROOT outputs

  // --- Stored run statistics ---
  RunTallyGlobalInput StatsGlobalInput;
  RunTallyInput StatsInput;
  RunTallyQuadrupoles StatsQuadrupoles;
  RunTallyCollimators StatsHorizontalColl;
  RunTallyCollimators StatsVerticalColl;
  RunTallyYAG StatsBSYAG;
  RunTallyYAG StatsBSPECYAG;

  size_t NEventsGenerated; ///< Number of events generated in the run
  G4bool flag_MT;          ///< Multithreading enabled flag

  // --- ROOT file and trees ---
  TFile *f = nullptr;
  TTree *Tree_GlobalInput = nullptr;
  TTree *Tree_Input = nullptr;
  TTree *Tree_Quadrupoles = nullptr;
  TTree *Tree_HorizontalColl = nullptr;
  TTree *Tree_VerticalColl = nullptr;
  TTree *Tree_BSYAG = nullptr;
  TTree *Tree_BSPECYAG = nullptr;
  TBranch *RunBranch = nullptr;

  time_t start; ///< Start time of the run

  // --- Thread-safety ---
  static std::atomic<int> activeThreads;
  static G4Mutex fileMutex;

protected:
  PlasmaMLPALLASPrimaryGeneratorAction* fPrimaryGenerator = nullptr; ///< Primary generator reference
  PlasmaMLPALLASGeometryConstruction* fGeometry = nullptr; ///< Geometry reference
};

#endif // PlasmaMLPALLASRunAction_h