Light Quenching
===============

Simulates the light quenching of scintillators using the GEANT4 toolkit. The geometry is currently a simple box volume, using a polystyrene based scintillator. The physcs of the simulation include OpticalPhysics for the simulation of the light yield.  The plastic scintillator propereties are taken from the G4 simulatione example in `examples/extended/optical/wls`.  A PVT based scintillator is also provided, but without setting the `birks parameter` the quenching is not obseravable.

Several runs are provided for the user. They are:

  + alphaRun.mac - Alpha from 10 keV to 2.15 MeV
  + eRun.mac - Electrons from 10 keV to 2.15 MeV
  + tritonRun.mac - Tritons from 10 keV to 2.15 MeV
  + demoRun.mac - A representation of a gamma and neutron

In addition, for convience a script has been written, `jobSub.sh`, which allows for the submission of all of the runs to the cluster system.

The analysis is completed without the use of the Singleton analysis class. This mgiht be recovered by using a previous commit.
A root script is provided, `LightQuench.C` which may be invoked by root that compares distrubtion of number of photons created for the particles in demoRun.mac.  This script also generates .csv files which can then be plotted in ones favorite package.
