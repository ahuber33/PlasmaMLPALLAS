/**
 * @file PlasmaMLPALLASMagneticField.cc
 * @brief Implementation of the PlasmaMLPALLASMagneticField class and related fit functions.
 *
 * This file implements the PlasmaMLPALLASMagneticField class, providing magnetic field
 * definitions for the PALLAS PlasmaMLP simulation, including:
 *  - Dipole field (constant or mapped)
 *  - Quadrupole field contributions
 *  - Fit functions for modeling magnetic field profiles along Y and S axes
 *    using combinations of error functions and Gaussians.
 *
 * The class provides methods to:
 *  - Compute the magnetic field at any point in space.
 *  - Set or retrieve quadrupole gradients, lengths, and drift distances.
 *  - Enable or disable mapped-field mode.
 *  - Configure a constant dipole field when mapped-field mode is disabled.
 *
 * Fit functions:
 *  - fitFunction()        : Computes a combination of an error function step and a Gaussian.
 *  - symmetrizedFunctionY(): Reflects fitFunction about x0 = 0 for Y-axis symmetry.
 *  - symmetrizedFunctionS(): Reflects fitFunction about x0 = 3.4495 for S-axis symmetry.
 *
 * Magnetic field computation:
 *  - Uses dipole field or mapped-field mode depending on StatusMapBField.
 *  - Adds quadrupole contributions according to the configured gradients, lengths, and drifts.
 *
 * This implementation is compatible with Geant4 and CLHEP units.
 *
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr>
 * @author Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 */


#include "PlasmaMLPALLASMagneticField.hh"
#include "TMath.h"
#include "TF1.h"

//--------------------------------------
// Constructor / Destructor
//--------------------------------------

/**
 * @brief Default constructor.
 */
PlasmaMLPALLASMagneticField::PlasmaMLPALLASMagneticField() {}

/**
 * @brief Destructor.
 */
PlasmaMLPALLASMagneticField::~PlasmaMLPALLASMagneticField() {}

//--------------------------------------
// Fit Functions
//--------------------------------------

/**
 * @brief Fit function for magnetic field profile.
 *
 * Computes a combination of an error function step and a Gaussian depending on position.
 *
 * @param x Pointer to position array
 * @param par Pointer to parameter array
 * @return Calculated magnetic field value
 */
G4double fitFunction(G4double *x, G4double *par)
{
    if (!x || !par) return 0;

    G4double x0 = par[0];
    G4double amp_e = par[1];
    G4double x0_e = par[2];
    G4double sigma_e = par[3];
    G4double amp_g = par[4];
    G4double x0_g = par[5];
    G4double sigma_g = par[6];

    G4double stepF = 0;
    G4double gausF = 0;

    if (x[0] < x0)
        stepF = amp_e * TMath::Erf((x[0] - x0_e) / sigma_e);
    else
        gausF = amp_g * TMath::Exp(-0.5 * TMath::Power((x[0] - x0_g) / sigma_g, 2));

    return stepF + gausF;
}

/**
 * @brief Symmetrized fit function along Y axis.
 *
 * Reflects the function about x0 = 0 to enforce symmetry.
 *
 * @param x Pointer to position array
 * @param par Pointer to parameter array
 * @return Magnetic field value
 */
G4double symmetrizedFunctionY(G4double *x, G4double *par)
{
    if (!x || !par) return 0;

    G4double x0 = 0;
    G4double x_temp = 2 * x0 - x[0];

    return (x[0] < x0) ? fitFunction(&x_temp, par) : fitFunction(x, par);
}

/**
 * @brief Symmetrized fit function along S axis.
 *
 * Reflects the function about x0 = 3.4495 to enforce symmetry.
 *
 * @param x Pointer to position array
 * @param par Pointer to parameter array
 * @return Magnetic field value
 */
G4double symmetrizedFunctionS(G4double *x, G4double *par)
{
    if (!x || !par) return 0;

    G4double x0 = 3.4495;
    G4double x_temp = 2 * x0 - x[0];

    return (x[0] < x0) ? fitFunction(&x_temp, par) : fitFunction(x, par);
}

//--------------------------------------
// Magnetic Field Methods
//--------------------------------------

/**
 * @brief Compute the magnetic field at a given point.
 *
 * Computes dipole and quadrupole contributions based on current configuration.
 *
 * @param point Array [x,y,z,t] with position and time
 * @param bField Output array [Bx,By,Bz]
 */
void PlasmaMLPALLASMagneticField::GetFieldValue(const G4double point[4], G4double *bField) const
{
    G4double x = point[0];
    G4double y = point[1];
    G4double z = point[2];

    bField[0] = 0.;
    bField[1] = 0.;
    bField[2] = 0.;

    if (!StatusMapBField)
    {
        // Constant dipole approximation
        static TF1 ConstantS("ConstantS", "x > 3270 && x < 3599 ? 1 : 0", 3100, 3800);
        static TF1 ConstantY("ConstantY", "x > -150 && x < 150 ? 1 : 0", -1000, 1000);

        bField[0] = -ConstantDipoleBField * ConstantS.Eval(y) * ConstantY.Eval(z);
    }
    else
    {
        // Field map mode using fitted functions
        static TF1 fitFuncY("fitFuncY", symmetrizedFunctionY, -0.15, 0.15, 7);
        static TF1 fitFuncS("fitFuncS", symmetrizedFunctionS, 3.0995, 3.7995, 7);

        static bool paramsInitialized = false;
        if (!paramsInitialized)
        {
            fitFuncY.SetParameters(-1.05579 + 1.15, -0.985, -1.03649 + 1.15, 0.0307999, 721.501, -1.58778 + 1.15, 0.141887);
            fitFuncS.SetParameters(0.169992 + 3.4495, -0.806796, 0.193481 + 3.4495, 0.0405178, 1.9817, 0.0119007 + 3.4495, 0.0946281);
            paramsInitialized = true;
        }

        bField[0] = -fitFuncY.Eval(z / 1000) * fitFuncS.Eval(y / 1000) * CLHEP::tesla;
    }

    // Quadrupole contributions
    G4double Q1_begin = qdrift[0];
    G4double Q1_end = Q1_begin + qlength[0];
    G4double Q2_begin = Q1_end + qdrift[1];
    G4double Q2_end = Q2_begin + qlength[1];
    G4double Q3_begin = Q2_end + qdrift[2];
    G4double Q3_end = Q3_begin + qlength[2];
    G4double Q4_begin = Q3_end + qdrift[3];
    G4double Q4_end = Q4_begin + qlength[3];

    if (x > -20 && x < 20 && z > -20 && z < 20)
    {
        if (y > Q1_begin && y < Q1_end)
        {
            bField[0] = gradients[0]*(z*CLHEP::mm);
            bField[2] = -gradients[0]*(x*CLHEP::mm);
        }

        if (y > Q2_begin && y < Q2_end)
        {
            bField[0] = gradients[1]*(z*CLHEP::mm);
            bField[2] = -gradients[1]*(x*CLHEP::mm);
        }

        if (y > Q3_begin && y < Q3_end)
        {
            bField[0] = gradients[2]*(z*CLHEP::mm);
            bField[2] = -gradients[2]*(x*CLHEP::mm);
        }

        if (y > Q4_begin && y < Q4_end)
        {
            bField[0] = gradients[3]*(z*CLHEP::mm);
            bField[2] = -gradients[3]*(x*CLHEP::mm);
        }
    }
}

//--------------------------------------
// Setter and Getter Methods
//--------------------------------------

/**
 * @brief Set the constant dipole field.
 * @param fieldValue Dipole field in tesla
 */
void PlasmaMLPALLASMagneticField::SetDipoleField(G4double fieldValue)
{
    ConstantDipoleBField = fieldValue;
    G4cout << "Dipole Field = " << ConstantDipoleBField/CLHEP::tesla << " tesla" << G4endl;
}

/**
 * @brief Enable or disable mapped-field mode.
 * @param status True for mapped-field, false for constant/parameterized
 */
void PlasmaMLPALLASMagneticField::SetMapBFieldStatus(G4bool status)
{
    StatusMapBField = status;
}

/**
 * @brief Set gradient of a quadrupole.
 * @param index Quadrupole index (0-based)
 * @param gradient Gradient in tesla/m
 */
void PlasmaMLPALLASMagneticField::SetGradient(size_t index, G4double gradient)
{
    if (index < NumQuadrupoles)
    {
        gradients[index] = gradient;
        G4cout << "SET Q" << index + 1 << " Gradient : " << gradient / CLHEP::tesla * CLHEP::m << " tesla/m" << G4endl;
    }
}

/**
 * @brief Get gradient of a quadrupole.
 * @param index Quadrupole index (0-based)
 * @return Gradient in tesla/m
 */
G4double PlasmaMLPALLASMagneticField::GetGradient(size_t index) const
{
    if (index < NumQuadrupoles)
        return gradients[index];
    else
    {
        G4cerr << "Error: Invalid quadrupole index " << index << G4endl;
        return 0.0;
    }
}

/**
 * @brief Set the length of a quadrupole.
 * @param index Quadrupole index (0-based)
 * @param length Length in meters
 */
void PlasmaMLPALLASMagneticField::SetQLength(size_t index, G4double length)
{
    if (index < NumQuadrupoles)
    {
        qlength[index] = length;
        G4cout << "SET QLength" << index + 1 << " Length : " << length / CLHEP::mm << " mm" << G4endl;
    }
}

/**
 * @brief Get the length of a quadrupole.
 * @param index Quadrupole index (0-based)
 * @return Length in meters
 */
G4double PlasmaMLPALLASMagneticField::GetQLength(size_t index) const
{
    if (index < NumQuadrupoles)
        return qlength[index];
    else
    {
        G4cerr << "Error: Invalid quadrupole index " << index << G4endl;
        return 0.0;
    }
}

/**
 * @brief Set drift distance before a quadrupole.
 * @param index Quadrupole index (0-based)
 * @param drift Drift distance in meters
 */
void PlasmaMLPALLASMagneticField::SetQDrift(size_t index, G4double drift)
{
    if (index < NumQuadrupoles)
    {
        qdrift[index] = drift;
        G4cout << "SET QDrift" << index + 1 << " Length : " << drift / CLHEP::mm << " mm" << G4endl;
    }
}

/**
 * @brief Get drift distance before a quadrupole.
 * @param index Quadrupole index (0-based)
 * @return Drift distance in meters
 */
G4double PlasmaMLPALLASMagneticField::GetQDrift(size_t index) const
{
    if (index < NumQuadrupoles)
        return qdrift[index];
    else
    {
        G4cerr << "Error: Invalid quadrupole index " << index << G4endl;
        return 0.0;
    }
}
