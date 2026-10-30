cmake_minimum_required(VERSION 3.16)

project(openstw-topotest LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(openstw-topotest
  main.cxx
  data/topology.xml
  data/teststadt.xml
)
target_link_libraries(openstw-topotest PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(openstw-topotest PRIVATE openstw-topo)

include(GNUInstallDirs)
install(TARGETS openstw-topotest
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/topology.xml
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/data/topology.xml
                ${CMAKE_CURRENT_BINARY_DIR}/topology.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/data/topology.xml
)

add_custom_target(copy_topo_xml DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/topology.xml)

add_dependencies(openstw-topotest copy_topo_xml)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/teststadt.xml
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/data/teststadt.xml
                ${CMAKE_CURRENT_BINARY_DIR}/teststadt.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/data/teststadt.xml
)

add_custom_target(copy_teststadt_xml DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/teststadt.xml)

add_dependencies(openstw-topotest copy_teststadt_xml)