cmake_minimum_required(VERSION 3.28)
project(3Dengine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 包含头文件目录
include_directories(${CMAKE_SOURCE_DIR}/math)
include_directories(${CMAKE_SOURCE_DIR}/render)
include_directories(${CMAKE_SOURCE_DIR}/core)

# 查找SDL2库
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# 查找SDL2_ttf库（通过pkg-config）
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
include_directories(${SDL2_TTF_INCLUDE_DIRS})
link_directories(${SDL2_TTF_LIBRARY_DIRS})

# 添加源文件
add_executable(3Dengine
        main.cpp
        math/Vector3.cpp
        math/Matrix4.cpp
        render/Render.cpp
        math/Point.cpp
        core/Camera.cpp
        core/Objects.cpp
        core/Shape.cpp
        core/Cube.cpp
        core/Ground.cpp
        core/Axis.cpp
        core/Football.cpp
)

# 链接SDL2库（关键！否则编译报错）
target_link_libraries(3Dengine ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# 构建后自动复制 assets 到可执行文件所在目录（解决跨平台字体路径问题）
add_custom_command(TARGET 3Dengine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:3Dengine>/assets
    COMMENT "复制 assets 资源文件到构建目录..."
)
