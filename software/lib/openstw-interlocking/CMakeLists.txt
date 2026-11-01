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
  interlocking/interlocking.cxx
  interlocking/interlocking.hxx
  interlocking/element.hxx interlocking/element.cxx
  interlocking/elementtype.hxx
  interlocking/elements/tracksection.hxx interlocking/elements/tracksection.cxx
  interlocking/axlecounterlink.hxx interlocking/axlecounterlink.cxx
  interlocking/axlecounterfacing.hxx interlocking/axlecounterfacing.cxx
  interlocking/axlecounteroperation.hxx
  interlocking/logging.hxx interlocking/logging.cxx
  interlocking/elements/signalmast.hxx interlocking/elements/signalmast.cxx
  interlocking/elements/lockableelement.hxx interlocking/elements/lockableelement.cxx
  interlocking/lockingreason.hxx
  interlocking/lockingreason.cxx
  interlocking/elements/signal.hxx interlocking/elements/signal.cxx
  interlocking/hauptsignalschirm.hxx interlocking/hauptsignalschirm.cxx
  interlocking/hauptsignalbild.hxx
  interlocking/vorsignalbild.hxx
  interlocking/vorsignalschirm.hxx interlocking/vorsignalschirm.cxx
  interlocking/route.hxx interlocking/route.cxx
  interlocking/routestate.hxx
  interlocking/routesegment.hxx interlocking/routesegment.cxx
  interlocking/switchposition.hxx
  interlocking/elementtype.cxx
  interlocking/elements/switch.hxx interlocking/elements/switch.cxx
  interlocking/routesegmenttype.hxx
  interlocking/requesttype.hxx
  interlocking/request.hxx interlocking/request.cxx
  interlocking/requeststate.hxx
  interlocking/requestdenialreason.hxx
  interlocking/requests/setzugstrasserequest.cxx interlocking/requests/setzugstrasserequest.hxx
)

# Allow the project itself to directly include headers,
# while requiring other projects to prepend "interlocking/" to their includes
# to avoid header name collisions.
target_include_directories(openstw-interlocking PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/interlocking)
target_include_directories(openstw-interlocking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(openstw-interlocking PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(openstw-interlocking PRIVATE OPENSTW_INTERLOCKING_LIBRARY)

target_link_libraries(openstw-interlocking PUBLIC openstw-topo)
target_link_libraries(openstw-interlocking PUBLIC openstw-core)