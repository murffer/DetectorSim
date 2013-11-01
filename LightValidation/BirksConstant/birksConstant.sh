#!/bin/bash

birks=(0.01 0.05 0.1)
for b in "${birks[@]}"
do
  # Changing the Birks
  sed -i -e "s|G4double birksConstant.*|G4double birksConstant = ${b}*mm/MeV;|g" src/Material.cc
  sed -i -e 's/^M//' src/Material.cc
  #cat src/Material.cc | col -b > src/Material.cc

  # Recompiling

  # Running the calculation
done
