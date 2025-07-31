 #!/bin/bash


for (( i=1 ; i<=8 ; i++ ))
do
    while [[ $(pgrep -x PALLAS_CollSim | wc -l) -gt 7 ]]
    do
	sleep 10
    done
    ./PALLAS_CollSim a_${i} 1000 vrml.mac &
    sleep 10
    done
