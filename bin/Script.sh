#!/bin/bash

# # Ensemble des valeurs pour variable1 et variable2
# Material=(G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb)
# Thickness=(50 50 50 75 75 75 100 100 100 15 15 15 23 23 23 30 30 30 20 20 20 35 35 35 50 50 50)
# Ecart=(0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6)
# count=0

# # Chemin vers le fichier macro
# macro_file="vrml_base.mac"



# # Boucles pour les deux ensembles de valeurs
# for value1 in "${Thickness[@]}"; do

#     while [[ $(pgrep -x PALLAS_CollSim | wc -l) -gt 6 ]]
#     do
# 	sleep 1
#     done

#     value2=${Material[count]}
#     value3=${Ecart[count]}
#     count=$((count+1))
#     cp vrml_base.mac base_tmp_$count.mac

#     # Remplacer les valeurs dans le fichier macro
#     sed -e "s/%Thickness/$value1/g" base_tmp_$count.mac > base_tmp2_$count.mac
#     sed -e "s/%Material/$value2/g" base_tmp2_$count.mac > base_tmp3_$count.mac
#     sed -e "s/%Ecart/$value3/g" base_tmp3_$count.mac > base_$count.mac
#     rm base_tmp_$count.mac
#     rm base_tmp2_$count.mac
#     rm base_tmp3_$count.mac
    
#     ./PALLAS_CollSim VerticalConfiguration_${value2}_epaisseur_${value1}_ecartement_${value3} 1 base_$count.mac &

#     sleep 5
#   done
# done





# Ensemble des valeurs pour variable1 et variable2
MaterialVC=(G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb)
MaterialHC=(G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W)
ThicknessVC=(110 110 110 110 110 110 110 110 110 120 120 120 120 120 120 120 120 120 130 130 130 130 130 130 130 130 130 140 140 140 140 140 140 140 140 140 150 150 150 150 150 150 150 150 150)
ThicknessHC=(20 30 40 50 60 70 80 90 100 20 30 40 50 60 70 80 90 100 20 30 40 50 60 70 80 90 100 20 30 40 50 60 70 80 90 100 20 30 40 50 60 70 80 90 100)
DistanceVH=(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
OpenVC=(0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4)
OpenHC=(0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2 0.2)
count=0

# Chemin vers le fichier macro
macro_file="vrml_base.mac"



# Boucles pour les deux ensembles de valeurs
for value1 in "${MaterialVC[@]}"; do

    while [[ $(pgrep -x PALLAS_CollSim | wc -l) -gt 25 ]]
    do
	sleep 1
    done

    value2=${MaterialHC[count]}
    value3=${ThicknessVC[count]}
    value4=${ThicknessHC[count]}
    value5=${DistanceVH[count]}
    value6=${OpenVC[count]}
    value7=${OpenHC[count]}
    value8=$((value3 + value4 + value5 + 50))
    count=$((count+1))
    cp vrml_base.mac base_tmp_$count.mac

    # Remplacer les valeurs dans le fichier macro
    sed -e "s/%VM/$value1/g" base_tmp_$count.mac > base_tmp2_$count.mac
    sed -e "s/%HM/$value2/g" base_tmp2_$count.mac > base_tmp3_$count.mac
    sed -e "s/%VCT/$value3/g" base_tmp3_$count.mac > base_tmp4_$count.mac
    sed -e "s/%HCT/$value4/g" base_tmp4_$count.mac > base_tmp5_$count.mac
    sed -e "s/%CVHD/$value5/g" base_tmp5_$count.mac > base_tmp6_$count.mac
    sed -e "s/%OVC/$value6/g" base_tmp6_$count.mac > base_tmp7_$count.mac
    sed -e "s/%OHC/$value7/g" base_tmp7_$count.mac > base_tmp8_$count.mac
    sed -e "s/%YPGO/$value8/g" base_tmp8_$count.mac > base_$count.mac
    rm base_tmp_$count.mac
    rm base_tmp2_$count.mac
    rm base_tmp3_$count.mac
    rm base_tmp4_$count.mac
    rm base_tmp5_$count.mac
    rm base_tmp6_$count.mac
    rm base_tmp7_$count.mac
    rm base_tmp8_$count.mac
    
    ./PALLAS_CollSim 2HVCollimators_V_${value1}_epaisseur_${value3}_ecart_${value6}_H_${value2}_epaisseur_${value4}_ecart_${value7}_Distance_${value5} 110000 base_$count.mac &

    sleep 5
  done





# Boucles pour les deux ensembles de valeurs
#for value1 in "${thickness[@]}"; do
#    for value2 in "${distance[@]}"; do

#    while [[ $(pgrep -x PALLAS_CollSim | wc -l) -gt 6 ]]
#    do
#	sleep 1
#    done

#    count=$((count+1))
#    cp vrml_base.mac base_tmp_$count.mac

    # Remplacer les valeurs dans le fichier macro
#    sed -e "s/%thickness/$value1/g" base_tmp_$count.mac > base_tmp2_$count.mac
#    sed -e "s/%distance/$value2/g" base_tmp2_$count.mac > base_$count.mac
#    rm base_tmp_$count.mac
#    rm base_tmp2_$count.mac
    
#    ./PALLAS_CollSim HorizontalConfiguration_epaisseur_${value1}_ecartement_${value2} 110000 base_$count.mac &

#    sleep 1
#  done
#done


