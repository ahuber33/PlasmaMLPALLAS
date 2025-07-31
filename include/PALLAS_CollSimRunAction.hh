/// PALLAS_CollSimRunAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimRunAction_h
#define PALLAS_CollSimRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "Randomize.hh"
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "TFile.h"
#include "G4AnalysisManager.hh"
#include <mutex>
#include <G4AutoLock.hh>
#include "G4RunManager.hh"
#include "PALLAS_CollSimPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimGeometryConstruction.hh"


  struct RunTallyGlobalInput {
  int Display_Cellule =0;
  int Display_LIF =0;
  int Display_Section1 =0;
  int Display_Section2 =0;
  int Display_Section3 =0;
  int Display_Section4 =0;
  int Display_Section4Dump =0;
  int Display_Collimators =0;
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
  int B_Dipole_Map =0;

};


class PALLAS_CollSimPrimaryGeneratorAction;


class PALLAS_CollSimRunAction : public G4UserRunAction
{

public:
  PALLAS_CollSimRunAction(const char*, G4bool);
  ~PALLAS_CollSimRunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  template<typename T>
  void UpdateStatistics(T& stats, const T& newStats, TTree* tree);
  //adds the photon fates from an event to the run tree
  void UpdateStatisticsGlobalInput(RunTallyGlobalInput);
  void UpdateStatisticsInput(RunTallyInput);
  void UpdateStatisticsHorizontalColl(RunTallyHorizontalColl);
  void UpdateStatisticsVerticalColl(RunTallyVerticalColl);
  void UpdateStatisticsBSYAG(RunTallyBSYAG);
  void UpdateStatisticsBSPECYAG(RunTallyBSPECYAG);

  void SetPrimaryGenerator(PALLAS_CollSimPrimaryGeneratorAction* gen);
  void SetGeometry(PALLAS_CollSimGeometryConstruction* geom);

private:
  G4String suffixe;
  G4String fileName;
  RunTallyGlobalInput StatsGlobalInput;
  RunTallyInput StatsInput;
  RunTallyHorizontalColl StatsHorizontalColl;
  RunTallyVerticalColl StatsVerticalColl;
  RunTallyBSYAG StatsBSYAG;
  RunTallyBSPECYAG StatsBSPECYAG;
  G4bool flag_MT;
  TFile *f=nullptr;
  TTree *Tree_GlobalInput=nullptr;
  TTree *Tree_Input=nullptr;
  TTree *Tree_HorizontalColl=nullptr;
  TTree *Tree_VerticalColl=nullptr;
  TTree *Tree_BSYAG=nullptr;
  TTree *Tree_BSPECYAG=nullptr;
  TBranch *RunBranch=nullptr;
  time_t start;
  static std::atomic<int> activeThreads;
  static G4Mutex fileMutex;


  protected:
  PALLAS_CollSimPrimaryGeneratorAction* fPrimaryGenerator = nullptr;
  PALLAS_CollSimGeometryConstruction* fGeometry = nullptr;
  

};

#endif
