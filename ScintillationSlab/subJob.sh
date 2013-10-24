#!/bin/bash
export G4WORKDIR=/home/murffer/G4DetectorSim/LightValidation/ScintillationSlab

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

function run()
{
  JobSetup
  echo "exec $G4WORKDIR/build/scintSlab $G4WORKDIR/ptSrc.in > 25PTSRCOutput.txt" >> job
  mv job Sub.qsub
  qsub Sub.qsub
}

# Running the jobs
run
# Cleaning up
rm Sub.qsub
