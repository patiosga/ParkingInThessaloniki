cmake_minimum_required(VERSION 3.21)
project(ParkingInThess)

set(CMAKE_CXX_STANDARD 14)

add_executable(ParkingInThess main.cpp ParkingState.h ParkingState.cpp Car.cpp Car.h)
