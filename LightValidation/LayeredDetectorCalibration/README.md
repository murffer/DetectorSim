Layered Detector Calibration
============================

Simulation of gammas and neutrons and the subsquent energy depsotion and optical photons of three layered detectors. 

## Geometry
The detector assembly is two thin films sandwhiched between three PMMA slices, mounted with optical grease. The entire assembly was wrapped with teflon and black tape. A fish tail light guide was used to collect the light.
The detector assembly is constructed by placing a number of volumes inside of each other. The largest volume for the detector is the teflon cladding. Inside the telfon cladding volume is slightly smaller PMMA volume to represent the arcylic.  Placed inside the PMMA volume are the detector slices.  The detector slices are actually slices of optical grease, which have the neutron absorbing detector inside of them.

## Histograms and Analysis
The analysis is completed through the GEANT4 analysis manager class, and set to use the ROOT output.  There are two histograms created:
	+ Number of optical photons created in detector
	+ Number of optical photons detected at PMT 

The number of optical photons detected in the PMT is found using a sensitive detector in the PMT, and the number of optical photons created is found by counting the number of optical photons in the stacking action.

If the user elected to build for the MPI library the following command may be used for combining the seperate ROOT files
  `hadd GS20_Neutron.root GS20_Neutron_*.root`

## Build Instructions
Options exists to build this a single core application or for multiple processes with the GEANT4 MPI library. To build for a single core application:

	 cd LayeredDetectorCalibration/
	 mkdir build; cd build;
	 cmake ..
	 make

To build for a multicore application one needs to turn on the `USE_MPI` preprocessor defination as follows:

	 cd LayeredDetectorCalibration/
	 mkdir build; cd build
	 cmake .. -DUSE_MPI=ON
	 make

## Run Instructions
If the user elected to build for the MPI libary then it is suggested to submit the job to the cluster through TOURQE/MAUI.
