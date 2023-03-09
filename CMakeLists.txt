# CXXPLOT: A C++ plotting library.

# AUTHORIZATION TO USE AND DISTRIBUTE. By using or distributing the CXXPLOT software
# ("THE SOFTWARE"), you agree to the following terms governing the use and redistribution of
# THE SOFTWARE originally developed at the U.S. Naval Research Laboratory ("NRL"), Computational
# Multiphysics Systems Lab., Code 6394.

# The modules of CXXPLOT containing an attribution in their header files to the NRL have been
# authored by federal employees. To the extent that a federal employee is an author of a portion of
# this software or a derivative work thereof, no copyright is claimed by the United States
# Government, as represented by the Secretary of the Navy ("GOVERNMENT") under Title 17, U.S. Code.
# All Other Rights Reserved.

# Download, redistribution and use of source and/or binary forms, with or without modification,
# constitute an acknowledgement and agreement to the following:

# (1) source code distributions retain the above notice, this list of conditions, and the following
# disclaimer in its entirety,
# (2) distributions including binary code include this paragraph in its entirety in the
# documentation or other materials provided with the distribution, and
# (3) all published research using this software display the following acknowledgment:
# "This work uses the software components contained within the NRL CXXPLOT computer package
# written and developed by the U.S. Naval Research Laboratory, Computational Multiphysics Systems
# lab., Code 6394"

# Neither the name of NRL or its contributors, nor any entity of the United States Government may
# be used to endorse or promote products derived from this software, nor does the inclusion of the
# NRL written and developed software directly or indirectly suggest NRL's or the United States
# Government's endorsement of this product.

# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR THE U.S. GOVERNMENT BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# NOTICE OF THIRD-PARTY SOFTWARE LICENSES. This software uses open source software packages from third
# parties. These are available on an "as is" basis and subject to their individual license agreements.
# Additional information can be found in the provided "licenses" folder.

cmake_minimum_required( VERSION 3.15 )

if( NOT DEFINED PROJECT_NAME )
  set( CXXPLOT_NOT_SUBPROJECT ON )
else()
  set( CXXPLOT_NOT_SUBPROJECT OFF )
endif()

set( cxxplot_VERSION 0.3.1 )

project( cxxplot VERSION ${cxxplot_VERSION} )

set( CXXPLOT_COMPILE_WITH_OPENGL YES CACHE BOOL "Compile with OpenGL support" )

message( STATUS "cxxplot version: " ${cxxplot_VERSION} )

# Basic
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON)
set( CXXPLOT_DIR_WITH_VERSION cxxplot-${cxxplot_VERSION} )

unset( QT_QMAKE_EXECUTABLE ) # Silence cmake when used from qt creator

# Dependencies

find_package( QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Gui PrintSupport )

set( QTWITHVER Qt${QT_VERSION_MAJOR} )

find_package( ${QTWITHVER} REQUIRED COMPONENTS Core Widgets Gui PrintSupport )

find_package( Threads REQUIRED )

set( CXXPLOTS_DEPS
  ${QTWITHVER}::Core
  ${QTWITHVER}::Widgets
  ${QTWITHVER}::Gui
  ${QTWITHVER}::PrintSupport
  Threads::Threads )

if( ${CXXPLOT_COMPILE_WITH_OPENGL} )

  find_package( QT NAMES Qt5 Qt6 REQUIRED COMPONENTS OpenGL )

  find_package( OpenGL REQUIRED )

  find_package( ${QTWITHVER} REQUIRED COMPONENTS OpenGL )

  set( CXXPLOTS_DEPS ${CXXPLOTS_DEPS} ${QTWITHVER}::OpenGL OpenGL::GL )
endif()

# Target
set( CXXPLOT_SRC
  src/align.cpp
  src/color.cpp
  src/execution.cpp
  src/figure.cpp
  src/graph.cpp
  src/image.cpp
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
  include/cxxplot/image.hpp
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
 ${CMAKE_CURRENT_BINARY_DIR}/cmake/cxxplotConfigVersion.cmake
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
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/cxxplotConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/cxxplotConfigVersion.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${CXXPLOT_DIR_WITH_VERSION}
  )

# tests/examples
if ( CXXPLOT_NOT_SUBPROJECT )

add_subdirectory( tests )
add_subdirectory( examples )

endif( CXXPLOT_NOT_SUBPROJECT )
