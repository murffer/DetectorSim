#!/bin/bash

echo "Cleaning up the queue files and combining histograms"
rm -f *.qsub.*

# PS Data output
if test -n "$(find . -maxdepth 1 -name 'PS_Neutron-0*.root' -print -quit)"
then
	hadd -f -v 1 PS_Neutron.root PS_Neutron-0*.root
	rm PS_Neutron-0*.root
fi
if test -n "$(find . -maxdepth 1 -name 'PS_Gamma-0*.root' -print -quit)"
then
	hadd -f -v 1 PS_Gamma.root PS_Gamma-0*.root
	rm PS_Gamma-0*.root
fi
