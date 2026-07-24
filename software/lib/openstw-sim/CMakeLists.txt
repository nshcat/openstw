cmake_minimum_required(VERSION 3.16)

project(openstw-sim LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(openstw-sim STATIC
  src/simulation.cxx
  src/simulation.hxx
  src/tile.hxx src/tile.cxx
  src/enums.hxx
  src/signal.hxx src/signal.cxx
  src/utility.hxx
  src/base.hxx
  src/tilepanel.hxx src/tilepanel.cxx
  src/utility.cxx
  src/isignalschirm.hxx src/isignalschirm.cxx
  src/hauptsignalschirm.hxx src/hauptsignalschirm.cxx
  src/vorsignalschirm.hxx src/vorsignalschirm.cxx
  src/imodifysignalschirm.hxx src/imodifysignalschirm.cxx
  src/tracksegment.hxx src/tracksegment.cxx
  src/directionarrows.hxx src/directionarrows.cxx
  src/erlaubnisfeld.hxx src/erlaubnisfeld.cxx
  src/zugnummernanzeige.hxx src/zugnummernanzeige.cxx
  src/zugnummer.hxx src/zugnummer.cxx
  src/feststellmelder.hxx src/feststellmelder.cxx
  src/zugdeckungssignalschirm.hxx
  src/zugdeckungssignalschirm.cxx
  src/switch.hxx src/switch.cxx
  src/simpleswitch.hxx src/simpleswitch.cxx
  src/tileelement.hxx src/tileelement.cxx
)

target_link_libraries(openstw-sim PUBLIC pugixml-static)


set(BOOST_MIN_VERSION 1.79.0)

find_package(
  Boost ${BOOST_MIN_VERSION} REQUIRED
)
target_link_libraries(openstw-sim PUBLIC
  Boost::boost
)

target_link_libraries(openstw-sim PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_include_directories(openstw-sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_definitions(openstw-sim PRIVATE OPENSTW_SIM_LIBRARY)

target_link_libraries(openstw-sim PUBLIC openstw-topo)
