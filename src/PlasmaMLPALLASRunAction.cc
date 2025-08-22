/**
 * @file PlasmaMLPALLASRunAction.cc
 * @brief Implements run-level setup, data collection, and output handling for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This file contains the method definitions for the `PlasmaMLPALLASRunAction` class
 * declared in `PlasmaMLPALLASRunAction.hh`. It manages:
 *  - Initialization of run-wide ROOT files and trees
 *  - Thread-safe data collection in multi-threaded runs
 *  - Branch creation for all recorded statistics
 *  - Interaction with primary generator and geometry to populate run metadata
 *  - Begin/end-of-run hooks to prepare and finalize data storage
 *
 * The run action workflow:
 *  - **BeginOfRunAction**:
 *      - Locks file access (multi-thread safety)
 *      - Builds the ROOT output file name based on threading context
 *      - Creates TTree objects for each statistics category
 *      - Defines ROOT branches for run-wide parameters and measurements
 *      - Initializes the random seed
 *  - **During the run**:
 *      - Updates statistics via `UpdateStatistics()` and specialized variants
 *  - **EndOfRunAction**:
 *      - Finalizes statistics
 *      - Writes all TTrees to the ROOT file
 *      - Closes the file and releases resources
 *
 * Thread safety is ensured via:
 *  - `std::atomic<int> activeThreads` for counting active threads
 *  - `G4Mutex fileMutex` for synchronized file access
 *
 * @note This class uses Geant4's ROOT integration to structure physics
 *       output in an analysis-friendly format.
 */

// Include class header
#include "PlasmaMLPALLASRunAction.hh"

// --- Static member initialization ---
std::atomic<int> PlasmaMLPALLASRunAction::activeThreads(0);       ///< Counter for active threads
G4Mutex PlasmaMLPALLASRunAction::fileMutex = G4MUTEX_INITIALIZER; ///< Mutex for file protection

// --- Constructor ---
PlasmaMLPALLASRunAction::PlasmaMLPALLASRunAction(const char *suff, size_t N, G4bool pMT)
    : suffixe(suff), NEventsGenerated(N), flag_MT(pMT)
{
}

// --- Destructor ---
PlasmaMLPALLASRunAction::~PlasmaMLPALLASRunAction() {}

// --- Primary generator reference setter ---
void PlasmaMLPALLASRunAction::SetPrimaryGenerator(PlasmaMLPALLASPrimaryGeneratorAction *gen)
{
  fPrimaryGenerator = gen;
}

// --- Geometry reference setter ---
void PlasmaMLPALLASRunAction::SetGeometry(PlasmaMLPALLASGeometryConstruction *geom)
{
  fGeometry = geom;
}

/**
 * @brief Utility template to create simple branches in a TTree.
 * @tparam T Data type (int, float, etc.)
 * @param tree Pointer to the TTree to populate
 * @param branches List of name-pointer pairs
 */
template <typename T>
static void CreateBranches(TTree *tree, const std::vector<std::pair<const char *, T *>> &branches)
{
  for (const auto &b : branches)
  {
    tree->Branch(
        b.first,
        b.second,
        (std::string(b.first) + "/" +
         (std::is_same<T, int>::value ? "I" : std::is_same<T, float>::value ? "F"
                                                                            : ""))
            .c_str());
  }
}

/**
 * @brief Creates ROOT branches specific to YAG detector statistics.
 * @param tree ROOT tree to populate
 * @param stats YAG statistics structure
 */
static void CreateYAGBranches(TTree *tree, RunTallyYAG &stats)
{
  tree->Branch("x_exit", "vector<float>", &stats.x_exit);
  tree->Branch("y_exit", "vector<float>", &stats.y_exit);
  tree->Branch("z_exit", "vector<float>", &stats.z_exit);
  tree->Branch("parentID", "vector<int>", &stats.parentID);
  tree->Branch("particleID", "vector<int>", &stats.particleID);
  tree->Branch("energy", "vector<float>", &stats.energy);
  tree->Branch("deposited_energy", "vector<float>", &stats.total_deposited_energy);
}

/**
 * @brief Creates ROOT branches specific to Collimators statistics.
 * @param tree ROOT tree to populate
 * @param stats Collimators statistics structure
 */
static void CreateCollimatorsBranches(TTree *tree, RunTallyCollimators &stats)
{
  tree->Branch("x_interaction", &stats.x, "x_interaction/F");
  tree->Branch("y_interaction", &stats.y, "y_interaction/F");
  tree->Branch("z_interaction", &stats.z, "z_interaction/F");
  tree->Branch("energy", &stats.energy, "energy/F");
}

//---------------------------------------------------------
//  Generic statistics update function
//---------------------------------------------------------
/**
 * @brief Thread-safe update of statistics and ROOT tree filling.
 * @tparam T Type of the statistics structure
 * @param stats Destination statistics object (persistent in the run)
 * @param newStats Source statistics (new values)
 * @param tree ROOT tree to fill
 */
template <typename T>
void PlasmaMLPALLASRunAction::UpdateStatistics(T &stats, const T &newStats, TTree *tree)
{
  std::lock_guard<std::mutex> lock(fileMutex);
  stats = newStats;
  if (tree)
    tree->Fill();
  else
    G4cerr << "Error: Tree is nullptr" << G4endl;
}

// --- Specific statistics update wrappers ---
void PlasmaMLPALLASRunAction::UpdateStatisticsGlobalInput(RunTallyGlobalInput a) { UpdateStatistics(StatsGlobalInput, a, Tree_GlobalInput); }
void PlasmaMLPALLASRunAction::UpdateStatisticsInput(RunTallyInput a) { UpdateStatistics(StatsInput, a, Tree_Input); }
void PlasmaMLPALLASRunAction::UpdateStatisticsQuadrupoles(RunTallyQuadrupoles a) { UpdateStatistics(StatsQuadrupoles, a, Tree_Quadrupoles); }
void PlasmaMLPALLASRunAction::UpdateStatisticsHorizontalColl(RunTallyCollimators a) { UpdateStatistics(StatsHorizontalColl, a, Tree_HorizontalColl); }
void PlasmaMLPALLASRunAction::UpdateStatisticsVerticalColl(RunTallyCollimators a) { UpdateStatistics(StatsVerticalColl, a, Tree_VerticalColl); }
void PlasmaMLPALLASRunAction::UpdateStatisticsBSYAG(RunTallyYAG a) { UpdateStatistics(StatsBSYAG, a, Tree_BSYAG); }
void PlasmaMLPALLASRunAction::UpdateStatisticsBSPECYAG(RunTallyYAG a) { UpdateStatistics(StatsBSPECYAG, a, Tree_BSPECYAG); }

/**
 * @brief Populates global input statistics from generator and geometry state.
 * @param gen Pointer to primary generator
 * @param geo Pointer to geometry construction
 * @param nEvents Total number of events in the run
 */
void RunTallyGlobalInput::FillFrom(const PlasmaMLPALLASPrimaryGeneratorAction *gen,
                                   const PlasmaMLPALLASGeometryConstruction *geo,
                                   int nEvents)
{
  // Extract beam parameters from generator
  if (gen)
  {
    Xoff = gen->GetML_Xoff();
    p = gen->GetML_Pressure();
    cN2 = gen->GetML_CN2();
    A0 = gen->GetML_A0();
    Ekin = gen->GetEkin();
    dEkin = gen->GetdEkin();
    Q = gen->GetQ();
    epsb = gen->GetEPSB();
  }

  // Extract geometry configuration
  if (geo)
  {
    NEvents = nEvents;
    Display_Geometry = geo->GetStatusDisplayGeometry();
    Display_Collimators = geo->GetStatusDisplayCollimators();
    Display_Quadrupoles = geo->GetStatusDisplayQuadrupoles();

    Q1_Grad = geo->GetQ1Gradient();
    Q2_Grad = geo->GetQ2Gradient();
    Q3_Grad = geo->GetQ3Gradient();
    Q4_Grad = geo->GetQ4Gradient();
    Q1_Length = geo->GetQ1Length();
    Q2_Length = geo->GetQ2Length();
    Q3_Length = geo->GetQ3Length();
    Q4_Length = geo->GetQ4Length();
    SourceQ1Distance = geo->GetSourceQ1Distance();
    Q1Q2Distance = geo->GetQ1Q2Distance();
    Q2Q3Distance = geo->GetQ2Q3Distance();
    Q3Q4Distance = geo->GetQ3Q4Distance();
    B_Dipole_Map = geo->GetStatusMapBField();
    B_Dipole = geo->GetConstantDipoleBField();
  }
}

//-----------------------------------------------------
//  BeginOfRunAction
//-----------------------------------------------------
/**
 * @brief Called at the start of each run to set up ROOT output structures and initialize state.
 * @param aRun Pointer to the current G4Run
 */
void PlasmaMLPALLASRunAction::BeginOfRunAction(const G4Run *aRun)
{
  // Populate branches for each TTree...
  G4AutoLock lock(&fileMutex); // Automatic mutex lock

  start = time(NULL); // start the timer clock to calculate run times

  int a = activeThreads;

  std::string s = flag_MT ? "_" + std::to_string(activeThreads) : "";
  fileName = suffixe + s + ".root";

  G4cout << "Filename = " << fileName << G4endl;

  f = new TFile(fileName.c_str(), "RECREATE");

  // Creating trees for different types of run information
  Tree_GlobalInput = new TTree("GlobalInput", "Global Input Information");                 // Tree to access Input information
  Tree_Input = new TTree("Input", "Input Information");                                    // Tree to access Input information
  Tree_Quadrupoles = new TTree("QuadrupolesTracking", "Quadrupoles Tracking Information"); // Tree to access Quadrupoles tracking information
  Tree_HorizontalColl = new TTree("Horizontal_Coll", "Horizontal Collimator Information"); // Tree to access Horizontal Collimator information
  Tree_VerticalColl = new TTree("Vertical_Coll", "Vertical Collimator Information");       // Tree to access Vertical Collimator information
  Tree_BSYAG = new TTree("BSYAG", "BS YAG Information");                                   // Tree to access Back Collimator infos
  Tree_BSPECYAG = new TTree("BSPECYAG", "BSPEC YAG Information");                          // Tree to access Back Collimator infos

  //*****************************INFORMATIONS FROM THE GLOBAL INPUT*******************************************
  std::vector<std::pair<const char *, int *>> globalIntBranches = {
      {"NEvents", &StatsGlobalInput.NEvents},
      {"Display_FullPALLASGeometry", &StatsGlobalInput.Display_Geometry},
      {"Display_Collimators", &StatsGlobalInput.Display_Collimators},
      {"Display_Quadrupoles", &StatsGlobalInput.Display_Quadrupoles},
      {"B_Dipole_Map", &StatsGlobalInput.B_Dipole_Map}};

  std::vector<std::pair<const char *, float *>> globalFloatBranches = {
      {"Q1_Length", &StatsGlobalInput.Q1_Length},
      {"Q2_Length", &StatsGlobalInput.Q2_Length},
      {"Q3_Length", &StatsGlobalInput.Q3_Length},
      {"Q4_Length", &StatsGlobalInput.Q4_Length},
      {"SourceQ1Distance", &StatsGlobalInput.SourceQ1Distance},
      {"Q1Q2Distance", &StatsGlobalInput.Q1Q2Distance},
      {"Q2Q3Distance", &StatsGlobalInput.Q2Q3Distance},
      {"Q3Q4Distance", &StatsGlobalInput.Q3Q4Distance},
      {"Q1_Grad", &StatsGlobalInput.Q1_Grad},
      {"Q2_Grad", &StatsGlobalInput.Q2_Grad},
      {"Q3_Grad", &StatsGlobalInput.Q3_Grad},
      {"Q4_Grad", &StatsGlobalInput.Q4_Grad},
      {"B_Dipole", &StatsGlobalInput.B_Dipole},
      {"Xoff", &StatsGlobalInput.Xoff},
      {"p", &StatsGlobalInput.p},
      {"cN2", &StatsGlobalInput.cN2},
      {"A0", &StatsGlobalInput.A0},
      {"Ekin", &StatsGlobalInput.Ekin},
      {"dEkin", &StatsGlobalInput.dEkin},
      {"Q", &StatsGlobalInput.Q},
      {"epsb", &StatsGlobalInput.epsb}};

  CreateBranches(Tree_GlobalInput, globalIntBranches);
  CreateBranches(Tree_GlobalInput, globalFloatBranches);

  //*****************************INFORMATIONS FROM THE INPUT*******************************************
  std::vector<std::pair<const char *, float *>> inputBranches = {
      {"x", &StatsInput.x}, {"xp", &StatsInput.xp}, {"y", &StatsInput.y}, {"yp", &StatsInput.yp}, {"z", &StatsInput.z}, {"zp", &StatsInput.zp}, {"energy", &StatsInput.energy}};
  CreateBranches(Tree_Input, inputBranches);

  //*****************************INFORMATIONS FROM THE QUADRUPOLES TRACKING*******************************************
  const char *quads[] = {"Q1", "Q2", "Q3", "Q4"};
  const char *parts[] = {"Begin", "End"};
  const char *coords[] = {"x", "y", "z"};
  const char *types[] = {"Pos", "Mom"}; // Position ou Momentum
  const int nQuads = 4;

  // Tableau de pointeurs sur les quadrupoles
  QuadrupoleState *Qs[nQuads] = {&StatsQuadrupoles.Q1, &StatsQuadrupoles.Q2,
                                 &StatsQuadrupoles.Q3, &StatsQuadrupoles.Q4};

  for (int q = 0; q < nQuads; ++q)
  {
    for (int t = 0; t < 2; ++t)
    { // Pos ou Mom
      for (int p = 0; p < 2; ++p)
      { // Begin ou End
        Vector3 *vec = (t == 0) ? ((p == 0) ? &Qs[q]->begin : &Qs[q]->end)
                                : ((p == 0) ? &Qs[q]->beginMomentum : &Qs[q]->endMomentum);

        for (int c = 0; c < 3; ++c)
        { // x, y, z
          float *ptr = nullptr;
          if (c == 0)
            ptr = &vec->x;
          else if (c == 1)
            ptr = &vec->y;
          else
            ptr = &vec->z;

          TString branchName = TString::Format("%s%s%s_%s", quads[q], parts[p], types[t], coords[c]);
          Tree_Quadrupoles->Branch(branchName, ptr, TString::Format("%s/F", branchName.Data()));
        }
      }
    }
  }

  // Branch pour l'énergie
  Tree_Quadrupoles->Branch("energy", &StatsQuadrupoles.energy, "energy/F");

  //************************************INFORMATIONS FROM THE HORIZONTAL COLLIMATOR*****************************************
  CreateCollimatorsBranches(Tree_HorizontalColl, StatsHorizontalColl);

  //************************************INFORMATIONS FROM THE VERTICAL COLLIMATOR*****************************************
  CreateCollimatorsBranches(Tree_VerticalColl, StatsVerticalColl);

  //************************************INFORMATIONS FROM THE YAGs*****************************************
  //************************************INFORMATIONS FROM THE BS YAG*****************************************
  CreateYAGBranches(Tree_BSYAG, StatsBSYAG);

  //************************************INFORMATIONS FROM THE BSPEC YAG*****************************************
  CreateYAGBranches(Tree_BSPECYAG, StatsBSPECYAG);

  // set the random seed to the CPU clock
  // G4Random::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed = time(NULL) + a;
  G4Random::setTheSeed(seed);
  // G4Random::setTheSeed(1712670533);
  G4cout << "seed = " << seed << G4endl;

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (G4VVisManager::GetConcreteInstance())
  {
    G4UImanager *UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }

  activeThreads++;
}

//-----------------------------------------------------
//  EndOfRunAction
//-----------------------------------------------------
/**
 * @brief Called at the end of each run to write collected data to the ROOT file and clean up.
 * @param aRun Pointer to the current G4Run
 */
void PlasmaMLPALLASRunAction::EndOfRunAction(const G4Run *aRun)
{
  StatsGlobalInput.FillFrom(fPrimaryGenerator, fGeometry, NEventsGenerated);
  UpdateStatisticsGlobalInput(StatsGlobalInput);

  G4AutoLock lock(&fileMutex);

  // Write all trees to ROOT file
  f->cd();
  Tree_GlobalInput->Write();
  Tree_Input->Write();
  Tree_Quadrupoles->Write();
  Tree_HorizontalColl->Write();
  Tree_VerticalColl->Write();
  Tree_BSYAG->Write();
  Tree_BSPECYAG->Write();
  f->Close();
  delete f;
  f = nullptr;

  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");

  G4cout << "Leaving Run Action" << G4endl;
}
