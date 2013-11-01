#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(birksConstant)

option(USE_MPI "Use the GEANT4 MPI Libararies" OFF)
if(USE_MPI)
	set(USE_MPI ON)
	add_definitions(-DUSE_MPI)
endif(USE_MPI)
#----------------------------------------------------------------------------
# check MPI package...
if(USE_MPI)
	find_package(MPI REQUIRED)
	set(CMAKE_CXX_COMPILER mpicxx)
endif(USE_MPI)
#----------------------------------------------------------------------------
# Find ROOT package
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
set(Geant4_DIR $ENV{G4INSTALL_CMAKE})
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Add G4mpi library
#
if(USE_MPI)
	SET(Geant4_LIBRARIES ${Geant4_LIBRARIES} G4mpi)
	ADD_LIBRARY(G4mpi SHARED IMPORTED)
	SET_PROPERTY(TARGET G4mpi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
	SET_TARGET_PROPERTIES(G4mpi PROPERTIES
		IMPORTED_LOCATION_RELEASE "${Geant4_DIR}/../libG4mpi.so"
		IMPORTED_SONAME_RELEASE "libG4mpi.so"
	)
endif(USE_MPI)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(birksConstant birksConstant.cc ${sources} ${headers})
target_link_libraries(birksConstant ${Geant4_LIBRARIES} )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build birksConstant. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(birksConstant_SCRIPTS
  macros/co60Source.mac
  macros/gRun.mac
  macros/nRun.mac
  macros/neutronSource.mac
  macros/BirksConstant.mac
  vis.mac
  gui.mac
  )

foreach(_script ${birksConstant_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS birksConstant DESTINATION bin)

