cmake_minimum_required(VERSION 3.16)

project(openstw-sim LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Xml)

add_library(openstw-sim STATIC
  src/simulation.cxx
  src/simulation.hxx
  src/logging.hxx src/logging.cxx
)

target_link_libraries(openstw-sim PUBLIC pugixml-static)


set(BOOST_MIN_VERSION 1.79.0)

find_package(
  Boost ${BOOST_MIN_VERSION} REQUIRED
)
target_link_libraries(openstw-sim PUBLIC
  Boost::boost
)

target_link_libraries(openstw-sim PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt6::Xml)
target_include_directories(openstw-sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_definitions(openstw-sim PRIVATE OPENSTW_SIM_LIBRARY)

target_link_libraries(openstw-sim PUBLIC openstw-topo)
target_link_libraries(openstw-sim PUBLIC openstw-interlocking)
target_link_libraries(openstw-sim PUBLIC openstw-spdr)
