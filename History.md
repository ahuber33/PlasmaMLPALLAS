## Commit #1 le 04/04/2024 [PALLAS_CollSim.0.0.0]
- Simulation issue de la version TP_Simulation.0.9.0 -> Commit initial avec changement des noms des fichiers et des variables
- Nettoyage complet de toutes les fonctions/variables non nécessaires à cette simulation
- Base de travail pour la simulation des collimateurs du projet PALLAS situés avant le spectromètre

## Commit #2 le 04/04/2024 [PALLAS_CollSim.0.1.0]
- Suppression des fichiers Materials.cc & hh
- Définition des matériaux se fait directement dans le fichier Geometry.cc avec l'aide de la base NIST
- Continuité du nettoyage du code
- Ajout d'un fichier .gitignore

## Commit #3 le 05/04/2024 [PALLAS_CollSim.0.1.1]
- Continuité nettoyage code
- Ajout de fonctions dans le fichier PALLAS_CollSimGeometry pour définir les couleurs ainsi que le World & Holder afin de gagner en visibilité et compression du code.
- Définition de volumes basiques d'intérêts (FrontOutput, BackOutput entourant le collimateur) permettant une étude préliminaire
- Creation de 3 Tree distincts pour pouvoir déterminer ce qui se passe dans le collimateur et ce qui en ressort en arrière et en avant
- Code en cours d'écriture pour accès aux informations dans le SteppingAction et fonctions en cours de développement

## Commit #4 le 09/04/2024 [PALLAS_CollSim.0.2.0]
- Correction bugs divers
- Ensemble des fonctions définis dans SteppingAction pour récupérer l'ensemble des informations nécessaires (pour le moment) avec prévisions également pour la suite.
- La création du string "GetProcessName" entraine un segmentation fault si le creatorprocess n'est pas crée. Il est donc indispensable de vérifier que celui-ci existe bien avant de faire appel à cette fonction (possible également en s'affranchissant juste du ParentID=0 qui n'a pas de processus créateur)
- 3 fonctions principales (UpdateCollimatorInformations, UpdateCollimatorFrontInformations & UpdateBackCollimatorInformations) permettant de remplir les 3 trees de stockage de données correspondant aux informations physique de ce qu'il se passe dans le collimateur, de ce qui ressort en face avant et en face arrière.
- Simulation simplifiée avec passage du cut physique à 1mm contre 1µm précédemment.

## Commit #5 le 02/05/2024 [PALLAS_CollSim.0.2.1]
- Commit provisoire de sauvergarde pendant la mise en place des Messenger

## Commit #6 le 07/05/2024 [PALLAS_CollSim.0.2.2]
- Implémentation des éléments de géométrie de la ligne PALLAS via plusieurs fichiers GDML. Ces fichiers ont été définis à partir des fichiers géométrie .step, maillé et tesselated via FREECAD. Pour éviter d'avoir des fichiers trop gros et afin d'avoir juste une vision globale de la ligne, les fichiers GDML ont été simplifiés au maximum.
- TO DO : Terminer la mise en place des Messenger
- Nettoyer Le Materials !!!

## Commit #7 le 30/05/2024 [PALLAS_CollSim.0.3.0]
- Changement de l'architecture du code désormais basé sur l'utilisation de la classe G4Messenger plutôt que sur l'utilisation d'un fichier deconfig lu au début de la simulation
- Chacun des Messengers (PrimaryGeneractorAction et GeometryConstruction) est défini à l'initialization de chacune de ces classes et non pas via l'utilisation d'une nouvelle classe PALLAS_CollSimMessenger
- Le nombre d'évènements désirés défini dans la ligne de commande du lancement de la simulation est maintenant récupéré afin de pouvoir être utilisé dans la partie PrimaryGeneractorAction
- Le fichier Geometry ne contient plus de dépendance de elcture au fichier de config lors de son initialisation
- Effacement du fichier de config qui n'est plus utilisé
- Suite à l'importation de la classe G4GenericMessenger, les informations permettant de faire varier certains apramètres sont désormais indiqués directement dans la macro via les fonctions suivantes :
```
######################################################
########## PART FOR MESSENGER INFORMATIONS ###########
######################################################
/geometry/SetCollimatorThickness 10 mm
/geometry/SetCollimatorInternalRadius 10 mm
/geometry/SetCollimatorExternalRadius 100 mm
/geometry/SetCollimatorSpectrometerDistance 200 mm

/display/SetStatusDisplayCelluleGeometry false
/display//SetStatusDisplayLIFGeometry false
/display/SetStatusDisplaySection1Geometry false
/display/SetStatusDisplaySection2Geometry false
/display/SetStatusDisplaySection3Geometry false
/display/SetStatusDisplaySection4Geometry true
/display/SetStatusDisplaySection4DumpGeometry false
/run/reinitializeGeometry

/field/SetStatusMapBField false
/field/SetConstantBField 0.4 tesla

/step/SetTrackingStatus false
```  
- La partie /geometry permet de modifier des éléments de la géométrie
- La partie /display permet de gérer la prise en compte ou non de certains éléments de la géométrie
- La partie /field permet de gérer les informations liées au champ magnétique
- La partie /step permet d'activer ou non le suivi des particules en fonction de ce qui est désiré dans la simulation (vérification des inputs ou non)


- Une refonte du PrimaryGeneratorAction a également été faite afin de pouvoir générer des particules à partir d'un fichier de config (espace des phases 6d)
- Pour cela, la génération doit se faire via le GunParticle et non plus avec GPS 
- Ajout des fonctions permettant de lire le fichier d'espace de phase et de l'utiliser pour la génération des particules
- Chacunes des macroparticules générés selon ce fichier correspond à une certaine macrocharge qu'il convient de prendre en compte afin de pouvoir normaliser l'impact desdifférentes macroparticules
- Pour cela, une fonction a été ajoutée afin de lire le fichier de macrocharge, de le transformer en nombre équivalents d'électrons à générer.
- On peut ainsi obtenir pour chaque macroparticule généréé, son nombre de particules à générer.
- En résumé, chaque ligne du fichier correspond à une macroparticule auquel est associé une macrocharge. Chaque "Event" va alors avoir un certain nombre de particules générées en accorrd avec la macrocharge.
- Dans ce cas, le nombre d'évènements générés ne correspond alors plus au nombre total de particulees générées.
- Le nombre total de particules générées est calculé lors de la lecture des fichiers.
- Des fonctions supplémentaires utilisant l'ensemble de ces informations permet alors d'afficher sur le terminal l'avancement de la simulation ainsi que le temps estimé avant la fin de celle-ci.
- Le Messenger de la partie gun est donc celui-ci :
```
######################################################
##### PART TO SIMULATE PARTICLE WITH PARTICLEGUN #####
######################################################
/gun/SetStatusGunParticle false
/gun/SetParticleName geantino
/gun/SetEnergyReference 247 MeV
/gun/SetPhaseSpaceFilename PALLAS_phasespace_1411_preCollimateur.txt
/gun/SetMacroChargeFilename PALLAS_qm1411_preCollimateur.txt
```
- SetStatusGunParticle permet de définir si on utilise GunParticle (avec la lecture des ficheirs en input) ou si on utiliser GPS
- SetParticleName permet de définir le type des particules générées
- SetEnergyReference permet de définir l'énergie de référence du faisceau d'électrons
- SetPhaseSpaceFilename permet de définir le fichier utilisé pour l'espace de phase des particules
- SetMacroChargeFilename permet de définir le fichier utilisé pour les macrocharges


- Importation des fichiers GDML correspondant aux YAG en sortie de collimateur (ligne non déviée et déviée)

- Ajout d'un nouveau Tree "Input" permettant de visulaiser et de vérifier les inputs fournies par les fichiers de génération des particules.
- Création des fonctions permettant de remplir et d'accéder aux informations ainsi que celles permettant de remplir l'arbre.
- Ajout d'un nouveau Tree "YAG" prévu pour recueillir les informations issues de ce dernier.
- Création des fonctions permettant de remplir et d'accéder aux informations ainsi que celles permettant de remplir l'arbre.

- Mise à jour du README avec création de ce fichier History pour suivre l'évolution du code.
- Le fichier README gardant la vocation d'être la documentation du code.


## Commit #8 le 20/06/2024 [PALLAS_CollSim.0.3.1]
- Mise à jour de certains fichiers GDML qui entrainaient des soucis de tracking car surement pas parfaitment défini dans le fichier .stp
- Le fichier GDML DiagsChamber a d'ailleurs un souci qui a été corrigé en plaçant un cylindre de même diamètre et de même épaisseur à cet endroit
- Changement dans l'appel de la fonction GetCollimator() afin de pouvoir l'appeler plusieurs fois pour définir les différents collimateurs. Il est maintenant nécessaire de fournir un string à la fonction permettant d'avoir un nom différent à chaque appel
- Ajout de l'information parentID dans le tree YAG
- Changement dans la macro et dans le code permettant de générer de façon automatique un collimateur vertical et un autre horizontal. Le README a d'aileurs été modifié.
- Les simulations étant trop longues (>2 mois), la macro charge a été modifiée d'un facteur 100 (1.6e-17 au lieu de 1.6e-19) qui correspondait exactement à la charge d'un électron.


## Commit #9 le 15/07/2024 [PALLAS_CollSim.0.4.0]
- Changement de la structure du code pour pouvoir utiliser le multithreading
- Changement de la ligne de lancement de simulation qui doit désormais indiquer le nombre de threads à utiliser.
- Création d'un ActionInitialization afin de pouvoir implémenter le multithreading (obligatoire)
- Création d'un ParticleData.hh afin de pouvoir y stocker les structures de données nécessaires aux inputs
- Les fichiers sont désormais lus au lancement de la simulation. en fonction du nombre d'évènements désirés et du nombre de threads utilisés, les données sont réparties dans des listes d'attentes (queues) afin de pouvoir être lues ensuite dans le PrimaryGeneratorAction afin de générer les particules
- Les fichiers, lu au départ de la simualtion dans le main, sont écrits en dur dans le code désormais. Les Messenger dans le PrimaryGenerator ont été enlevés.
- La définition du champ magnétique a du être modifiée afin d'être thread-safe. La nouvelle méthode se base sur les exemples de GEANT4 avec la fonction ConstructSDandField(). Le volume LogicalBFieldVolume est ainsi crée et on associe ensuite dans la fonction SDandField le champ crée à ce volume logique. La défintion du champ magnétique est alors géré par les fichiers PALLAS_CollSimMagneticField.cc et hh
- Chaque thread lancé va créer son propre fichier root qui est protégé via des mutex (création, remplissage, écriture, fermeture). A la fin de la simulation, les différents fichiers sont mergés via la commande hadd vers un fichier dont le nom dépend de la variable affectée dans la ligne de lancement de simulation. A la suite du merge, les fichiers temporaires sont effacés.
- L'affichage de l'avancement de la simualation est basé sur le thread ID 0 (suffisant)

## Commit #10 le 31/07/2024 [PALLAS_CollSim.0.4.1]
- Changement de l'appel au RunManager dans le fichier main afin que le RunManagerMT soit appelé dans le cas où il y a le bon nombre d'arguments pour lancer une simulation et dans le cas contraire le RunManager de base est appelé afin de rendre les simulations avec visualisations plus rapides pour debug.
- Utilisation du 2e YAG sur l'axe devié avec utilisaiton du champ magnétique. Changement du nom de l'ensemble des variables correspondant au 1er YAG (YAG -> BSYAG) et création de l'ensemble des fonctions/variables correspondant au 2e YAG (BSPECYAG)
- Importation de la map réelle du champ B via les fichiers PALLAS_CollSimMagneticField. Map 3D avec ajustement selon les 2 axes où le champ peut varier (à aprtir du centre des deux axes) => Plot_BField.cc. Plus rapide pour l'accès à la valeur du champ d'utiliser une fonction avec la fonction Eval que de relire à chaque fois l'intégralité du tableau afin de savoir à quelle ligne on se situe pour lire la valeur correspondante de B.

## Commit #11 le 07/08/2024 [PALLAS_CollSim.0.4.2]
- Incorporation du modèle de Machine Learning développé à la base par l'IJCLab (Pierre Drobniak notamment) et mise en forme dans GEANT4 à l'aide de Alexey Sytov. Ce modèle permet de générer des outputs de génération d'électrons à partir d'inputs de laser/plasmas. Le modèle se base sur une importante base de données issues de nombreuses simulations PIC.
- Remise en forme du code afin de pouvoir traiter l'intégralité des configurations à savoir :
    - Simulation avec lecture de fichiers en input sans MT
    - Simulation avec lecture de fichiers en input avec MT
    - Simulation sans lecture de fichiers en input sans model ONNX
    - Simulation sans lecture de fichiers en input avec model ONNX
    - Simulation rapide avec GPS
- Pour cela, 2 flags (flag_FileReader & flag_MT) sont nécessaires par le biais de la ligne permettant le lancement de la simulation à savoir si c'est ON ou OFF. Si les paramètres ne sont pas bien définis où s'il manque une information, une G4Exception est affichée tout en arrêtant la simulation.

## Commit #12 le 30/09/2024 [PALLAS_CollSim.0.4.3]
- Corrections de bugs mineurs (Déclaration complète de certaines variables)
- Ajout de la prise en compte de l'énergie déposée dans les scintillateurs. Modification de l'algo en se basant sur le PostStep plutot que le PreStep. Précédemment, à aprtir du moment où une particule entrait en contact avec le scintillateur, on arrêtait la particule et on enregistrait son énergie. Hors cela ne prend pas en compte la possibilité que le gamma n'interagisse pas avec le scintillateur. Désormais, le suivi est activé dans le scintillateur.
- Optimisation de certaines parties de code par le biais de template (UpdateStatistics)
- Optimsisation et nettoyage de PALLAS_CollSim.cc
- Création de nouvelles variables et messengers pour la future implémentation des quadrupoles -> Changement dans les macros effectués
- SetConstantBField modifié en SetConstantDipoleBField.
- Modification des fichiers MagnetifField en accord avec les éléments précédents.

## Commit #13 le 30/09/2024 [PALLAS_CollSim.0.4.3]
- MAJ du README !!!

## Commit #14 le 10/12/2024 [PALLAS_CollSim.0.5.0]
- MAJ des chemins pour trouver les librairies de ONNX de façon automatique peu importe la session utilisée
- Nettoyage des Trees utilisés en début d'étude (RunTallyCollimator, RunTallyFrontCollimator, RunTallyBackCollimator) et des fonctions associées
- Ajout de 2 nouveaux Trees pour tracker l'énergie déposée dans les collimateurs (précédente méthode entrainait des erreurs à cause de la précision sur des grands nombres). Maintenant, à chaque particule, on enregistre quelle énergie est déposée dans chacun des collimateurs ce qui permet ensuite lors de l'analyse de sommer ces éléments.
- Ajout de 2 autres Trees permettant de regrouper les informations concernant le nombre d'itérations de cetains processus ayant eu lien au sein des collimateurs. Création des fonctions associées.
- Ajout de l'information ParticleID dans les Trees YAG et créationd es diverses fonctions d'accès
- Ajout de la physique neutronique pour pouvoir suivre la création et les interactions des neutrons dans la simulation notamment via les processus photonNuclear et Radioactivation ainsi que tous les processus en lien avec l'interaction neutronique
- Cela a entrainé une refonte du fichier Physics en utilisant dorénavant les fonctions "RegisterPhysics"

## Commit #14 le 17/03/2025 [PALLAS_CollSim.0.5.1]
- Suite au passage sur WSL (Windows), la visu ne marchait pas correctement à cause d'un overlap entre collimateurs et BFieldVolume. Afin de corriger cela et vu que le volume BField n'était plus nécessaire, celui-ci a été enlevé et n'est plus utilisé.

## Commit #15 le 09/04/2025 [PALLAS_CollSim.0.5.2]
- Correction de l'appel au champ magnétique. Résolution fuite mémoire et simplification du code
- Ajout des géométries réelles des 2 collimateurs -> étude énergie déposée dans les mors 
- Ajout Tree correspondant 
- Simplification appel des volumes GDML. 1 seule fonction de base pouvant se décliner sur les différents fichiers
    - Fonction GetGDMLVolume(const char* path, const char* VName, G4Material* material)

## Commit #16 le 28/07/2025 [PALLAS_CollSim.0.5.3]
- Retrait des Tree information pour étude Mors en vue simplificaiton code pour release Examples

## Commit #17 le 31/07/2025 [PALLAS_CollSim.0.5.4]
- Dernière version avant nettoyage pour application exemple GEANT4.
- Version tag pour retour et étude collimateurs si nécessaire
- Automatisation des placements des quadrupoles et des champs quadrupolaires via fichier de config/messenger
    - Couplage RunAction avec GeometryConstruction pour récupération information
- Enregistrement des informations globales du messenger dans un Tree spécifique
- Changement du fichier ONNX (correction Mykyta sur une normalisation de l'énergie des électrons)
- Modification fichier settings.json et autres (avec CMakeLists égalements) pour pouvoir correctement compiler sur VSCode et avoir une autocomplétion qui fonctionne
- Création d'un QuadrupoleVolume (vide) afin de pouvoir identifier les particules dans les champs quadrupolaires
- Modification de vis.mac et vrml.mac afin de prendre en compte les nouvelles fonctions du Messenger
- Replacement du collimateur vertical au centre (décalage présent avant du au fichier CAD)

## Commit #18 le 31/07/2025 [PALLAS_CollSim.0.5.5] -- RELEASE TAG Collimators_Study
- Modification du README pour être en accord avec modifications Commit #17