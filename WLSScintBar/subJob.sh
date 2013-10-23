#!/bin/bash
export G4WORKDIR=/home/murffer/G4DetectorSim/WLSScintBar

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

function runClad()
{
  JobSetup
  echo "exec $G4WORKDIR/build/scintSlab $G4WORKDIR/CladdingTest.mac > CladOutput.txt" >> job
  mv job CladdSub.qsub
  qsub CladdSub.qsub
}

function runWLS()
{
  JobSetup
  echo "exec $G4WORKDIR/build/scintSlab $G4WORKDIR/WLSCases.mac > WLSOutput.txt" >> job
  mv job WLSSub.qsub
  qsub WLSSub.qsub
}

# Running the jobs
runClad
runWLS
# Cleaning up
rm CladSub.qsub
rm WLSSub.qsub
