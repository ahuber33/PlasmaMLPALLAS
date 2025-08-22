#ifndef PlasmaMLPALLASPhysics_h
#define PlasmaMLPALLASPhysics_h 1

/**
 * @class PlasmaMLPALLASPhysics
 * @brief Custom physics list for the PALLAS project.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 * This class defines a modular physics list for Geant4 simulations
 * in the PALLAS project. It configures a combination of electromagnetic,
 * hadronic, ion, and decay physics, using high-precision (HP) models
 * where relevant for accurate plasma and nuclear interaction modeling.
 *
 * Key features:
 * - Uses HP models for neutron elastic and inelastic scattering
 * - Includes EM Option3 physics for improved multiple scattering accuracy
 * - Registers radioactive decay physics for isotope handling
 * - Configures nuclide table thresholds for short-lived isotopes
 *
 * @note This physics list is optimized for applications involving
 *      plasma physics and detailed nuclear interactions.
 */

// =============================
// Geant4 Base Class
// =============================
#include "G4VModularPhysicsList.hh"

// =============================
// Units
// =============================
#include "CLHEP/Units/SystemOfUnits.h"

// =============================
// Nuclide Table (for radioactive decay threshold setup)
// =============================
#include "G4NuclideTable.hh"

// =============================
// Physics Modules
// =============================

// --- Decay Physics ---
#include "G4DecayPhysics.hh"              ///< Handles particle decays
#include "G4RadioactiveDecayPhysics.hh"   ///< Handles radioactive isotope decays

// --- Electromagnetic Physics ---
#include "G4EmStandardPhysics_option3.hh" ///< EM physics with high-accuracy multiple scattering
#include "G4EmExtraPhysics.hh"            ///< Extra EM processes (gamma-nuclear, synchrotron, etc.)

// --- Hadronic Physics (Elastic & Inelastic) ---
#include "G4HadronElasticPhysicsHP.hh"    ///< High-precision neutron elastic scattering
#include "G4HadronElasticPhysicsXS.hh"    ///< Cross-section based hadron elastic scattering
#include "G4HadronPhysicsQGSP_BIC_HP.hh"  ///< Binary cascade + HP neutron physics
#include "G4HadronPhysicsFTFP_BERT_HP.hh" ///< FTFP_BERT with HP neutron models
#include "G4HadronPhysicsINCLXX.hh"       ///< Liège intranuclear cascade
#include "G4HadronInelasticQBBC.hh"       ///< Alternative inelastic hadronic physics

// --- Ion Physics ---
#include "G4IonElasticPhysics.hh"         ///< Ion elastic scattering
#include "G4IonPhysicsXS.hh"              ///< Ion inelastic physics using cross-section data
#include "G4IonINCLXXPhysics.hh"          ///< Ion inelastic physics using INCLXX model

// --- Stopping Physics ---
#include "G4StoppingPhysics.hh"           ///< Stopping of charged particles (e.g., muons)


// =============================
// PlasmaMLPALLASPhysics Class
// =============================

/**
 * @class PlasmaMLPALLASPhysics
 * @brief Modular physics list for PALLAS simulations.
 *
 * This class registers the set of physics processes used in the simulation.
 * The configuration prioritizes high-precision neutron physics and
 * detailed EM modeling for plasma and nuclear physics applications.
 *
 * Example usage:
 * @code
 * auto physicsList = new PlasmaMLPALLASPhysics();
 * runManager->SetUserInitialization(physicsList);
 * @endcode
 */
class PlasmaMLPALLASPhysics : public G4VModularPhysicsList
{
public:
    /// Constructor: registers the selected physics modules
    PlasmaMLPALLASPhysics();

    /// Destructor
    virtual ~PlasmaMLPALLASPhysics();

protected:
    /**
     * @brief Defines default production thresholds for secondary particles.
     *
     * Called automatically by Geant4 during initialization.
     * Adjust this method to modify cut values globally.
     */
    virtual void SetCuts() override;
};

#endif // PlasmaMLPALLASPhysics_h
