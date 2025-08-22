#ifndef PlasmaMLPALLASGeometryMessenger_H
#define PlasmaMLPALLASGeometryMessenger_H

/**
 * @class PlasmaMLPALLASGeometryMessenger
 * @brief UI messenger for the Geometry constrcution in PALLAS simulation
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 * 
 * Provides UI commands to setup detector and readout geometry (prior to
 * initialization). Length, distance, gradients and display can be changed.
 */

#include "G4UIcmdWithADoubleAndUnit.hh"          // for G4UIcmdWithADoubleAndUnit
#include "G4UIcmdWithAnInteger.hh"               // for G4UIcmdWithAnInteger
#include "G4UIcmdWithoutParameter.hh"            // for G4UIcmdWithoutParameter
#include "G4UIdirectory.hh"                      // for G4UIdirectory
#include "PlasmaMLPALLASGeometryConstruction.hh" // for PlasmaMLPALLASGeometryConstruction

class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIdirectory;
class PlasmaMLPALLASGeometryConstruction;

class PlasmaMLPALLASGeometryMessenger : public G4UImessenger
{
public:
    PlasmaMLPALLASGeometryMessenger(PlasmaMLPALLASGeometryConstruction *);
    ~PlasmaMLPALLASGeometryMessenger();

    /// Invokes appropriate methods based on the typed command
    virtual void SetNewValue(G4UIcommand *, G4String) final;
    /// Retrieves the current settings
    virtual G4String GetCurrentValue(G4UIcommand *) final;

private:
    /// Detector construction to setup
    PlasmaMLPALLASGeometryConstruction *fGeometry = nullptr;
    /// Command to set the directory common to all messengers in this example
    /// /Par04
    G4UIdirectory *fExampleDir = nullptr;
    /// Command to set the directory for detector settings /PlasmaMLPALLAS/display
    G4UIdirectory *fDisplayDir = nullptr;
    /// Command to set the directory for detector settings /PlasmaMLPALLAS/geometry
    G4UIdirectory *fGeometryDir = nullptr;
    /// Command to set the directory for detector settings /PlasmaMLPALLAS/field
    G4UIdirectory *fFieldDir = nullptr;
    /// Command printing current settings
    G4UIcmdWithoutParameter *fPrintCmd;

    ///DISPLAY
    /// Command to set the status display of Full PALLAS geometry
    G4UIcmdWithAnInteger *fGeometryStatusDisplayCmd = nullptr;
    /// Command to set the status display of Quadrupoles geometry
    G4UIcmdWithAnInteger *fGeometryStatusDisplayQuadrupolesCmd = nullptr;
    /// Command to set the status display of Collimators geometry
    G4UIcmdWithAnInteger *fGeometryStatusDisplayCollimatorsCmd = nullptr;

    ///GEOMETRY
    /// Command to set the Q1 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ1LengthCmd = nullptr;
    /// Command to set the Q2 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ2LengthCmd = nullptr;
    /// Command to set the Q3 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ3LengthCmd = nullptr;
    /// Command to set the Q4 Length
    G4UIcmdWithADoubleAndUnit *fGeometryQ4LengthCmd = nullptr;
    /// Command to set the Source -- Q1 Distance
    G4UIcmdWithADoubleAndUnit *fGeometrySourceQ1DistanceCmd = nullptr;
    /// Command to set the Q1 -- Q2 Distance
    G4UIcmdWithADoubleAndUnit *fGeometryQ1Q2DistanceCmd = nullptr;
    /// Command to set the Q2 -- Q3 Distance
    G4UIcmdWithADoubleAndUnit *fGeometryQ2Q3DistanceCmd = nullptr;
    /// Command to set the Q3 -- Q4 Distance
    G4UIcmdWithADoubleAndUnit *fGeometryQ3Q4DistanceCmd = nullptr;
    /// Command to set the Source -- Collimators Distance
    G4UIcmdWithADoubleAndUnit *fGeometrySourceCollimatorsDistanceCmd = nullptr;

    ///FIELD
    /// Command to set the Q1 Gradient
    G4UIcmdWithADoubleAndUnit *fFieldQ1GradientCmd = nullptr;
    /// Command to set the Q2 Gradient
    G4UIcmdWithADoubleAndUnit *fFieldQ2GradientCmd = nullptr;
    /// Command to set the Q3 Gradient
    G4UIcmdWithADoubleAndUnit *fFieldQ3GradientCmd = nullptr;
    /// Command to set the Q4 Gradient
    G4UIcmdWithADoubleAndUnit *fFieldQ4GradientCmd = nullptr;
    /// Command to set the status of Map B Field (used/not used)
    G4UIcmdWithAnInteger *fFieldStatusMapBFieldCmd = nullptr;
    /// Command to set the Constant Dipole B Field
    G4UIcmdWithADoubleAndUnit *fFieldConstantDipoleBFieldCmd = nullptr;

};

#endif
