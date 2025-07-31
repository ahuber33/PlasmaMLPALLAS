/// PALLAS_CollSimPhysics.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimPhysics.hh"

using namespace CLHEP;

// Taken from N06 and LXe examples in GEANT4

PALLAS_CollSimPhysics::PALLAS_CollSimPhysics()// : G4VModularPhysicsList()
{

  G4int verb = 1;
  SetVerboseLevel(verb);
  
  // mandatory for G4NuclideTable
  //
  const G4double meanLife = 1*nanosecond, halfLife = meanLife*std::log(2);
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(halfLife);
     
  // Hadron Elastic scattering
  //RegisterPhysics( new G4HadronElasticPhysicsXS(verb) );  
  RegisterPhysics( new G4HadronElasticPhysicsHP(verb) );
  
  // Hadron Inelastic Physics
  //RegisterPhysics( new G4HadronPhysicsFTFP_BERT(verb));
  ////RegisterPhysics( new G4HadronPhysicsQGSP_BIC(verb));
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
  ////RegisterPhysics( new G4HadronPhysicsQGSP_BIC_AllHP(verb));
  ////RegisterPhysics( new G4HadronInelasticQBBC(verb));
  ////RegisterPhysics( new G4HadronPhysicsINCLXX(verb));
  
  // Ion Elastic scattering
  //
  RegisterPhysics( new G4IonElasticPhysics(verb));
  
  // Ion Inelastic physics
  RegisterPhysics( new G4IonPhysicsXS(verb));
  ////RegisterPhysics( new G4IonINCLXXPhysics(verb));
  
  // stopping Particles
  RegisterPhysics( new G4StoppingPhysics(verb));
      
  // Gamma-Nuclear Physics
  //RegisterPhysics( new GammaNuclearPhysics("gamma"));
  ////RegisterPhysics( new GammaNuclearPhysicsLEND("gamma"));
  RegisterPhysics( new G4EmExtraPhysics());
      
  // EM physics
  //RegisterPhysics(new ElectromagneticPhysics());
  RegisterPhysics(new G4EmStandardPhysics_option3());
  
  // Decay
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  //RegisterPhysics(new RadioactiveDecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());  


}

PALLAS_CollSimPhysics::~PALLAS_CollSimPhysics()
{}


void PALLAS_CollSimPhysics::SetCuts()
{
  if (verboseLevel > 1)
  {
    G4cout << "opticalPhysics::SetCuts:";
  }
  SetCutsWithDefault();
}
