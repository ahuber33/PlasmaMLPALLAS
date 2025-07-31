/// PlasmaAccelerationFacilitySimEventAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PlasmaAccelerationFacilitySimSteppingAction.hh"
#include "PlasmaAccelerationFacilitySimEventAction.hh"
#include "PlasmaAccelerationFacilitySimRunAction.hh"

using namespace CLHEP;

PlasmaAccelerationFacilitySimEventAction::PlasmaAccelerationFacilitySimEventAction(const char *suff) : suffixe(suff) {}

PlasmaAccelerationFacilitySimEventAction::~PlasmaAccelerationFacilitySimEventAction() {}

// Initialize all counters and set up the event branches for
// filling histograms with ROOT
void PlasmaAccelerationFacilitySimEventAction::BeginOfEventAction(const G4Event *evt)
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

void PlasmaAccelerationFacilitySimEventAction::EndOfEventAction(const G4Event *evt)
{

  PlasmaAccelerationFacilitySimRunAction *runac = (PlasmaAccelerationFacilitySimRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());

  if (StatsInput.energy>0) runac->UpdateStatisticsInput(StatsInput);
  if (!StatsBSYAG.energy.empty()) runac->UpdateStatisticsBSYAG(StatsBSYAG);
  if (!StatsBSPECYAG.energy.empty()) runac->UpdateStatisticsBSPECYAG(StatsBSPECYAG);
  runac->UpdateStatisticsHorizontalColl(StatsHorizontalColl);
  runac->UpdateStatisticsVerticalColl(StatsVerticalColl);

}
