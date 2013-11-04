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
	hadd -f -v 1 EJ426_Gamma.root EJ426_Gamma-0*.root
	rm EJ426_Gamma-0*.root
fi

# PS Data output
if test -n "$(find . -maxdepth 1 -name 'PSLiF_neutron-0*.root' -print -quit)"
then
	hadd -f -v 1 PSLiF_neutron.root PSLiF_neutron-0*.root
	rm PSLiF_neutron-0*.root
fi
if test -n "$(find . -maxdepth 1 -name 'PSLiF_gamma-0*.root' -print -quit)"
then
	hadd -f -v 1 PSLiF_Gamma.root PSLiF_Gamma-0*.root
	rm PSLiF_Gamma-0*.root
fi
