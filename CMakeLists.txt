cmake_minimum_required(VERSION 3.10)
project(FloorThreeD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (OpenCV_DIR ${CMAKE_SOURCE_DIR}/Vendor/opencv/build)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

#custom
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)


add_executable(main main.cpp ${SOURCES})
target_link_libraries(main ${OpenCV_LIBS} )
# Link the raylib static library
target_link_libraries(main ${CMAKE_SOURCE_DIR}/Vendor/libraylib.a)

if (APPLE)
    target_link_libraries(main "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()
#
