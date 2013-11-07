DetectorSim
===========

Simulation of Radiation Detectors with GEANT4 and MCNPX

## Simulated Physics and Detectors
  
These simulations are intended to highlight basic physics of scintillators

  + ParticleTracks - A demonstration of the track structure from different charged particles
  + RangeSim - Simulates the range of various charged particles
  + ElectronEnergyDeposition - Simulates the cumulative energy deposition for electrons in a material.
  + EnergyDeposition - Simulates the energy deposition in a material. This was used primary to determine the fraction of energy deposited by the Co-60 and the thermal neutrons.

The following simulations are intended to explore more complex interactions of a detector.

  + LightQuenching - Studies the light quenching in a simple scintillator
  + LightYieldEDep - The light yield as a function of energy deposition and film thickness

## Light Background
  
  + ScintillationSlab - A single scintillation bar. The optical photons are collected on a single PMT
  + WLSScintBar - A single scintillating bar sandwiched between two WLS shifting bars

## Light Validation

Simulations for the light validation of different detector designs. The simulations included are:

  + GS20Calibration
  + PSCalibration
  + LayeredDetectorValidation

## MCNPX Models

The MCNPX models are a layered detector model in which planes of detector material are stacked atop each other and a wrapped cylinder model in which concentric cylinders of material are simulated. The layered detector models is primary intended for plotting the results of the optimal geometries found by the genetic algorithm simulations.
