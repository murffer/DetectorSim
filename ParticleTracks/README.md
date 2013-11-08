Tracking
========

Provides tracking of primary particle. Currently the defaults are an alpha (2.05 MeV), triton (2.78 MeV)  and two electrons (10 keV and 100 keV). Currently this is only intended to demonstrate the different particle track structures, and thus no analysis is provided.

## Compile Instructions

This program was built with CMAKE.  It is assumed that the environment is setup such that the variable `G4INSTALL_CMAKE` is set to the installation files for GEANT4 for cmake. The code can be built by the following commands:

  > mkdir build;
  > cd build;
  > cmake ..
  > make

The executable and run scripts will now be in the build directory.  The compiled code can be run with:

  > ./tracking
  > ./tracking run1.mac

The first will bring up an interactive session, while the second will run the four charged particles in the run script.

