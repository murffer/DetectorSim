Electron Energy Deposition Simulation
=====================================

Simulation of energy deposition of electrons in materials.

Geometry
--------
The geometry is a box representing a 'semi infinite' homegeonous medium. The default geometry is constructed in DetectorConstruction class, but all of the above parameters can be changed interactively via the commands defined in the DetectorMessenger class.
 
Physics List
------------

The physics lists are from the the G4 kernal, with specialized cuts. The options of the physics list are:
   - "emstandard_opt0" recommended standard EM physics for LHC
   - "emstandard_opt1" best CPU performance standard physics for LHC
   - "emstandard_opt2"     
   - "emstandard_opt3" best current advanced EM options. 
  
  PhysicsList contains also G4Decay and G4RadioactiveDecay processes
             
  Physics lists and options can be (re)set with UI commands. A few commands have been added to PhysicsList, in order to set the production threshold for secondaries for gamma and e-/e+.
  
Run / Compiling
---------------
The code is setup to use the CMakeEnviroment distrubuted with GEANT4.
 
  - execute electronEDep in 'batch' mode from macro files
	% electronEDep   eDep.mac
 		
  - execute electronEDep in 'interactive mode' with visualization
	% electronEDep
	....
	Idle> type your commands
	....
	Idle> exit
		
Histograms and Analysis
-----------------------

No histograms are produced in this simulation. Instead, the analysis of the simulation can be summerized into a csv with the provided python script, `analysis.py`

