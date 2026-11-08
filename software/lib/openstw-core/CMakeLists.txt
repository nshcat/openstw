cmake_minimum_required(VERSION 3.16)

project(openstw-core LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Xml)

add_library(openstw-core STATIC
    core/exception.hxx core/exception.cxx
    core/enums.hxx
    core/xmlutility.hxx core/xmlutility.cxx
)

# Allow the project itself to directly include headers,
# while requiring other projects to prepend "core/" to their includes
# to avoid header name collisions.
target_include_directories(openstw-core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_include_directories(openstw-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(openstw-core PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt6::Xml)

target_compile_definitions(openstw-core PRIVATE OPENSTW_CORE_LIBRARY)
