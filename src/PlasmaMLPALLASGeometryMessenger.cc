#include "PlasmaMLPALLASGeometryMessenger.hh"

/**
 * @file PlasmaMLPALLASGeometryMessenger.cc
 * @brief Implementation of the GeometryMessenger class for the PALLAS PlasmaMLP simulation.
 *
 * This file defines the PlasmaMLPALLASGeometryMessenger class, which inherits from G4UImessenger.
 * It provides an interface for the Geant4 UI to control geometry and field parameters 
 * for the PlasmaMLPALLAS simulation.
 *
 * Responsibilities include:
 *  - Creating UI directories and commands for geometry, display, and magnetic field controls.
 *  - Enabling/disabling visualization of geometry components (full geometry, quadrupoles, collimators).
 *  - Setting geometry parameters such as quadrupole lengths and distances between elements.
 *  - Setting magnetic field gradients and dipole field options.
 *  - Passing user-specified values to the PlasmaMLPALLASGeometryConstruction class.
 *
 * Each UI command is linked to a setter function in the geometry construction to update
 * the simulation configuration dynamically.
 *
 * The messenger also provides functions to:
 *  - Retrieve current command values for UI display.
 *  - Delete all UI commands upon destruction to free memory.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @author Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Constructor
 * @param aDetector Pointer to the PlasmaMLPALLAS geometry construction class
 *
 * This constructor initializes all UI commands and directories for geometry, display, and field control.
 */
PlasmaMLPALLASGeometryMessenger::PlasmaMLPALLASGeometryMessenger(PlasmaMLPALLASGeometryConstruction *aDetector)
    : G4UImessenger(), fGeometry(aDetector)
{
    // Create a main directory for all UI commands related to PlasmaMLPALLAS
    fExampleDir = new G4UIdirectory("/PlasmaMLPALLAS/");
    fExampleDir->SetGuidance("UI commands specific to this example");

    // Create sub-directory for display-related commands
    fDisplayDir = new G4UIdirectory("/PlasmaMLPALLAS/display/");
    fDisplayDir->SetGuidance("Display Geometry construction UI commands");

    // Create sub-directory for geometry-related commands
    fGeometryDir = new G4UIdirectory("/PlasmaMLPALLAS/geometry/");
    fGeometryDir->SetGuidance("Geometry construction UI commands");

    // Create sub-directory for magnetic field commands
    fFieldDir = new G4UIdirectory("/PlasmaMLPALLAS/field/");
    fFieldDir->SetGuidance("Field UI commands");

    // Command to print current geometry and field settings
    fPrintCmd = new G4UIcmdWithoutParameter("/PlasmaMLPALLAS/print", this);
    fPrintCmd->SetGuidance("Print current settings.");


    //=====================================
    // Display Commands
    //=====================================

     /**
     * @brief Command to enable/disable display of Full PALLAS geometry
     *
     * Parameter: StatusDisplayGeometry (0 or 1)
     * - 0: Disable Full PALLAS Geometry Display
     * - 1: Enable Full PALLAS Geometry Display
     */
    fGeometryStatusDisplayCmd = new G4UIcmdWithAnInteger("/PlasmaMLPALLAS/display/setStatusDisplayGeometry", this);
    fGeometryStatusDisplayCmd->SetGuidance("Enable or disable the feature (0/1)");
    fGeometryStatusDisplayCmd->SetParameterName("StatusDisplayGeometry", false);
    fGeometryStatusDisplayCmd->SetRange("StatusDisplayGeometry==0 || StatusDisplayGeometry==1");
    fGeometryStatusDisplayCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_GeomClosed);
    fGeometryStatusDisplayCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to enable/disable display of Quadrupoles geometries
     *
     * Parameter: StatusDisplayQuadrupolesGeometry (0 or 1)
     * - 0: Disable Quadrupoles Display
     * - 1: Enable Quadrupoles Display
     */
    fGeometryStatusDisplayQuadrupolesCmd = new G4UIcmdWithAnInteger("/PlasmaMLPALLAS/display/setStatusDisplayQuadrupoles", this);
    fGeometryStatusDisplayQuadrupolesCmd->SetGuidance("Enable or disable the feature (0/1)");
    fGeometryStatusDisplayQuadrupolesCmd->SetParameterName("StatusDisplayQuadrupoles", false);
    fGeometryStatusDisplayQuadrupolesCmd->SetRange("StatusDisplayQuadrupoles==0 || StatusDisplayQuadrupoles==1");
    fGeometryStatusDisplayQuadrupolesCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_GeomClosed);
    fGeometryStatusDisplayQuadrupolesCmd->SetToBeBroadcasted(false);


    /**
     * @brief Command to enable/disable display of Collimators geometries
     *
     * Parameter: StatusDisplayCollimatorsGeometry (0 or 1)
     * - 0: Disable Collimators Display
     * - 1: Enable Collimators Display
     */
    fGeometryStatusDisplayCollimatorsCmd = new G4UIcmdWithAnInteger("/PlasmaMLPALLAS/display/setStatusDisplayCollimators", this);
    fGeometryStatusDisplayCollimatorsCmd->SetGuidance("Enable or disable the feature (0/1)");
    fGeometryStatusDisplayCollimatorsCmd->SetParameterName("StatusDisplayCollimators", false);
    fGeometryStatusDisplayCollimatorsCmd->SetRange("StatusDisplayCollimators==0 || StatusDisplayCollimators==1");
    fGeometryStatusDisplayCollimatorsCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_GeomClosed);
    fGeometryStatusDisplayCollimatorsCmd->SetToBeBroadcasted(false);


    //=====================================
    // Geometry Commands
    //=====================================

    /**
     * @brief Command to set the Q1 Length.
     *
     * Parameter: Q1Length (double) unit
     */
    fGeometryQ1LengthCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setQ1Length", this);
    fGeometryQ1LengthCmd->SetGuidance("Set Q1 Length");
    fGeometryQ1LengthCmd->SetParameterName("Q1Length", false);
    fGeometryQ1LengthCmd->SetRange("Q1Length>0.");
    fGeometryQ1LengthCmd->SetUnitCategory("Length");
    fGeometryQ1LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ1LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q2 Length.
     *
     * Parameter: Q2Length (double) unit
     */
    fGeometryQ2LengthCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setQ2Length", this);
    fGeometryQ2LengthCmd->SetGuidance("Set Q2 Length");
    fGeometryQ2LengthCmd->SetParameterName("Q2Length", false);
    fGeometryQ2LengthCmd->SetRange("Q2Length>0.");
    fGeometryQ2LengthCmd->SetUnitCategory("Length");
    fGeometryQ2LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ2LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q3 Length.
     *
     * Parameter: Q3Length (double) unit
     */
    fGeometryQ3LengthCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setQ3Length", this);
    fGeometryQ3LengthCmd->SetGuidance("Set Q3 Length");
    fGeometryQ3LengthCmd->SetParameterName("Q3Length", false);
    fGeometryQ3LengthCmd->SetRange("Q3Length>0.");
    fGeometryQ3LengthCmd->SetUnitCategory("Length");
    fGeometryQ3LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ3LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q4 Length.
     *
     * Parameter: Q4Length (double) unit
     */
    fGeometryQ4LengthCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setQ4Length", this);
    fGeometryQ4LengthCmd->SetGuidance("Set Q4 Length");
    fGeometryQ4LengthCmd->SetParameterName("Q4Length", false);
    fGeometryQ4LengthCmd->SetRange("Q4Length>0.");
    fGeometryQ4LengthCmd->SetUnitCategory("Length");
    fGeometryQ4LengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ4LengthCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Source - Q1 Distance.
     *
     * Parameter: SourceQ1Distance (double) unit
     */
    fGeometrySourceQ1DistanceCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setSourceQ1Distance", this);
    fGeometrySourceQ1DistanceCmd->SetGuidance("Set Source -- Q1 Distance");
    fGeometrySourceQ1DistanceCmd->SetParameterName("SourceQ1Distance", false);
    fGeometrySourceQ1DistanceCmd->SetRange("SourceQ1Distance>0.");
    fGeometrySourceQ1DistanceCmd->SetUnitCategory("Length");
    fGeometrySourceQ1DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometrySourceQ1DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q1 - Q2 Distance.
     *
     * Parameter: Q1Q2Distance (double) unit
     */
    fGeometryQ1Q2DistanceCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setQ1Q2Distance", this);
    fGeometryQ1Q2DistanceCmd->SetGuidance("Set Q1 -- Q2 Distance");
    fGeometryQ1Q2DistanceCmd->SetParameterName("Q1Q2Distance", false);
    fGeometryQ1Q2DistanceCmd->SetRange("Q1Q2Distance>0.");
    fGeometryQ1Q2DistanceCmd->SetUnitCategory("Length");
    fGeometryQ1Q2DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ1Q2DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q2 - Q3 Distance.
     *
     * Parameter: Q2Q3Distance (double) unit
     */
    fGeometryQ2Q3DistanceCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setQ2Q3Distance", this);
    fGeometryQ2Q3DistanceCmd->SetGuidance("Set Q2 -- Q3 Distance");
    fGeometryQ2Q3DistanceCmd->SetParameterName("Q2Q3Distance", false);
    fGeometryQ2Q3DistanceCmd->SetRange("Q2Q3Distance>0.");
    fGeometryQ2Q3DistanceCmd->SetUnitCategory("Length");
    fGeometryQ2Q3DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ2Q3DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q3 - Q4 Distance.
     *
     * Parameter: Q3Q4Distance (double) unit
     */
    fGeometryQ3Q4DistanceCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setQ3Q4Distance", this);
    fGeometryQ3Q4DistanceCmd->SetGuidance("Set Q3 -- Q4 Distance");
    fGeometryQ3Q4DistanceCmd->SetParameterName("Q3Q4Distance", false);
    fGeometryQ3Q4DistanceCmd->SetRange("Q3Q4Distance>0.");
    fGeometryQ3Q4DistanceCmd->SetUnitCategory("Length");
    fGeometryQ3Q4DistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometryQ3Q4DistanceCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Source - Collimators Distance.
     *
     * Parameter: SourceCollimatorsDistance (double) unit
     */
    fGeometrySourceCollimatorsDistanceCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/geometry/setSourceCollimatorsDistance", this);
    fGeometrySourceCollimatorsDistanceCmd->SetGuidance("Set Source -- Collimators Distance");
    fGeometrySourceCollimatorsDistanceCmd->SetParameterName("SourceCollimatorsDistance", false);
    fGeometrySourceCollimatorsDistanceCmd->SetRange("SourceCollimatorsDistance>0.");
    fGeometrySourceCollimatorsDistanceCmd->SetUnitCategory("Length");
    fGeometrySourceCollimatorsDistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fGeometrySourceCollimatorsDistanceCmd->SetToBeBroadcasted(false);


    //=====================================
    // Field Commands
    //=====================================

    new G4UnitDefinition("tesla/m", "T/m", "MagneticGradient", tesla/m);

    /**
     * @brief Command to set the Q1Gradient .
     *
     * Parameter: Q1Gradient (double) in tesla/m
     */
    fFieldQ1GradientCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/field/setQ1Gradient", this);
    fFieldQ1GradientCmd->SetGuidance("Set Q1 Gradient");
    fFieldQ1GradientCmd->SetParameterName("Q1Gradient", false);
    //fFieldQ1GradientCmd->SetRange("Q1Gradient>-100 && Q1Gradient < 100");
    fFieldQ1GradientCmd->SetUnitCategory("MagneticGradient");
    fFieldQ1GradientCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFieldQ1GradientCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q2Gradient .
     *
     * Parameter: Q2Gradient (double) in tesla/m
     */
    fFieldQ2GradientCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/field/setQ2Gradient", this);
    fFieldQ2GradientCmd->SetGuidance("Set Q2 Gradient");
    fFieldQ2GradientCmd->SetParameterName("Q2Gradient", false);
    //fFieldQ2GradientCmd->SetRange("Q2Gradient>-100 && Q2Gradient < 100");
    fFieldQ2GradientCmd->SetUnitCategory("MagneticGradient");
    fFieldQ2GradientCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFieldQ2GradientCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q3Gradient .
     *
     * Parameter: Q3Gradient (double) in tesla/m
     */
    fFieldQ3GradientCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/field/setQ3Gradient", this);
    fFieldQ3GradientCmd->SetGuidance("Set Q3 Gradient");
    fFieldQ3GradientCmd->SetParameterName("Q3Gradient", false);
    //fFieldQ3GradientCmd->SetRange("Q3Gradient>-100 && Q3Gradient < 100");
    fFieldQ3GradientCmd->SetUnitCategory("MagneticGradient");
    fFieldQ3GradientCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFieldQ3GradientCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Q4Gradient .
     *
     * Parameter: Q4Gradient (double) in tesla/m
     */
    fFieldQ4GradientCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/field/setQ4Gradient", this);
    fFieldQ4GradientCmd->SetGuidance("Set Q4 Gradient");
    fFieldQ4GradientCmd->SetParameterName("Q4Gradient", false);
    //fFieldQ4GradientCmd->SetRange("Q4Gradient>-100 && Q4Gradient < 100");
    fFieldQ4GradientCmd->SetUnitCategory("MagneticGradient");
    fFieldQ4GradientCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFieldQ4GradientCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to enable/disable use of Map Dipole spectrometer
     *
     * Parameter: StatusMapBField (0 or 1)
     * - 0: Disable Map B Field
     * - 1: Enable Map B Field
     */
    fFieldStatusMapBFieldCmd = new G4UIcmdWithAnInteger("/PlasmaMLPALLAS/field/setStatusMapBField", this);
    fFieldStatusMapBFieldCmd->SetGuidance("Enable or disable the feature (0/1)");
    fFieldStatusMapBFieldCmd->SetParameterName("StatusMapBField", false);
    fFieldStatusMapBFieldCmd->SetRange("StatusMapBField==0 || StatusMapBField==1");
    fFieldStatusMapBFieldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFieldStatusMapBFieldCmd->SetToBeBroadcasted(false);

    /**
     * @brief Command to set the Constant Dipole spectrometer B Field Value if Status Map B Field is disabled.
     *
     * Parameter: ConstantDipoleBField (double) in tesla
     */
    fFieldConstantDipoleBFieldCmd = new G4UIcmdWithADoubleAndUnit("/PlasmaMLPALLAS/field/setConstantDipoleBField", this);
    fFieldConstantDipoleBFieldCmd->SetGuidance("Set Constant Dipole B Field value");
    fFieldConstantDipoleBFieldCmd->SetParameterName("ConstantDipoleBField", false);
    //fFieldConstantDipoleBFieldCmd->SetRange("abs(ConstantDipoleBField) > 0");
    fFieldConstantDipoleBFieldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fFieldConstantDipoleBFieldCmd->SetToBeBroadcasted(false);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Destructor.
 *
 * Deletes all UI commands to free memory.
 */
PlasmaMLPALLASGeometryMessenger::~PlasmaMLPALLASGeometryMessenger()
{
    delete fPrintCmd;
    delete fGeometryStatusDisplayCmd;
    delete fGeometryStatusDisplayCollimatorsCmd;
    delete fGeometryStatusDisplayQuadrupolesCmd;
    delete fGeometryQ1LengthCmd;
    delete fGeometryQ2LengthCmd;
    delete fGeometryQ3LengthCmd;
    delete fGeometryQ4LengthCmd;
    delete fGeometrySourceQ1DistanceCmd;
    delete fGeometryQ1Q2DistanceCmd;
    delete fGeometryQ2Q3DistanceCmd;
    delete fGeometryQ3Q4DistanceCmd;
    delete fGeometrySourceCollimatorsDistanceCmd;
    delete fFieldQ1GradientCmd;
    delete fFieldQ2GradientCmd;
    delete fFieldQ3GradientCmd;
    delete fFieldQ4GradientCmd;
    delete fFieldStatusMapBFieldCmd;
    delete fFieldConstantDipoleBFieldCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Apply a new value from the UI command.
 * @param aCommand Pointer to the UI command used.
 * @param aNewValue New value as string from the UI.
 *
 * Converts the string to the appropriate type and sets it in the
 * geometry construction.
 */
void PlasmaMLPALLASGeometryMessenger::SetNewValue(G4UIcommand *aCommand, G4String aNewValue)
{
    if (aCommand == fPrintCmd)
    {
        fGeometry->Print();
    }
    else if (aCommand == fGeometryStatusDisplayCmd)
    {
        fGeometry->SetStatusDisplayGeometry(fGeometryStatusDisplayCmd->GetNewIntValue(aNewValue));
    }
    else if (aCommand == fGeometryStatusDisplayCollimatorsCmd)
    {
        fGeometry->SetStatusDisplayCollimators(fGeometryStatusDisplayCollimatorsCmd->GetNewIntValue(aNewValue));
    }
    else if (aCommand == fGeometryStatusDisplayQuadrupolesCmd)
    {
        fGeometry->SetStatusDisplayQuadrupoles(fGeometryStatusDisplayQuadrupolesCmd->GetNewIntValue(aNewValue));
    }
    else if (aCommand == fGeometryQ1LengthCmd)
    {
        fGeometry->SetQ1Length(fGeometryQ1LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ2LengthCmd)
    {
        fGeometry->SetQ2Length(fGeometryQ2LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ3LengthCmd)
    {
        fGeometry->SetQ3Length(fGeometryQ3LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ4LengthCmd)
    {
        fGeometry->SetQ4Length(fGeometryQ4LengthCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometrySourceQ1DistanceCmd)
    {
        fGeometry->SetSourceQ1Distance(fGeometrySourceQ1DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ1Q2DistanceCmd)
    {
        fGeometry->SetQ1Q2Distance(fGeometryQ1Q2DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ2Q3DistanceCmd)
    {
        fGeometry->SetQ2Q3Distance(fGeometryQ2Q3DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometryQ3Q4DistanceCmd)
    {
        fGeometry->SetQ3Q4Distance(fGeometryQ3Q4DistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fGeometrySourceCollimatorsDistanceCmd)
    {
        fGeometry->SetSourceCollimatorsDistance(fGeometrySourceCollimatorsDistanceCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fFieldQ1GradientCmd)
    {
        fGeometry->SetQ1Gradient(fFieldQ1GradientCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fFieldQ2GradientCmd)
    {
        fGeometry->SetQ2Gradient(fFieldQ2GradientCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fFieldQ3GradientCmd)
    {
        fGeometry->SetQ3Gradient(fFieldQ3GradientCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fFieldQ4GradientCmd)
    {
        fGeometry->SetQ4Gradient(fFieldQ4GradientCmd->GetNewDoubleValue(aNewValue));
    }
    else if (aCommand == fFieldStatusMapBFieldCmd)
    {
        fGeometry->SetStatusMapBField(fFieldStatusMapBFieldCmd->GetNewIntValue(aNewValue));
    }
    else if (aCommand == fFieldConstantDipoleBFieldCmd)
    {
        fGeometry->SetConstantDipoleBField(fFieldConstantDipoleBFieldCmd->GetNewDoubleValue(aNewValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Get the current value of a UI command.
 * @param aCommand Pointer to the UI command.
 * @return Current value of the command as a string.
 *
 * Retrieves the current value from the geometry construction and
 * converts it to a string for display in the UI.
 */
G4String PlasmaMLPALLASGeometryMessenger::GetCurrentValue(G4UIcommand *aCommand)
{
    G4String cv;

    if (aCommand == fGeometryStatusDisplayCmd)
    {
        cv = fGeometryStatusDisplayCmd->ConvertToString(fGeometry->GetStatusDisplayGeometry());
    }
    else if (aCommand == fGeometryStatusDisplayCollimatorsCmd)
    {
        cv = fGeometryStatusDisplayCollimatorsCmd->ConvertToString(fGeometry->GetStatusDisplayCollimators());
    }
    else if (aCommand == fGeometryStatusDisplayQuadrupolesCmd)
    {
        cv = fGeometryStatusDisplayQuadrupolesCmd->ConvertToString(fGeometry->GetStatusDisplayQuadrupoles());
    }
    else if (aCommand == fGeometryQ1LengthCmd)
    {
        cv = fGeometryQ1LengthCmd->ConvertToString(fGeometry->GetQ1Length(), "m");
    }
        else if (aCommand == fGeometryQ2LengthCmd)
    {
        cv = fGeometryQ2LengthCmd->ConvertToString(fGeometry->GetQ2Length(), "m");
    }
        else if (aCommand == fGeometryQ3LengthCmd)
    {
        cv = fGeometryQ3LengthCmd->ConvertToString(fGeometry->GetQ3Length(), "m");
    }
        else if (aCommand == fGeometryQ4LengthCmd)
    {
        cv = fGeometryQ4LengthCmd->ConvertToString(fGeometry->GetQ4Length(), "m");
    }
    else if (aCommand == fGeometrySourceQ1DistanceCmd)
    {
        cv = fGeometrySourceQ1DistanceCmd->ConvertToString(fGeometry->GetSourceQ1Distance(), "m");
    }
    else if (aCommand == fGeometryQ1Q2DistanceCmd)
    {
        cv = fGeometryQ1Q2DistanceCmd->ConvertToString(fGeometry->GetQ1Q2Distance(), "m");
    }
    else if (aCommand == fGeometryQ2Q3DistanceCmd)
    {
        cv = fGeometryQ2Q3DistanceCmd->ConvertToString(fGeometry->GetQ2Q3Distance(), "m");
    }
    else if (aCommand == fGeometryQ3Q4DistanceCmd)
    {
        cv = fGeometryQ3Q4DistanceCmd->ConvertToString(fGeometry->GetQ3Q4Distance(), "m");
    }
    else if (aCommand == fGeometrySourceCollimatorsDistanceCmd)
    {
        cv = fGeometrySourceCollimatorsDistanceCmd->ConvertToString(fGeometry->GetSourceCollimatorsDistance(), "m");
    }
    else if (aCommand == fFieldQ1GradientCmd)
    {
        cv = fFieldQ1GradientCmd->ConvertToString(fGeometry->GetQ1Gradient(), "T/m");
    }
    else if (aCommand == fFieldQ2GradientCmd)
    {
        cv = fFieldQ2GradientCmd->ConvertToString(fGeometry->GetQ2Gradient(), "T/m");
    }
    else if (aCommand == fFieldQ3GradientCmd)
    {
        cv = fFieldQ3GradientCmd->ConvertToString(fGeometry->GetQ3Gradient(), "T/m");
    }
    else if (aCommand == fFieldQ4GradientCmd)
    {
        cv = fFieldQ4GradientCmd->ConvertToString(fGeometry->GetQ4Gradient(), "T/m");
    }
    else if (aCommand == fFieldStatusMapBFieldCmd)
    {
        cv = fFieldStatusMapBFieldCmd->ConvertToString(fGeometry->GetStatusMapBField());
    }
    else if (aCommand == fFieldConstantDipoleBFieldCmd)
    {
        cv = fFieldConstantDipoleBFieldCmd->ConvertToString(fGeometry->GetConstantDipoleBField(), "T");
    }
    return cv;
}
