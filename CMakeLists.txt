cmake_minimum_required(VERSION 3.29)
project(LargeDiceSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(LargeDiceSimulator main.cpp
        mainwindow.cpp
        mainwindow.h
        dicesimulator.cpp
        dicesimulator.h
)

target_link_libraries(LargeDiceSimulator PRIVATE Qt6::Widgets)
