// Geometry.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "Geometry.hh"

using namespace CLHEP;

// ***********************
// Constructor
// ***********************

Geometry::Geometry()
{
}
// ***********************
// Destructor
// ***********************
Geometry::~Geometry()
{
}


G4LogicalVolume* Geometry::GetGDMLVolume(const char* path, const char* VName, G4Material* material)
{
  Material = material;

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read(G4String(path), false);
  LogicalVolume = parser->GetVolume(G4String(VName));
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}

G4LogicalVolume *Geometry::GetQuadrupoleVolume(G4String name, G4double dx, G4double dy, G4double dz)
{

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  Box = new G4Box("Box",                                 // its name
                         dx/2 *mm, dy/2*mm, dz/2*mm); // its size

  LogicalVolume = new G4LogicalVolume(Box, Material, name, 0, 0, 0);

  return LogicalVolume;
}

// ################################################################################################################
// ################################################################################################################
// SECTION 4
// ################################################################################################################
// ################################################################################################################

G4LogicalVolume *Geometry::GetFakeDiagsChamber()
{
  // SimGeometry = new PALLAS_CollSimGeometry();
  // CollimatorInternalRadius = SimGeometry->GetCollimatorInternalRadius();
  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  G4RotationMatrix Rotate;
  Rotate.rotateX(90*deg);

  G4Tubs *Tubs1 = new G4Tubs("Tubs1",                                  // its name
                          156 * mm, 159 * mm, 220 * mm, 0, 360 * deg); // its size

  G4Tubs *Tubs2 = new G4Tubs("Tubs2",                                  // its name
                            0 * mm, 22 * mm, 200 * mm, 0, 360 * deg); // its size          

   G4SubtractionSolid* Tubs = new G4SubtractionSolid("Tubs", Tubs1, Tubs2, G4Transform3D(Rotate,G4ThreeVector(0,0,0.-150)));                                                                        

  LogicalVolume = new G4LogicalVolume(Tubs, Material, "FakeDiagsChamber", 0, 0, 0);

  // G4RunManager::GetRunManager()->GeometryHasBeenModified();

  return LogicalVolume;
}

// Alu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
// Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");
// Water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
// Laiton = G4NistManager::Instance()->FindOrBuildMaterial("G4_BRASS");
// Kapton = G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
// Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
// Cuivre = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
// Fer = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
// Tantale = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ta");
// Tungstene = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
// Polystyrene = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYSTYRENE");
