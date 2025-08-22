/**
 * @file PlasmaMLPALLASGeometryConstruction.cc
 * @brief Implements the detector geometry and magnetic field setup for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This file contains the method definitions for the `PlasmaMLPALLASGeometryConstruction` class
 * declared in `PlasmaMLPALLASGeometryConstruction.hh`. It manages:
 *  - Construction of the full and simplified detector geometry
 *  - Loading of GDML models for realistic component shapes
 *  - Definition of visualization attributes for logical volumes
 *  - Creation of world and holder volumes
 *  - Construction of quadrupoles, collimators, laser-induced fluorescence (LIF) parts, and other experimental sections
 *  - Assignment of materials to all volumes
 *  - Configuration of the magnetic field and field manager
 *  - Optional multithread-safe handling of magnetic field instances
 *
 * The geometry construction workflow:
 *  - **Construct()**:
 *      - Cleans any existing geometry to prevent duplication
 *      - Defines rotation matrices for volume placement
 *      - Calls `CreateWorldAndHolder()` to create the world and main holder volume
 *      - Constructs quadrupole containers via `ConstructQuadrupolesVolume()`
 *      - Builds either the full PALLAS geometry (`ConstructFullPALLASGeometry()`) or a simplified version (`ConstructSimplifiedPALLASGeometry()`)
 *      - Optionally adds collimators and realistic quadrupoles if display flags are enabled
 *  - **ConstructFullPALLASGeometry()**:
 *      - Calls construction routines for all experimental sections (Cellule, LIF, Section1-4, Section4Dump)
 *  - **ConstructSimplifiedPALLASGeometry()**:
 *      - Constructs only Section4 to avoid track-stopping issues from CAD-based GDML files
 *  - **ConstructSDandField()**:
 *      - Initializes and configures the magnetic field
 *      - Sets dipole and quadrupole components, lengths, and drift distances
 *      - Creates and attaches a `G4FieldManager` with a Runge–Kutta stepper
 *
 * Thread safety is ensured via:
 *  - `G4Mutex fieldManagerMutex` for synchronized access to the magnetic field manager
 *  - `G4ThreadLocal` instances of `PlasmaMLPALLASMagneticField` and `G4FieldManager`
 *
 * Visualization colors for logical volumes:
 *  - "invis", "black", "white", "gray", "red", "orange", "yellow", "green", "cyan", "blue", "magenta"
 *
 * Notes:
 *  - Full PALLAS geometry is mostly used for visualization purposes.
 *  - Simplified geometry is recommended for production runs to avoid issues with CAD-based GDML files.
 *  - Magnetic field configuration directly impacts particle tracking and beam optics.
 */


#include "PlasmaMLPALLASGeometryConstruction.hh"
#include "PlasmaMLPALLASGeometryMessenger.hh"
#include <Geant4/G4Types.hh>

using namespace CLHEP;

//! Mutex to synchronize access to the magnetic field manager in multithreaded mode
G4Mutex fieldManagerMutex = G4MUTEX_INITIALIZER;

//! Thread-local magnetic field instance
G4ThreadLocal PlasmaMLPALLASMagneticField *PlasmaMLPALLASGeometryConstruction::fMagneticField = nullptr;

//! Thread-local field manager instance
G4ThreadLocal G4FieldManager *PlasmaMLPALLASGeometryConstruction::fFieldMgr = nullptr;

/**
 * @brief Constructor for PlasmaMLPALLASGeometryConstruction.
 *
 * Initializes the base geometry and attaches the geometry messenger
 * for interactive user control via macro commands.
 */
PlasmaMLPALLASGeometryConstruction::PlasmaMLPALLASGeometryConstruction()
    : G4VUserDetectorConstruction(), Geom()
{
    Geom = new Geometry();
    fGeometryMessenger = new PlasmaMLPALLASGeometryMessenger(this);
}

/**
 * @brief Destructor for PlasmaMLPALLASGeometryConstruction.
 */
PlasmaMLPALLASGeometryConstruction::~PlasmaMLPALLASGeometryConstruction() = default;

/**
 * @brief Print a summary of the current geometry setup.
 */
void PlasmaMLPALLASGeometryConstruction::Print() const
{
  G4cout << "\n------------------------------------------------------" << G4endl;
  G4cout << "-----------------------------------------------------" << G4endl;
}

/**
 * @brief Assign a visualization color to a logical volume.
 *
 * Predefined colors can be applied to logical volumes for debugging or visualization.
 *
 * @param LogicalVolume Pointer to the logical volume to colorize.
 * @param Color Name of the color (e.g. "red", "green", "gray").
 */
void PlasmaMLPALLASGeometryConstruction::SetLogicalVolumeColor(
    G4LogicalVolume *LogicalVolume, G4String Color)
{
    // ***********************
    // Visualization Colors
    // ***********************
    invis   = new G4VisAttributes(G4Colour(255 / 255., 255 / 255., 255 / 255.));
    invis->SetVisibility(false);

    black   = new G4VisAttributes(G4Colour(0, 0, 0, 0.9));
    black->SetForceSolid(true);
    black->SetVisibility(true);

    white   = new G4VisAttributes(G4Colour(1, 1, 1, 1.));
    white->SetForceSolid(true);
    white->SetVisibility(true);

    gray    = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
    gray->SetVisibility(true);

    red     = new G4VisAttributes(G4Colour(1, 0, 0, 0.3));
    red->SetForceSolid(true);
    red->SetVisibility(true);

    orange  = new G4VisAttributes(G4Colour(1, 0.5, 0, 0.3));
    orange->SetForceSolid(true);
    orange->SetVisibility(true);

    yellow  = new G4VisAttributes(G4Colour(1, 1, 0, 0.5));
    yellow->SetForceSolid(true);
    yellow->SetVisibility(true);

    green   = new G4VisAttributes(G4Colour(0, 1, 0, 0.3));
    green->SetForceSolid(true);
    green->SetVisibility(true);

    cyan    = new G4VisAttributes(G4Colour(0, 1, 1, 0.3));
    cyan->SetForceSolid(true);
    cyan->SetVisibility(true);

    blue    = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));
    blue->SetForceSolid(true);
    blue->SetVisibility(true);

    magenta = new G4VisAttributes(G4Colour(1, 0, 1, 0.3));
    magenta->SetVisibility(true);

    // Apply selected color
    if      (Color == "invis")   LogicalVolume->SetVisAttributes(invis);
    else if (Color == "black")   LogicalVolume->SetVisAttributes(black);
    else if (Color == "white")   LogicalVolume->SetVisAttributes(white);
    else if (Color == "gray")    LogicalVolume->SetVisAttributes(gray);
    else if (Color == "red")     LogicalVolume->SetVisAttributes(red);
    else if (Color == "orange")  LogicalVolume->SetVisAttributes(orange);
    else if (Color == "yellow")  LogicalVolume->SetVisAttributes(yellow);
    else if (Color == "green")   LogicalVolume->SetVisAttributes(green);
    else if (Color == "cyan")    LogicalVolume->SetVisAttributes(cyan);
    else if (Color == "blue")    LogicalVolume->SetVisAttributes(blue);
    else if (Color == "magenta") LogicalVolume->SetVisAttributes(magenta);
}

/**
 * @brief Create the world and holder volumes.
 *
 * Defines the simulation world as a large vacuum box and 
 * places a holder volume inside it for containing components.
 */
void PlasmaMLPALLASGeometryConstruction::CreateWorldAndHolder()
{
    Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    G4Box *SolidWorld = new G4Box("SolidWorld", 2.1 * m, 15.1 * m, 2.1 * m);
    LogicalWorld = new G4LogicalVolume(SolidWorld, Vacuum, "LogicalWorld");
    SetLogicalVolumeColor(LogicalWorld, "invis");

    G4Box *SolidHolder = new G4Box("SolidHolder", 2.05 * m, 15.05 * m, 2.05 * m);
    LogicalHolder = new G4LogicalVolume(SolidHolder, Vacuum, "LogicalHolder");
    SetLogicalVolumeColor(LogicalHolder, "invis");

    PhysicalWorld = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)),
                                      "World", LogicalWorld, NULL, false, 0);

    PhysicalHolder = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)),
                                       LogicalHolder, "Holder", LogicalWorld, false, 0);
}

/**
 * @brief Construct quadrupole volumes with simplified shapes.
 *
 * Creates approximate quadrupole representations using parameterized boxes
 * and places them in the beamline.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructQuadrupolesVolume()
{
    G4RotationMatrix Rotation;
    Rotation.rotateY(45 * deg);

    auto LogicalQ1Volume = Geom->GetQuadrupoleVolume("Q1", 34, fQ1Length, 34);
    auto LogicalQ2Volume = Geom->GetQuadrupoleVolume("Q2", 41, fQ2Length, 41);
    auto LogicalQ3Volume = Geom->GetQuadrupoleVolume("Q3", 49, fQ3Length, 49);
    auto LogicalQ4Volume = Geom->GetQuadrupoleVolume("Q4", 44, fQ4Length, 44);

    // Assign colors
    SetLogicalVolumeColor(LogicalQ1Volume, "gray");
    SetLogicalVolumeColor(LogicalQ2Volume, "gray");
    SetLogicalVolumeColor(LogicalQ3Volume, "gray");
    SetLogicalVolumeColor(LogicalQ4Volume, "gray");

    // Place volumes
    G4double Position_Q1 = fSourceQ1Distance + fQ1Length / 2;
    G4double Position_Q2 = Position_Q1 + fQ1Length / 2 + fQ1Q2Distance + fQ2Length / 2;
    G4double Position_Q3 = Position_Q2 + fQ2Length / 2 + fQ2Q3Distance + fQ3Length / 2;
    G4double Position_Q4 = Position_Q3 + fQ3Length / 2 + fQ3Q4Distance + fQ4Length / 2;

    new G4PVPlacement(G4Transform3D(Rotation, G4ThreeVector(0, Position_Q1, 0)),
                      LogicalQ1Volume, "Q1Volume", LogicalHolder, false, 0);

    new G4PVPlacement(G4Transform3D(Rotation, G4ThreeVector(0, Position_Q2, 0)),
                      LogicalQ2Volume, "Q2Volume", LogicalHolder, false, 0);

    new G4PVPlacement(G4Transform3D(Rotation, G4ThreeVector(0, Position_Q3, 0)),
                      LogicalQ3Volume, "Q3Volume", LogicalHolder, false, 0);

    new G4PVPlacement(G4Transform3D(Rotation, G4ThreeVector(0, Position_Q4, 0)),
                      LogicalQ4Volume, "Q4Volume", LogicalHolder, false, 0);
}

/**
 * @brief Construct realistic quadrupoles from GDML models.
 *
 * Loads quadrupole geometries from GDML files (CAD models) 
 * and places them in the beamline.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructQuadrupoles()
{
    auto Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    LogicalPALLAS_QuadrupoleQ1 = Geom->GetGDMLVolume("../gdml_models/S1/Quadrupole_Q1.gdml", "Quadrupole_Q1", Inox);
    LogicalPALLAS_QuadrupoleQ2 = Geom->GetGDMLVolume("../gdml_models/S1/Quadrupole_Q2.gdml", "Quadrupole_Q2", Inox);
    LogicalPALLAS_QuadrupoleQ3 = Geom->GetGDMLVolume("../gdml_models/S2/QuadrupoleQ3.gdml", "Quadrupole_Q3", Inox);
    LogicalPALLAS_QuadrupoleQ4 = Geom->GetGDMLVolume("../gdml_models/S2/QuadrupoleQ4.gdml", "Quadrupole_Q4", Inox);

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ1, "red");
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ2, "red");            
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ3, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ4, "cyan");

    // Place volumes
    G4double Position_Q1 = fSourceQ1Distance - 140; 
    G4double Position_Q2 = fSourceQ1Distance + fQ1Length + fQ1Q2Distance - 420;
    G4double Position_Q3 = fSourceQ1Distance + fQ1Length + fQ1Q2Distance + fQ2Length + fQ2Q3Distance - 720;
    G4double Position_Q4 = fSourceQ1Distance + fQ1Length + fQ1Q2Distance + fQ2Length + fQ2Q3Distance + fQ3Length + fQ3Q4Distance - 1570;

    new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, Position_Q1, 0)),
                      LogicalPALLAS_QuadrupoleQ1, "QuadrupoleQ1", LogicalHolder, false, 0);

    new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, Position_Q2, 0)),
                      LogicalPALLAS_QuadrupoleQ2, "QuadrupoleQ2", LogicalHolder, false, 0);

    new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, Position_Q3, 0)),
                      LogicalPALLAS_QuadrupoleQ3, "QuadrupoleQ3", LogicalHolder, false, 0);

    new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, Position_Q4, 0)),
                      LogicalPALLAS_QuadrupoleQ4, "QuadrupoleQ4", LogicalHolder, false, 0);
}

/**
 * @brief Construct the cell assembly component.
 *
 * Loads a GDML model representing two connected plasma cells
 * and places it inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructCellulePart()
{
    auto Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    LogicalPALLAS_Assemblage2Cellules =
        Geom->GetGDMLVolume("../gdml_models/Assemblage_2_Cellules.gdml", "Assemblage_2_Cellules", Inox);

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_Assemblage2Cellules, "magenta");

    // Place volumes
    PhysicalPALLAS_Assemblage2Cellules = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)),
        LogicalPALLAS_Assemblage2Cellules, "Assemblage2Cellules", LogicalHolder, false, 0);
}

/**
 * @brief Construct the LIF (Laser-Induced Fluorescence) diagnostic components.
 *
 * Loads GDML models for optical diagnostics (windows, cross, breadboards, etc.)
 * and places them inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructLIFPart()
{
    auto Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    LogicalPALLAS_Croix              = Geom->GetGDMLVolume("../gdml_models/LIF/Croix.gdml", "Croix", Inox);
    LogicalPALLAS_LIFHublot1         = Geom->GetGDMLVolume("../gdml_models/LIF/LIF_Hublot1.gdml", "LIF_Hublot1", Inox);
    LogicalPALLAS_LIFHublot2         = Geom->GetGDMLVolume("../gdml_models/LIF/LIF_Hublot2.gdml", "LIF_Hublot2", Inox);
    LogicalPALLAS_LIFHublot3         = Geom->GetGDMLVolume("../gdml_models/LIF/LIF_Hublot3.gdml", "LIF_Hublot3", Inox);
    LogicalPALLAS_LIFHublot4         = Geom->GetGDMLVolume("../gdml_models/LIF/LIF_Hublot4.gdml", "LIF_Hublot4", Inox);
    LogicalPALLAS_LIFHublot5         = Geom->GetGDMLVolume("../gdml_models/LIF/LIF_Hublot5.gdml", "LIF_Hublot5", Inox);
    LogicalPALLAS_LIF_IBX_DD         = Geom->GetGDMLVolume("../gdml_models/LIF/LIF_IBX_DD.gdml", "LIF_IBX_DD", Inox);
    LogicalPALLAS_LIF_SQLT           = Geom->GetGDMLVolume("../gdml_models/LIF/LIF_SQLT.gdml", "LIF_SQLT", Inox);
    LogicalPALLAS_MarbreBreadboard1  = Geom->GetGDMLVolume("../gdml_models/LIF/Marbre_Breadboard1.gdml", "Marbre_Breadboard1", Inox);
    LogicalPALLAS_MarbreBreadboard2  = Geom->GetGDMLVolume("../gdml_models/LIF/Marbre_Breadboard2.gdml", "Marbre_Breadboard2", Inox);
    LogicalPALLAS_OptoMeK            = Geom->GetGDMLVolume("../gdml_models/LIF/OptoMeK.gdml", "OptoMeK", Inox);

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_Croix, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot1, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot2, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot3, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot4, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot5, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIF_IBX_DD, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIF_SQLT, "green");
    SetLogicalVolumeColor(LogicalPALLAS_MarbreBreadboard1, "green");
    SetLogicalVolumeColor(LogicalPALLAS_MarbreBreadboard2, "green");
    SetLogicalVolumeColor(LogicalPALLAS_OptoMeK, "green");

    // Place volumes
    PhysicalPALLAS_Croix = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_Croix, "Croix", LogicalHolder, false, 0);

    PhysicalPALLAS_OptoMeK = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_OptoMeK, "OptoMeK", LogicalHolder, false, 0);

    PhysicalPALLAS_LIFHublot1 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_LIFHublot1, "LIFHublot1", LogicalHolder, false, 0);

    PhysicalPALLAS_LIFHublot2 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_LIFHublot2, "LIFHublot2", LogicalHolder, false, 0);

    PhysicalPALLAS_LIFHublot3 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_LIFHublot3, "LIFHublot3", LogicalHolder, false, 0);

    PhysicalPALLAS_LIFHublot4 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_LIFHublot4, "LIFHublot4", LogicalHolder, false, 0);

    PhysicalPALLAS_LIFHublot5 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_LIFHublot5, "LIFHublot5", LogicalHolder, false, 0);

    PhysicalPALLAS_LIF_IBX_DD = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_LIF_IBX_DD, "LIF_IBX_DD", LogicalHolder, false, 0);

    PhysicalPALLAS_LIF_SQLT = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_LIF_SQLT, "LIF_SQLT", LogicalHolder, false, 0);

    PhysicalPALLAS_MarbreBreadboard1 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_MarbreBreadboard1, "MarbreBreadboard1", LogicalHolder,
        false, 0);

    PhysicalPALLAS_MarbreBreadboard2 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_MarbreBreadboard2, "MarbreBreadboard2", LogicalHolder,
        false, 0);

}

/**
 * @brief Construct the Section1 part (ISO chamber between Q1 & Q2).
 *
 * Loads GDML models for section1 and places them inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructSection1Part()
{
    auto Inox =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    LogicalPALLAS_ATH500_DN100 = Geom->GetGDMLVolume(
        "../gdml_models/S1/ATH500_DN100.gdml", "ATH500_DN100", Inox);
    LogicalPALLAS_BaseMarbre = Geom->GetGDMLVolume(
        "../gdml_models/S1/Base_Marbre.gdml", "Base_Marbre", Inox);
    LogicalPALLAS_ChambreISO = Geom->GetGDMLVolume(
        "../gdml_models/S1/Chambre_ISO.gdml", "Chambre_ISO", Inox);

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_ATH500_DN100, "red");
    SetLogicalVolumeColor(LogicalPALLAS_ChambreISO, "red");
    SetLogicalVolumeColor(LogicalPALLAS_BaseMarbre, "red");

    // Place volumes
    PhysicalPALLAS_ATH500_DN100 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_ATH500_DN100, "ATH500_DN100", LogicalHolder, false, 0);

    PhysicalPALLAS_BaseMarbre = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_BaseMarbre, "BaseMarbre", LogicalHolder, false, 0);

    ///!!!! Problem with ISO Chamber GDML file !!!!!
    PhysicalPALLAS_ChambreISO = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_ChambreISO, "ChambreISO", LogicalHolder, false, 0);
    
}


/**
 * @brief Construct the Section2 part (ASMRemovalChamber, BreadboardRemovalChamber, ChassisReùovalChamber and ISOTubes).
 *
 * Loads GDML models for section2 and places them inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructSection2Part()
{
    auto Inox =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    LogicalPALLAS_ASMRemovalChamber =
        Geom->GetGDMLVolume("../gdml_models/S2/ASM_Removal_Chamber.gdml",
                            "ASMRemovalChamber", Inox);
    LogicalPALLAS_BreadboardRemovalChamber = Geom->GetGDMLVolume(
        "../gdml_models/S2/Assemblage_Breadboard_Thorlabs_Removal_Chamber.gdml",
        "Breadboard_Removal_Chamber", Inox);
    LogicalPALLAS_ChassisRemovalChamber = Geom->GetGDMLVolume(
        "../gdml_models/S2/Chassis_PALLAS_Removal_Chamber.gdml",
        "Chassis_PALLAS_Removal_Chamber", Inox);
    LogicalPALLAS_TubeISO1 = Geom->GetGDMLVolume(
        "../gdml_models/S2/Tube_ISO_1.gdml", "Tube_ISO_1", Inox);
    LogicalPALLAS_TubeISO2 = Geom->GetGDMLVolume(
        "../gdml_models/S2/Tube_ISO_2.gdml", "Tube_ISO_2", Inox);

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_ASMRemovalChamber, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_BreadboardRemovalChamber, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_ChassisRemovalChamber, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_TubeISO1, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_TubeISO2, "cyan");

    // Place volumes
    PhysicalPALLAS_ASMRemovalChamber = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_ASMRemovalChamber, "ASMRemovalChamber", LogicalHolder,
        false, 0);

    PhysicalPALLAS_BreadboardRemovalChamber = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_BreadboardRemovalChamber, "BreadboardRemovalChamber",
        LogicalHolder, false, 0);

    PhysicalPALLAS_ChassisRemovalChamber = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_ChassisRemovalChamber, "ChassisRemovalChamber",
        LogicalHolder, false, 0);

    PhysicalPALLAS_TubeISO1 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_TubeISO1, "TubeISO1", LogicalHolder, false, 0);

    PhysicalPALLAS_TubeISO2 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_TubeISO2, "TubeISO2", LogicalHolder, false, 0);
    
}


/**
 * @brief Construct the Section3 part (ASMPoutre & YAGStation).
 *
 * Loads GDML models for section3 and places them inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructSection3Part()
{
    auto Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    LogicalPALLAS_ASMPoutre = Geom->GetGDMLVolume(
        "../gdml_models/S3/ASM_Poutre.gdml", "ASM_Poutre", Inox);
    LogicalPALLAS_StationYAG = Geom->GetGDMLVolume(
        "../gdml_models/S3/Station_YAG.gdml", "Station_YAG", Inox);

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_ASMPoutre, "orange");
    SetLogicalVolumeColor(LogicalPALLAS_StationYAG, "orange");

    // Place volumes
    PhysicalPALLAS_ASMPoutre = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_ASMPoutre, "ASMPoutre", LogicalHolder, false, 0);

    PhysicalPALLAS_StationYAG = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, -300 * mm, 0 * mm)),
        LogicalPALLAS_StationYAG, "StationYAG", LogicalHolder, false, 0);

}


/**
 * @brief Construct the Section4 part (DipoleChamber, Dipole & 2 YAGs).
 *
 * Loads GDML models for section4 and places them inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructSection4Part()
{
    auto Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    auto Fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
    auto elementY = G4NistManager::Instance()->FindOrBuildElement("Y");
    auto elementO = G4NistManager::Instance()->FindOrBuildElement("O");
    auto elementAl = G4NistManager::Instance()->FindOrBuildElement("Al");

    auto YAG = new G4Material("YAG",
                              4.57 * g / cm3, // 1.053
                              3, kStateSolid, 273.15 * kelvin, 1.0 * atmosphere);

    YAG->AddElement(elementY, 3);
    YAG->AddElement(elementAl, 5);
    YAG->AddElement(elementO, 12);

    LogicalPALLAS_ChambreDipole = Geom->GetGDMLVolume(
        "../gdml_models/S4/Chambre_Dipole.gdml", "Chambre_Dipole", Al);
    LogicalPALLAS_Dipole =
        Geom->GetGDMLVolume("../gdml_models/S4/Dipole.gdml", "Dipole", Fe);
    LogicalPALLAS_BS1YAG =
        Geom->GetGDMLVolume("../gdml_models/S4/BS1_YAG.gdml", "BS1_YAG", YAG);
    LogicalPALLAS_BSPEC1YAG = Geom->GetGDMLVolume(
        "../gdml_models/S4/BSPEC1_YAG.gdml", "BSPEC1_YAG", YAG);
    LogicalPALLAS_DiagsChamber = Geom->GetGDMLVolume(
        "../gdml_models/S4/Diags_Chamber.gdml", "Diags_Chamber", Al);
    LogicalPALLAS_S4Tube =
        Geom->GetGDMLVolume("../gdml_models/S4/Tube.gdml", "Tube", Al);
    LogicalPALLAS_S4Tube1 =
        Geom->GetGDMLVolume("../gdml_models/S4/Tube1.gdml", "Tube1", Al);
    LogicalPALLAS_S4Soufflet =
        Geom->GetGDMLVolume("../gdml_models/S4/Soufflet.gdml", "Soufflet", Al);
    LogicalPALLAS_S4Croix =
        Geom->GetGDMLVolume("../gdml_models/S4/Croix.gdml", "Croix", Al);
    LogicalFakeDiagsChamber = Geom->GetFakeDiagsChamber();

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_ChambreDipole, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_Dipole, "red");
    SetLogicalVolumeColor(LogicalPALLAS_BSPEC1YAG, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_BS1YAG, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_DiagsChamber, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_S4Tube, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_S4Tube1, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_S4Soufflet, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_S4Croix, "yellow");
    SetLogicalVolumeColor(LogicalFakeDiagsChamber, "yellow");

    // Place volumes
    PhysicalPALLAS_ChambreDipole = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0. * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_ChambreDipole, "ChambreDipole", LogicalHolder, false, 0);

    PhysicalPALLAS_Dipole = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0. * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_Dipole, "Dipole", LogicalHolder, false, 0);

    PhysicalPALLAS_BS1YAG = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_BS1YAG, "BS1_YAG", LogicalHolder, false, 0);

    PhysicalPALLAS_BSPEC1YAG = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_BSPEC1YAG, "BSPEC1_YAG", LogicalHolder, false, 0);

    // PhysicalPALLAS_DiagsChamber = new G4PVPlacement(G4Transform3D(DontRotate,
    // G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
    //                                                 LogicalPALLAS_DiagsChamber,
    //                                                 "DiagsChamber",
    //                                                 LogicalHolder, false, 0);

    PhysicalFakeDiagsChamber = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 3960 * mm, 150 * mm)),
        LogicalFakeDiagsChamber, "FakeDiagsChamber", LogicalHolder, false, 0);

    PhysicalPALLAS_S4Tube = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_S4Tube, "S4Tube", LogicalHolder, false, 0);

    PhysicalPALLAS_S4Tube1 = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_S4Tube1, "S4Tube1", LogicalHolder, false, 0);

    PhysicalPALLAS_S4Soufflet = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_S4Soufflet, "S4Soufflet", LogicalHolder, false, 0);

    PhysicalPALLAS_S4Croix = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_S4Croix, "S4Croix", LogicalHolder, false, 0);
}


/**
 * @brief Construct the Collimators part.
 *
 * Loads GDML models for section1 and places them inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructCollimators()
{
    auto Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    // rotationMatrix->rotateX(90.0 * deg);
    rotationMatrix->rotateZ(90.0 * deg);

    if (fStatusDisplayCollimators)
    {
        LogicalPALLAS_Collimator_H1 = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Mors_H_1.gdml",
            "Collimator_H1", Inox);
        LogicalPALLAS_Collimator_H2 = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Mors_H_2.gdml",
            "Collimator_H2", Inox);
        LogicalPALLAS_Collimator_Arbre_H = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Arbre_H.gdml",
            "Collimator_Arbre_H", Inox);
        LogicalPALLAS_Collimator_Bride_H = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Bride_H.gdml",
            "Collimator_Bride_H", Inox);
        LogicalPALLAS_Collimator_Palier_H = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Palier_H.gdml",
            "Collimator_Palier_H", Inox);

        // LogicalPALLAS_Collimator_Soufflet_H =
        // Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Soufflet_H.gdml",
        // "Collimator_Soufflet_H", Inox);

        // Assign colors
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_H1, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_H2, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_Arbre_H, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_Bride_H, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_Palier_H, "yellow");
        // SetLogicalVolumeColor(LogicalPALLAS_Collimator_Soufflet_H, "yellow");

        LogicalPALLAS_Collimator_V1 = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Mors_V_1.gdml",
            "Collimator_V1", Inox);
        LogicalPALLAS_Collimator_V2 = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Mors_V_2.gdml",
            "Collimator_V2", Inox);
        LogicalPALLAS_Collimator_Arbre_V = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Arbre_V.gdml",
            "Collimator_Arbre_V", Inox);
        LogicalPALLAS_Collimator_Bride_V = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Bride_V.gdml",
            "Collimator_Bride_V", Inox);
        LogicalPALLAS_Collimator_Palier_V = Geom->GetGDMLVolume(
            "../gdml_models/Collimators/Collimator_Palier_V.gdml",
            "Collimator_Palier_V", Inox);
        // LogicalPALLAS_Collimator_Soufflet_V =
        // Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Soufflet_V.gdml",
        // "Collimator_Soufflet_V", Inox);
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_V1, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_V2, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_Arbre_V, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_Bride_V, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_Collimator_Palier_V, "blue");
        // SetLogicalVolumeColor(LogicalPALLAS_Collimator_Soufflet_V, "blue");

        // Place volumes
        // Translation of 122.670433mm due to CAD Files -> alignment with font of first collimator to correspond to the distance source/collimatr !!!
        PhysicalPALLAS_Collimator_H1 = new G4PVPlacement(
            rotationMatrix, G4ThreeVector(0 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_H1, "HorizontalCollimator", LogicalHolder,
            false, 0);

        PhysicalPALLAS_Collimator_H2 = new G4PVPlacement(
            rotationMatrix, G4ThreeVector(0 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_H2, "HorizontalCollimator", LogicalHolder,
            false, 0);

        PhysicalPALLAS_Collimator_Arbre_H = new G4PVPlacement(
            rotationMatrix, G4ThreeVector(0 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_Arbre_H, "Arbre_H", LogicalHolder, false, 0);

        PhysicalPALLAS_Collimator_Bride_H = new G4PVPlacement(
            rotationMatrix, G4ThreeVector(0 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_Bride_H, "Bride_H", LogicalHolder, false, 0);

        PhysicalPALLAS_Collimator_Palier_H = new G4PVPlacement(
            rotationMatrix, G4ThreeVector(0 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_Palier_H, "Palier_H", LogicalHolder, false,
            0);

        // PhysicalPALLAS_Collimator_Soufflet_H = new G4PVPlacement(
        //     rotationMatrix, G4ThreeVector(0 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
        //     LogicalPALLAS_Collimator_Soufflet_H, "Soufflet_H", LogicalHolder,
        //     false, 0);

        // Translation of 0.4mm due to CAD Files -> alignment with propagation axes !!!
        PhysicalPALLAS_Collimator_V1 = new G4PVPlacement(
            rotationMatrix,
            G4ThreeVector(-0.4 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_V1, "VerticalCollimator", LogicalHolder,
            false, 0);

        PhysicalPALLAS_Collimator_V2 = new G4PVPlacement(
            rotationMatrix,
            G4ThreeVector(-0.4 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_V2, "VerticalCollimator", LogicalHolder,
            false, 0);

        PhysicalPALLAS_Collimator_Arbre_V = new G4PVPlacement(
            rotationMatrix,
            G4ThreeVector(-0.4 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_Arbre_V, "Arbre_V", LogicalHolder, false, 0);

        PhysicalPALLAS_Collimator_Bride_V = new G4PVPlacement(
            rotationMatrix,
            G4ThreeVector(-0.4 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_Bride_V, "Bride_V", LogicalHolder, false, 0);

        PhysicalPALLAS_Collimator_Palier_V = new G4PVPlacement(
            rotationMatrix,
            G4ThreeVector(-0.4 * mm, fSourceCollimatorsDistance + 122.670433, 0 * mm),
            LogicalPALLAS_Collimator_Palier_V, "Palier_V", LogicalHolder, false,
            0);

        // PhysicalPALLAS_Collimator_Soufflet_V = new G4PVPlacement(
        //     rotationMatrix, G4ThreeVector(-0.552 * mm, fSourceCollimatorDistance + 122.670433, 0 *
        //     mm), LogicalPALLAS_Collimator_Soufflet_V, "Soufflet_V",
        //     LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}


/**
 * @brief Construct the Section4Dump part (Shieldings, chassis & DiagsChamber).
 *
 * Loads GDML models for section4 dump and places them inside the holder volume.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructSection4DumpPart()
{
    auto Pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
    auto Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    LogicalPALLAS_BlindageBD = Geom->GetGDMLVolume(
        "../gdml_models/S4/Blindage_BD.gdml", "Blindage_BD", Pb);
    LogicalPALLAS_BlindageCBD = Geom->GetGDMLVolume(
        "../gdml_models/S4/Blindage_CBD.gdml", "Blindage_CBD", Pb);
    LogicalPALLAS_ChassisDipoleYAG = Geom->GetGDMLVolume(
        "../gdml_models/S4/Chassis_Dipole_YAG.gdml", "Chassis_Dipole_YAG", Al);

    // Assign colors
    SetLogicalVolumeColor(LogicalPALLAS_BlindageBD, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_BlindageCBD, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_ChassisDipoleYAG, "blue");

    // Place volumes
    PhysicalPALLAS_BlindageBD = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_BlindageBD, "BlindageBD", LogicalHolder, false, 0);

    PhysicalPALLAS_BlindageCBD = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalPALLAS_BlindageCBD, "BlindageCBD", LogicalHolder, false, 0);

    // PhysicalPALLAS_ChassisDipoleYAG = new G4PVPlacement(
    //     G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
    //     LogicalPALLAS_ChassisDipoleYAG, "ChassisDipoleYAG", LogicalHolder,
    //     false, 0);
}

/**
 * @brief Construct sensitive detectors and define the magnetic field for the geometry.
 *
 * This method configures the magnetic field used in the simulation.
 * It sets dipole and quadrupole field components, assigns quadrupole lengths 
 * and drift distances, and attaches the field manager to the geometry's 
 * logical volume hierarchy.
 *
 * Steps performed:
 * - Create and configure a custom magnetic field (`PlasmaMLPALLASMagneticField`).
 * - Set dipole field and map field status.
 * - Define quadrupole gradients, lengths, and drift distances.
 * - Initialize the `G4FieldManager` and attach a chord finder with a 
 *   Runge–Kutta 4th order stepper.
 * - Apply the magnetic field manager to all daughter logical volumes.
 *
 * @note The magnetic field configuration directly affects particle 
 *       transport and beam optics in the Geant4 simulation.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructSDandField()
{
    // --- Magnetic field configuration ---------------------------------------
    fMagneticField = new PlasmaMLPALLASMagneticField();

    /// Set constant dipole magnetic field
    fMagneticField->SetDipoleField(fConstantDipoleBField);

    /// Enable or disable field map usage
    fMagneticField->SetMapBFieldStatus(fStatusMapBField);

    /// Configure quadrupole gradients (Tesla = 0.001 * MV * ns / mm²)
    fMagneticField->SetGradient(0, fQ1Gradient);
    fMagneticField->SetGradient(1, fQ2Gradient);
    fMagneticField->SetGradient(2, fQ3Gradient);
    fMagneticField->SetGradient(3, fQ4Gradient);

    /// Configure quadrupole lengths
    fMagneticField->SetQLength(0, GetQ1Length());
    fMagneticField->SetQLength(1, GetQ2Length());
    fMagneticField->SetQLength(2, GetQ3Length());
    fMagneticField->SetQLength(3, GetQ4Length());

    /// Configure quadrupole drift distances
    fMagneticField->SetQDrift(0, GetSourceQ1Distance());
    fMagneticField->SetQDrift(1, GetQ1Q2Distance());
    fMagneticField->SetQDrift(2, GetQ2Q3Distance());
    fMagneticField->SetQDrift(3, GetQ3Q4Distance());

    // --- Field manager setup -------------------------------------------------
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(fMagneticField);
    fFieldMgr->CreateChordFinder(fMagneticField);

    /// Create a chord finder with stepper precision of 1 µm
    G4ChordFinder *chordFinder = new G4ChordFinder(
        fMagneticField,
        1e-3 * mm,
        new G4ClassicalRK4(new G4Mag_UsualEqRhs(fMagneticField))
    );
    fFieldMgr->SetChordFinder(chordFinder);

    // --- Apply field manager to logical volume hierarchy --------------------
    G4bool forceToAllDaughters = true;
    LogicalHolder->SetFieldManager(fFieldMgr, forceToAllDaughters);
}

/**
 * @brief Construct the FULL PALLAS Geometry (mostly for VISUALIZATION !!!).
 *
 * Call of the previous construction function.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructFullPALLASGeometry()
{
    ConstructCellulePart();
    ConstructLIFPart();
    ConstructSection1Part();
    ConstructSection2Part();
    ConstructSection3Part();
    ConstructSection4Part();
    ConstructSection4DumpPart();    
}

/**
 * @brief Construct the Simplified PALLAS Geometry (mostly for PRODUCTION !!!).
 *
 * Call only Section4 because other parts can induce stuck Track due to GDML files reconstruction from CAD files.
 */
void PlasmaMLPALLASGeometryConstruction::ConstructSimplifiedPALLASGeometry()
{
    ConstructSection4Part();
}

/**
 * @brief Construct the full detector geometry for the simulation.
 *
 * This method initializes and builds all the physical volumes required
 * for the simulation. It cleans up any previously defined geometry,
 * applies common rotation matrices, and constructs the detector
 * components according to display flags.
 *
 * Steps performed:
 * - Clean existing geometry and volume stores to avoid duplication.
 * - Define common rotation matrices used for detector components.
 * - Create the world volume and geometry holder.
 * - Construct quadrupoles volume.
 * - Build either the full or simplified PALLAS geometry depending
 *   on configuration flags.
 * - Optionally construct collimators and quadrupoles if enabled.
 * - Return the fully initialized world volume.
 *
 * @return Pointer to the top-level physical volume (`PhysicalWorld`)
 *         containing the entire detector setup.
 */
G4VPhysicalVolume *PlasmaMLPALLASGeometryConstruction::Construct()
{
    // --- Cleanup of previous geometry ----------------------------------------
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    // --- Define common rotation matrices -------------------------------------
    DontRotate.rotateX(0.0 * deg);
    Flip.rotateZ(0 * deg);
    Flip.rotateX(90 * deg);
    Flip.rotateY(0 * deg);

    // #########################################################################
    //  DEFINE GEOMETRY VOLUMES
    // #########################################################################

    /// Create the world and main holder volume
    CreateWorldAndHolder();

    /// Construct quadrupoles container volume
    ConstructQuadrupolesVolume();

    /// Choose between full or simplified PALLAS geometry
    if(fStatusDisplayGeometry == 1) 
        ConstructFullPALLASGeometry();
    else
        ConstructSimplifiedPALLASGeometry();

    /// Optionally construct collimators
    if(fStatusDisplayCollimators == 1) 
        ConstructCollimators();

    /// Optionally construct quadrupoles
    if(fStatusDisplayQuadrupoles == 1) 
        ConstructQuadrupoles();

    G4cout << "END OF THE DETECTOR CONSTRUCTION" << G4endl;

    // --- Return the fully constructed world volume ---------------------------
    return PhysicalWorld;
}

