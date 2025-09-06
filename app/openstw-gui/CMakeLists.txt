cmake_minimum_required(VERSION 3.16)

project(openstw-gui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cxx
    src/mainwindow.cxx
    src/mainwindow.hxx
    src/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Svg)


    qt_add_executable(openstw-gui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/tilepanelview.hxx src/tilepanelview.cxx
        src/tilegraphicsobject.hxx src/tilegraphicsobject.cxx


        src/rendering/tilerenderingconstants.hxx

        src/rendering/tilecomponentgraphicsobject.hxx src/rendering/tilecomponentgraphicsobject.cxx
        src/rendering/trackgraphicsobject.hxx src/rendering/trackgraphicsobject.cxx
        data/sim_template.xml
        src/rendering/renderinghelpers.hxx src/rendering/renderinghelpers.cxx
        src/rendering/signalgraphicsobject.hxx src/rendering/signalgraphicsobject.cxx
        src/rendering/arrowgraphicsobject.hxx
        src/rendering/arrowgraphicsobject.cxx
        src/rendering/arrowgraphicsobject.cxx
        src/rendering/erlaubnisfeldgraphicsobject.hxx
        src/rendering/erlaubnisfeldgraphicsobject.cxx
        src/rendering/znagraphicsobject.hxx
        src/rendering/znagraphicsobject.cxx
        src/rendering/sevensegmentrenderer.hxx src/rendering/sevensegmentrenderer.cxx
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET openstw-gui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(openstw-gui SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(openstw-gui
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(openstw-gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.openstw-gui)
endif()
set_target_properties(openstw-gui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS openstw-gui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(
        TARGET openstw-gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/data/sim_template.xml
                ${CMAKE_CURRENT_BINARY_DIR}/sim_template.xml)

target_link_libraries(openstw-gui PRIVATE openstw-sim)
target_link_libraries(openstw-gui PRIVATE Qt6::Core Qt6::Widgets Qt6::Svg)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(openstw-gui)
endif()

