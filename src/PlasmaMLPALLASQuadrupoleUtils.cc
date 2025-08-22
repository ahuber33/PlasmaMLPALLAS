/**
 * @file PlasmaMLPALLASQuadrupoleUtils.cc
 * @brief Utility functions for updating quadrupole vectors in simulation statistics.
 *
 * This file provides helper functions to modify specific components of
 * quadrupole position or momentum vectors, either at the beginning or
 * the end of the quadrupole element during a simulation run.
 *
 * Functions in this file are intended to simplify bookkeeping of
 * quadrupole data in `RunTallyQuadrupoles` structures.
 * 
 * Example usage:
 * @code
 * SetQuadrupoleValue(stats, QuadID::Q1, PositionType::Begin, VectorType::Position, Axis::X, 0.1f);
 * @endcode
 *
 * @authors Arnaud HUBER
 * @authors Alexei SYTOV
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */

/**
 * @brief Sets a specific component of a quadrupole vector.
 *
 * Allows modification of a single axis (X, Y, Z) of either the position
 * or momentum vector for a given quadrupole, at either the beginning
 * or the end of the quadrupole element.
 *
 * @param stats    Reference to the `RunTally*


#include "PlasmaMLPALLASQuadrupoleUtils.hh"

// ============================================================
// SetQuadrupoleValue
// ============================================================
/**
 * @brief Sets a specific component of a quadrupole vector in the simulation statistics.
 *
 * This function allows fine-grained modification of a single vector component
 * (X, Y, or Z) for either the position or momentum vector of a quadrupole,
 * at the beginning or end of the element.
 *
 * @param stats    Reference to the `RunTallyQuadrupoles` structure containing all quadrupole data.
 * @param qid      Identifier of the quadrupole (`Q1`, `Q2`, `Q3`, `Q4`).
 * @param posType  Position type: whether to modify the vector at the beginning or end of the quadrupole.
 * @param vecType  Vector type: position or momentum.
 * @param axis     Component of the vector to modify (`X`, `Y`, `Z`).
 * @param value    The new value to assign to the selected component.
 */
void SetQuadrupoleValue(RunTallyQuadrupoles &stats,
                        QuadID qid,
                        PositionType posType,
                        VectorType vecType,
                        Axis axis,
                        float value)
{
    // Pointer to the selected quadrupole state
    QuadrupoleState *q = nullptr;

    // Select the quadrupole based on its ID
    switch (qid)
    {
        case QuadID::Q1: q = &stats.Q1; break;
        case QuadID::Q2: q = &stats.Q2; break;
        case QuadID::Q3: q = &stats.Q3; break;
        case QuadID::Q4: q = &stats.Q4; break;
    }

    // Pointer to the vector to modify (position or momentum)
    Vector3 *vec = nullptr;

    // Select vector based on position type (begin/end) and vector type (pos/momentum)
    if (posType == PositionType::Begin)
        vec = (vecType == VectorType::Position) ? &q->begin : &q->beginMomentum;
    else
        vec = (vecType == VectorType::Position) ? &q->end : &q->endMomentum;

    // Modify the specified component of the vector
    switch (axis)
    {
        case Axis::X: vec->x = value; break;
        case Axis::Y: vec->y = value; break;
        case Axis::Z: vec->z = value; break;
    }
}
