/// PALLAS_CollSimVisManager.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifdef G4VIS_USE

#include "PALLAS_CollSimVisManager.hh"

// Supported drivers...uncomment the driver of your preference
// Current default is VRML files

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
// #include "G4VRML1.hh"
//#include "G4VRML2.hh"
//#endif

//#ifdef G4VIS_USE_VRMLFILE
//#include "G4VRML1File.hh"
//#include "G4VRML2File.hh"
//#endif

PALLAS_CollSimVisManager::PALLAS_CollSimVisManager () {}

void PALLAS_CollSimVisManager::RegisterGraphicsSystems () {

  //  RegisterGraphicsSystem (new G4HepRepFile);
  //  RegisterGraphicsSystem (new G4HepRep);


  //#ifdef G4VIS_USE_DAWN
  //  RegisterGraphicsSystem (new G4FukuiRenderer);
  //#endif

  //#ifdef G4VIS_USE_DAWNFILE
  //  RegisterGraphicsSystem (new G4DAWNFILE);
  //#endif


  //#ifdef G4VIS_USE_OPENGLX
  //RegisterGraphicsSystem (new G4OpenGLImmediateX);
  //RegisterGraphicsSystem (new G4OpenGLStoredX);
  //#endif

    #ifdef G4VIS_USE_TOOLSSG_X11_GLES
    RegisterGraphicsSystem (new G4ToolSSGX11GLES);  // Remplacez OpenGL par TOOLSSG_X11_GLES
#endif


//#ifdef G4VIS_USE_VRML
  //  RegisterGraphicsSystem (new G4VRML1);
  //RegisterGraphicsSystem (new G4VRML2);
//#endif

//#ifdef G4VIS_USE_VRMLFILE
  //  RegisterGraphicsSystem (new G4VRML1File);
  //RegisterGraphicsSystem (new G4VRML2File);
//#endif

  if (fVerbose > 0) {
    G4cout <<
      "\nYou have successfully chosen to use the following graphics systems."
	 << G4endl;
    PrintAvailableGraphicsSystems ();
    
  }
}

#endif
















































