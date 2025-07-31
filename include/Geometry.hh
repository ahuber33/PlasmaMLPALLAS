/// Geometry.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

// This class is used to create a plastic Geometry.

#ifndef Geometry_h
#define Geometry_h

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include <G4Polycone.hh>
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"
#include "../PALLAS_CollSimGlobal.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

//#ifndef disable_gdml
#include "G4GDMLParser.hh"
//#endif

class PALLAS_CollSimMaterials;
class PALLAS_CollSimGeometryConstruction;
class G4GenericMessenger;
class Geometry

{
protected:

public:
  // constructor, builds from keys specified in buildfile
  Geometry();
  ~Geometry();

public:
  G4LogicalVolume *GetGDMLVolume(const char*, const char*, G4Material*);
  G4LogicalVolume* GetQuadrupoleVolume(G4String, G4double, G4double, G4double);
  //SECTION 4
  G4LogicalVolume *GetFakeDiagsChamber();
  
  void Cleanup();
  
private:
  Geometry *Geom;
  PALLAS_CollSimGeometryConstruction *SimGeometry;
  G4GenericMessenger* gMessenger;
  G4Material *Material;
  G4LogicalVolume* LogicalVolume;  // Garde la trace de l'allocation
  G4Box* Box;  // Garde la trace de l'allocation
  G4Tubs* Tubs;  // Garde la trace de l'allocation
  G4GDMLParser* parser;

  // Other
  G4VisAttributes *clear;
};
#endif
