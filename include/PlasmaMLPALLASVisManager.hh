#ifndef PlasmaMLPALLASVisManager_h
#define PlasmaMLPALLASVisManager_h 1

/**
 * @brief Custom visualization manager for the PALLAS project.
 * @class PlasmaMLPALLASVisManager
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 * 
 * This class extends Geant4's `G4VisManager` to configure and register
 * the set of visualization drivers used in the PALLAS simulation.
 * It allows the simulation to support various graphics systems such as
 * OpenGL, HepRApp, VRML, or other Geant4 visualization backends.
 * The class primarily overrides the `RegisterGraphicsSystems()` method
 * to add the desired set of graphics systems when the visualization
 * manager is initialized.
 */

#include "G4VisManager.hh"

class PlasmaMLPALLASVisManager : public G4VisManager
{
public:
    /// Constructor: prepares the visualization manager
    PlasmaMLPALLASVisManager();

private:
    /**
     * @brief Registers the visualization drivers to be used.
     *
     * This method is called internally by `G4VisManager::Initialize()`.
     * Override this method to register custom or specific graphics systems.
     */
    void RegisterGraphicsSystems() override;
};

#endif // PlasmaMLPALLASVisManager_h
