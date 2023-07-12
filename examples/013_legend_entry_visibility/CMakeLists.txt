cmake_minimum_required( VERSION 3.9 )

get_filename_component( cxxplot_Current_Target_Name ${CMAKE_CURRENT_LIST_DIR} NAME )

project( ${cxxplot_Current_Target_Name} )

file( GLOB CPPS ./*.cpp )
file( GLOB HPPS ./*.hpp )

add_executable( ${cxxplot_Current_Target_Name} ${CPPS} ${HPPS} )

target_link_libraries( ${cxxplot_Current_Target_Name} cxxplot )
