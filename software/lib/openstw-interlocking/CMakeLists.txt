cmake_minimum_required(VERSION 3.16)

project(openstw-interlocking LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(openstw-interlocking STATIC
  interlocking.cxx
  interlocking.hxx
)

target_include_directories(openstw-interlocking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(openstw-interlocking PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(openstw-interlocking PRIVATE OPENSTW_INTERLOCKING_LIBRARY)

target_link_libraries(openstw-interlocking PUBLIC openstw-topo)
target_link_libraries(openstw-interlocking PUBLIC openstw-core)