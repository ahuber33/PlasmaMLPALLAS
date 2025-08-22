#ifndef Geometry_h
#define Geometry_h

/**
 * @class Geometry
 * @brief Definition of the Geometry class for the PALLAS project.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This class provides methods to construct and retrieve different detector 
 * and beamline components (volumes, chambers, quadrupoles) used in the 
 * Geant4 simulation of the PALLAS experiment.
 */

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GDMLParser.hh"

class PlasmaMLPALLASGeometryConstruction;

/**
 * @class Geometry
 * @brief Provides methods to create, retrieve, and manage geometry volumes.
 *
 * The Geometry class is responsible for constructing and managing Geant4 
 * geometrical objects such as quadrupoles, diagnostic chambers, and 
 * GDML-defined volumes. It also provides cleanup functions for memory 
 * management.
 */
class Geometry
{
public:
  /**
   * @brief Constructor for Geometry class.
   *
   * Initializes internal pointers and sets up the geometry environment.
   */
  Geometry();

  /**
   * @brief Destructor for Geometry class.
   *
   * Cleans up allocated resources.
   */
  ~Geometry();

  /**
   * @brief Load and retrieve a GDML-defined volume.
   * @param fileName Path to the GDML file.
   * @param volumeName Name of the volume to retrieve.
   * @param material Material to assign to the volume.
   * @return Pointer to the created G4LogicalVolume.
   */
  G4LogicalVolume *GetGDMLVolume(const char* fileName, const char* volumeName, G4Material* material);

  /**
   * @brief Create a quadrupole magnet volume.
   * @param name Name of the quadrupole volume.
   * @param dx dx length.
   * @param dy dy length.
   * @param dz dz length.
   * @return Pointer to the created G4LogicalVolume.
   */
  G4LogicalVolume* GetQuadrupoleVolume(G4String name, G4double dx, G4double dy, G4double sz);

  /**
   * @brief Create a diagnostic chamber volume (simplified).
   * @return Pointer to the created G4LogicalVolume.
   */
  G4LogicalVolume *GetFakeDiagsChamber();

  /**
   * @brief Cleanup allocated geometry components.
   *
   * Frees memory for logical volumes and solid shapes.
   */
  void Cleanup();

private:
  Geometry *Geom;  ///< Pointer to another Geometry instance (if needed).
  PlasmaMLPALLASGeometryConstruction *SimGeometry; ///< Pointer to main simulation geometry.
  G4Material *Material; ///< Default material used in volumes.
  G4LogicalVolume* LogicalVolume; ///< Keeps track of allocated logical volume.
  G4Box* Box; ///< Keeps track of allocated box solid.
  G4Tubs* Tubs; ///< Keeps track of allocated tube solid.
  G4GDMLParser* parser; ///< Parser for GDML geometry files.
  G4VisAttributes *clear; ///< Visualization attributes (transparency).
};

#endif
