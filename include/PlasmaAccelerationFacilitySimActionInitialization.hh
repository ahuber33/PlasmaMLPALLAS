/// PlasmaAccelerationFacilitySimActionInitialization.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PlasmaAccelerationFacilitySimActionInitialization_h
#define PlasmaAccelerationFacilitySimActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"
#include "ParticleData.hh"
#include "PlasmaAccelerationFacilitySimGeometryConstruction.hh"

class PlasmaAccelerationFacilitySimGeometryConstruction;
class PlasmaAccelerationFacilitySimPrimaryGeneratorAction;

class PlasmaAccelerationFacilitySimActionInitialization : public G4VUserActionInitialization
{
public:
  PlasmaAccelerationFacilitySimActionInitialization(const char*, size_t, size_t, bool, PlasmaAccelerationFacilitySimGeometryConstruction*);
  virtual ~PlasmaAccelerationFacilitySimActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;
  char* NEvents;
  G4String suffixe;
  size_t NEventsGenerated;
  size_t numThreads;
  G4bool flag_MT=false;

private:
  PlasmaAccelerationFacilitySimGeometryConstruction* fGeometry;
};

#endif