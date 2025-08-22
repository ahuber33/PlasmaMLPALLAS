/**
 * @file Geometry.cc
 * @brief Implementation of the Geometry class for the PALLAS project.
 *
 * This file implements the methods defined in Geometry.hh to construct and
 * retrieve Geant4 geometry volumes used in the PALLAS simulation framework.
 * It provides functions to:
 *   - Load GDML-defined volumes and assign materials.
 *   - Create simplified quadrupole magnets as box volumes with vacuum material.
 *   - Construct diagnostic chambers using subtraction of cylindrical volumes.
 *
 * The Geometry class acts as a utility for building and managing
 * logical volumes in Geant4 simulations, facilitating the definition
 * of both realistic and simplified detector components.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @author Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */


#include "Geometry.hh"

// ***********************
// Constructor
// ***********************

/**
 * @brief Default constructor.
 *
 * Initializes internal pointers and sets up the geometry environment.
 */
Geometry::Geometry()
{
}

// ***********************
// Destructor
// ***********************

/**
 * @brief Destructor.
 *
 * Cleans up any allocated resources associated with the geometry.
 */
Geometry::~Geometry()
{
}

/**
 * @brief Load and retrieve a GDML-defined volume.
 *
 * This method uses a GDML parser to read a geometry description from a GDML file,
 * retrieve the specified volume, and assign it the provided material.
 *
 * @param path Path to the GDML file.
 * @param VName Name of the volume inside the GDML file.
 * @param material Material to assign to the retrieved logical volume.
 * @return Pointer to the created G4LogicalVolume.
 */
G4LogicalVolume* Geometry::GetGDMLVolume(const char* path, const char* VName, G4Material* material)
{
  Material = material;

  G4GDMLParser* parser = new G4GDMLParser();
  // Create tessellated volume of the requested component
  parser->Clear();
  parser->Read(G4String(path), false);
  LogicalVolume = parser->GetVolume(G4String(VName));
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}

/**
 * @brief Create a quadrupole magnet volume.
 *
 * Constructs a simple box volume with the given dimensions and assigns it a vacuum material.
 *
 * @param name Name of the quadrupole logical volume.
 * @param dx Size of the box along the X-axis [mm].
 * @param dy Size of the box along the Y-axis [mm].
 * @param dz Size of the box along the Z-axis [mm].
 * @return Pointer to the created G4LogicalVolume.
 */
G4LogicalVolume *Geometry::GetQuadrupoleVolume(G4String name, G4double dx, G4double dy, G4double dz)
{
  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  Box = new G4Box("Box", 
                  dx/2 *CLHEP::mm, 
                  dy/2*CLHEP::mm, 
                  dz/2*CLHEP::mm); 

  LogicalVolume = new G4LogicalVolume(Box, Material, name, 0, 0, 0);

  return LogicalVolume;
}

/**
 * @brief Create a simplified diagnostic chamber volume.
 *
 * This method constructs a cylindrical chamber by subtracting two tubular
 * volumes, representing the diagnostic chamber. The material used is aluminum.
 *
 * @return Pointer to the created G4LogicalVolume.
 */
G4LogicalVolume *Geometry::GetFakeDiagsChamber()
{
  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

  G4RotationMatrix Rotate;
  Rotate.rotateX(90*deg);

  // Outer tube
  G4Tubs *Tubs1 = new G4Tubs("Tubs1",
                             156 *CLHEP::mm, 159 *CLHEP::mm, 220 *CLHEP::mm,
                             0, 360 * deg);

  // Inner hole
  G4Tubs *Tubs2 = new G4Tubs("Tubs2",
                             0 *CLHEP::mm, 22 *CLHEP::mm, 200 *CLHEP::mm,
                             0, 360 * deg);          

  // Subtract inner tube from outer tube to form chamber
  G4SubtractionSolid* Tubs = new G4SubtractionSolid("Tubs", Tubs1, Tubs2, 
                                                    G4Transform3D(Rotate,G4ThreeVector(0,0,0.-150)));                                                                        

  LogicalVolume = new G4LogicalVolume(Tubs, Material, "FakeDiagsChamber", 0, 0, 0);

  return LogicalVolume;
}
