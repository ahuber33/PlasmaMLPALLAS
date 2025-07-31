/// PlasmaAccelerationFacilitySimMagneticField.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PlasmaAccelerationFacilitySimMagneticField_h
#define PlasmaAccelerationFacilitySimMagneticField_h 1

#include "G4MagneticField.hh"
#include "globals.hh"
#include <array>
#include <CLHEP/Units/SystemOfUnits.h>

/// Taille du tableau de gradients
const size_t NumQuadrupoles = 4;

class PlasmaAccelerationFacilitySimMagneticField : public G4MagneticField
{
public:
    PlasmaAccelerationFacilitySimMagneticField();
    ~PlasmaAccelerationFacilitySimMagneticField() override;

    void GetFieldValue(const G4double point[4], double *bField) const override;

    void SetDipoleField(G4double val);
    void SetGradient(size_t index, G4double gradient);
    G4double GetGradient(size_t index) const;
    void SetQLength(size_t index, G4double length);
    G4double GetQLength(size_t index) const;
    void SetQDrift(size_t index, G4double drift);
    G4double GetQDrift(size_t index) const;
    void SetMapBFieldStatus(G4bool val);

private:
    void DefineCommands();
    G4double ConstantDipoleBField = 0.0 * CLHEP::tesla;
    std::array<G4double, NumQuadrupoles> gradients = {}; // tesla/meter
    std::array<G4double, NumQuadrupoles> qlength = {}; // mm
    std::array<G4double, NumQuadrupoles> qdrift = {}; // mm
    G4bool StatusMapBField = false;
};

// Déclaration des fonctions de fit
G4double fitFunction(G4double *x, G4double *par);
G4double symmetrizedFunctionY(G4double *x, G4double *par);
G4double symmetrizedFunctionS(G4double *x, G4double *par);

#endif
