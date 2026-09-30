cmake_minimum_required(VERSION 3.29)
project(LargeDieSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(LargeDieSimulator main.cpp
        mainwindow.cpp
        mainwindow.h
        dicesimulator.cpp
        dicesimulator.h
)

target_link_libraries(LargeDieSimulator PRIVATE Qt6::Widgets)
