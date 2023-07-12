cmake_minimum_required( VERSION 3.10 )

project( 102_cmake_findpackage )

# find_package( cxxplot 0.4.1 REQUIRED )
find_package( cxxplot REQUIRED ) # If you don't specify a version, the latest one will be used

add_executable( 102_cmake_findpackage main.cpp )

target_link_libraries( 102_cmake_findpackage PRIVATE cxxplot::cxxplot )
