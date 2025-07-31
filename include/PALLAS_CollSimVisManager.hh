/// PALLAS_CollSimVisManager.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimVisManager_h
#define PALLAS_CollSimVisManager_h 1

#include "G4VisManager.hh"


class PALLAS_CollSimVisManager: public G4VisManager {

public:

  PALLAS_CollSimVisManager ();

private:

  void RegisterGraphicsSystems ();

};

#endif
