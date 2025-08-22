/**
 * @file PlasmaMLPALLASVisManager.cc
 * @brief Implementation of the PlasmaMLPALLASVisManager visualization manager.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2024
 * @copyright PALLAS Project - GEANT4 Collaboration
 * This file defines the methods for the `PlasmaMLPALLASVisManager` class,
 * which is responsible for registering the desired visualization
 * drivers for the PALLAS simulation.
 * @note This implementation is only compiled if `G4VIS_USE` is defined.
 *       Uncomment the desired visualization drivers according to your
 *       simulation requirements and the drivers enabled in Geant4.
 */

#ifdef G4VIS_USE

#include "PlasmaMLPALLASVisManager.hh"

// ===== Optional Geant4 Visualization Drivers =====
// Uncomment the includes and registration lines as needed.

//#include "G4HepRepFile.hh"
//#include "G4HepRep.hh"

//#ifdef G4VIS_USE_DAWN
//#include "G4FukuiRenderer.hh"
//#endif

//#ifdef G4VIS_USE_DAWNFILE
//#include "G4DAWNFILE.hh"
//#endif

//#ifdef G4VIS_USE_OPENGLX
//#include "G4OpenGLImmediateX.hh"
//#include "G4OpenGLStoredX.hh"
//#endif

#ifdef G4VIS_USE_TOOLSSG_X11_GLES
#include "G4ToolSSGX11GLES.hh"
#endif

//#ifdef G4VIS_USE_VRML
//#include "G4VRML1.hh"
//#include "G4VRML2.hh"
//#endif

//#ifdef G4VIS_USE_VRMLFILE
//#include "G4VRML1File.hh"
//#include "G4VRML2File.hh"
//#endif

// ============================================================
// Constructor
// ============================================================
PlasmaMLPALLASVisManager::PlasmaMLPALLASVisManager() {}

// ============================================================
// RegisterGraphicsSystems
// ============================================================
/**
 * @brief Registers the available and desired visualization systems.
 *
 * This method is called during the initialization of the visualization
 * manager to add the graphics systems that can be used during the run.
 *
 * To enable a driver:
 *  - Make sure the driver is enabled in your Geant4 build (CMake options).
 *  - Uncomment both its `#include` line and its `RegisterGraphicsSystem(...)` call.
 */
void PlasmaMLPALLASVisManager::RegisterGraphicsSystems()
{
    // === HepRep (hierarchical event representation) ===
    // RegisterGraphicsSystem(new G4HepRepFile);
    // RegisterGraphicsSystem(new G4HepRep);

    // === DAWN Renderer ===
    //#ifdef G4VIS_USE_DAWN
    // RegisterGraphicsSystem(new G4FukuiRenderer);
    //#endif

    //#ifdef G4VIS_USE_DAWNFILE
    // RegisterGraphicsSystem(new G4DAWNFILE);
    //#endif

    // === OpenGL (X11) ===
    //#ifdef G4VIS_USE_OPENGLX
    // RegisterGraphicsSystem(new G4OpenGLImmediateX);
    // RegisterGraphicsSystem(new G4OpenGLStoredX);
    //#endif

    // === ToolSSG (X11 GLES) ===
    #ifdef G4VIS_USE_TOOLSSG_X11_GLES
    RegisterGraphicsSystem(new G4ToolSSGX11GLES); ///< Preferred for modern OpenGL ES rendering
    #endif

    // === VRML (runtime) ===
    //#ifdef G4VIS_USE_VRML
    // RegisterGraphicsSystem(new G4VRML1);
    // RegisterGraphicsSystem(new G4VRML2);
    //#endif

    // === VRML (file output) ===
    //#ifdef G4VIS_USE_VRMLFILE
    // RegisterGraphicsSystem(new G4VRML1File);
    // RegisterGraphicsSystem(new G4VRML2File);
    //#endif

    // === Verbose output ===
    if (fVerbose > 0) {
        G4cout << "\nVisualization systems registered successfully:" << G4endl;
        PrintAvailableGraphicsSystems();
    }
}

#endif // G4VIS_USE
