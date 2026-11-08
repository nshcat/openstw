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
  spdr/stelltisch.cxx
  spdr/stelltisch.hxx
  spdr/tile.hxx spdr/tile.cxx
  spdr/aperturestate.hxx spdr/aperturestate.cxx
  spdr/blinkmode.hxx
  spdr/aperturecolor.hxx
  spdr/aperture.hxx spdr/aperture.cxx
  spdr/lampposition.hxx spdr/lampposition.cxx
  spdr/featurelayer.hxx spdr/featurelayer.cxx
  spdr/featuretype.hxx
  spdr/featureorientation.hxx

  spdr/tilelocation.hxx spdr/tilelocation.cxx
  spdr/features/trackindicatorfeature.hxx spdr/features/trackindicatorfeature.cxx
  spdr/buttonstate.hxx
  spdr/button.hxx spdr/button.cxx
  spdr/buttonrole.hxx
  spdr/inputprocessor.hxx spdr/inputprocessor.cxx
  spdr/stellwerk.hxx spdr/stellwerk.cxx
  spdr/logging.hxx spdr/logging.cxx
  spdr/features/trackindicatorgeometry.hxx
)

target_link_libraries(openstw-spdr PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# Allow the project itself to directly include headers,
# while requiring other projects to prepend "spdr/" to their includes
# to avoid header name collisions.
target_include_directories(openstw-spdr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/spdr)
target_include_directories(openstw-spdr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(openstw-spdr PRIVATE OPENSTW_SPDR_LIBRARY)
target_link_libraries(openstw-spdr PUBLIC openstw-topo)
target_link_libraries(openstw-spdr PUBLIC openstw-interlocking)
target_link_libraries(openstw-spdr PUBLIC openstw-core)
