Range Simulation
================

Simulation of ranges of alpha, tritons, electrons and photons.

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
 
  - execute rangeSim in 'batch' mode from macro files
	% rangeSim   runs.mac
 		
  - execute rangeSim in 'interactive mode' with visualization
	% rangeSim
	....
	Idle> type your commands
	....
	Idle> exit
		
Histograms and Analysis
-----------------------
 
rangeSim produces several histo which are saved as testem1.root by default.Content of these histo:  

 1.  track length of primary particle
 2.  number of steps primary particle
 3.  step size of primary particle
 4.  total energy deposit
 5.  energy of charged secondaries at creation
 6.  energy of neutral secondaries at creation      
      
The histograms are managed by G4AnalysisManager class and its Messenger. The histos can be individually activated with the command :
   `/analysis/h1/set id nbBins  valMin valMax unit` 
where unit is the desired unit for the histo (MeV or keV, deg or mrad, etc..)
   
One can control the name of the histograms file with the command:
   `/analysis/setFileName  name`  (default testem1)
   
     
   It is also possible to print selected histograms on an ascii file:
   /analysis/h1/setAscii id
   All selected histos will be written on a file name.ascii  (default testem1)

A utility file, `ChargedParticleSecondaryEnergy.C` has been written to plot the chaged particle creation energies for the alpha, triton, and Co60 runs.  This can be loaded into the root enviroment with:
    
    root[#] .L ChargedParticleSecondaryEnergy.C
    root[#] main()

Upon the invocation of main the script will run
