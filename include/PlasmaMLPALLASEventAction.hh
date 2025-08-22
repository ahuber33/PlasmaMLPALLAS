#ifndef PlasmaMLPALLASEventAction_h
#define PlasmaMLPALLASEventAction_h 1

/**
 * @class PlasmaMLPALLASEventAction
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 * @brief Header file for event-level actions in PlasmaMLPALLAS, including per-event statistics structures.
 */

#include "G4UserEventAction.hh"
#include "PlasmaMLPALLASQuadrupoleUtils.hh"

class G4Event;

/**
 * @brief Structure to store per-event input particle data
 *
 * Holds position, direction, and energy information for the particle at the start of the event.
 */
struct RunTallyInput
{
    float x = 0.0;
    float xp = 0.0;
    float y = 0.0;
    float yp = 0.0;
    float z = 0.0;
    float zp = 0.0;
    float energy = 0.0;
};


/**
 * @brief Structure for collimators statistics
 *
 * Tracks interaction coordinates, energies between primary e- and collimators.
 */
struct RunTallyCollimators
{
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float energy = 0.0;
    bool flag =false;
    
    // Methods to set data
    void SetXInteraction(float d) { x =d; }
    void SetYInteraction(float d) { y =d; }
    void SetZInteraction(float d) { z =d; }
    void SetEnergy(float d) { energy =d; }
    void ActiveFlag(){flag=true;}
    void ResetFlag(){flag=false;}

    // Methods to access data
    float GetXInteraction() const { return x; }
    float GetYInteraction() const { return y; }
    float GetZInteraction() const { return z; }
    float GetEnergy() const { return energy; }
    bool GetFlag() {return flag;}

  
};

/**
 * @brief Structure for YAG detector statistics
 *
 * Tracks exit coordinates, particle IDs, energies, deposited energy, and flags.
 */
struct RunTallyYAG
{
    std::vector<float> x_exit;
    std::vector<float> y_exit;
    std::vector<float> z_exit;
    std::vector<int> parentID;
    std::vector<int> particleID;
    std::vector<float> energy;
    float deposited_energy = 0.0;
    std::vector<float> total_deposited_energy;
    G4bool flag = false;

    // Methods to add data
    void AddXExit(float d) { x_exit.push_back(d); }
    void AddYExit(float d) { y_exit.push_back(d); }
    void AddZExit(float d) { z_exit.push_back(d); }
    void AddParentID(int d) { parentID.push_back(d); }
    void AddParticleID(int d) { particleID.push_back(d); }
    void AddEnergy(float d) { energy.push_back(d); }
    void AddDepositedEnergy(float d) { deposited_energy += d; }
    void AddTotalDepositedEnergy(float d) { total_deposited_energy.push_back(d); }

    // Size accessors
    size_t XExitSize() const { return x_exit.size(); }
    size_t YExitSize() const { return y_exit.size(); }
    size_t ZExitSize() const { return z_exit.size(); }
    size_t ParentIDSize() const { return parentID.size(); }
    size_t ParticleIDSize() const { return particleID.size(); }
    size_t EnergySize() const { return energy.size(); }
    size_t TotalDepositedEnergySize() const { return total_deposited_energy.size(); }

    // Index accessors
    float GetXExit(size_t i) const { return x_exit.at(i); }
    float GetYExit(size_t i) const { return y_exit.at(i); }
    float GetZExit(size_t i) const { return z_exit.at(i); }
    int GetParentID(size_t i) const { return parentID.at(i); }
    int GetParticleID(size_t i) const { return particleID.at(i); }
    float GetEnergy(size_t i) const { return energy.at(i); }
    float GetTotalDepositedEnergy(size_t i) const { return total_deposited_energy.at(i); }

    // Flags
    void ActivateFlag() { flag = true; }
    void ResetFlag() { flag = false; }
    G4bool ReturnFlag() const { return flag; }

    void ResetDepositedEnergy() { deposited_energy = 0; }
    float GetDepositedEnergy() const { return deposited_energy; }
};

/**
 * @brief Event action class for PlasmaMLPALLAS
 *
 * Implements G4UserEventAction interface to handle per-event statistics,
 * including input particle, collimator, quadrupole, and detector data.
 */
class PlasmaMLPALLASEventAction : public G4UserEventAction
{
public:
    /** Constructor */
    PlasmaMLPALLASEventAction(const char *);
    
    /** Destructor */
    ~PlasmaMLPALLASEventAction();

    /** Called at the beginning of each event */
    void BeginOfEventAction(const G4Event *);
    
    /** Called at the end of each event */
    void EndOfEventAction(const G4Event *);

    /** Setters for input particle data */
    void SetXStart(G4float d) { StatsInput.x = d; }
    void SetXpStart(G4float d) { StatsInput.xp = d; }
    void SetYStart(G4float d) { StatsInput.y = d; }
    void SetYpStart(G4float d) { StatsInput.yp = d; }
    void SetZStart(G4float d) { StatsInput.z = d; }
    void SetZpStart(G4float d) { StatsInput.zp = d; }
    void SetEnergyStart(G4float d) { StatsInput.energy = d; }

    /** Setter for quadrupole energy */
    void SetEnergy(G4float d) { StatsQuadrupoles.energy = d; }

    /** Horizontal collimator flag and energy accessors */
    
    /** Vertical collimator flag and energy accessors */
    

    /** Accessors for quadrupole statistics */
    RunTallyQuadrupoles& GetStatsQuadrupoles() { return StatsQuadrupoles; }

        /** Accessors for generic detector statistics */
    RunTallyCollimators& GetVerticalCollimators() { return StatsVerticalColl; }
    RunTallyCollimators& GetHorizontalCollimators() { return StatsHorizontalColl; }

    /** Accessors for generic detector statistics */
    RunTallyYAG& GetBSYAG() { return StatsBSYAG; }
    RunTallyYAG& GetBSPECYAG() { return StatsBSPECYAG; }

private:
    TTree *EventTree;                        ///< ROOT tree for per-event data
    TBranch *EventBranch;                    ///< ROOT branch for event tree
    RunTallyInput StatsInput;                ///< Input particle statistics
    RunTallyQuadrupoles StatsQuadrupoles;    ///< Quadrupole statistics
    RunTallyCollimators StatsHorizontalColl; ///< Horizontal collimator statistics
    RunTallyCollimators StatsVerticalColl;   ///< Vertical collimator statistics
    RunTallyYAG StatsBSYAG;                  ///< Beam Stop YAG detector statistics
    RunTallyYAG StatsBSPECYAG;               ///< Beam Stop SPEC YAG detector statistics
    G4String suffixe;                        ///< Suffix for output naming
};

#endif
