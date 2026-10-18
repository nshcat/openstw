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
  element.hxx element.cxx
  port.hxx
  node.hxx node.cxx
  nodetype.hxx
  connector.hxx connector.cxx
  signal.hxx signal.cxx
  axlecounter.hxx axlecounter.cxx
  boundary.hxx boundary.cxx
  bumper.hxx bumper.cxx
  simpleswitch.hxx simpleswitch.cxx
  frontbacknode.hxx frontbacknode.cxx
)

set(BOOST_MIN_VERSION 1.79.0)

find_package(
  Boost ${BOOST_MIN_VERSION} REQUIRED
)
target_link_libraries(openstw-sim PUBLIC
  Boost::boost
)

target_link_libraries(openstw-topo PUBLIC pugixml-static)

target_link_libraries(openstw-topo PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(openstw-topo PRIVATE OPENSTW_TOPO_LIBRARY)
