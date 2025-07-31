/// PlasmaAccelerationFacilitySimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PlasmaAccelerationFacilitySimRunAction.hh"
std::atomic<int> PlasmaAccelerationFacilitySimRunAction::activeThreads(0);
G4Mutex PlasmaAccelerationFacilitySimRunAction::fileMutex = G4MUTEX_INITIALIZER;

PlasmaAccelerationFacilitySimRunAction::PlasmaAccelerationFacilitySimRunAction(const char* suff, G4bool pMT)
                                                :suffixe(suff),
                                                flag_MT(pMT)
{}

PlasmaAccelerationFacilitySimRunAction::~PlasmaAccelerationFacilitySimRunAction()
{}

//-----------------------------------------------------
//  BeginOfRunAction:  used to calculate the start time and
//  to set up information in the run tree.
//-----------------------------------------------------

void PlasmaAccelerationFacilitySimRunAction::SetPrimaryGenerator(PlasmaAccelerationFacilitySimPrimaryGeneratorAction* gen) {
  fPrimaryGenerator = gen;
}

void PlasmaAccelerationFacilitySimRunAction::SetGeometry(PlasmaAccelerationFacilitySimGeometryConstruction* geom) {
  fGeometry = geom;
}



void PlasmaAccelerationFacilitySimRunAction::BeginOfRunAction(const G4Run* aRun){

  G4AutoLock lock(&fileMutex); // Verrouillage automatique du mutex

  start = time(NULL);     //start the timer clock to calculate run times

  int a=activeThreads;

  if (flag_MT == true) 
  {
    G4String s = std::to_string(a);
    fileName = suffixe + "_" +s+".root";
  }

  else 
  {
    fileName = suffixe +".root";
  }

  G4cout << "Filename = " << fileName << G4endl;


  f = new TFile(fileName.c_str(),"RECREATE");

  Tree_GlobalInput = new TTree("GlobalInput","Global Input Information");  //Tree to access Input information
  Tree_Input = new TTree("Input","Input Information");  //Tree to access Input information
  Tree_HorizontalColl = new TTree("Horizontal_Coll","Horizontal Collimator Information");  //Tree to access Horizontal Collimator information
  Tree_VerticalColl = new TTree("Vertical_Coll","Vertical Collimator Information");  //Tree to access Vertical Collimator information
  Tree_BSYAG = new TTree("BSYAG","BS YAG Information");  //Tree to access Back Collimator infos
  Tree_BSPECYAG = new TTree("BSPECYAG","BSPEC YAG Information");  //Tree to access Back Collimator infos


  //*****************************INFORMATIONS FROM THE GLOBAL INPUT*******************************************
  RunBranch = Tree_GlobalInput->Branch("Display_Cellule [y/n]", &StatsGlobalInput.Display_Cellule, "Display_Cellule/I" );
  RunBranch = Tree_GlobalInput->Branch("Display_LIF [y/n]", &StatsGlobalInput.Display_LIF, "Display_LIF/I" );
  RunBranch = Tree_GlobalInput->Branch("Display_Section1 [y/n]", &StatsGlobalInput.Display_Section1, "Display_Section1/I" );
  RunBranch = Tree_GlobalInput->Branch("Display_Section2 [y/n]", &StatsGlobalInput.Display_Section2, "Display_Section2/I" );
  RunBranch = Tree_GlobalInput->Branch("Display_Section3 [y/n]", &StatsGlobalInput.Display_Section3, "Display_Section3/I" );
  RunBranch = Tree_GlobalInput->Branch("Display_Section4 [y/n]", &StatsGlobalInput.Display_Section4, "Display_Section4/I" );
  RunBranch = Tree_GlobalInput->Branch("Display_Section4Dump [y/n]", &StatsGlobalInput.Display_Section4Dump, "Display_Section4Dump/I" );
  RunBranch = Tree_GlobalInput->Branch("Display_Collimators [y/n]", &StatsGlobalInput.Display_Collimators, "Display_Collimators/I" );
  RunBranch = Tree_GlobalInput->Branch("Q1_Length [mm]", &StatsGlobalInput.Q1_Length, "Q1_Length/F" );
  RunBranch = Tree_GlobalInput->Branch("Q2_Length [mm]", &StatsGlobalInput.Q2_Length, "Q2_Length/F" );
  RunBranch = Tree_GlobalInput->Branch("Q3_Length [mm]", &StatsGlobalInput.Q3_Length, "Q3_Length/F" );
  RunBranch = Tree_GlobalInput->Branch("Q4_Length [mm]", &StatsGlobalInput.Q4_Length, "Q4_Length/F" );
  RunBranch = Tree_GlobalInput->Branch("SourceQ1Distance [mm]", &StatsGlobalInput.SourceQ1Distance, "SourceQ1Distance/F" );
  RunBranch = Tree_GlobalInput->Branch("Q1Q2Distance [mm]", &StatsGlobalInput.Q1Q2Distance, "Q1Q2Distance/F" );
  RunBranch = Tree_GlobalInput->Branch("Q2Q3Distance [mm]", &StatsGlobalInput.Q2Q3Distance, "Q2Q3Distance/F" );
  RunBranch = Tree_GlobalInput->Branch("Q3Q4Distance [mm]", &StatsGlobalInput.Q3Q4Distance, "Q3Q4Distance/F" );
  RunBranch = Tree_GlobalInput->Branch("Q1_Grad [T/m]", &StatsGlobalInput.Q1_Grad, "Q1_Grad/F" );
  RunBranch = Tree_GlobalInput->Branch("Q2_Grad [T/m]", &StatsGlobalInput.Q2_Grad, "Q2_Grad/F" );
  RunBranch = Tree_GlobalInput->Branch("Q3_Grad [T/m]", &StatsGlobalInput.Q3_Grad, "Q3_Grad/F" );
  RunBranch = Tree_GlobalInput->Branch("Q4_Grad [T/m]", &StatsGlobalInput.Q4_Grad, "Q4_Grad/F" );
  RunBranch = Tree_GlobalInput->Branch("B_Dipole_Map [y/n]", &StatsGlobalInput.B_Dipole_Map, "B_Dipole_Map/I" );
  RunBranch = Tree_GlobalInput->Branch("B_Dipole [T]", &StatsGlobalInput.B_Dipole, "B_Dipole/F" );
  RunBranch = Tree_GlobalInput->Branch("Xoff [um]", &StatsGlobalInput.Xoff, "Xoff/F" );
  RunBranch = Tree_GlobalInput->Branch("p [mbar]", &StatsGlobalInput.p, "p/F" );
  RunBranch = Tree_GlobalInput->Branch("cN2 [%]", &StatsGlobalInput.cN2, "cN2/F" );
  RunBranch = Tree_GlobalInput->Branch("A0", &StatsGlobalInput.A0, "A0/F" );
  RunBranch = Tree_GlobalInput->Branch("Ekin [MeV]", &StatsGlobalInput.Ekin, "Ekin/F" );
  RunBranch = Tree_GlobalInput->Branch("dEkin [%]", &StatsGlobalInput.dEkin, "dEkin/F" );
  RunBranch = Tree_GlobalInput->Branch("Q [pC]", &StatsGlobalInput.Q, "Q/F" );
  RunBranch = Tree_GlobalInput->Branch("epsb [um]", &StatsGlobalInput.epsb, "epsb/F" );

  //*****************************INFORMATIONS FROM THE INPUT*******************************************
  RunBranch = Tree_Input->Branch("x", &StatsInput.x, "x/F" );
  RunBranch = Tree_Input->Branch("xoffset", &StatsInput.xoffset, "xoffset/F" );
  RunBranch = Tree_Input->Branch("xp", &StatsInput.xp, "xp/F" );
  RunBranch = Tree_Input->Branch("y", &StatsInput.y, "y/F" );
  RunBranch = Tree_Input->Branch("yoffset", &StatsInput.yoffset, "yoffset/F" );
  RunBranch = Tree_Input->Branch("yp", &StatsInput.yp, "yp/F" );
  RunBranch = Tree_Input->Branch("s", &StatsInput.s, "s/F" );
  RunBranch = Tree_Input->Branch("soffset", &StatsInput.soffset, "soffset/F" );
  RunBranch = Tree_Input->Branch("p", &StatsInput.p, "p/F" );
  RunBranch = Tree_Input->Branch("delta", &StatsInput.delta, "delta/F" );
  RunBranch = Tree_Input->Branch("energy", &StatsInput.energy, "energy/F" );
  RunBranch = Tree_Input->Branch("Nevent", &StatsInput.Nevent, "Nevent/I" );

//************************************INFORMATIONS FROM THE HORIZONTAL COLLIMATOR*****************************************
  //RunBranch = Tree_HorizontalColl->Branch("parentID", "vector<int>" , &StatsHorizontalColl.parentID);
  RunBranch = Tree_HorizontalColl->Branch("particleID", "vector<int>" , &StatsHorizontalColl.particleID);
  RunBranch = Tree_HorizontalColl->Branch("energy", "vector<float>" , &StatsHorizontalColl.energy);
  RunBranch = Tree_HorizontalColl->Branch("Edep", &StatsHorizontalColl.Edep, "Edep/F" );

  //************************************INFORMATIONS FROM THE VERTICAL COLLIMATOR*****************************************
  //RunBranch = Tree_VerticalColl->Branch("parentID", "vector<int>" , &StatsVerticalColl.parentID);
  RunBranch = Tree_VerticalColl->Branch("particleID", "vector<int>" , &StatsVerticalColl.particleID);
  RunBranch = Tree_VerticalColl->Branch("energy", "vector<float>" , &StatsVerticalColl.energy);
  RunBranch = Tree_VerticalColl->Branch("Edep", &StatsVerticalColl.Edep, "Edep/F" );

  //************************************INFORMATIONS FROM THE BS YAG*****************************************
  RunBranch = Tree_BSYAG->Branch("x_exit", "vector<float>" , &StatsBSYAG.x_exit);
  RunBranch = Tree_BSYAG->Branch("y_exit", "vector<float>" , &StatsBSYAG.y_exit);
  RunBranch = Tree_BSYAG->Branch("z_exit", "vector<float>" , &StatsBSYAG.z_exit);
  RunBranch = Tree_BSYAG->Branch("parentID", "vector<int>" , &StatsBSYAG.parentID);
  RunBranch = Tree_BSYAG->Branch("particleID", "vector<int>" , &StatsBSYAG.particleID);
  RunBranch = Tree_BSYAG->Branch("energy", "vector<float>" , &StatsBSYAG.energy);
  RunBranch = Tree_BSYAG->Branch("deposited_energy", "vector<float>" , &StatsBSYAG.total_deposited_energy);

//************************************INFORMATIONS FROM THE BSPEC YAG*****************************************
  RunBranch = Tree_BSPECYAG->Branch("x_exit", "vector<float>" , &StatsBSPECYAG.x_exit);
  RunBranch = Tree_BSPECYAG->Branch("y_exit", "vector<float>" , &StatsBSPECYAG.y_exit);
  RunBranch = Tree_BSPECYAG->Branch("z_exit", "vector<float>" , &StatsBSPECYAG.z_exit);
  RunBranch = Tree_BSPECYAG->Branch("parentID", "vector<int>" , &StatsBSPECYAG.parentID);
  RunBranch = Tree_BSPECYAG->Branch("particleID", "vector<int>" , &StatsBSPECYAG.particleID);
  RunBranch = Tree_BSPECYAG->Branch("energy", "vector<float>" , &StatsBSPECYAG.energy);
  RunBranch = Tree_BSPECYAG->Branch("deposited_energy", "vector<float>" , &StatsBSPECYAG.total_deposited_energy);



  //set the random seed to the CPU clock
  //G4Random::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed = time(NULL) + a;
  G4Random::setTheSeed(seed);
  //G4Random::setTheSeed(1712670533);
  G4cout << "seed = " << seed << G4endl;

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }

  activeThreads++;

}  //end BeginOfRunAction


//-----------------------------------------------------
//  EndOfRunAction:  used to calculate the end time and
//  to write information to the run tree.
//-----------------------------------------------------
void PlasmaAccelerationFacilitySimRunAction::EndOfRunAction(const G4Run *aRun) {
  if (fPrimaryGenerator != nullptr) {
    StatsGlobalInput.Xoff = fPrimaryGenerator->GetXoff();
    StatsGlobalInput.p = fPrimaryGenerator->GetP();
    StatsGlobalInput.cN2 = fPrimaryGenerator->GetCN2();
    StatsGlobalInput.A0 = fPrimaryGenerator->GetA0();
    StatsGlobalInput.Ekin = fPrimaryGenerator->GetEkin();
    StatsGlobalInput.dEkin = fPrimaryGenerator->GetdEkin();
    StatsGlobalInput.Q = fPrimaryGenerator->GetQ();
    StatsGlobalInput.epsb = fPrimaryGenerator->GetEPSB();
  }

  if (fGeometry != nullptr) {
    StatsGlobalInput.Display_Cellule = fGeometry->GetStatusDisplayCelluleGeometry();
    StatsGlobalInput.Display_LIF = fGeometry->GetStatusDisplayLIFGeometry();
    StatsGlobalInput.Display_Section1 = fGeometry->GetStatusDisplaySection1Geometry();
    StatsGlobalInput.Display_Section2 = fGeometry->GetStatusDisplaySection2Geometry();
    StatsGlobalInput.Display_Section3 = fGeometry->GetStatusDisplaySection3Geometry();
    StatsGlobalInput.Display_Section4 = fGeometry->GetStatusDisplaySection4Geometry();
    StatsGlobalInput.Display_Section4Dump = fGeometry->GetStatusDisplaySection4DumpGeometry();
    StatsGlobalInput.Display_Collimators = fGeometry->GetStatusDisplayCollimators();
    StatsGlobalInput.Q1_Grad = fGeometry->GetQ1Grad();
    StatsGlobalInput.Q2_Grad = fGeometry->GetQ2Grad();
    StatsGlobalInput.Q3_Grad = fGeometry->GetQ3Grad();
    StatsGlobalInput.Q4_Grad = fGeometry->GetQ4Grad();
    StatsGlobalInput.Q1_Length = fGeometry->GetQ1Length();
    StatsGlobalInput.Q2_Length = fGeometry->GetQ2Length();
    StatsGlobalInput.Q3_Length = fGeometry->GetQ3Length();
    StatsGlobalInput.Q4_Length = fGeometry->GetQ4Length();
    StatsGlobalInput.SourceQ1Distance = fGeometry->GetSourceQ1Distance();
    StatsGlobalInput.Q1Q2Distance = fGeometry->GetQ1Q2Distance();
    StatsGlobalInput.Q2Q3Distance = fGeometry->GetQ2Q3Distance();
    StatsGlobalInput.Q3Q4Distance = fGeometry->GetQ3Q4Distance();
    StatsGlobalInput.B_Dipole_Map = fGeometry->GetBDipoleMap();
    StatsGlobalInput.B_Dipole = fGeometry->GetBDipole();
  
  }

  UpdateStatisticsGlobalInput(StatsGlobalInput);

  G4AutoLock lock(&fileMutex); // Verrouillage automatique du mutex
  
  f->cd();
  Tree_GlobalInput->Write();
  Tree_Input->Write();
  Tree_HorizontalColl->Write();
  Tree_VerticalColl->Write();
  Tree_BSYAG->Write();
  Tree_BSPECYAG->Write();
  f->Close();
  delete f;
  f=nullptr;

  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }

  //display run time and write to file Rntime.out
  time_t end = time(NULL);
  G4int elapsed = end-start;
  G4cout << "Run Completed in " << elapsed/3600
  << ":" << (elapsed%3600)/60 << ":"
  << ((elapsed%3600)%60) << G4endl;

  // Output the time in the file Runtime.out
  std::ofstream timeout;
  timeout.open("Runtime.out",std::ios::app);
  timeout << "Run " << aRun->GetRunID()
  << ": " <<elapsed/3600
  << ":" <<(elapsed%3600)/60
  << ":" <<((elapsed%3600)%60) << G4endl;
  timeout.close();


  G4cout<<"Leaving Run Action"<<G4endl;
}

//---------------------------------------------------------
//  For each event update the statistics in the Run tree
//---------------------------------------------------------

template <typename T>
void PlasmaAccelerationFacilitySimRunAction::UpdateStatistics(T& stats, const T& newStats, TTree* tree) {
    std::lock_guard<std::mutex> lock(fileMutex);  // Protéger avec un verrou
    stats = newStats;  // Assigner les nouvelles statistiques
    if (tree) {
        tree->Fill();  // Remplir l'arbre
    } else {
        G4cerr << "Error: Tree is nullptr" << G4endl;
    }
}

// Implémentations spécifiques utilisant le template
void PlasmaAccelerationFacilitySimRunAction::UpdateStatisticsGlobalInput(RunTallyGlobalInput aRunTallyGlobalInput) {
    UpdateStatistics(StatsGlobalInput, aRunTallyGlobalInput, Tree_GlobalInput);
}


void PlasmaAccelerationFacilitySimRunAction::UpdateStatisticsInput(RunTallyInput aRunTallyInput) {
    UpdateStatistics(StatsInput, aRunTallyInput, Tree_Input);
}

void PlasmaAccelerationFacilitySimRunAction::UpdateStatisticsHorizontalColl(RunTallyHorizontalColl aRunTallyHorizontalColl) {
    UpdateStatistics(StatsHorizontalColl, aRunTallyHorizontalColl, Tree_HorizontalColl);
}

void PlasmaAccelerationFacilitySimRunAction::UpdateStatisticsVerticalColl(RunTallyVerticalColl aRunTallyVerticalColl) {
    UpdateStatistics(StatsVerticalColl, aRunTallyVerticalColl, Tree_VerticalColl);
}

void PlasmaAccelerationFacilitySimRunAction::UpdateStatisticsBSYAG(RunTallyBSYAG aRunTallyBSYAG) {
    UpdateStatistics(StatsBSYAG, aRunTallyBSYAG, Tree_BSYAG);
}

void PlasmaAccelerationFacilitySimRunAction::UpdateStatisticsBSPECYAG(RunTallyBSPECYAG aRunTallyBSPECYAG) {
    UpdateStatistics(StatsBSPECYAG, aRunTallyBSPECYAG, Tree_BSPECYAG);
}