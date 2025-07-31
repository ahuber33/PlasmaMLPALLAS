/// PALLAS_CollSimActionInitialization.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimActionInitialization_h
#define PALLAS_CollSimActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"
#include "ParticleData.hh"
#include "PALLAS_CollSimGeometryConstruction.hh"

class PALLAS_CollSimGeometryConstruction;
class PALLAS_CollSimPrimaryGeneratorAction;

class PALLAS_CollSimActionInitialization : public G4VUserActionInitialization
{
public:
  PALLAS_CollSimActionInitialization(const char*, size_t, size_t, bool, PALLAS_CollSimGeometryConstruction*);
  virtual ~PALLAS_CollSimActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;
  char* NEvents;
  G4String suffixe;
  size_t NEventsGenerated;
  size_t numThreads;
  G4bool flag_MT=false;

private:
  PALLAS_CollSimGeometryConstruction* fGeometry;
};

#endif