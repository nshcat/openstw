cmake_minimum_required(VERSION 3.16)

project(openstw-sim LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(openstw-sim STATIC
  simulation.cpp
  simulation.h
)

target_link_libraries(openstw-sim PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_include_directories(openstw-sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(openstw-sim PRIVATE OPENSTW_SIM_LIBRARY)
