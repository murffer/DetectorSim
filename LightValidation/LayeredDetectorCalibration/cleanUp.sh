#!/bin/bash

echo "Cleaning up the queue files and combining histograms"
rm -f *.qsub.*

# EJ-426 Data output
if test -n "$(find . -maxdepth 1 -name 'EJ426_neutron-0*.root' -print -quit)"
then
	hadd -f -v 1 EJ426_neutron.root EJ426_neutron-0*.root
	rm EJ426_neutron-0*.root
fi
if test -n "$(find . -maxdepth 1 -name 'EJ426_gamma-0*.root' -print -quit)"
then
	hadd -f -v 1 EJ426_Gamma.root EJ426_gamma-0*.root
	rm EJ426_gamma-0*.root
fi

# PS Data output
if test -n "$(find . -maxdepth 1 -name 'PS_neutron-0*.root' -print -quit)"
then
	hadd -f -v 1 PSLiF_neutron.root PS_neutron-0*.root
	rm PS_neutron-0*.root
fi
if test -n "$(find . -maxdepth 1 -name 'PS_gamma-0*.root' -print -quit)"
then
	hadd -f -v 1 PSLiF_Gamma.root PS_gamma-0*.root
	rm PS_gamma-0*.root
fi
