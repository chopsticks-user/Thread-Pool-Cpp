


cmake_minimum_required(VERSION 3.15)

project(Ushi VERSION 1.0.0 LANGUAGES CXX)

include(CMake/Options.cmake)

# Library
include(CMake/CompilerFlags.cmake)

add_subdirectory(Source)
add_subdirectory(Examples)