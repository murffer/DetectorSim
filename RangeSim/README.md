Range Simulation
================

Simulation of ranges of alpha, tritons, electrons and photons.

## Geometry

The geometry is a box representing a 'semi infinite' homogeneous medium. The default geometry is constructed in DetectorConstruction class, but all of the above parameters can be changed interactively via the commands defined in the DetectorMessenger class.
 
## Physics List

The physics lists are from the the G4 kernel, with specialized cuts. The options of the physics list are:
   - "emstandard_opt0" recommended standard EM physics for LHC
   - "emstandard_opt1" best CPU performance standard physics for LHC
   - "emstandard_opt2"     
   - "emstandard_opt3" best current advanced EM options. 
  
  PhysicsList contains also G4Decay and G4RadioactiveDecay processes
             
  Physics lists and options can be (re)set with UI commands. A few commands have been added to PhysicsList, in order to set the production threshold for secondaries for gamma and e-/e+.
  
## Run / Compiling

The code is setup to use the CMakeEnviroment distributed with GEANT4.
 
  - execute rangeSim in 'batch' mode from macro files
	% rangeSim   runs.mac
 		
  - execute rangeSim in 'interactive mode' with visualization
	% rangeSim
	....
	Idle> type your commands
	....
	Idle> exit

Five run macros have been provided to simulate the ranges of various particles of interest. They are:
  + eRangeDist.mac - the distribution of an electron in polystyrene
  + erange.mac - the range of different energy electrons in polystyrene
  + gammaRange.mac - the range of gammas in polystyrene
  + rxnProductRange.mac - the range of the reaction products of pertinent neutron interactions

## Histograms and Analysis
 
rangeSim produces several histograms which are saved as rangeSim.root by default. Content of these histograms:  

 + track length of primary particle
 + number of steps primary particle
 + step size of primary particle
 + total energy deposit
 + energy of charged secondaries at creation
 + energy of neutral secondaries at creation      
      
The histograms are managed by G4AnalysisManager class and its Messenger. The histograms can be individually activated with the command :
   `/analysis/h1/set id nbBins  valMin valMax unit` 
where unit is the desired unit for the histograms (MeV or keV, deg or mrad, etc..)
   
One can control the name of the histograms file with the command:
   `/analysis/setFileName  name`  (default rangeSim)
   
It is also possible to print selected histograms on an ascii file:
   `/analysis/h1/setAscii id`
All selected histograms will be written on a file name.ascii  (default rangeSim)

A utility file, `Range.C` has been written to plot the charged particle creation energies for the alpha, triton, and Co60 runs.  This can be loaded into the root enviroment with:
    
    root[#] .L Range.C
    root[#] main()

