cmake_minimum_required(VERSION 3.16)

project(openstw-topo LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(openstw-topo STATIC
  topology.cxx
  topology.hxx
)

target_link_libraries(openstw-topo PUBLIC pugixml-static)

target_link_libraries(openstw-topo PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(openstw-topo PRIVATE OPENSTW_TOPO_LIBRARY)
