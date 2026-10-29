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
  nodes/connector.hxx nodes/connector.cxx
  nodes/signal.hxx nodes/signal.cxx
  nodes/axlecounter.hxx nodes/axlecounter.cxx
  nodes/boundary.hxx nodes/boundary.cxx
  nodes/bumper.hxx nodes/bumper.cxx
  nodes/simpleswitch.hxx nodes/simpleswitch.cxx
  nodes/frontbacknode.hxx nodes/frontbacknode.cxx
  elementcategory.hxx
  edge.hxx edge.cxx
  edgetype.hxx
  edges/track.hxx edges/track.cxx
  nodes/routedestination.hxx nodes/routedestination.cxx
  path.hxx path.cxx
  port.cxx
  utility.hxx
  pathsegment.hxx pathsegment.cxx
)

set(BOOST_MIN_VERSION 1.79.0)

find_package(
  Boost ${BOOST_MIN_VERSION} REQUIRED
)
target_link_libraries(openstw-topo PUBLIC
  Boost::boost
)

target_include_directories(openstw-topo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(openstw-topo PUBLIC pugixml-static)

target_link_libraries(openstw-topo PUBLIC openstw-core)

target_link_libraries(openstw-topo PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(openstw-topo PRIVATE OPENSTW_TOPO_LIBRARY)
