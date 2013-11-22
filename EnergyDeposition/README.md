EnergyDeposition
================

  Simulation of the energy deposition in a thin film.

## Geometry

The geometry of this simulation is a cylindrical detector mounted on a thin PMMA base. The source is a beam of particles shot perpendicular into the detector. Various materials are available for use for the simulation, including polystyrene and methyltyrene of various loadings. It should be notated that the loadings of the films could be improved to use a method that takes in the percent loading and either the chemical formula or compound name and calculates the materials from there.

## Run Macros

Two run macros are included in this for the simulation of neutrons and gammas. They are:

  + macros/nRun.mac
  + macros/gRun.mac
  
The other two macros (`macros/neutronSource.mac` and `macros/gammaSource.mac`) are utility scripts that setup the Co-60 source or a 0.025 eV neutron beam source.

## Compile and building

This project is setup to use the CMAKE tools as described previously. In addition, `subJobs.sh` can be used to submit jobs to the cluster. These jobs are not paralleled.

## Analysis and Histograms

The output of this simulation is a ROOT file that contains two histograms (when the histograms are activated). The first, `eDep`, is the total energy deposited in the event. The second, `posEDep`, is a 2D histogram of the position of first interaction and the corresponding total energy deposition. An analysis script, `analysis.py` has been written in python to assist in the summarizing and plotting of this histograms. It may be desirable to pipe the output of the python script directly into a file an allow the ROOT messages to print to the screen; this can be accomplished with `python analysis.py > output.csv`
