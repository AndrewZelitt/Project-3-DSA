# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Project-3-DSA")

# Include sub-projects.
add_subdirectory ("Project3app")

add_subdirectory("fltk")
