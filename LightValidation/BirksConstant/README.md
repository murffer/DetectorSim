Birks Constant Simulation
=========================

Simulation of gammas and neutrons and the subsquent energy deposition, focusing on the effect that the Birks Constant has on the optical photon yield.

# Geometry
The detector is a simple box, into which the reaction products are shot. Once an optical photon is generated it's tracked is tallied and the particle is killed.

Histograms and Analysis
-----------------------
The analysis is completed through the GEANT4 analysis manager class, and set to use the ROOT output.  There is a single histogram created:

  + Number of optical photons created in detector

The number of optical photons created is found by counting the number of optical photons in the stacking action, and once counted the track is killed.

Build Instructions
-------------------
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

Run Instructions
----------------
If the user elected to build for the MPI libary then it is suggested to submit the job to the cluster through TOURQE/MAUI.
