#!/bin/bash

# Setting up the default parameters
export NPS=1000000
export MAT=MS1
if [ $# -eq 2 ] 
then
  MAT=$1
  NPS=$2
elif [ $# -eq 1 ]
then
  MAT=$1
fi
# Checking for the right format
if [[ ! -n $G4WORKDIR ]]
then
  echo "G4WORKDIR is not defined"
  exit
fi
if [ "$NPS" -ne "$NPS" ] 2>/dev/null; then
  echo "USAGE: ./msRun MAT NPS"
  exit
fi
if [ "$MAT" -eq "$MAT" ] 2>/dev/null; then
  echo "USAGE: ./msRun MAT NPS"
  exit
fi
echo "Running $NPS particles through material $MAT"

#
# Setting up the job
#
function JobSetup()
{
  echo "#PBS -q gen1" > job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=1:ppn=2" >> job
  echo "export PBS_O_WORKDIR=$G4WORKDIR" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runGamma()
{
  JobSetup
  cp MSSamplesMacros/gRun.mac gRun_${NPS}_${MAT}.mac
  cat MSSamplesMacros/Thickness.mac >> gRun_${NPS}_${MAT}.mac
  # Replacing the number of paritlces and detecotr materials
  sed -i "s/NUMPARTICLES/$NPS/g" gRun_${NPS}_${MAT}.mac
  sed -i "s/DETECTORMAT/$MAT/g" gRun_${NPS}_${MAT}.mac
  echo "exec $G4WORKDIR/bin/Linux-g++/polymerFilm $G4WORKDIR/gRun_${NPS}_${MAT}.mac > MS_GammaOutput.txt" >> job
  mv job gSub.qsub
  qsub gSub.qsub
}
function runNeutron()
{
  JobSetup
  cp MSSamplesMacros/nRun.mac nRun_${NPS}_${MAT}.mac
  cat MSSamplesMacros/Thickness.mac >> nRun_${NPS}_${MAT}.mac
  # Replacing the number of paritlces and detecotr materials
  sed -i "s/NUMPARTICLES/$NPS/g" nRun_${NPS}_${MAT}.mac
  sed -i "s/DETECTORMAT/$MAT/g" nRun_${NPS}_${MAT}.mac
  echo "exec $G4WORKDIR/bin/Linux-g++/polymerFilm $G4WORKDIR/nRun_${NPS}_${MAT}.mac > MS_NeutronOutput.txt" >> job
  mv job nSub.qsub
  qsub nSub.qsub
}

# Running the jobs
runGamma
runNeutron
# Cleaning up
rm nSub.qsub
rm gSub.qsub

