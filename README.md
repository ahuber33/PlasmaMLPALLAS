# PALLAS Simulation – Plasma Beam Generation with Geant4 & ML Integration

**Project Authors:** Arnaud HUBER & Alexei SYTOV  
**Date:** 2024–2025  
**Collaboration:** GEANT4, PALLAS Project

---

## Table of Contents

- Introduction
- Features
- Dependencies
- Installation
- Building the Simulation
- Running the Simulation
- Primary Generator & ONNX Integration
- Magnetic Field Configuration
- Physics List
- Quadrupole Utilities
- User Interface Commands
- ROOT Output
- Architecture Diagram
- Documentation
- License

---

## Introduction

The PALLAS simulation is a Geant4-based framework designed for modeling plasma-based particle beams.  
It combines traditional Geant4 particle sources with machine learning-based ONNX inference for beam generation, and provides high-fidelity magnetic field and quadrupole tracking.

---

## Features

- Primary particle generation via:
  - ONNX ML model
  - Geant4 General Particle Source (GPS)
- Custom physics list optimized for plasma and nuclear physics:
  - High-precision hadronic and electromagnetic physics
  - Ion interactions and stopping physics
  - Decay and radioactive decay
- Magnetic field simulation:
  - Dipole and quadrupole contributions
  - Fitted field maps (error function + Gaussian)
- Quadrupole tracking utilities
- User interface commands for interactive parameter adjustment
- Multithreaded event generation with progress display
- Doxygen-documented codebase

---

## Dependencies

- Geant4 >= 11.1
- CLHEP (units & constants)
- ONNX Runtime
- C++17 or later
- Standard libraries: `<atomic>`, `<chrono>`, `<vector>`, `<string>`, `<cmath>`, `<memory>`

---

## Installation

```bash
git clone https://github.com/ahuber33/PlasmaMLPALLAS
```

---

## Building the Simulation

```bash
mkdir build && cd build
cmake -DGeant4_DIR=$G4COMP ../ -DOnnxRuntime_LIBRARY=../onnxruntime-linux-x64-1.17.1/lib/libonnxruntime.so.1.17.1
make -j$(nproc)
```

The executable `PlasmaMLPALLAS` will be added to your `bin` folder.

---

## Running the Simulation

- **With visualization:**

```bash
./PlasmaMLPALLAS [name_of_ROOT_file]
```

It will generate particles according to the macro (e.g., `vis.mac`) and produce a ROOT file in the `Resultats` folder.

- **Without visualization (statistics only):**

```bash
./PlasmaMLPALLAS [name_of_ROOT_file] [number_of_events] [macro_file] [MT ON/OFF] [number_of_threads]
```

**Notes:**
- If MT is ON, temporary ROOT files for each thread are merged at the end.
- If MT is OFF, no need to specify the number of threads.
- You can use any macro file (e.g., `vrml.mac`).

---

## Primary Generator & ONNX Integration

**Class:** `PlasmaMLPALLASPrimaryGeneratorAction`

**Modes:**
- ONNX ML-based beam generation
- GPS particle source

**Beam parameters:**
- `Ekin` – kinetic energy
- `dEkin` – energy spread
- `Q` – charge
- `epsb` – transverse emittance
- `x, z` – transverse positions
- `xp, zp` – angles

**Example UI commands:**

```bash
/PlasmaMLPALLAS/gun/setStatusONNX 1
/PlasmaMLPALLAS/gun/setParticleName electron
/PlasmaMLPALLAS/laser/setOffsetLaserFocus 0.5
/PlasmaMLPALLAS/laser/setNormVecPotential 1.2
/PlasmaMLPALLAS/laser/setFracDopTargetChamber 0.0188
/PlasmaMLPALLAS/laser/setPressure 58.6
```

---

## Magnetic Field Configuration

**Class:** `PlasmaMLPALLASMagneticField`

**Features:**
- Dipole field (constant or parameterized)
- Quadrupole contributions
- Field maps with symmetrized error function + Gaussian fits

**Quadrupole settings:**

```cpp
SetGradient(index, value)
SetQLength(index, length)
SetQDrift(index, drift)
```

---

## Physics List

**Class:** `PlasmaMLPALLASPhysics`  
Derived from `G4VModularPhysicsList`

**Includes:**
- High-precision hadronic and elastic models
- Electromagnetic physics (`G4EmStandardPhysics_option3`)
- Stopping physics
- Decay and radioactive decay

---

## Quadrupole Utilities

**File:** `PlasmaMLPALLASQuadrupoleUtils.cc`  

Update individual vector components of quadrupoles (position or momentum, start or end).

**Example:**

```cpp
SetQuadrupoleValue(stats, QuadID::Q1, PositionType::Begin, VectorType::Position, Axis::X, 0.1f);
```

---

## User Interface Commands

- `/PlasmaMLPALLAS/gun/...` – Particle generation
- `/PlasmaMLPALLAS/laser/...` – ML laser parameters

**Controls:**
- ONNX enable/disable
- Particle name
- Laser focus offset, normalized vector potential, dopant fraction, chamber pressure

---

## ROOT Output

- `BeginOfRunAction` creates six trees:
    - `Tree_GlobalInput` – Simulation and ML parameters
    - `Tree_Input` – Particle initial positions, momenta, offsets
    - `Tree_HorizontalColl` – Energy deposited in horizontal collimator
    - `Tree_VerticalColl` – Energy deposited in vertical collimator
    - `Tree_BSYAG` – Particles interacting with YAG, magnet OFF
    - `Tree_BSPECYAG` – Particles interacting with YAG, magnet ON

- Variables are initialized in `BeginOfEventAction` and filled in `EndOfEventAction`.
- Particle propagation and interaction info is collected in `PlasmaMLPALLASSteppingAction.cc`.

---

## Architecture Diagram

```
+------------------------------+
|        PALLAS Simulation     |
+------------------------------+
            |
            v
+------------------------------+
|  PrimaryGeneratorAction      |
|  - GPS / ONNX ML Beam        |
+------------------------------+
            |
            v
+------------------------------+
|       ONNXInference          |
|  - GenerateBeam(params)      |
+------------------------------+
            |
            v
+------------------------------+      +-------------------------+
|   MagneticField (Dipole +    |<---->| Quadrupole Utilities    |
|   Quadrupoles / Fitted Maps) |      | - Set/Update Vectors   |
+------------------------------+      +-------------------------+
            |
            v
+------------------------------+
|    Physics List (EM + Had)   |
|  - G4EmStandardPhysics_option3|
|  - G4HadronPhysicsQGSP_BIC_HP|
|  - Ion, Stopping, Decay      |
+------------------------------+
            |
            v
+------------------------------+
|   Event Generation & Output  |
+------------------------------+
```

---

## Documentation

Full Doxygen documentation available.

**Generate with:**

```bash
doxygen Doxyfile
```

**Access via:** `docs/html/index.html`

---

## License

PALLAS Project – GEANT4 Collaboration  

See the LICENSE file for details.
