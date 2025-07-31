# PALLAS_CollSim for PALLAS_Coll Experiment [huber@lp2ib.in2p3.fr, huberarnaud@gmail.com]

## INSTRUCTIONS TO USE THE SIMULATION
- Download the VMWare [Geant4.11.2.1](https://heberge.lp2ib.in2p3.fr/G4VM/index.html)

```
git clone https://github.com/ahuber33/PALLAS_Collimateurs_Simulation
```

- Go to build Folder and use this command :
```
 cmake -DGeant4_DIR=$G4COMP ../ -DOnnxRuntime_LIBRARY=../onnxruntime-linux-x64-1.17.1/lib/libonnxruntime.so.1.17.1
```  
then compile it with make

- The executable PALLAS_CollSim will be add to your bin folder

- If you want to have a visualization, launch this command : 
```
./PALLAS_CollSim [name of ROOT file ]
```  
It will generate x particle according to the vis.mac with QT and you will have a ROOT file with the name you gave located in the Resultats folder.

- If you want to have statistics without the visualization, use this command : 
```
./PALLAS_CollSim [name of ROOT file] [number of events generated] [name of macro] [FileReader ON/OFF] [MultiThreading ON/OFF] [number of threads]

```  
For example, if you want to use the "PALLAS_phasespace_qm(1411)_preCollimateur.txt" file, you need to activate the FileReader [ON]. In this file, there is 106435 lines so 106435 events to generated with some macroparticles associated. According to the number of threads used if MT is ON, the simulation will create a ROOT file for each thread and at the end of the simulation. All ROOT files will be merged together with a name correspoding to the name given in [name of ROOT file]. The temporary ROOT files will be removed after the merge.

Note that it's not necessary to indicate a [number of threads] if the condition on MT is OFF. In opposite, you need to put a value if MT is ON.

Concerning the macro, personnaly I used the vrml.mac but you can create another one. Just to remember that you need to write the name of your macro when you launch the simulation.

- In your macro, you need to specifiy some informations for the Messenger used :
```
######################################################
########## PART FOR MESSENGER INFORMATIONS ###########
######################################################
/geometry/SetQ1Length 0.1 m
/geometry/SetQ2Length 0.1 m
/geometry/SetQ3Length 0.2 m
/geometry/SetQ4Length 0.1 m
/geometry/SetSourceQ1Distance 0.173331 m
/geometry/SetQ1Q2Distance 0.165062 m
/geometry/SetQ2Q3Distance 0.259041 m
/geometry/SetQ3Q4Distance 0.86536 m
/geometry/SetSourceCollimatorDistance 2884.5 mm 


/display/SetStatusDisplayCelluleGeometry false
/display/SetStatusDisplayLIFGeometry false
/display/SetStatusDisplaySection1Geometry true
/display/SetStatusDisplaySection2Geometry true
/display/SetStatusDisplaySection3Geometry false
/display/SetStatusDisplayCollimators true
/display/SetStatusDisplaySection4Geometry true
/display/SetStatusDisplaySection4DumpGeometry false
/run/reinitializeGeometry

/field/SetQ1Gradient -40.777059   #tesla/meter
/field/SetQ2Gradient 29.517133    #tesla/meter
/field/SetQ3Gradient -7.928288    #tesla/meter
/field/SetQ4Gradient 8.503759     #tesla/meter
/field/SetStatusMapBField false   #For the Dipole spectrometer
/field/SetConstantDipoleBField 0.65 tesla #0.4 tesla 0.75

/step/SetTrackingStatus true

```
- **/geometry/** corresponds to definition of specific volume & distance (here the Collimator) :
    - SetQ1Length define the length of 1st Quadrupole
    - SetQ2Length define the length of 2nd Quadrupole
    - SetQ3Length define the length of 3rd Quadrupole
    - SetQ4Length define the length of 4th Quadrupole
    - SetSourceQ1Distance define the distance between the source and the 1st Quadrupole
    - SetQ1Q2Distance define the distance between the 1st Quadrupole and the 2nd Quadrupole
    - SetQ2Q3Distance define the distance between the 2nd Quadrupole and the 3rd Quadrupole
    - SetQ3Q4Distance define the distance between the 3rd Quadrupole and the 4th Quadrupole
    - SetSourceCollimatorDistance define the distance between the source and the front of the 1st collimator
    - **IMPORTANT** You can find a **Geometry.cc** file where all the possible LogicalVolume are created and a **PALLAS_CollSimGeometryConstruction.cc** where these functions are call to construct the geometry.

- **/display/** manages if some part of the geometry are taken into account or no :
    - SetStatusDisplayCelluleGeometry for the "2 cells part"
    - SetStatusDisplayLIFGeometry for the LIF part
    - SetStatusDisplaySection1Geometry for the 2 first Quadrupole with ISOChamber
    - SetStatusDisplaySection2Geometry for Q3, Q4, ASMRemovalChamber, BreadboardRemovalChamber, ChassisRemovalChamber and ISOTubes
    - SetStatusDisplaySection3Geometry for ASMPoutre & YAGStation
    - SetStatusDisplaySection4Geometry for DipoleChamber, Dipole & 2 YAGs
    - SetStatusDisplaySection4DumpGeometry for shieldings, chassis & DiagsChamber
    - SetStatusDisplayCollimators for Vertical & Horizontal Collimators
    - /run/reinitializeGeometry is mandatory to take into account the messegner informations

- **/field/** manages the magnetic field inside the spectrometer :
    - SetStatusMapBField allows to the user to choose the application of a B Map (if available) or a constant B value.
    - SetConstantDipoleBField defines the value of magnetic field's Dipole if the SetStatusMapBField is set to false
    - SetQ1Gradient defines the gradient value of Q1
    - SetQ2Gradient defines the gradient value of Q2
    - SetQ3Gradient defines the gradient value of Q3
    - SetQ4Gradient defines the gradient value of Q4

- **/step/** manages the informations inside the SteppingAction :
    - SetTrackingStatus allows to user to activate/desactivate the tracking of particles. It can be useful for inputs verification.


- In your macro, you have also the possibility to use ParticleGun (for the generation of particle with inputs from text files) or GPS (useful for standard generation)
- If you want to use the ParticleGun, it is **mandatory** to use **/gun** messenger in your macro :
```
######################################################
##### PART TO SIMULATE PARTICLE WITH PARTICLEGUN #####
######################################################
/gun/SetStatusONNX true
/gun/SetParticleName e-
```
- **/gun/** manages the PrimaryGeneratorAction with GunParticle :
    - SetStatusONNX defines if the simulation will use this model to generate particles or not. If not, you will need to activate the FileReader parameter if you want to generate a bunch of particles similar at what happend in PALLAS configuration. If not, you will have a "normal" generation of particles according to the part wrote in PALLAS_CollSimPrimaryGeneratorAction.cc
    - SetParticleName defines the particle 
    
- If you want to use the ONNX model, you need to specify the inputs parameter needed by the ML model with **/laser/** commands :   
```
/laser/SetOffsetLaserFocus 558 # um
/laser/SetNormVecPotential 1.43
/laser/SetFracDopTargetChamber 0.0188 # %(/100)
/laser/SetPressure 58.6 #mbar
``` 
For more informations about theses parameters and the model, go check this article : https://link.aps.org/doi/10.1103/PhysRevAccelBeams.26.091302

- If you want to use GPS, you can use for example this kind of messengers :
```
/gps/number 1
/gps/particle e-
/gps/pos/type Point
/gps/pos/centre -0.152 2500.0 0.08 mm
/gps/energy 250. MeV
/gps/direction 0.0 1.0 0.0
```

- **IMPORTANT** : According to the PALLAS step file, the particle propagation is along the y-axis !!!

- Based on G4EmStandardPhysics_option3.

- DO NOT HESITATE TO REPORT BUGS OR ANY IDEAS THAT WILL IMPROVE THE SIMULATION !!!!


## ACCESS TO SIMULATION INFORMATIONS

- 6 Trees can be created in the **BeginOfRunAction** :
    -   **Tree_GlobalInput** with the global inputs information from simulation :
        - Display_Cellule : Activation [y/n] of Cellule display
        - Display_LIF : Activation [y/n] of LIF display
        - Display_Section1 : Activation [y/n] of Section1 display
        - Display_Section2 : Activation [y/n] of Section2 display
        - Display_Section3 : Activation [y/n] of Section3 display
        - Display_Section4 : Activation [y/n] of Section4 display
        - Display_Section4Dump : Activation [y/n] of Section4Dump display
        - Display_Collimators : Activation [y/n] of Collimators display
        - Q1_Length : Value of Q1 length in mm
        - Q2_Length : Value of Q2 length in mm
        - Q3_Length : Value of Q3 length in mm
        - Q4_Length : Value of Q4 length in mm
        - SourceQ1Distance : Distance between Source & Q1 in mm
        - Q1Q2Distance : Distance between Q1 & Q2 in mm
        - Q2Q3Distance : Distance between Q2 & Q3 in mm
        - Q3Q4Distance : Distance between Q3 & Q4 in mm
        - Q1_Grad : Value of Q1 gradient in T/m
        - Q2_Grad : Value of Q2 gradient in T/m
        - Q3_Grad : Value of Q3 gradient in T/m
        - Q4_Grad : Value of Q4 gradient in T/m
        - B_Dipole_Map : Activation [y/n] of the Dipole Map
        - B_Dipole : Value of Homogeneous Dipole BField in T if B_Dipole_Map is not activated
        - Xoff : ML Input parameter value of "laser focal position offset" in µm
        - p : ML Input parameter value of "pressure" in mbar
        - cN2 : ML Input parameter value of "dopant concentration" in %
        - A0 : ML Input parameter value of "normalized peak vector potential"
        - Ekin : ML Output parameter value for "beam Energy" in MeV
        - dEkin : ML Output parameter value for "beam spread energy" in %
        - Q : ML Output parameter value for "beam charge" in pC
        - epsb : ML Output parameter value for "beam emittance" in µm

    -   **Tree_Input** with the inputs information from particle generation :
        - x : X position red from input file
        - xoffset : Offset needed to be in the center of spectrometer tube according to step file. Currently, this value is fixed at -0.152 mm
        - xp : X momentum red from input file
        - y : Y position red from input file but coming from z information in simulation
        - yoffset : Offset needed in **z-Axis** to be in the center of spectrometer tube according to step file. Currently, this value is fixed at 0.08 mm
        - yp : X momentum red from input file but which correspond to zp in simulation
        - s : Gap between particle & reference particle in **z-Axis** according to input file but which corresponds to the position in **y-Axis** in simulation.
        - soffset : Corresponds to the **Y-Position**  of the spectrometer entrance. Currently, this value is fixed at 3114.5 mm
        - p : Z momentum red from input file but which correspond to yp in simulation
        - delta : Corresponds to the energy difference between a particle and the reference particle
        - energy : From delta information and Energy reference, the energy of a particle
        - Nevent : According to the macrocharge file, it corresponds to the number of particles generated for a given pahsespace configuration.


    - **Tree_HorizontalColl** with the informations concerning the energy deposited in Horizontal collimator:
        - particleID : ID of the particle 
            - e- =11
            - e+ = -11
            - gamma = 22
            - proton = 2212
            - neutron = 2112
            - for more informations, look at PDG encoding GEANT4
        - energy : Kinetic energy of the particle before the interaction
        - deposited_energy : Sum of all energy deposited 


    - **Tree_VerticalColl** with the informations concerning the energy deposited in Vertical collimator:
        - particleID : ID of the particle 
            - e- =11
            - e+ = -11
            - gamma = 22
            - proton = 2212
            - neutron = 2112
            - for more informations, look at PDG encoding GEANT4
        - energy : Kinetic energy of the particle before the interaction
        - deposited_energy : Sum of all energy deposited 

        
    - **Tree_BSYAG** with the informations of interacting particles with YAG when the magnet is OFF:
        - x_exit : X Position of the particle
        - y_exit : Y Position of the particle
        - z_exit : Z Position of the particle
        - parentID : ParentID of the particle
        - energy : Energy of the particle
        
    - **Tree_BSPECYAG** with the informations of interacting particles with YAG when the magnet is ON:
        - x_exit : X Position of the particle
        - y_exit : Y Position of the particle
        - z_exit : Z Position of the particle
        - parentID : ParentID of the particle
        - energy : Energy of the particle

- Each variables is initialized at the **BeginOfEventAction**
- You can find all the variable functions on the **PALLAS_ColSimEventAction.hh**
- Each Trees is filled (sometimes under conditions) at the **EndOfEventAction**
- If you want to access where the informations are extracted, go to **PALLAS_CollSimSteppingAction.cc**
- A ROOT file with [name of ROOT file] given at the launch is created and wrote at the **EndOfEventAction** 



## DRAW SIMULATION RESULTS

- After the simulation, you can obtain some graphic visualization of the results according to the Plot_2VD.cc file (Magnet OFF) or Plot_HV_devie.cc (Magnet ON).

- To run the program, run this command :
```
root '[name file].cc("[name of root file]")'
```

