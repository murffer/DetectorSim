#!/bin/bash

echo "Cleaning up all of the output files"
echo "   Removing ROOT output"
rm -f *.root
echo "   Removing submission output"
rm -f *.qsub.*
echo "   Removing run output"
rm -f MS_GammaOutput.txt MS_NeutronOutput.txt
