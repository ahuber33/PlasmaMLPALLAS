/// PALLAS_CollSimEventAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "PALLAS_CollSimRunAction.hh"

using namespace CLHEP;

PALLAS_CollSimEventAction::PALLAS_CollSimEventAction(const char *suff) : suffixe(suff) {}

PALLAS_CollSimEventAction::~PALLAS_CollSimEventAction() {}

// Initialize all counters and set up the event branches for
// filling histograms with ROOT
void PALLAS_CollSimEventAction::BeginOfEventAction(const G4Event *evt)
{
  // Input
  StatsInput = {};
  
  //Horizontal & Vertical
  StatsHorizontalColl = {};
  StatsVerticalColl = {};

  //BS & BSPEC YAG
  StatsBSYAG = {};
  StatsBSPECYAG = {};
  
}

void PALLAS_CollSimEventAction::EndOfEventAction(const G4Event *evt)
{

  PALLAS_CollSimRunAction *runac = (PALLAS_CollSimRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());

  if (StatsInput.energy>0) runac->UpdateStatisticsInput(StatsInput);
  if (!StatsBSYAG.energy.empty()) runac->UpdateStatisticsBSYAG(StatsBSYAG);
  if (!StatsBSPECYAG.energy.empty()) runac->UpdateStatisticsBSPECYAG(StatsBSPECYAG);
  runac->UpdateStatisticsHorizontalColl(StatsHorizontalColl);
  runac->UpdateStatisticsVerticalColl(StatsVerticalColl);

}
