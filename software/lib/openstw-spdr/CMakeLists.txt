cmake_minimum_required(VERSION 3.16)

project(openstw-spdr LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(openstw-spdr STATIC
  stelltisch.cxx
  stelltisch.hxx
  tile.hxx tile.cxx
  elementstate.hxx elementstate.cxx
  blinkmode.hxx
  elementcolor.hxx
  displayelement.hxx displayelement.cxx
  lampposition.hxx lampposition.cxx
  featurelayer.hxx featurelayer.cxx
  featuretype.hxx
  featureorientation.hxx
  featuretype.cxx
)

target_link_libraries(openstw-spdr PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(openstw-spdr PRIVATE OPENSTW_SPDR_LIBRARY)
target_link_libraries(openstw-spdr PUBLIC openstw-topo)
target_link_libraries(openstw-spdr PUBLIC openstw-interlocking)
target_link_libraries(openstw-spdr PUBLIC openstw-core)
