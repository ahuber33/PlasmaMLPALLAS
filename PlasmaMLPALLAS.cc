/**
 * @file PlasmaMLPALLAS.cc
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 * @brief Main program for the PlasmaMLPALLAS simulation using Geant4.
 */

/** Include necessary Geant4 headers and project-specific headers */
#include "G4UIterminal.hh"
#include "PlasmaMLPALLASSteppingAction.hh"
#include "PlasmaMLPALLASPhysics.hh"
#include "PlasmaMLPALLASPrimaryGeneratorAction.hh"
#include "PlasmaMLPALLASActionInitialization.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Geometry.hh"
#include "G4MTRunManager.hh"
#include <thread>
#include <mutex>
#include <fstream>

/**
 * @brief Main function of the PlasmaMLPALLAS program.
 * @param argc Number of command-line arguments
 * @param argv Array of command-line arguments
 * @return Exit code (0 = success, 1 = failure)
 *
 * This program can run in two modes:
 * 1. Visualization mode: `./PlasmaMLPALLAS outputFile`
 * 2. Batch mode: `./PlasmaMLPALLAS outputFile NParticles macro MT ON/OFF [threads]`
 */
int main(int argc, char **argv)
{
    /** Check minimum number of arguments */
    if (argc < 2)
    {
        G4Exception("Main", "main0004", FatalException,
                    "Insufficient input arguments. Usage: ./PlasmaMLPALLAS [ROOT file name] [events] [macro] [MT ON/OFF] [threads (if MT ON)]");
        return 1;
    }

    /** Output file name */
    char *outputFile = argv[1];

    /** Total number of particles to simulate (for batch mode) */
    size_t TotalNParticles = 0;

    /** Flag to indicate multi-threading */
    bool flag_MT = false;

    /** Number of CPU cores available */
    size_t Ncores = std::thread::hardware_concurrency();

    /** Pointer to the Geant4 run manager (single or multi-threaded) */
    G4RunManager *runManager;

    /** Determine execution mode */
    if (argc == 2) // VISUALIZATION MODE
    {
        runManager = new G4RunManager;
    }
    else if (argc >= 5) // BATCH MODE
    {
        TotalNParticles = std::stoul(argv[2]);
        G4String pMT = argv[4];

        /** Multi-threaded mode */
        if (pMT == "ON")
        {
            flag_MT = true;
            runManager = new G4MTRunManager;

            if (argc == 6)
                Ncores = std::stoul(argv[5]);

            runManager->SetNumberOfThreads(Ncores);
        }
        /** Single-threaded mode */
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

    /** Setup geometry */
    Geometry *Geom = new Geometry();
    PlasmaMLPALLASGeometryConstruction *GeomCons = new PlasmaMLPALLASGeometryConstruction;
    runManager->SetUserInitialization(GeomCons);

    /** Initialize physics */
    runManager->SetUserInitialization(new PlasmaMLPALLASPhysics);

    /** Initialize user actions */
    runManager->SetUserInitialization(new PlasmaMLPALLASActionInitialization(
        outputFile, TotalNParticles, Ncores, flag_MT, GeomCons));

    /** Initialize visualization manager */
    G4VisManager *visManager = new G4VisExecutive;
    visManager->Initialize();

    /** Initialize Geant4 kernel */
    runManager->Initialize();

    /** Get UI manager pointer */
    G4UImanager *UI = G4UImanager::GetUIpointer();

    /** Visualization mode: launch interactive session */
    if (argc == 2)
    {
        G4UIExecutive *ui = new G4UIExecutive(argc, argv);
        UI->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }
    /** Batch mode: execute macro and run simulation */
    else if (argc >= 5)
    {
        G4String command = "/control/execute ";
        G4String macro = argv[3];
        UI->ApplyCommand(command + macro);

        std::string runCommand = "/run/beamOn " + std::string(argv[2]);
        UI->ApplyCommand(runCommand);

        /** Multi-threaded: merge output ROOT files */
        if (flag_MT)
        {
            std::string mergeCommand = "/control/shell hadd -k -f " + std::string(outputFile) + ".root";
            for (size_t i = 1; i <= Ncores; ++i)
            {
                mergeCommand += " " + std::string(outputFile) + "_" + std::to_string(i) + ".root";
            }
            UI->ApplyCommand(mergeCommand);

            /** Clean up temporary files */
            for (size_t i = 1; i <= Ncores; ++i)
            {
                std::string removeCommand = "/control/shell rm -f " + std::string(outputFile) + "_" + std::to_string(i) + ".root";
                UI->ApplyCommand(removeCommand);
            }
        }
    }

    /** Move final ROOT file to results folder */
    std::string movefile = "/control/shell mv " + std::string(outputFile) + ".root ../Resultats";
    UI->ApplyCommand(movefile);
    G4cout << "Output saved in Resultats folder to file " << outputFile << ".root" << G4endl;

    /** Final cleanup */
    delete visManager;
    delete runManager;

    return 0;
}
