#---Adding examples subdirectories explicitly 
#   and a custom target to for building all code bases -------------

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

add_subdirectory(EnergyDeposition)
add_subdirectory(EnergyDepTracking)

add_custom_target(DetectorSim DEPENDS EDepTracking EnergyDeposition)
