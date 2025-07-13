cmake_minimum_required(VERSION 3.10)
project(NBodySim)
set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR}/src)
find_package(SFML REQUIRED COMPONENTS graphics window system)

add_executable(main2D main2D.cpp Particle.hpp)
add_executable(main_barneshut2d main_barneshut2d.cpp BarnesHut2D.cpp Particle.hpp)

target_link_libraries(main2D sfml-graphics sfml-window sfml-system)
target_link_libraries(main_barneshut2d sfml-graphics sfml-window sfml-system)
