#!/bin/bash

echo "Cleaning up the queue files and combining histograms"
rm -f *.qsub.*

# GS20 Data output
if test -n "$(find . -maxdepth 1 -name 'GS20_Neutron-0*.root' -print -quit)"
then
	hadd -f -v 1 GS20_Neutron.root GS20_Neutron-0*.root
	rm GS20_Neutron-0*.root
fi
if test -n "$(find . -maxdepth 1 -name 'GS20_Gamma-0*.root' -print -quit)"
then
	hadd -f -v 1 GS20_Gamma.root GS20_Gamma-0*.root
	rm GS20_Gamma-0*.root
fi
