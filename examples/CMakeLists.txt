add_subdirectory( 01_basic )
add_subdirectory( 02_basic_styling )
add_subdirectory( 03_barnsley_fern )
add_subdirectory( 04_compute_and_plot )
add_subdirectory( 05_simple )


# TODO: When ranges are mature make sure the following works on MSVC, INTEL and CLANG.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_subdirectory( 06_ranges )
  add_subdirectory( 07_line_color_order )
  add_subdirectory( 08_animation )
  add_subdirectory( 09_basic_shapes )
endif()

add_subdirectory( 10_opengl )
add_subdirectory( 11_logplots )
add_subdirectory( 12_legend_columns )
