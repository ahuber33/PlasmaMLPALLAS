/// PlasmaAccelerationFacilitySimVisManager.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PlasmaAccelerationFacilitySimVisManager_h
#define PlasmaAccelerationFacilitySimVisManager_h 1

#include "G4VisManager.hh"


class PlasmaAccelerationFacilitySimVisManager: public G4VisManager {

public:

  PlasmaAccelerationFacilitySimVisManager ();

private:

  void RegisterGraphicsSystems ();

};

#endif
