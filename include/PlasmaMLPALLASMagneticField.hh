#ifndef PlasmaMLPALLASMagneticField_h
#define PlasmaMLPALLASMagneticField_h 1

/**
 * @class PlasmaMLPALLASMagneticField
 * @brief Defines the magnetic field model for the PALLAS simulation.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This header declares the `PlasmaMLPALLASMagneticField` class, which extends 
 * Geant4's `G4MagneticField` to provide a configurable representation of the 
 * magnetic fields in the beamline, including dipole and quadrupole elements.
 *
 * Responsibilities:
 *  - Store and provide access to magnetic field parameters (dipole strength, quadrupole gradients, lengths, drifts)
 *  - Compute the field value at any given point for use in particle tracking
 *  - Allow toggling between constant-field mode and mapped-field mode
 *  - Provide fitting functions for magnetic field profile parameterization
 *
 * Key features:
 *  - Configurable for up to four quadrupoles (`NumQuadrupoles`)
 *  - Units consistent with CLHEP system (tesla, mm, etc.)
 *  - Command interface for runtime parameter changes
 *
 * @note This class is typically used by the geometry and tracking systems in 
 *       conjunction with Geant4's field propagation.
 */

// --- Includes ---
#include "G4MagneticField.hh"       ///< Base class for defining a magnetic field in Geant4
#include <array>                    ///< For fixed-size gradient and length storage
#include <CLHEP/Units/SystemOfUnits.h> ///< Units definitions (T, mm, etc.)

/// Number of quadrupoles in the beamline model
constexpr size_t NumQuadrupoles = 4;

/**
 * @class PlasmaMLPALLASMagneticField
 * @brief Implements the PALLAS magnetic field configuration for dipole and quadrupole elements.
 *
 * This class allows setting and retrieving the parameters of dipole and quadrupole fields
 * and calculates the magnetic field vector at any requested point.
 */
class PlasmaMLPALLASMagneticField : public G4MagneticField
{
public:
    /// Constructor
    PlasmaMLPALLASMagneticField();

    /// Destructor
    ~PlasmaMLPALLASMagneticField() override;

    /**
     * @brief Compute the magnetic field at a given location.
     * @param point Position (and time) array [x, y, z, t] in global coordinates
     * @param bField Output array [Bx, By, Bz] to store field components in tesla
     */
    void GetFieldValue(const G4double point[4], double *bField) const override;

    /**
     * @brief Set the constant dipole magnetic field value.
     * @param val Field strength in tesla
     */
    void SetDipoleField(G4double val);

    /**
     * @brief Set the gradient of a quadrupole.
     * @param index Quadrupole index (0-based, < NumQuadrupoles)
     * @param gradient Gradient value in tesla/meter
     */
    void SetGradient(size_t index, G4double gradient);

    /**
     * @brief Get the gradient of a quadrupole.
     * @param index Quadrupole index (0-based, < NumQuadrupoles)
     * @return Gradient value in tesla/meter
     */
    G4double GetGradient(size_t index) const;

    /**
     * @brief Set the length of a quadrupole.
     * @param index Quadrupole index (0-based, < NumQuadrupoles)
     * @param length Length in mm
     */
    void SetQLength(size_t index, G4double length);

    /**
     * @brief Get the length of a quadrupole.
     * @param index Quadrupole index (0-based, < NumQuadrupoles)
     * @return Length in mm
     */
    G4double GetQLength(size_t index) const;

    /**
     * @brief Set the drift distance before a quadrupole.
     * @param index Quadrupole index (0-based, < NumQuadrupoles)
     * @param drift Distance in mm
     */
    void SetQDrift(size_t index, G4double drift);

    /**
     * @brief Get the drift distance before a quadrupole.
     * @param index Quadrupole index (0-based, < NumQuadrupoles)
     * @return Drift distance in mm
     */
    G4double GetQDrift(size_t index) const;

    /**
     * @brief Enable or disable mapped magnetic field usage.
     * @param val True to use a field map, false for constant/parameterized fields
     */
    void SetMapBFieldStatus(G4bool val);

private:
    /**
     * @brief Define UI commands for field configuration.
     *
     * This allows runtime changes of field parameters through the Geant4 UI.
     */
    void DefineCommands();

    G4double ConstantDipoleBField = 0.0 * CLHEP::tesla; ///< Constant dipole field value
    std::array<G4double, NumQuadrupoles> gradients = {}; ///< Quadrupole gradients [T/m]
    std::array<G4double, NumQuadrupoles> qlength = {};   ///< Quadrupole lengths [mm]
    std::array<G4double, NumQuadrupoles> qdrift = {};    ///< Quadrupole drifts [mm]
    G4bool StatusMapBField = false;                      ///< Mapped-field usage flag
};

/**
 * @brief Fit function for magnetic field profiles.
 * @param x Position array (dependent variable)
 * @param par Parameter array for fit
 * @return Magnetic field value
 */
G4double fitFunction(G4double *x, G4double *par);

/**
 * @brief Symmetrized fit function along the Y-axis.
 * @param x Position array
 * @param par Parameter array
 * @return Magnetic field value
 */
G4double symmetrizedFunctionY(G4double *x, G4double *par);

/**
 * @brief Symmetrized fit function along the S-axis.
 * @param x Position array
 * @param par Parameter array
 * @return Magnetic field value
 */
G4double symmetrizedFunctionS(G4double *x, G4double *par);

#endif // PlasmaMLPALLASMagneticField_h
