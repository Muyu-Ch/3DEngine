cmake_minimum_required(VERSION 4.1)
project(3Dengine)

# 设置 C++20 标准（强制生效，避免编译器兼容问题）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含 math 目录（让 #include "Vector3.h" 生效）
include_directories(${CMAKE_SOURCE_DIR}/math)
# 补充包含 render 目录（Render.h 所在目录）
include_directories(${CMAKE_SOURCE_DIR}/render)

# 仅添加 .cpp 源文件（移除所有 .h 文件）
add_executable(3Dengine
        main.cpp
        math/Vector3.cpp
        math/Matrix4.cpp
        render/Render.cpp
        math/Matrix4.h
)
