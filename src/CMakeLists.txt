include_directories(${CMAKE_SOURCE_DIR})
cmake_minimum_required(VERSION 3.10)
project(NBodySim)
set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(NBodySim main.cpp)
target_link_libraries(NBodySim sfml-graphics sfml-window sfml-system)
