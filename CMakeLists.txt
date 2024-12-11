cmake_minimum_required(VERSION 3.10)
project(FloorThreeD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (OpenCV_DIR ${CMAKE_SOURCE_DIR}/Vendor/opencv/build)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS}/include)
link_directories(${OpenCV_LIB_DIR})

#custom
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/Vendor/glfw-3.4/include)
include_directories(${CMAKE_SOURCE_DIR}/include/imgui)
include_directories(${CMAKE_SOURCE_DIR}/include/rlImgui)

file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB IMGUI_SOURCES ${CMAKE_SOURCE_DIR}/src/imgui/*.cpp)

add_executable(main main.cpp ${SOURCES} ${IMGUI_SOURCES} src/tinyfiledialogs.c)

target_link_libraries(main ${OpenCV_LIBS} )
# Link the raylib static library
target_link_libraries(main ${CMAKE_SOURCE_DIR}/Vendor/libraylib.a)
target_link_libraries(main ${CMAKE_SOURCE_DIR}/Vendor/glfw-3.4/libglfw3.a)

if (APPLE)
    target_link_libraries(main "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()
#
