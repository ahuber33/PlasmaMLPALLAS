#ifndef PlasmaMLPALLASGeometryConstruction_h
#define PlasmaMLPALLASGeometryConstruction_h 1

/**
 * @class PlasmaMLPALLASGeometryConstruction
 * @brief Defines the detector geometry and magnetic fields for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This class is responsible for:
 *  - Building the world and detector components.
 *  - Setting visualization attributes.
 *  - Defining quadrupoles, collimators, and experimental sections.
 *  - Handling field configurations (quadrupole gradients, dipole fields).
 *  - Providing user control over geometry display.
 */

#include "Geometry.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"
#include "PlasmaMLPALLASMagneticField.hh"

class Geometry;
class G4FieldManager;
class PlasmaMLPALLASGeometryMessenger;
class PlasmaMLPALLASMagneticField;


class PlasmaMLPALLASGeometryConstruction : public G4VUserDetectorConstruction
{
public:
  /** @brief Default constructor. */
  PlasmaMLPALLASGeometryConstruction();

  /** @brief Destructor. */
  ~PlasmaMLPALLASGeometryConstruction();

  /** @name Geometry Construction */
  ///@{
  /** @brief Set color for a given logical volume. */
  void SetLogicalVolumeColor(G4LogicalVolume* LogicalVolume, G4String color);

  /** @brief Create world and base holder. */
  void CreateWorldAndHolder();

  /** @brief Build quadrupole logical volumes. */
  void ConstructQuadrupolesVolume();

  /** @brief Place quadrupoles into the geometry. */
  void ConstructQuadrupoles();

  /** @brief Construct the "Cellule" part of the setup. */
  void ConstructCellulePart();

  /** @brief Construct the LIF (Laser Induced Fluorescence) section. */
  void ConstructLIFPart();

  /** @brief Construct Section 1. */
  void ConstructSection1Part();

  /** @brief Construct Section 2. */
  void ConstructSection2Part();

  /** @brief Construct Section 3. */
  void ConstructSection3Part();

  /** @brief Construct Section 4. */
  void ConstructSection4Part();

  /** @brief Construct all collimators. */
  void ConstructCollimators();

  /** @brief Construct dump section in Section 4. */
  void ConstructSection4DumpPart();

  /** @brief Build the full detailed PALLAS geometry. */
  void ConstructFullPALLASGeometry();

  /** @brief Build a simplified version of the PALLAS geometry. */
  void ConstructSimplifiedPALLASGeometry();

  /** @brief Construct sensitive detectors and magnetic fields. */
  void ConstructSDandField() override;

  /** @brief Construct method required by Geant4 kernel. */
  G4VPhysicalVolume *Construct() override;

  /** @brief Print geometry parameters. */
  void Print() const;
  ///@}

  /** @name Display Control */
  ///@{
  void SetStatusDisplayGeometry(G4int statusdisplay) {fStatusDisplayGeometry = statusdisplay;};
  void SetStatusDisplayCollimators(G4int statusdisplay) {fStatusDisplayCollimators = statusdisplay;};
  void SetStatusDisplayQuadrupoles(G4int statusdisplay) {fStatusDisplayQuadrupoles = statusdisplay;};

  const int GetStatusDisplayGeometry() const {return fStatusDisplayGeometry;}
  const int GetStatusDisplayCollimators() const {return fStatusDisplayCollimators;}
  const int GetStatusDisplayQuadrupoles() const {return fStatusDisplayQuadrupoles;}
  ///@}

  /** @name Geometry Parameters */
  ///@{
  void SetQ1Length(G4double QLength) {fQ1Length = QLength;};
  void SetQ2Length(G4double QLength) {fQ2Length = QLength;};
  void SetQ3Length(G4double QLength) {fQ3Length = QLength;};
  void SetQ4Length(G4double QLength) {fQ4Length = QLength;};
  void SetSourceQ1Distance(G4double distance) {fSourceQ1Distance = distance;};
  void SetQ1Q2Distance(G4double distance) {fQ1Q2Distance = distance;};
  void SetQ2Q3Distance(G4double distance) {fQ2Q3Distance = distance;};
  void SetQ3Q4Distance(G4double distance) {fQ3Q4Distance = distance;};
  void SetSourceCollimatorsDistance(G4double distance) {fSourceCollimatorsDistance = distance;};

  const float GetQ1Length() const {return fQ1Length;}
  const float GetQ2Length() const {return fQ2Length;}
  const float GetQ3Length() const {return fQ3Length;}
  const float GetQ4Length() const {return fQ4Length;}
  const float GetSourceQ1Distance() const {return fSourceQ1Distance;}
  const float GetQ1Q2Distance() const {return fQ1Q2Distance;}
  const float GetQ2Q3Distance() const {return fQ2Q3Distance;}
  const float GetQ3Q4Distance() const {return fQ3Q4Distance;}
  const float GetSourceCollimatorsDistance() const {return fSourceCollimatorsDistance;}
  ///@}

  /** @name Magnetic Field Parameters */
  ///@{
  void SetQ1Gradient(G4double QGrad) {fQ1Gradient = QGrad;};
  void SetQ2Gradient(G4double QGrad) {fQ2Gradient = QGrad;};
  void SetQ3Gradient(G4double QGrad) {fQ3Gradient = QGrad;};
  void SetQ4Gradient(G4double QGrad) {fQ4Gradient = QGrad;};
  void SetStatusMapBField(G4int status) {fStatusMapBField = status;};
  void SetConstantDipoleBField(G4double BField) {fConstantDipoleBField = BField;};

  const float GetQ1Gradient() const {return fQ1Gradient;}
  const float GetQ2Gradient() const {return fQ2Gradient;}
  const float GetQ3Gradient() const {return fQ3Gradient;}
  const float GetQ4Gradient() const {return fQ4Gradient;}
  const int GetStatusMapBField() const {return fStatusMapBField;}
  const float GetConstantDipoleBField() const {return fConstantDipoleBField;}
  ///@}

private:
  /** @brief Geometry handler. */
  Geometry *Geom;

  /** @brief Default materials. */
  G4Material *Vacuum;
  G4Material *Material;
  
  /** @brief UI messenger for runtime commands. */
  PlasmaMLPALLASGeometryMessenger* fGeometryMessenger = nullptr;

  /** @brief Display and field status flags. */
  G4int fStatusDisplayGeometry=0;
  G4int fStatusDisplayCollimators=0;
  G4int fStatusDisplayQuadrupoles=0;
  G4int fStatusMapBField=0;

  /** @brief Default magnetic and geometry values. */
  G4double fConstantDipoleBField =0.4*CLHEP::tesla;
  G4double fQ1Length = 0.1*CLHEP::m;
  G4double fQ2Length = 0.1*CLHEP::m;
  G4double fQ3Length = 0.2*CLHEP::m;
  G4double fQ4Length = 0.1*CLHEP::m;
  G4double fQ1Gradient =0.0*CLHEP::tesla/CLHEP::m;
  G4double fQ2Gradient =0.0*CLHEP::tesla/CLHEP::m;
  G4double fQ3Gradient =0.0*CLHEP::tesla/CLHEP::m;
  G4double fQ4Gradient =0.0*CLHEP::tesla/CLHEP::m;
  G4double fSourceQ1Distance =0.152*CLHEP::m;
  G4double fQ1Q2Distance = 0.168*CLHEP::m;
  G4double fQ2Q3Distance = 0.23*CLHEP::m;
  G4double fQ3Q4Distance = 0.751*CLHEP::m;
  G4double fSourceCollimatorsDistance = 3000*CLHEP::mm;

  /** @brief Visualization attributes (colors). */
  G4VisAttributes *invis;
  G4VisAttributes *white;
  G4VisAttributes *gray;
  G4VisAttributes *black;
  G4VisAttributes *red;
  G4VisAttributes *orange;
  G4VisAttributes *yellow;
  G4VisAttributes *green;
  G4VisAttributes *cyan;
  G4VisAttributes *blue;
  G4VisAttributes *magenta;

  /** @brief Logical volumes (geometry definitions). */
  G4LogicalVolume *LogicalWorld=nullptr;
  G4LogicalVolume *LogicalHolder=nullptr;
  G4LogicalVolume *LogicalFakeDiagsChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ3=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ4=nullptr;
  G4LogicalVolume *LogicalPALLAS_ASMRemovalChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_BreadboardRemovalChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChassisRemovalChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_TubeISO1=nullptr;
  G4LogicalVolume *LogicalPALLAS_TubeISO2=nullptr;
  G4LogicalVolume *LogicalPALLAS_ATH500_DN100=nullptr;
  G4LogicalVolume *LogicalPALLAS_BaseMarbre=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChambreISO=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ1=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ2=nullptr;
  G4LogicalVolume *LogicalPALLAS_Croix=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot1=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot2=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot3=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot4=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot5=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIF_IBX_DD=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIF_SQLT=nullptr;
  G4LogicalVolume *LogicalPALLAS_MarbreBreadboard1=nullptr;
  G4LogicalVolume *LogicalPALLAS_MarbreBreadboard2=nullptr;
  G4LogicalVolume *LogicalPALLAS_OptoMeK=nullptr;
  G4LogicalVolume *LogicalPALLAS_ASMPoutre=nullptr;
  G4LogicalVolume *LogicalPALLAS_StationYAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_BlindageBD=nullptr;
  G4LogicalVolume *LogicalPALLAS_BlindageCBD=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChambreDipole=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChassisDipoleYAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_DiagsChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_Dipole=nullptr;
  G4LogicalVolume *LogicalPALLAS_BS1YAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_BSPEC1YAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Tube=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Tube1=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Soufflet=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Croix=nullptr;
  G4LogicalVolume *LogicalPALLAS_Assemblage2Cellules=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_H1=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_H2=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Arbre_H=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Bride_H=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Palier_H=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Soufflet_H=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_V1=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_V2=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Arbre_V=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Bride_V=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Palier_V=nullptr;
  G4LogicalVolume *LogicalPALLAS_Collimator_Soufflet_V=nullptr;

  /** @brief Physical volumes (placements in space). */
  G4VPhysicalVolume *PhysicalWorld=nullptr;
  G4VPhysicalVolume *PhysicalHolder=nullptr;
  G4VPhysicalVolume *PhysicalFakeDiagsChamber=nullptr; 
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ3=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ4=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ASMRemovalChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BreadboardRemovalChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChassisRemovalChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_TubeISO1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_TubeISO2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ATH500_DN100=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BaseMarbre=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChambreISO=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Croix=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot3=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot4=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot5=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIF_IBX_DD=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIF_SQLT=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_MarbreBreadboard1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_MarbreBreadboard2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_OptoMeK=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ASMPoutre=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_StationYAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BlindageBD=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BlindageCBD=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChambreDipole=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChassisDipoleYAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_DiagsChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Dipole=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BS1YAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BSPEC1YAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Tube=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Tube1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Soufflet=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Croix=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Assemblage2Cellules=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_H1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_H2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Arbre_H=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Bride_H=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Palier_H=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Soufflet_H=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_V1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_V2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Arbre_V=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Bride_V=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Palier_V=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Collimator_Soufflet_V=nullptr;

  /** @brief Rotation matrices for component orientation. */
  G4RotationMatrix DontRotate;
  G4RotationMatrix Flip;
  G4RotationMatrix* RotationMatrix;

  /** @brief Thread-local magnetic field and field manager. */
  static G4ThreadLocal PlasmaMLPALLASMagneticField* fMagneticField;
  static G4ThreadLocal G4FieldManager* fFieldMgr;

};
#endif
