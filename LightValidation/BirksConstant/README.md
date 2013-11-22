Birks Constant Simulation
=========================

Simulation of gammas and neutrons and the subsquent energy deposition, focusing on the effect that the Birks Constant has on the optical photon yield in EJ426, GS20 and PSLiF.

The variation of the Birks constant is not completed by setting the material property parameters, but rather by using a script to replace the value in the source code and then recompiling and running. 
While this is definately not the elegant solution to this problem, setting the Birks parameter through the use of a DetectorMessenger did not seem to change the value used in the simulation.

## Geometry
The detector is a simple box, into which the reaction products are shot. Once an optical photon is generated it's tracked is tallied and the particle is killed.

## Histograms and Analysis
The analysis is completed through the GEANT4 analysis manager class, and set to use the ROOT output.  There is a single histogram created, the Number of optical photons created in detector.
The number of optical photons created is found by counting the number of optical photons in the stacking action, and once counted the track is killed.

## Build Instructions
Options exists to build this a single core application or for multiple processes with the GEANT4 MPI library. To build for a single core application:

	 cd BirksConstant/
	 mkdir build; cd build;
	 cmake ..
	 make

To build for a multicore application one needs to turn on the `USE_MPI` preprocessor defination as follows:

	 cd BirksConstant/
	 mkdir build; cd build
	 cmake .. -DUSE_MPI=ON
	 make

## Run Instructions
If the user elected to build for the MPI libary then it is suggested to submit the job to the cluster through TOURQE/MAUI. 
However, it is suggested that if they are running the paramater checks to use a single node. 
A BASH script has been provided, `birksConstant.sh` that can be run to vary the Birks constnat for the materials considered in the simulation. 
A python analysis script has also been provided, `analysis.py` to summerize the run output, saving the results to `results.csv`.
