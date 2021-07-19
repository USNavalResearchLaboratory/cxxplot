cmake_minimum_required( VERSION 3.9 )

project( libcxxplot_04_test )

set( cxxplot_Current_Target_Name ${PROJECT_NAME} )

file(GLOB_RECURSE CPPS  ./*.cpp )
file(GLOB_RECURSE HPPS  ./*.hpp )

add_definitions(${GCC_CONCEPTS_COMPILE_FLAGS})
add_executable(${cxxplot_Current_Target_Name} ${CPPS} ${HPPS} )

target_link_libraries(${cxxplot_Current_Target_Name} cxxplot )
