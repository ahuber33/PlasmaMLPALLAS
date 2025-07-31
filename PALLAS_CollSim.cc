/// PALLAS_CollSim.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "G4UIterminal.hh"
#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimPhysics.hh"
#include "PALLAS_CollSimPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimActionInitialization.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Geometry.hh"
#include "G4MTRunManager.hh"
#include "ParticleData.hh"
#include <thread>
#include <mutex>
#include <fstream>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    G4Exception("Main", "main0004", FatalException,
                "Insufficient input arguments. Usage: ./PALLAS_CollSim [ROOT file name] [events] [macro] [FileReader ON/OFF] [MT ON/OFF] [threads (if MT ON)]");
    return 1;
  }

  char *outputFile = argv[1];
  size_t TotalNParticles = 0;
  bool flag_MT = false;
  size_t Ncores = std::thread::hardware_concurrency();
  G4RunManager *runManager;

  if (argc == 2) // VISUALIZATION MODE
  {
    runManager = new G4RunManager;
  }
  else if (argc >= 5) // BATCH MODE
  {
    TotalNParticles = std::stoul(argv[2]);
    G4String pMT = argv[4];

    if (pMT == "ON")
    {
      flag_MT = true;
      runManager = new G4MTRunManager;
      if (argc == 6)
        Ncores = std::stoul(argv[5]);

      runManager->SetNumberOfThreads(Ncores);
    }
    else if (pMT == "OFF")
    {
      flag_MT = false;
      runManager = new G4RunManager;
    }
    else
    {
      G4Exception("Main", "main0002", FatalException,
                  "MT parameter (5th argument) must be ON or OFF.");
    }
  }

  else
  {
    G4Exception("Main", "main0003", FatalException,
                "Incorrect number of input parameters.");
  }

  // Setup geometry
  Geometry *Geom = new Geometry();
  PALLAS_CollSimGeometryConstruction *GeomCons = new PALLAS_CollSimGeometryConstruction;
  runManager->SetUserInitialization(GeomCons);

  // Initialize physics
  runManager->SetUserInitialization(new PALLAS_CollSimPhysics);

  // Initialize user actions
    runManager->SetUserInitialization(new PALLAS_CollSimActionInitialization(outputFile, TotalNParticles, Ncores, flag_MT, GeomCons));

  // Visualization
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();

  // Initialize the kernel
  runManager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();

  if (argc == 2) // VISUALIZATION MODE
  {
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
    UI->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else if (argc >= 5) // BATCH MODE
  {
    G4String command = "/control/execute ";
    G4String macro = argv[3];
    UI->ApplyCommand(command + macro);

    std::string runCommand = "/run/beamOn " + std::string(argv[2]);
    UI->ApplyCommand(runCommand);

    if (flag_MT)
    {
      std::string mergeCommand = "/control/shell hadd -k -f " + std::string(outputFile) + ".root";
      for (size_t i = 1; i <= Ncores; ++i)
      {
        mergeCommand += " " + std::string(outputFile) + "_" + std::to_string(i) + ".root";
      }
      UI->ApplyCommand(mergeCommand);

      // Clean up temporary files
      for (size_t i = 1; i <= Ncores; ++i)
      {
        std::string removeCommand = "/control/shell rm -f " + std::string(outputFile) + "_" + std::to_string(i) + ".root";
        UI->ApplyCommand(removeCommand);
      }
    }
  }

  std::string movefile = "/control/shell mv " + std::string(outputFile) + ".root ../Resultats";
  UI->ApplyCommand(movefile);
  G4cout << "Output saved in Resultats fodler to file " << outputFile << ".root" << G4endl;

  // Final clean-up
  delete visManager;
  delete runManager;

  return 0;
}
