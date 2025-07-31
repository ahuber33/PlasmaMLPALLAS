/// PlasmaAccelerationFacilitySimGeometryConstruction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include <Geant4/G4Material.hh>
#include <Geant4/G4VUserDetectorConstruction.hh>
#ifndef PlasmaAccelerationFacilitySimGeometryConstruction_h
#define PlasmaAccelerationFacilitySimGeometryConstruction_h 1

#include "Geometry.hh"
#include "G4UserLimits.hh"
#include "G4GenericMessenger.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4ElectroMagneticField.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4UniformElectricField.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4ConstRK4.hh"
#include "G4NystromRK4.hh"
#include "G4HelixMixedStepper.hh"
#include "G4ExactHelixStepper.hh"
#include "G4InterpolationDriver.hh"
#include "G4IntegrationDriver.hh"
#include "G4VIntegrationDriver.hh"
#include "G4BFieldIntegrationDriver.hh"
#include <G4HelixHeum.hh>
// Newest steppers - from Release 10.3-beta (June 2013)
#include "G4BogackiShampine23.hh"
#include "G4BogackiShampine45.hh"
#include "G4DormandPrince745.hh"
#include "G4DormandPrinceRK56.hh"
#include "G4DormandPrinceRK78.hh"
#include "G4TsitourasRK45.hh"
#include "G4GeometryTolerance.hh"
#include "G4AutoLock.hh"
#include "PlasmaAccelerationFacilitySimMagneticField.hh"
#include "G4SystemOfUnits.hh"

class Geometry;
class G4FieldManager;

class PlasmaAccelerationFacilitySimMagneticField;

class PlasmaAccelerationFacilitySimGeometryConstruction : public G4VUserDetectorConstruction
{
public:
  PlasmaAccelerationFacilitySimGeometryConstruction();
  ~PlasmaAccelerationFacilitySimGeometryConstruction();

public:
  void SetLogicalVolumeColor(G4LogicalVolume* LogicalVolume, G4String color);
  void CreateWorldAndHolder();
  void ConstructQuadrupoleVolume();
  void ConstructCellulePart();
  void ConstructLIFPart();
  void ConstructSection1Part();
  void ConstructSection2Part();
  void ConstructSection3Part();
  void ConstructSection4Part();
  void ConstructCollimators();
  void ConstructSection4DumpPart();
  void ConstructSDandField() override;
  G4VPhysicalVolume *Construct() override;
  
  const float GetQ1Grad(){return Q1Gradient;}
  const float GetQ2Grad(){return Q2Gradient;}
  const float GetQ3Grad(){return Q3Gradient;}
  const float GetQ4Grad(){return Q4Gradient;}
  const float GetBDipole(){return ConstantDipoleBField*1e3;}
  const float GetQ1Length(){return Q1Length;}
  const float GetQ2Length(){return Q2Length;}
  const float GetQ3Length(){return Q3Length;}
  const float GetQ4Length(){return Q4Length;}
  const float GetSourceQ1Distance(){return SourceQ1Distance;}
  const float GetQ1Q2Distance(){return Q1Q2Distance;}
  const float GetQ2Q3Distance(){return Q2Q3Distance;}
  const float GetQ3Q4Distance(){return Q3Q4Distance;}
  const int GetBDipoleMap()
  {
    if(StatusMapBField == true) return 1;
    if(StatusMapBField == false) return 0;

    return -1;
  }
  const int GetStatusDisplayCelluleGeometry()
  {
    if(StatusDisplayCelluleGeometry == true) return 1;
    if(StatusDisplayCelluleGeometry == false) return 0;

    return -1;
  }
  const int GetStatusDisplayLIFGeometry()
  {
    if(StatusDisplayLIFGeometry == true) return 1;
    if(StatusDisplayLIFGeometry == false) return 0;

    return -1;
  }
  const int GetStatusDisplaySection1Geometry()
  {
    if(StatusDisplaySection1Geometry== true) return 1;
    if(StatusDisplaySection1Geometry == false) return 0;

    return -1;
  }
  const int GetStatusDisplaySection2Geometry()
  {
    if(StatusDisplaySection2Geometry== true) return 1;
    if(StatusDisplaySection2Geometry == false) return 0;

    return -1;
  }
  const int GetStatusDisplaySection3Geometry()
  {
    if(StatusDisplaySection3Geometry== true) return 1;
    if(StatusDisplaySection3Geometry == false) return 0;

    return -1;
  }
  const int GetStatusDisplaySection4Geometry()
  {
    if(StatusDisplaySection4Geometry== true) return 1;
    if(StatusDisplaySection4Geometry == false) return 0;

    return -1;
  }
  const int GetStatusDisplayCollimators()
  {
    if(StatusDisplayCollimators== true) return 1;
    if(StatusDisplayCollimators == false) return 0;

    return -1;
  }
  const int GetStatusDisplaySection4DumpGeometry()
  {
    if(StatusDisplaySection4DumpGeometry== true) return 1;
    if(StatusDisplaySection4DumpGeometry == false) return 0;

    return -1;
  }

private:
  Geometry *Geom;
  G4Material *Vacuum;
  G4Material *Material;
  
  G4GenericMessenger* fMessenger;
  G4GenericMessenger* gMessenger;
  G4GenericMessenger* bMessenger;

  G4bool StatusDisplayCelluleGeometry=false;
  G4bool StatusDisplayLIFGeometry=false;
  G4bool StatusDisplaySection1Geometry=false;
  G4bool StatusDisplaySection2Geometry=false;
  G4bool StatusDisplaySection3Geometry=false;
  G4bool StatusDisplaySection4Geometry=false;
  G4bool StatusDisplayCollimators=false;
  G4bool StatusDisplaySection4DumpGeometry=false;
  G4bool StatusRoundCollimator=false;
  G4bool StatusMapBField=false;

  // Dimension values
  G4double ConstantDipoleBField =0.4*tesla;
  G4double Q1Length = 0.1*m;
  G4double Q2Length = 0.1*m;
  G4double Q3Length = 0.2*m;
  G4double Q4Length = 0.1*m;
  G4double Q1Gradient =0.0;
  G4double Q2Gradient =0.0;
  G4double Q3Gradient =0.0;
  G4double Q4Gradient =0.0;
  G4double SourceQ1Distance =0.152*m;
  G4double Q1Q2Distance = 0.168*m;
  G4double Q2Q3Distance = 0.23*m;
  G4double Q3Q4Distance = 0.751*m;
  G4double SourceCollimatorDistance = 3000*mm;

  // Colors for visualizations
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

  // Logical Volumes
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


  // Physical volumes
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

  //Rotation Matrix
  G4RotationMatrix DontRotate;
  G4RotationMatrix Flip;
  G4RotationMatrix* RotationMatrix;

  static G4ThreadLocal PlasmaAccelerationFacilitySimMagneticField* fMagneticField;
  static G4ThreadLocal G4FieldManager* fFieldMgr;

};
#endif
