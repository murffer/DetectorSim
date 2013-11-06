Simulation of RPM8 with MCNPX
=============================

Simulation of the RPM8 geometries with MCNPX. Two differnet types of models are considered, a layered detector design and a wrapped cylinder design. The layered detector design consits of planar layers of detector material. The wrapped cylinders is a central cylinder that has detector layers wrapped around it.

A utility file, `MCNPMaterial.py` is included to assit in the creation of various mixtures of LiF loaded PEN and PS. It is expected to be used primary for the `GetPSLiF` method and the `GetPENLiF` methods. 

The TwoTubeData.txt is the results of the runs with two tubes of material.
   `cylinderPositions = ((4.23,10.16),(4.23,-10.16))`
    geoParam={'RPM8Size':12.7,'DetectorThickness':0.01,'DetectorSpacing':0.4,
               'CylinderLightGuideRadius':0.5,'CylinderRadius':2.5}

The ThreeTubeData.txt is the results fo the runs having three tubes of material in the detector. The cylinders were placed at one-thrid of the way into the detector in the x cordinate (4.23 cm), and at the center of the y-axis with the other two tubes being placed equidistant in the moderator at 7.625 and -7.625 cm
   `cylinderPositions = ((4.23,7.625),(4.23,0),(4.23,-7.625))`
The FourTubeData.csv is the results for having four tubes spaced equally in the RPM8 footprint, spaced equally.
	 `cylinderPositions = ((4.23,9.15),(4.23,3.05),(4.23,-3.05),(4.23,-9.15))`
