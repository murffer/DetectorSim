EnergyDeposition

  Simulation of the energy deposition in a thin film.

  Setup Simulations
    msRun - runs methylstyrene samples of various thickness

TO RUN:
  > make GNUmakefile
  > ./bin/Linux-g++/polymerFilm

  To run the methylstyrene simulations:
  > ./msRun SAMPLENAME (i.e. MS1, MS2 .... MS6)

CONTENTS:
  
  MACROS:
    gui.mac - GUI for the GEANT4 
    vis.mac - Detector visulationization macro
    MSSamplesMacros/gRun.mac       - gamma gun macro 
    MSSamplesMacros/nRun.mac       - neutron gun macro
    MSSamplesMacros/Thickness.mac  - thickness runs

analysis.py
MS_GammaOutput.txt
MS_NeutronOutput.txt
msRun.sh
polymerFilm.cc
README
run.sh
subJobs.sh

analysis:
Makefile
PlotHistograms.C

bin:

include:
Analysis.hh
AnalysisMessenger.hh
CaloHit.hh
CaloSensitiveDetector.hh
DetectorConstruction.hh
DetectorMessenger.hh
EventAction.hh
HadronicPhysics.hh
PhysicsList.hh
PrimaryGeneratorAction.hh
RunAction.hh

macros:
co60Source.mac
gRun.mac
neutronSource.mac
nRun.mac


src:
Analysis.cc
AnalysisMessenger.cc
CaloHit.cc
CaloSensitiveDetector.cc
DetectorConstruction.cc
DetectorMessenger.cc
EventAction.cc
HadronicPhysics.cc
PhysicsList.cc
PrimaryGeneratorAction.cc
RunAction.cc

tmp:
Linux-g++