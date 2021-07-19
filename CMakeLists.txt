cmake_minimum_required( VERSION 3.15 )

if( NOT DEFINED PROJECT_NAME )
  set( CXXPLOT_NOT_SUBPROJECT ON )
else()
  set( CXXPLOT_NOT_SUBPROJECT OFF )
endif()

set( cxxplot_VERSION 0.2.1 )

project( cxxplot VERSION ${cxxplot_VERSION} )

set( CXXPLOT_COMPILE_WITH_OPENGL YES CACHE BOOL "Compile with OpenGL support" )

message( STATUS "cxxplot version: " ${cxxplot_VERSION} )

# Basic
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON)
set( CXXPLOT_DIR_WITH_VERSION cxxplot-${cxxplot_VERSION} )

unset( QT_QMAKE_EXECUTABLE ) # Silence cmake when used from qt creator

# Dependencies

find_package( Qt6 COMPONENTS
    Core
    Widgets
    Gui
    PrintSupport )

if (NOT Qt6_FOUND)
    find_package( Qt5 REQUIRED COMPONENTS
        Core
        Widgets
        Gui
        PrintSupport )
endif()

find_package( Threads REQUIRED )

set( CXXPLOTS_DEPS Qt::Core Qt::Widgets Qt::Gui Qt::PrintSupport Threads::Threads )

if( ${CXXPLOT_COMPILE_WITH_OPENGL} )

  find_package( Qt6  COMPONENTS OpenGL )

  if (NOT Qt6_FOUND)
    find_package( Qt5 REQUIRED COMPONENTS  OpenGL )
  endif()

  find_package( OpenGL REQUIRED )

  set( CXXPLOTS_DEPS ${CXXPLOTS_DEPS} Qt::OpenGL OpenGL::GL )
endif()

# Target
set( CXXPLOT_SRC
  src/align.cpp
  src/color.cpp
  src/execution.cpp
  src/figure.cpp
  src/graph.cpp
  src/qcustomplot.cpp
  src/range.cpp
  src/styles.cpp
  src/utils.cpp
  src/widget.cpp
  src/window.cpp
  src/version.cpp
)

if(Qt6_FOUND)
    qt_add_resources( CXXPLOT_RESOURCES src/images.qrc)
else()
    qt5_add_resources( CXXPLOT_RESOURCES src/images.qrc)
endif()


set (CXXPLOT_PRIVATE_HDR
  src/execution_p.hpp
  src/qcustomplot.h
  src/styles_p.hpp
  src/widget.hpp
)

set( CXXPLOT_PUBLIC_HDR
  include/cxxplot/align.hpp
  include/cxxplot/color.hpp
  include/cxxplot/concepts.hpp
  include/cxxplot/cxxplot
  include/cxxplot/execution.hpp
  include/cxxplot/figure.hpp
  include/cxxplot/gettersetter.hpp
  include/cxxplot/graph.hpp
  include/cxxplot/named_parameter.hpp
  include/cxxplot/point2d.hpp
  include/cxxplot/range.hpp
  include/cxxplot/styles.hpp
  include/cxxplot/typetraits.hpp
  include/cxxplot/utils.hpp
  include/cxxplot/window.hpp
)

add_library( cxxplot ${CXXPLOT_RESOURCES} ${CXXPLOT_SRC} ${CXXPLOT_PRIVATE_HDR} ${CXXPLOT_PUBLIC_HDR} )
add_library( cxxplot::cxxplot ALIAS cxxplot )

target_compile_definitions( cxxplot PRIVATE CXXPLOT_VERSION="${cxxplot_VERSION}" )

if ( ${CXXPLOT_COMPILE_WITH_OPENGL} )
  target_compile_definitions( cxxplot PRIVATE QCUSTOMPLOT_USE_OPENGL )
endif()

target_compile_features( cxxplot PUBLIC cxx_std_20 )

target_include_directories( cxxplot
    PUBLIC
        $<INSTALL_INTERFACE:include/${CXXPLOT_DIR_WITH_VERSION}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries( cxxplot
  PUBLIC
    ${CXXPLOTS_DEPS}
)

target_compile_options( cxxplot PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:> # TODO ADD /W4 /WX here by fixing cxxplot_WARNOFF / ON
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
)

# Install, export targets, cmake config and version
include( GNUInstallDirs )

install( TARGETS cxxplot
    EXPORT cxxplotTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CXXPLOT_DIR_WITH_VERSION}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${CXXPLOT_DIR_WITH_VERSION}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include( CMakePackageConfigHelpers )

if ( ${CXXPLOT_COMPILE_WITH_OPENGL} )
  set( CXXPLOT_CONFIG_IN cxxplotConfigOpenGL.cmake.in)
else()
  set( CXXPLOT_CONFIG_IN cxxplotConfig.cmake.in)
endif()

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CXXPLOT_CONFIG_IN}
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/cxxplotConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CXXPLOT_DIR_WITH_VERSION}
  )

write_basic_package_version_file(
 ${CMAKE_BINARY_DIR}/cmake/cxxplotConfigVersion.cmake
 VERSION ${cxxplot_VERSION}
 COMPATIBILITY SameMajorVersion
)

# Install
install(
  FILES
    ${CXXPLOT_PUBLIC_HDR}
  DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CXXPLOT_DIR_WITH_VERSION}/cxxplot
)

install(EXPORT cxxplotTargets
  FILE
    cxxplotTargets.cmake
  NAMESPACE
    cxxplot::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${CXXPLOT_DIR_WITH_VERSION}
)

install(
  FILES
    ${CMAKE_BINARY_DIR}/cmake/cxxplotConfig.cmake
    ${CMAKE_BINARY_DIR}/cmake/cxxplotConfigVersion.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${CXXPLOT_DIR_WITH_VERSION}
  )

# tests/examples
if ( CXXPLOT_NOT_SUBPROJECT )

add_subdirectory( tests )
add_subdirectory( examples )

endif( CXXPLOT_NOT_SUBPROJECT )
