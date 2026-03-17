cmake_minimum_required(VERSION 4.1)
project(3Dengine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/math)
include_directories(${CMAKE_SOURCE_DIR}/render)

# 查找SDL2库
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# 添加源文件
add_executable(3Dengine
        main.cpp
        math/Vector3.cpp
        math/Matrix4.cpp
        render/Render.cpp
        math/Point.cpp
)

# 链接SDL2库（关键！否则编译报错）
target_link_libraries(3Dengine ${SDL2_LIBRARIES})
