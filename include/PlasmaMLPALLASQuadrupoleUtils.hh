#ifndef QUADRUPOLEUTILS_HH
#define QUADRUPOLEUTILS_HH

/**
 * @class QuadrupoleUtils
 * @brief Data structures and utilities for managing quadrupole magnet simulation results.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This header defines simple vector and state structures used to store 
 * the position and momentum of particles as they pass through quadrupole magnets. 
 * It also provides enumerations for identifying quadrupoles and selecting 
 * specific components of their state, as well as a utility function for 
 * setting individual values.
 *
 * The data is typically recorded at both the entrance (Begin) and exit (End)
 * of each quadrupole, and can be used for analysis, diagnostics, and visualization
 * of beam dynamics in accelerator simulations.
 *
 * Structures:
 *  - Vector3: 3D vector for position or momentum.
 *  - QuadrupoleState: State of a single quadrupole (position & momentum at Begin/End).
 *  - RunTallyQuadrupoles: Collection of four quadrupoles plus total run energy.
 *
 * Enumerations:
 *  - QuadID: Identifies which quadrupole (Q1, Q2, Q3, Q4).
 *  - PositionType: Whether the value refers to the beginning or end of the quadrupole.
 *  - VectorType: Whether the vector represents position or momentum.
 *  - Axis: Selects a vector component (X, Y, Z).
 *
 * Function:
 *  - SetQuadrupoleValue: Assigns a specific vector component of a quadrupole's state.
 *
 * @note The implementation of SetQuadrupoleValue is in PlasmaMLPALLASQuadrupoleUtils.cc
 */

// Simple 3D vector structure to store x, y, z components
struct Vector3
{
    float x, y, z;
};

// Structure representing the state of a single quadrupole
// It stores the position and momentum vectors at both the beginning and end of the quadrupole
struct QuadrupoleState
{
    Vector3 begin;         // Position at the beginning of the quadrupole
    Vector3 end;           // Position at the end of the quadrupole
    Vector3 beginMomentum; // Momentum at the beginning
    Vector3 endMomentum;   // Momentum at the end
};

// Structure storing all quadrupoles in a run, plus additional statistics like energy
struct RunTallyQuadrupoles
{
    QuadrupoleState Q1;
    QuadrupoleState Q2;
    QuadrupoleState Q3;
    QuadrupoleState Q4;
    float energy; // Total energy of the system/run
};

// Enumeration to identify each quadrupole
enum class QuadID { Q1 = 1, Q2, Q3, Q4 };

// Enumeration to specify whether we are referring to the beginning or end of a quadrupole
enum class PositionType { Begin, End };

// Enumeration to select between position vector or momentum vector
enum class VectorType { Position, Momentum };

// Enumeration to select a vector component
enum class Axis { X, Y, Z };

// Function declaration for setting a specific component of a quadrupole vector
// Implementation is in PlasmaMLPALLASQuadrupoleUtils.cc
void SetQuadrupoleValue(RunTallyQuadrupoles &stats,
                        QuadID qid,
                        PositionType posType,
                        VectorType vecType,
                        Axis axis,
                        float value);

#endif
