cmake_minimum_required(VERSION 4.1)
project(Trip)

set(CMAKE_CXX_STANDARD 20)

add_executable(Trip main.cpp
        System.cpp
        System.h
        Destination.h
        Train.h
        Router.h
        CityInput.h
        UI.h)
