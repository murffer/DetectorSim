#---Adding examples subdirectories explicitly 
#   and a custom target to for building all code bases -------------

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

add_subdirectory(GS20Calibration)
add_custom_target(LightValidation DEPENDS lightYield)
