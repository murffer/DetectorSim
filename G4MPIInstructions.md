The setup of Geant4 MPI on the necluster is explained.

--------
	ERROR: currently these instructions work for compiling but fail to work for the actually running of an example!
--------

Build Instructions
==================

	The G4 MPI libary chosen was the [/extended/parallel/MPI](http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/Examples_MPI.html) by Koichi Murakami. This was chosen because it does not require the dependances (TOP-c) of the other dependacies.

## Compile libG4mpi.so

In order to compile `libG4mpi.so` the source code in `$G4EXAMPLES/extended/parallel/MPI` needs to be built.  If the user does not have administrative privilages on the cluster it is suggested to copy the source files to a directory and build locally.

	cd /home/murffer
	cp -r $G4EXAMPLES/extended/parallel/MPI .
 	mkdir MPI/source/build
	cd MPI/source/build
  cmake -DGeant4_DIR=/opt/geant4/geant4-9.6.1/lib64/Geant4-9.6.1 /home/murffer/MPI/source
	make
	make install # May fail if not adminstrator

After the make finishes there should be `libG4mpi.so` in the build directory.

## Compile Examples

To compile the examples the CMakeLists.txt needs to be edited so that `cmake` can find library if it has been installed in a non-standard location. To do so edit


	IMPORTED_LOCATION_RELEASE "${Geant4_DIR}/../libG4mpi.so"
	
and replace it with

	IMPORTED_LOCATION_RELEASE "/home/murffer/MPI/source/build/libG4mpi.so"

It addition, one needs to copy the headers from `MPI/source/include` to the include directory of the respective example projects.

 	cd ~/MPI/source
 	cp include/* ../examples/exMPI01/include/
 	cp include/* ../examples/exMPI02/include/
