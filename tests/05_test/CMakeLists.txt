cmake_minimum_required( VERSION 3.9 )

project( libcxxplot_05_test )

set( cxxplot_Current_Target_Name ${PROJECT_NAME} )

file(GLOB_RECURSE CPPS  ./*.cpp )
file(GLOB_RECURSE HPPS  ./*.hpp )

add_executable(${cxxplot_Current_Target_Name} ${CPPS} ${HPPS} )

target_compile_features(${cxxplot_Current_Target_Name} PRIVATE cxx_std_20)

target_link_libraries(${cxxplot_Current_Target_Name} cxxplot )
