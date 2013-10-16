#!/bin/bash
export G4WORKDIR=/home/murffer/G4DetectorSim/LightQuenching

#
# Setting up the job
#
function JobSetup()
{
  echo "#!/bin/bash" > job
  echo "#PBS -q gen1" >> job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=1:ppn=2" >> job
  echo "export PBS_O_WORKDIR=$G4WORKDIR" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runElectron()
{
  JobSetup
  echo "exec $G4WORKDIR/build/lightYield $G4WORKDIR/eRun.mac > ElectronOutput.txt" >> job
  mv job eSub.qsub
  qsub eSub.qsub
}
function runTriton()
{
  JobSetup
  echo "exec $G4WORKDIR/build/lightYield $G4WORKDIR/tritonRun.mac > TritonOutput.txt" >> job
  mv job tSub.qsub
  qsub tSub.qsub
}
function runAlpha()
{
  JobSetup
  echo "exec $G4WORKDIR/build/lightYield $G4WORKDIR/alphaRun.mac > AlphaOutput.txt" >> job
  mv job aSub.qsub
  qsub aSub.qsub
}
function run7Li()
{
  JobSetup
  echo "exec $G4WORKDIR/build/lightYield $G4WORKDIR/7LiRun.mac > 7LiOutput.txt" >> job
  mv job 7LiSub.qsub
  qsub 7LiSub.qsub
}
function runDemo()
{
  JobSetup
  echo "exec $G4WORKDIR/build/lightYield $G4WORKDIR/demoRun.mac > DemoOutput.txt" >> job
  mv job dSub.qsub
  qsub dSub.qsub
}

# Running the jobs
runElectron
runTriton
runAlpha
run7Li
runDemo
# Cleaning up
rm eSub.qsub
rm aSub.qsub
rm tSub.qsub
rm dSub.qsub
rm 7LiSub.qsub
