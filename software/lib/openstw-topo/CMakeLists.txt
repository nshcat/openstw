cmake_minimum_required(VERSION 3.16)

project(openstw-topo LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Xml)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Xml)

find_package(Qt6 REQUIRED COMPONENTS Core Xml)

add_library(openstw-topo STATIC
  topo/topology.cxx
  topo/topology.hxx
  topo/element.hxx topo/element.cxx
  topo/port.hxx
  topo/node.hxx topo/node.cxx
  topo/nodetype.hxx
  topo/nodes/connector.hxx topo/nodes/connector.cxx
  topo/nodes/signal.hxx topo/nodes/signal.cxx
  topo/nodes/axlecounter.hxx topo/nodes/axlecounter.cxx
  topo/nodes/boundary.hxx topo/nodes/boundary.cxx
  topo/nodes/bumper.hxx topo/nodes/bumper.cxx
  topo/nodes/simpleswitch.hxx topo/nodes/simpleswitch.cxx
  topo/nodes/frontbacknode.hxx topo/nodes/frontbacknode.cxx
  topo/elementcategory.hxx
  topo/edge.hxx topo/edge.cxx
  topo/edgetype.hxx
  topo/edges/track.hxx topo/edges/track.cxx
  topo/nodes/routedestination.hxx topo/nodes/routedestination.cxx
  topo/path.hxx topo/path.cxx
  topo/port.cxx
  topo/utility.hxx
  topo/pathsegment.hxx topo/pathsegment.cxx
)

set(BOOST_MIN_VERSION 1.79.0)

find_package(
  Boost ${BOOST_MIN_VERSION} REQUIRED
)
target_link_libraries(openstw-topo PUBLIC
  Boost::boost
)

# Allow the project itself to directly include headers,
# while requiring other projects to prepend "topo/" to their includes
# to avoid header name collisions.
target_include_directories(openstw-topo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/topo)
target_include_directories(openstw-topo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(openstw-topo PUBLIC pugixml-static)

target_link_libraries(openstw-topo PUBLIC openstw-core)

target_link_libraries(openstw-topo PUBLIC Qt6::Core Qt6::Xml)

target_compile_definitions(openstw-topo PRIVATE OPENSTW_TOPO_LIBRARY)
