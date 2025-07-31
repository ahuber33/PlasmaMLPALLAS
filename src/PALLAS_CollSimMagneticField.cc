//// PALLAS_CollSimMagneticfield.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimMagneticField.hh"
#include "TMath.h"
#include "TF1.h"

PALLAS_CollSimMagneticField::PALLAS_CollSimMagneticField() {}
PALLAS_CollSimMagneticField::~PALLAS_CollSimMagneticField() {}

G4double fitFunction(G4double *x, G4double *par)
{
    if (!x || !par) return 0; // Vérification sécurité

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

G4double symmetrizedFunctionY(G4double *x, G4double *par)
{
    if (!x || !par) return 0; // Vérification sécurité

    G4double x0 = 0;
    G4double x_temp = 2 * x0 - x[0];

    return (x[0] < x0) ? fitFunction(&x_temp, par) : fitFunction(x, par);
}

G4double symmetrizedFunctionS(G4double *x, G4double *par)
{
    if (!x || !par) return 0; // Vérification sécurité

    G4double x0 = 3.4495;
    G4double x_temp = 2 * x0 - x[0];

    return (x[0] < x0) ? fitFunction(&x_temp, par) : fitFunction(x, par);
}

void PALLAS_CollSimMagneticField::GetFieldValue(const G4double point[4], G4double *bField) const
{
    G4double x = point[0];
    G4double y = point[1];
    G4double z = point[2];

    bField[0] = 0.;
    bField[1] = 0.;
    bField[2] = 0.;

    if (!StatusMapBField)
    {
        static TF1 ConstantS("ConstantS", "x > 3270 && x < 3599 ? 1 : 0", 3100, 3800);
        static TF1 ConstantY("ConstantY", "x > -150 && x < 150 ? 1 : 0", -1000, 1000);

        bField[0] = -ConstantDipoleBField * ConstantS.Eval(y) * ConstantY.Eval(z);
    }
    else
    {
        static TF1 fitFuncY("fitFuncY", symmetrizedFunctionY, -0.15, 0.15, 7);
        static TF1 fitFuncS("fitFuncS", symmetrizedFunctionS, 3.0995, 3.7995, 7);

        // Initialisation des paramètres une seule fois
        static bool paramsInitialized = false;
        if (!paramsInitialized)
        {
            fitFuncY.SetParameters(-1.05579 + 1.15, -0.985, -1.03649 + 1.15, 0.0307999, 721.501, -1.58778 + 1.15, 0.141887);
            fitFuncS.SetParameters(0.169992 + 3.4495, -0.806796, 0.193481 + 3.4495, 0.0405178, 1.9817, 0.0119007 + 3.4495, 0.0946281);
            paramsInitialized = true;
        }

        bField[0] = -fitFuncY.Eval(z / 1000) * fitFuncS.Eval(y / 1000) * CLHEP::tesla;
    }


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
            bField[0] = gradients[0] * z / 1000;
            bField[2] = gradients[0] * x / 1000;
        }

        if (y > Q2_begin && y < Q2_end)
        {
            bField[0] = gradients[1] * z / 1000;
            bField[2] = gradients[1] * x / 1000;
        }

        if (y > Q3_begin && y < Q3_end)
        {
            bField[0] = gradients[2] * z / 1000;
            bField[2] = gradients[2] * x / 1000;
        }

        if (y > Q4_begin && y < Q4_end)
        {
            bField[0] = gradients[3] * z / 1000;
            bField[2] = gradients[3] * x / 1000;
        }
    }
}

void PALLAS_CollSimMagneticField::SetDipoleField(G4double fieldValue)
{
    ConstantDipoleBField = fieldValue;
}

void PALLAS_CollSimMagneticField::SetMapBFieldStatus(G4bool status)
{
    StatusMapBField = status;
}

void PALLAS_CollSimMagneticField::SetGradient(size_t index, G4double gradient)
{
    if (index < NumQuadrupoles)
    {
        gradients[index] = gradient;
        G4cout << "SET Q" << index + 1 << " Gradient : " << gradient << " G/m" << G4endl;
    }
}

G4double PALLAS_CollSimMagneticField::GetGradient(size_t index) const
{
    if (index < NumQuadrupoles)
        return gradients[index];
    else
    {
        G4cerr << "Error: Invalid quadrupole index " << index << G4endl;
        return 0.0;
    }
}


void PALLAS_CollSimMagneticField::SetQLength(size_t index, G4double length)
{
    if (index < NumQuadrupoles)
    {
        qlength[index] = length;
        G4cout << "SET QLength" << index + 1 << " Length : " << length << " mm" << G4endl;
    }
}

G4double PALLAS_CollSimMagneticField::GetQLength(size_t index) const
{
    if (index < NumQuadrupoles)
        return qlength[index];
    else
    {
        G4cerr << "Error: Invalid quadrupole index " << index << G4endl;
        return 0.0;
    }
}


void PALLAS_CollSimMagneticField::SetQDrift(size_t index, G4double drift)
{
    if (index < NumQuadrupoles)
    {
        qdrift[index] = drift;
        G4cout << "SET QDrift" << index + 1 << " Length : " << drift << " mm" << G4endl;
    }
}

G4double PALLAS_CollSimMagneticField::GetQDrift(size_t index) const
{
    if (index < NumQuadrupoles)
        return qdrift[index];
    else
    {
        G4cerr << "Error: Invalid quadrupole index " << index << G4endl;
        return 0.0;
    }
}