#ifndef PlasmaMLPALLASPrimaryGeneratorMessenger_H
#define PlasmaMLPALLASPrimaryGeneratorMessenger_H

/**
 * @class PlasmaMLPALLASPrimaryGeneratorMessenger
 * @brief Provides UI commands to configure the primary generator
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 * 
 * This class allows the user to set various parameters of the primary
 * particle generator, including ONNX model usage, particle type,
 * laser focus, and plasma parameters via the Geant4 UI.
 */

#include "G4UIcmdWithAString.hh"                   // for G4UIcmdWithAString
#include "G4UIcmdWithADouble.hh"                   // for G4UIcmdWithADouble
#include "G4UIcmdWithAnInteger.hh"                 // for G4UIcmdWithAnInteger
#include "G4UIdirectory.hh"                        // for G4UIdirectory
#include "PlasmaMLPALLASOnnxParameters.hh"
#include "G4UImessenger.hh"


class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIdirectory;

class PlasmaMLPALLASPrimaryGeneratorAction;

class PlasmaMLPALLASPrimaryGeneratorMessenger : public G4UImessenger
{
public:
    /**
     * @brief Constructor
     * @param primaryGenerator Pointer to the associated primary generator action
     *
     * Sets up UI commands and directories for controlling the primary generator.
     */
    PlasmaMLPALLASPrimaryGeneratorMessenger(PlasmaMLPALLASPrimaryGeneratorAction *primaryGenerator);

    /// Destructor
    ~PlasmaMLPALLASPrimaryGeneratorMessenger();

    /**
     * @brief Executes a command with a new value
     * @param command Pointer to the issued command
     * @param newValue String representation of the new value
     *
     * Calls the appropriate method in the primary generator based on the command.
     */
    virtual void SetNewValue(G4UIcommand *command, G4String newValue) final;

    /**
     * @brief Retrieves the current value of a command
     * @param command Pointer to the command
     * @return String representing the current value
     */
    virtual G4String GetCurrentValue(G4UIcommand *command) final;

private:
    /// Associated primary generator action
    PlasmaMLPALLASPrimaryGeneratorAction *fPrimaryGeneratorAction = nullptr;

    /// UI directories
    G4UIdirectory *fExampleDir = nullptr; ///< Common directory
    G4UIdirectory *fGunDir = nullptr;     ///< Directory for gun settings /PlasmaMLPALLAS/gun
    G4UIdirectory *fLaserDir = nullptr;   ///< Directory for laser settings /PlasmaMLPALLAS/laser

    // Gun-related commands
    G4UIcmdWithAnInteger *fGunStatusONNXCmd = nullptr; ///< ONNX usage status
    G4UIcmdWithAString  *fGunParticleNameCmd = nullptr;///< Primary particle name

    // Laser-related commands
    G4UIcmdWithADouble *fLaserOffsetCmd = nullptr;                 ///< Laser focus offset (ML parameter)
    G4UIcmdWithADouble *fLaserNormVecPotentialCmd = nullptr;      ///< Normalized vector potential (ML parameter)
    G4UIcmdWithADouble *fLaserFracDopTargetChamberCmd = nullptr;  ///< Fractional Doppler target chamber (ML parameter)
    G4UIcmdWithADouble *fLaserPressureCmd = nullptr;              ///< Gas pressure (ML parameter)
};

#endif
