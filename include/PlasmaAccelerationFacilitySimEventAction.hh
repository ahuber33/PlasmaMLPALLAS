/// PlasmaAccelerationFacilitySimEventAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS


#ifndef PlasmaAccelerationFacilitySimEventAction_h
#define PlasmaAccelerationFacilitySimEventAction_h 1
#include "G4UserEventAction.hh"
#include "TTree.h"
#include "TBranch.h"


class G4Event;

// Structure pour gérer les collimateurs (avant et arrière) => EXEMPLE A SUIVRE POUR PLUS TARD
// struct RunTallyCollimator : public CoordExitData<float> {
//     std::vector<int> particleID;
//     std::vector<int> parentID;
//     std::vector<float> E_exit;
//     std::vector<float> px_exit, py_exit, pz_exit;
//     float E_start = 0.0f;
//     float E_dep = 0.0f;
//     float E_dep_e = 0.0f;
//     float E_dep_g = 0.0f;
//     std::vector<float> Energy_Brem_created;

//     void AddParticleID(int id) { particleID.push_back(id); }
//     size_t GetParticleIDSize() const { return particleID.size(); }
//     int GetParticleID(size_t a) const { return particleID.at(a); }
// };


struct RunTallyInput {
  float x = 0.0;
  float xoffset = 0.0;
  float xp = 0.0;
  float y = 0.0;
  float yoffset = 0.0;
  float yp = 0.0;
  float s = 0.0;
  float soffset = 0.0;
  float p = 0.0;
  float delta = 0.0;
  float energy = 0.0;
  int Nevent = 0.0;
};


struct RunTallyHorizontalColl {
  std::vector<int> parentID;
  std::vector<int> particleID;  
  std::vector<float> energy;
  float Edep;
  bool flag_particle;
};


struct RunTallyVerticalColl {
  std::vector<int> parentID;
  std::vector<int> particleID;
  std::vector<float> energy;
  bool flag_particle;
  float Edep;
};


struct RunTallyBSYAG {
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<int> parentID;
  std::vector<int> particleID;
  std::vector<float> energy;
  float deposited_energy = 0.0;
  std::vector<float> total_deposited_energy;
  G4bool flag_BSYAG = false;
};



struct RunTallyBSPECYAG {
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<int> parentID;
  std::vector<int> particleID;
  std::vector<float> energy;
  float deposited_energy = 0.0;
  std::vector<float> total_deposited_energy;
  G4bool flag_BSPECYAG = false;
};


class PlasmaAccelerationFacilitySimEventAction : public G4UserEventAction
{
public:
  PlasmaAccelerationFacilitySimEventAction(const char*);
  ~PlasmaAccelerationFacilitySimEventAction();

public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);  

  //Functions for Input Tree
  void SetXStart(G4float d){StatsInput.x=d;}
  void SetXOffsetStart(G4float d){StatsInput.xoffset=d;}
  void SetXpStart(G4float d){StatsInput.xp=d;}
  void SetYStart(G4float d){StatsInput.y=d;}
  void SetYOffsetStart(G4float d){StatsInput.yoffset=d;}
  void SetYpStart(G4float d){StatsInput.yp=d;}
  void SetSStart(G4float d){StatsInput.s=d;}
  void SetSOffsetStart(G4float d){StatsInput.soffset=d;}
  void SetPStart(G4float d){StatsInput.p=d;}
  void SetDeltaStart(G4float d){StatsInput.delta=d;}
  void SetEnergyStart(G4float d){StatsInput.energy=d;}
  void AddNeventStart(){StatsInput.Nevent++;}
  int GetNeventStart(){return StatsInput.Nevent;}

  void ActiveFlagHorizontalColl(){StatsHorizontalColl.flag_particle =true;}
  void ResetFlagHorizontalColl(){StatsHorizontalColl.flag_particle =false;}
  G4bool ReturnFlagHorizontalColl(){return StatsHorizontalColl.flag_particle;}
  void AddEnergyHorizontalColl(G4float d){StatsHorizontalColl.energy.push_back(d);}
  int GetEnergyHorizontalCollSize(){return StatsHorizontalColl.energy.size();}
  float GetEnergyHorizontalColl(G4float a){return StatsHorizontalColl.energy.at(a);}
  void AddParentIDHorizontalColl(G4float d){StatsHorizontalColl.parentID.push_back(d);}
  int GetParentIDHorizontalCollSize(){return StatsHorizontalColl.parentID.size();}
  float GetParentIDHorizontalColl(G4float a){return StatsHorizontalColl.parentID.at(a);}
  void AddParticleIDHorizontalColl(G4int d){StatsHorizontalColl.particleID.push_back(d);}
  int GetParticleIDHorizontalCollSize(){return StatsHorizontalColl.particleID.size();}
  float GetParticleIDHorizontalColl(G4int a){return StatsHorizontalColl.particleID.at(a);}
  void AddEdepHorizontalColl(G4float d){StatsHorizontalColl.Edep +=d;}
  float GetEdepHorizontalColl(){return StatsHorizontalColl.Edep;}

  void ActiveFlagVerticalColl(){StatsVerticalColl.flag_particle =true;}
  void ResetFlagVerticalColl(){StatsVerticalColl.flag_particle =false;}
  G4bool ReturnFlagVerticalColl(){return StatsVerticalColl.flag_particle;}
  void AddEnergyVerticalColl(G4float d){StatsVerticalColl.energy.push_back(d);}
  int GetEnergyVerticalCollSize(){return StatsVerticalColl.energy.size();}
  float GetEnergyVerticalColl(G4float a){return StatsVerticalColl.energy.at(a);}
  void AddParentIDVerticalColl(G4float d){StatsVerticalColl.parentID.push_back(d);}
  int GetParentIDVerticalCollSize(){return StatsVerticalColl.parentID.size();}
  float GetParentIDVerticalColl(G4float a){return StatsVerticalColl.parentID.at(a);}
  void AddParticleIDVerticalColl(G4int d){StatsVerticalColl.particleID.push_back(d);}
  int GetParticleIDVerticalCollSize(){return StatsVerticalColl.particleID.size();}
  float GetParticleIDVerticalColl(G4int a){return StatsVerticalColl.particleID.at(a);}
  void AddEdepVerticalColl(G4float d){StatsVerticalColl.Edep +=d;}
  float GetEdepVerticalColl(){return StatsVerticalColl.Edep;}

  void AddXExitBSYAG(G4float d){StatsBSYAG.x_exit.push_back(d);}
  int GetXExitBSYAGSize(){return StatsBSYAG.x_exit.size();}
  float GetXExitBSYAG(G4float a){return StatsBSYAG.x_exit.at(a);}
  void AddYExitBSYAG(G4float d){StatsBSYAG.y_exit.push_back(d);}
  int GetYExitBSYAGSize(){return StatsBSYAG.y_exit.size();}
  float GetYExitBSYAG(G4float a){return StatsBSYAG.y_exit.at(a);}
  void AddZExitBSYAG(G4float d){StatsBSYAG.z_exit.push_back(d);}
  int GetZExitBSYAGSize(){return StatsBSYAG.z_exit.size();}
  float GetZExitBSYAG(G4float a){return StatsBSYAG.z_exit.at(a);}
  void ActiveFlagBSYAG(){StatsBSYAG.flag_BSYAG =true;}
  void ResetFlagBSYAG(){StatsBSYAG.flag_BSYAG =false;}
  G4bool ReturnFlagBSYAG(){return StatsBSYAG.flag_BSYAG;}
  void AddEnergyBSYAG(G4float d){StatsBSYAG.energy.push_back(d);}
  int GetEnergyBSYAGSize(){return StatsBSYAG.energy.size();}
  float GetEnergyBSYAG(G4float a){return StatsBSYAG.energy.at(a);}
  void AddDepositedEnergyBSYAG(G4float d){StatsBSYAG.deposited_energy+=d;}
  void ResetDepositedEnergyBSYAG(){StatsBSYAG.deposited_energy=0;}
  float GetDepositedEnergyBSYAG(){return StatsBSYAG.deposited_energy;}
  void AddTotalDepositedEnergyBSYAG(G4float d){StatsBSYAG.total_deposited_energy.push_back(d);}
  int GetTotalDepositedEnergyBSYAGSize(){return StatsBSYAG.total_deposited_energy.size();}
  float GetTotalDepositedEnergyBSYAG(G4float a){return StatsBSYAG.total_deposited_energy.at(a);}
  void AddParentIDBSYAG(G4float d){StatsBSYAG.parentID.push_back(d);}
  int GetParentIDBSYAGSize(){return StatsBSYAG.parentID.size();}
  float GetParentIDBSYAG(G4float a){return StatsBSYAG.parentID.at(a);}
  void AddParticleIDBSYAG(G4int d){StatsBSYAG.particleID.push_back(d);}
  int GetParticleIDBSYAGSize(){return StatsBSYAG.particleID.size();}
  float GetParticleIDBSYAG(G4int a){return StatsBSYAG.particleID.at(a);}


  void AddXExitBSPECYAG(G4float d){StatsBSPECYAG.x_exit.push_back(d);}
  int GetXExitBSPECYAGSize(){return StatsBSPECYAG.x_exit.size();}
  float GetXExitBSPECYAG(G4float a){return StatsBSPECYAG.x_exit.at(a);}
  void AddYExitBSPECYAG(G4float d){StatsBSPECYAG.y_exit.push_back(d);}
  int GetYExitBSPECYAGSize(){return StatsBSPECYAG.y_exit.size();}
  float GetYExitBSPECYAG(G4float a){return StatsBSPECYAG.y_exit.at(a);}
  void AddZExitBSPECYAG(G4float d){StatsBSPECYAG.z_exit.push_back(d);}
  int GetZExitBSPECYAGSize(){return StatsBSPECYAG.z_exit.size();}
  float GetZExitBSPECYAG(G4float a){return StatsBSPECYAG.z_exit.at(a);}
  void ActiveFlagBSPECYAG(){StatsBSPECYAG.flag_BSPECYAG =true;}
  void ResetFlagBSPECYAG(){StatsBSPECYAG.flag_BSPECYAG =false;}
  G4bool ReturnFlagBSPECYAG(){return StatsBSPECYAG.flag_BSPECYAG;}
  void AddEnergyBSPECYAG(G4float d){StatsBSPECYAG.energy.push_back(d);}
  int GetEnergyBSPECYAGSize(){return StatsBSPECYAG.energy.size();}
  float GetEnergyBSPECYAG(G4float a){return StatsBSPECYAG.energy.at(a);}
  void AddDepositedEnergyBSPECYAG(G4float d){StatsBSPECYAG.deposited_energy+=d;}
  void ResetDepositedEnergyBSPECYAG(){StatsBSPECYAG.deposited_energy=0;}
  float GetDepositedEnergyBSPECYAG(){return StatsBSPECYAG.deposited_energy;}
  void AddTotalDepositedEnergyBSPECYAG(G4float d){StatsBSPECYAG.total_deposited_energy.push_back(d);}
  int GetTotalDepositedEnergyBSPECYAGSize(){return StatsBSPECYAG.total_deposited_energy.size();}
  float GetTotalDepositedEnergyBSPECYAG(G4float a){return StatsBSPECYAG.total_deposited_energy.at(a);}
  void AddParentIDBSPECYAG(G4float d){StatsBSPECYAG.parentID.push_back(d);}
  int GetParentIDBSPECYAGSize(){return StatsBSPECYAG.parentID.size();}
  float GetParentIDBSPECYAG(G4float a){return StatsBSPECYAG.parentID.at(a);}
  void AddParticleIDBSPECYAG(G4int d){StatsBSPECYAG.particleID.push_back(d);}
  int GetParticleIDBSPECYAGSize(){return StatsBSPECYAG.particleID.size();}
  float GetParticleIDBSPECYAG(G4int a){return StatsBSPECYAG.particleID.at(a);}


  // EXEMPLE A SUIVRE POUR PLUS TARD AFIN D OPTIMISER LE CODE !!!!
  // void AddFrontCollimatorData(int particleID, float E_exit, float x, float y, float z) {
  //       StatsFrontCollimator.AddParticleID(particleID);
  //       StatsFrontCollimator.E_exit.push_back(E_exit);
  //       StatsFrontCollimator.AddXExit(x);
  //       StatsFrontCollimator.AddYExit(y);
  //       StatsFrontCollimator.AddZExit(z);
  //   }

private:

  TTree *EventTree;
  TBranch *EventBranch;
  RunTallyInput StatsInput;
  RunTallyHorizontalColl StatsHorizontalColl;
  RunTallyVerticalColl StatsVerticalColl;
  RunTallyBSYAG StatsBSYAG;
  RunTallyBSPECYAG StatsBSPECYAG;

  
  G4String suffixe;
  
};


#endif
