/// PlasmaAccelerationFacilitySimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PlasmaAccelerationFacilitySimActionInitialization.hh"
#include "PlasmaAccelerationFacilitySimPrimaryGeneratorAction.hh"
#include "PlasmaAccelerationFacilitySimRunAction.hh"
#include "PlasmaAccelerationFacilitySimEventAction.hh"
#include "PlasmaAccelerationFacilitySimSteppingAction.hh"
#include "PlasmaAccelerationFacilitySimGeometryConstruction.hh"
#include "G4MTRunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PlasmaAccelerationFacilitySimActionInitialization::PlasmaAccelerationFacilitySimActionInitialization(const char *suff, size_t N, size_t Ncores, G4bool pMT, PlasmaAccelerationFacilitySimGeometryConstruction* geometry)
    : G4VUserActionInitialization(), 
      suffixe(suff),
      NEventsGenerated(N),
      numThreads(Ncores),
      flag_MT(pMT),
      fGeometry(geometry)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PlasmaAccelerationFacilitySimActionInitialization::~PlasmaAccelerationFacilitySimActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PlasmaAccelerationFacilitySimActionInitialization::BuildForMaster() const
{
    // Action pour le processus maître (uniquement utile en mode multithreading)
    SetUserAction(new PlasmaAccelerationFacilitySimRunAction(suffixe, flag_MT));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PlasmaAccelerationFacilitySimActionInitialization::Build() const
{
    // Création et affectation des actions pour la simulation
    auto *generator = new PlasmaAccelerationFacilitySimPrimaryGeneratorAction(NEventsGenerated, numThreads, flag_MT);
    auto *runAction = new PlasmaAccelerationFacilitySimRunAction(suffixe, flag_MT);
    auto *eventAction = new PlasmaAccelerationFacilitySimEventAction(suffixe);

    runAction->SetPrimaryGenerator(generator);
    runAction->SetGeometry(fGeometry);
    
    // Assigner les actions utilisateur
    SetUserAction(generator);
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new PlasmaAccelerationFacilitySimSteppingAction());
}
