#!/bin/bash
data=($( shuf -i 75-1000 -n 6))
data2=($( shuf -i 75-450 -n 6))
#echo ${data[1]}
#echo ${data[2]}
cwd=$(pwd)
export PATH=$PATH:$(pwd)

NCLIENTS=10

for (( i=1; i<=$NCLIENTS; i++ ))
do
	xterm -geometry 150×32+${data[1]}+${data2[1]} -e bash -c 'command; RA_client' &
done
