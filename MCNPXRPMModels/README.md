Simulation of RPM8 with MCNPX
=============================

Simulation of the RPM8 geometries with MCNPX. Two different types of models are considered, a layered detector design and a wrapped cylinder design. The layered detector design consist of planar layers of detector material. The wrapped cylinders is a central cylinder that has detector layers wrapped around it.

A utility file, `MCNPMaterial.py` is included to assist in the creation of various mixtures of LiF loaded PEN and PS. It is expected to be used primary for the `GetPSLiF` method and the `GetPENLiF` methods. 

## Cylinder Optimization

The TwoTubeData.txt is the results of the runs with two tubes of material.
   `cylinderPositions = ((4.23,10.16),(4.23,-10.16))`
    geoParam={'RPM8Size':12.7,'DetectorThickness':0.01,'DetectorSpacing':0.4,
               'CylinderLightGuideRadius':0.5,'CylinderRadius':2.5}

The ThreeTubeData.txt is the results fo the runs having three tubes of material in the detector. The cylinders were placed at one-third of the way into the detector in the x coordinate (4.23 cm), and at the center of the y-axis with the other two tubes being placed equidistant in the moderator at 7.625 and -7.625 cm
   `cylinderPositions = ((4.23,7.625),(4.23,0),(4.23,-7.625))`
The FourTubeData.csv is the results for having four tubes spaced equally in the RPM8 footprint, spaced equally.
	 `cylinderPositions = ((4.23,9.15),(4.23,3.05),(4.23,-3.05),(4.23,-9.15))`

Optimization of the cylinders was completed using a matrix of positions. These results can be obtained by running the `CylinderMCNPX.py` with the appropriate arguments.

## Layered Detector

A brief script was written to analyze create input decks for the plotting of the optimal geometries, and for writing out the geometries.
The geometries can be made into a form suitable for latex by the following commands:

		 cat TableOutput.txt | cut -d':' -f1 --complement | tr '[:blank:]' ' & '  > output
		 cat output | sed 's/$/\\\\/g'


## Miller Irridiator

Simulation of the double well neutron irridiator in Pasqua. These simulations are used to validate between the simulated count rate and the measured count rate in order to ensure the correct material composition and the gain skills in using MCNPX simulations.

