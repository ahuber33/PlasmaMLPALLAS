// #ifndef WITCH_GLOBAL_HH
// #define WITCH_GLOBAL_HH


// standard C include files
#include <stdio.h>
#include <stdlib.h>

// standard C++ include files
#include <sstream>
#include <iomanip>
#include <string>

using namespace std ;

// standard Geant4 include files
#include "globals.hh"

#include "G4ios.hh"
#include "G4Colour.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"

// CLHEP units
#include <CLHEP/Units/SystemOfUnits.h>

using CLHEP::millimeter;
using CLHEP::millimeter2;
using CLHEP::millimeter3;
using CLHEP::centimeter;
using CLHEP::centimeter2;
using CLHEP::centimeter3;
using CLHEP::meter;
using CLHEP::meter2;
using CLHEP::meter3;
using CLHEP::kilometer;
using CLHEP::kilometer2;
using CLHEP::kilometer3;
using CLHEP::parsec;
using CLHEP::micrometer;
using CLHEP::nanometer;
using CLHEP::angstrom;
using CLHEP::fermi;
using CLHEP::barn;
using CLHEP::millibarn;
using CLHEP::microbarn;
using CLHEP::nanobarn;
using CLHEP::picobarn;
using CLHEP::mm;
using CLHEP::um;
using CLHEP::nm;
using CLHEP::mm2;
using CLHEP::mm3;
using CLHEP::cm;
using CLHEP::cm2;
using CLHEP::cm3;
using CLHEP::m;
using CLHEP::m2;
using CLHEP::m3;
using CLHEP::km;
using CLHEP::km2;
using CLHEP::km3;
using CLHEP::pc;
using CLHEP::radian;
using CLHEP::milliradian;
using CLHEP::degree;
using CLHEP::steradian;
using CLHEP::rad;
using CLHEP::mrad;
using CLHEP::sr;
using CLHEP::deg;
using CLHEP::nanosecond;
using CLHEP::second;
using CLHEP::millisecond;
using CLHEP::microsecond;
using CLHEP::picosecond;
using CLHEP::hertz;
using CLHEP::kilohertz;
using CLHEP::megahertz;
using CLHEP::ns;
// using CLHEP::s;
using CLHEP::ms;
using CLHEP::eplus;
using CLHEP::e_SI;
using CLHEP::coulomb;
using CLHEP::megaelectronvolt;
using CLHEP::electronvolt;
using CLHEP::kiloelectronvolt;
using CLHEP::gigaelectronvolt;
using CLHEP::teraelectronvolt;
using CLHEP::petaelectronvolt;
using CLHEP::joule;
using CLHEP::MeV;
using CLHEP::eV;
using CLHEP::keV;
using CLHEP::GeV;
using CLHEP::TeV;
using CLHEP::PeV;
using CLHEP::kilogram;
using CLHEP::gram;
using CLHEP::milligram;
using CLHEP::kg;
using CLHEP::g;
using CLHEP::mg;
using CLHEP::watt;
using CLHEP::newton;
using CLHEP::hep_pascal;
using CLHEP::bar;
using CLHEP::atmosphere;
using CLHEP::ampere;
using CLHEP::milliampere;
using CLHEP::microampere;
using CLHEP::nanoampere;
using CLHEP::megavolt;
using CLHEP::kilovolt;
using CLHEP::volt;
using CLHEP::ohm;
using CLHEP::farad;
using CLHEP::millifarad;
using CLHEP::microfarad;
using CLHEP::nanofarad;
using CLHEP::picofarad;
using CLHEP::weber;
using CLHEP::tesla;
using CLHEP::gauss;
using CLHEP::kilogauss;
using CLHEP::henry;
using CLHEP::kelvin;
using CLHEP::mole;
using CLHEP::becquerel;
using CLHEP::curie;
using CLHEP::gray;
using CLHEP::candela;
using CLHEP::lumen;
using CLHEP::lux;
using CLHEP::perCent;
using CLHEP::perThousand;
using CLHEP::perMillion;

//----------------------------------------------------------------------
// CLHEP physics constants
#include <CLHEP/Units/PhysicalConstants.h>

using CLHEP::pi;
using CLHEP::twopi;
using CLHEP::halfpi;
using CLHEP::pi2;
using CLHEP::Avogadro;
using CLHEP::c_light;
using CLHEP::c_squared;
using CLHEP::h_Planck;
using CLHEP::hbar_Planck;
using CLHEP::hbarc;
using CLHEP::hbarc_squared;
using CLHEP::electron_charge;
using CLHEP::e_squared;
using CLHEP::electron_mass_c2;
using CLHEP::proton_mass_c2;
using CLHEP::neutron_mass_c2;
using CLHEP::amu_c2;
using CLHEP::amu;
using CLHEP::mu0;
using CLHEP::epsilon0;
using CLHEP::elm_coupling;
using CLHEP::fine_structure_const;
using CLHEP::classic_electr_radius;
using CLHEP::electron_Compton_length;
using CLHEP::Bohr_radius;
using CLHEP::alpha_rcl2;
using CLHEP::twopi_mc2_rcl2;
using CLHEP::k_Boltzmann;
using CLHEP::STP_Temperature;
using CLHEP::STP_Pressure;
using CLHEP::kGasThreshold;
using CLHEP::universe_mean_density;

// #endif
