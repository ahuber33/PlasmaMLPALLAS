#include "PlasmaMLPALLASPrimaryGeneratorMessenger.hh"

/**
 * @file PlasmaMLPALLASPrimaryGeneratorMessenger.cc
 * @brief User interface (UI) messenger for the PlasmaMLPALLAS primary generator.
 *
 * This class provides a set of Geant4 UI commands to control the primary particle
 * generator and machine-learning-based laser parameters in the PALLAS simulation.
 * 
 * It allows users to:
 *  - Enable or disable ONNX-based particle generation.
 *  - Set the particle type for the simulation.
 *  - Configure laser parameters such as focus offset, normalized vector potential,
 *    dopant fraction in the target chamber, and gas pressure.
 *
 * Commands are organized in directories:
 *  - /PlasmaMLPALLAS/gun/ for particle gun-related commands
 *  - /PlasmaMLPALLAS/laser/ for laser-related parameters
 *
 * The messenger interacts with the `PlasmaMLPALLASPrimaryGeneratorAction` and
 * `OnnxParameters` singleton to apply changes in real time.
 *
 * Usage:
 *  - Instantiate the messenger with a pointer to the primary generator action.
 *  - Commands are available in Geant4 UI macros or interactive session.
 *
 * @authors Arnaud HUBER
 * @authors Alexei SYTOV
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */

 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Constructor.
 * @param aGun Pointer to the primary generator action.
 *
 * Creates UI directories and commands for both the particle gun and the laser parameters.
 */
PlasmaMLPALLASPrimaryGeneratorMessenger::PlasmaMLPALLASPrimaryGeneratorMessenger(PlasmaMLPALLASPrimaryGeneratorAction *aGun)
    : G4UImessenger(), fPrimaryGeneratorAction(aGun)
{
    // General UI directory
    fExampleDir = new G4UIdirectory("/PlasmaMLPALLAS/");
    fExampleDir->SetGuidance("UI commands specific to this example");

    // Particle gun commands directory
    fGunDir = new G4UIdirectory("/PlasmaMLPALLAS/gun/");
    fGunDir->SetGuidance("Particle Gun UI commands");

    // Laser commands directory
    fLaserDir = new G4UIdirectory("/PlasmaMLPALLAS/laser/");
    fLaserDir->SetGuidance("ML laser parameters UI commands");

    //=====================================
    // Gun Commands
    //=====================================

    /**
     * @brief Command to enable or disable ONNX feature for the gun.
     *
     * Parameter: StatusONNX (0 or 1)
     * - 0: Disable ONNX
     * - 1: Enable ONNX
     */
    fGunStatusONNXCmd = new G4UIcmdWithAnInteger("/PlasmaMLPALLAS/gun/setStatusONNX", this);
    fGunStatusONNXCmd->SetGuidance("Enable or disable the feature (0/1)");
    fGunStatusONNXCmd->SetParameterName("StatusONNX", false);
    fGunStatusONNXCmd->SetRange("StatusONNX==0 || StatusONNX==1");
    fGunStatusONNXCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_GeomClosed);
    fGunStatusONNXCmd->SetToBeBroadcasted(true);

    /**
     * @brief Command to set the particle name for the gun.
     *
     * Parameter: ParticleName (string)
     */
    fGunParticleNameCmd = new G4UIcmdWithAString("/PlasmaMLPALLAS/gun/setParticleName", this);
    fGunParticleNameCmd->SetGuidance("Set the original particle name");
    fGunParticleNameCmd->SetParameterName("ParticleName", false);
    fGunParticleNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle, G4State_GeomClosed);
    fGunParticleNameCmd->SetToBeBroadcasted(true);

    //=====================================
    // Laser Commands
    //=====================================

    /**
     * @brief Command to set the offset of the laser focus.
     *
     * Parameter: OffsetLaserFocus (double)
     */
    fLaserOffsetCmd = new G4UIcmdWithADouble("/PlasmaMLPALLAS/laser/setOffsetLaserFocus", this);
    fLaserOffsetCmd->SetGuidance("Set Offset Laser Focus ML ONNX Parameter");
    fLaserOffsetCmd->SetParameterName("OffsetLaserFocus", false);
    fLaserOffsetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fLaserOffsetCmd->SetToBeBroadcasted(true);

    /**
     * @brief Command to set the normalized vector potential of the laser.
     *
     * Parameter: NormVecPotential (double)
     */
    fLaserNormVecPotentialCmd = new G4UIcmdWithADouble("/PlasmaMLPALLAS/laser/setNormVecPotential", this);
    fLaserNormVecPotentialCmd->SetGuidance("Set Norm Vec Potential ML ONNX Parameter");
    fLaserNormVecPotentialCmd->SetParameterName("NormVecPotential", false);
    fLaserNormVecPotentialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fLaserNormVecPotentialCmd->SetToBeBroadcasted(true);

    /**
     * @brief Command to set the fraction of dopant in the target chamber.
     *
     * Parameter: FracDopTargChamber (double)
     */
    fLaserFracDopTargetChamberCmd = new G4UIcmdWithADouble("/PlasmaMLPALLAS/laser/setFracDopTargetChamber", this);
    fLaserFracDopTargetChamberCmd->SetGuidance("Set Frac Dop Target Chamber ML ONNX Parameter");
    fLaserFracDopTargetChamberCmd->SetParameterName("FracDopTargChamber", false);
    fLaserFracDopTargetChamberCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fLaserFracDopTargetChamberCmd->SetToBeBroadcasted(true);

    /**
     * @brief Command to set the gaz chamber pressure.
     *
     * Parameter: Pressure (double)
     */
    fLaserPressureCmd = new G4UIcmdWithADouble("/PlasmaMLPALLAS/laser/setPressure", this);
    fLaserPressureCmd->SetGuidance("Set Pressure ML ONNX Parameter");
    fLaserPressureCmd->SetParameterName("Pressure", false);
    fLaserPressureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fLaserPressureCmd->SetToBeBroadcasted(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Destructor.
 *
 * Deletes all UI commands to free memory.
 */
PlasmaMLPALLASPrimaryGeneratorMessenger::~PlasmaMLPALLASPrimaryGeneratorMessenger()
{
    delete fGunStatusONNXCmd;
    delete fGunParticleNameCmd;
    delete fLaserOffsetCmd;
    delete fLaserNormVecPotentialCmd;
    delete fLaserFracDopTargetChamberCmd;
    delete fLaserPressureCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Apply a new value from the UI command.
 * @param aCommand Pointer to the UI command used.
 * @param aNewValue New value as string from the UI.
 *
 * Converts the string to the appropriate type and sets it in the
 * primary generator action.
 */

void PlasmaMLPALLASPrimaryGeneratorMessenger::SetNewValue(G4UIcommand *aCommand, G4String aNewValue)
{
    if (aCommand == fGunStatusONNXCmd)
        OnnxParameters::Instance().SetStatusONNX(fGunStatusONNXCmd->GetNewIntValue(aNewValue));
    else if (aCommand == fGunParticleNameCmd)
        OnnxParameters::Instance().SetParticleName(aNewValue);
    else if (aCommand == fLaserOffsetCmd)
        OnnxParameters::Instance().SetXoff(fLaserOffsetCmd->GetNewDoubleValue(aNewValue));
    else if (aCommand == fLaserNormVecPotentialCmd)
        OnnxParameters::Instance().SetA0(fLaserNormVecPotentialCmd->GetNewDoubleValue(aNewValue));
    else if (aCommand == fLaserFracDopTargetChamberCmd)
        OnnxParameters::Instance().SetCN2(fLaserFracDopTargetChamberCmd->GetNewDoubleValue(aNewValue));
    else if (aCommand == fLaserPressureCmd)
        OnnxParameters::Instance().SetPressure(fLaserPressureCmd->GetNewDoubleValue(aNewValue));
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/**
 * @brief Get the current value of a UI command.
 * @param aCommand Pointer to the UI command.
 * @return Current value of the command as a string.
 *
 * Retrieves the current value from the primary generator action and
 * converts it to a string for display in the UI.
 */
G4String PlasmaMLPALLASPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand *aCommand)
{
    G4String cv;

    if (aCommand == fGunStatusONNXCmd)
        cv = fGunStatusONNXCmd->ConvertToString(OnnxParameters::Instance().GetStatusONNX());
    else if (aCommand == fGunParticleNameCmd)
        cv = OnnxParameters::Instance().GetParticleName();
    else if (aCommand == fLaserOffsetCmd)
        cv = fLaserOffsetCmd->ConvertToString(OnnxParameters::Instance().GetXoff());
    else if (aCommand == fLaserNormVecPotentialCmd)
        cv = fLaserNormVecPotentialCmd->ConvertToString(OnnxParameters::Instance().GetA0());
    else if (aCommand == fLaserFracDopTargetChamberCmd)
        cv = fLaserFracDopTargetChamberCmd->ConvertToString(OnnxParameters::Instance().GetCN2());
    else if (aCommand == fLaserPressureCmd)
        cv = fLaserPressureCmd->ConvertToString(OnnxParameters::Instance().GetPressure());

    return cv;
}

