Simulation of RPM8 with MCNPX
=============================

Simulation of the RPM8 geometries with MCNPX. Two differnet types of models are considered, a layered detector design and a wrapped cylinder design. The layered detector design consits of planar layers of detector material. The wrapped cylinders is a central cylinder that has detector layers wrapped around it.

A utility file, `MCNPMaterial.py` is included to assit in the creation of various mixtures of LiF loaded PEN and PS. It is expected to be used primary for the `GetPSLiF` method and the `GetPENLiF` methods. 
