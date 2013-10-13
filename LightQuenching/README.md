Light Quenching
===============

Simulates the light quenching of scintillators using the GEANT4 toolkit. The geometry is currently a simple box volume, using a polystyrene based scintillator. The physcs of the simulation include OpticalPhysics for the simulation of the light yield.  The plastic scintillator propereties are taken from the G4 simulatione example in `examples/extended/optical/wls`.  A PVT based scintillator is also provided, but without setting the `birks parameter` the quenching is not obseravable.

Several runs are provided for the user. They are:

  + alphaRun.mac - Alpha from 10 keV to 2.15 MeV
  + eRun.mac - Electrons from 10 keV to 2.15 MeV
  + tritonRun.mac - Tritons from 10 keV to 2.15 MeV
  + demoRun.mac - A representation of a gamma and neutron

The analysis is completed without the use of the Singleton analysis class. This mgiht be recovered by using a previous commit.
