cmake_minimum_required(VERSION 3.17)
project(Pex48Device)

set(CMAKE_CXX_STANDARD 14)

set(IXPIO_INCLUDE_DIR "../ixpio/include")
include_directories(${IXPIO_INCLUDE_DIR})


add_executable(Pex48Device main.cpp Pex48Device.cpp Pex48Device.h)