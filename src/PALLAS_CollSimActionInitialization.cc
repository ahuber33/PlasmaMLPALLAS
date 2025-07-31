/// PALLAS_CollSimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimActionInitialization.hh"
#include "PALLAS_CollSimPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimRunAction.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimGeometryConstruction.hh"
#include "G4MTRunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimActionInitialization::PALLAS_CollSimActionInitialization(const char *suff, size_t N, size_t Ncores, G4bool pMT, PALLAS_CollSimGeometryConstruction* geometry)
    : G4VUserActionInitialization(), 
      suffixe(suff),
      NEventsGenerated(N),
      numThreads(Ncores),
      flag_MT(pMT),
      fGeometry(geometry)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimActionInitialization::~PALLAS_CollSimActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimActionInitialization::BuildForMaster() const
{
    // Action pour le processus maître (uniquement utile en mode multithreading)
    SetUserAction(new PALLAS_CollSimRunAction(suffixe, flag_MT));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimActionInitialization::Build() const
{
    // Création et affectation des actions pour la simulation
    auto *generator = new PALLAS_CollSimPrimaryGeneratorAction(NEventsGenerated, numThreads, flag_MT);
    auto *runAction = new PALLAS_CollSimRunAction(suffixe, flag_MT);
    auto *eventAction = new PALLAS_CollSimEventAction(suffixe);

    runAction->SetPrimaryGenerator(generator);
    runAction->SetGeometry(fGeometry);
    
    // Assigner les actions utilisateur
    SetUserAction(generator);
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new PALLAS_CollSimSteppingAction());
}
