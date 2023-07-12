cmake_minimum_required( VERSION 3.10 )

project( 101_cmake_fetchcontent )

include( FetchContent )

FetchContent_Declare(
  cxxplot
  GIT_REPOSITORY https://github.com/USNavalResearchLaboratory/cxxplot.git
  GIT_TAG        v0.4.1
)
FetchContent_MakeAvailable(cxxplot)

add_executable( 101_cmake_fetchcontent main.cpp )

target_link_libraries( 101_cmake_fetchcontent PRIVATE cxxplot::cxxplot )
