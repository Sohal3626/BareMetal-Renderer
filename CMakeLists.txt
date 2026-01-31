cmake_minimum_required(VERSION 4.1)
project(toyEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(toyEngine main.cpp
        include/Canvas.h
        include/Geometry.h
        include/Renderer.h
        src/Canvas.cpp
        src/Renderer.cpp
)
