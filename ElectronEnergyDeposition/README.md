Electron Energy Deposition Simulation
=====================================

Simulation of energy deposition of electrons in materials. The material is divided into logarithmic bins, and the energy deposited in a step is accumulated for an event, and all of the events in a run are accumulated for an average value and variance.

## Geometry

The geometry is a box representing a 'semi infinite' homogeneous medium. The default geometry is constructed in DetectorConstruction class, but all of the above parameters can be changed interactively via the commands defined in the DetectorMessenger class.
  
## Run / Compiling

The code is setup to use the CMakeEnviroment distributed with GEANT4.
 
  - execute electronEDep in 'batch' mode from macro files
	% electronEDep   eDep.mac
 		
  - execute electronEDep in 'interactive mode' with visualization
	% electronEDep
	....
	Idle> type your commands
	....
	Idle> exit
		
## Histograms and Analysis

No histograms are produced in this simulation. Instead, the analysis of the simulation can be summarized into a csv with the provided python script, `analysis.py`. 
