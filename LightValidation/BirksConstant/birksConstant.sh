#!/bin/bash
export G4WORKDIR=/home/murffer/G4DetectorSim/LightValidation/BirksConstant
#
# Setting up the job
#
function JobSetup()
{
  echo "#!/bin/bash" > job
  echo "#PBS -q gen2" >> job
  echo "#PBS -V" >> job
  echo "#PBS -l nodes=1:ppn=1" >> job
  echo "export PBS_O_WORKDIR=$G4WORKDIR" >> job
  echo 'cd $PBS_O_WORKDIR' >> job
}

function runGamma()
{
  JobSetup
  echo "exec $G4WORKDIR/build_${b}/birksConstant $G4WORKDIR/macros/gRun.mac > Birks_${b}_GammaOutput.txt" >> job
  mv job gSub.qsub
  qsub gSub.qsub
}
function runNeutron()
{
  JobSetup
  echo "exec $G4WORKDIR/build_${b}/birksConstant $G4WORKDIR/macros/nRun.mac > Birks_${b}_NeutronOutput.txt" >> job
  mv job nSub.qsub
  qsub nSub.qsub
}



birks=(0.01 0.012 0.014 0.016 0.018 0.02 0.025 0.03 0.035 0.04 0.045 0.05 0.06 0.07 0.08 0.09 0.1)
for b in "${birks[@]}"
do
  # Changing the Birks
  sed -i "s|G4double birksConstant.*|G4double birksConstant = ${b}*mm/MeV;|g" src/Material.cc
	dos2unix src/Material.cc

  # Recompiling
	mkdir build_${b}; cd build_${b};
	cmake ..
	make -j4
	cd ..

  # Running the calculation
	## Running the jobs
	runGamma
	runNeutron
	## Cleaning up
	rm nSub.qsub
	rm gSub.qsub
	
done
