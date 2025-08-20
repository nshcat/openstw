cmake_minimum_required(VERSION 3.16)

project(openstw-sim LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_subdirectory(./../thirdparty/pugixml ${CMAKE_CURRENT_BINARY_DIR}/pugixml)

add_library(openstw-sim STATIC
  simulation.cxx
  simulation.hxx
  tile.hxx tile.cxx
  enums.hxx
  signal.hxx signal.cxx
  utility.hxx
  base.hxx
  tilepanel.hxx tilepanel.cxx
  utility.cxx
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
target_include_directories(openstw-sim PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(openstw-sim PRIVATE OPENSTW_SIM_LIBRARY)
