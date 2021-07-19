cmake_minimum_required( VERSION 3.9 )

project( libcxxplot_06_test_csv)

set( cxxplot_Current_Target_Name ${PROJECT_NAME} )

file(GLOB_RECURSE CPPS  ./*.cpp )
file(GLOB_RECURSE HPPS  ./*.hpp )

#SET(GCC_CONCEPTS_COMPILE_FLAGS "-fconcepts-diagnostics-depth=4")

add_definitions(${GCC_CONCEPTS_COMPILE_FLAGS})
add_executable(${cxxplot_Current_Target_Name} ${CPPS} ${HPPS} )

target_compile_features(${cxxplot_Current_Target_Name} PRIVATE cxx_std_20)

target_link_libraries(${cxxplot_Current_Target_Name} cxxplot )
