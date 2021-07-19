cmake_minimum_required( VERSION 3.10 )

project( cxxplotexample )

# find_package( cxxplot 0.1.3 REQUIRED )
find_package( cxxplot REQUIRED ) # If you don't specify a version, the latest one will be used

add_executable( cxxplotexample main.cpp )

target_link_libraries( cxxplotexample PRIVATE cxxplot::cxxplot )
