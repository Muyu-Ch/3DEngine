cmake_minimum_required(VERSION 4.1)
project(3Dengine)

set(CMAKE_CXX_STANDARD 20)

add_executable(3Dengine main.cpp
        math/Vector3.h
        math/Vector3.cpp
        math/Matrix4.cpp
        math/Matrix4.h
        render/Render.h
        render/Render.cpp)
