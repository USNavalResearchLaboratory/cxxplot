#include <cxxplot/cxxplot>

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    // Generate some data
    std::vector< plt::point2d > data;

    for ( int i = 0; i <= 10; i++ )
    {
      double x = i;
      data.push_back( { x, x * x } );
    }

    // Use the named parameters feature
    using namespace plt::named_parameters;

    // Plot using custom default parameters. The xxx_ directives are used as the named
    // parameters to the plot function.
    plt::plot( data,
               window_title_ = "Quadratic function",
               window_size_  = { 800, 600 },

               line_color_rgb_ = { 255, 0, 0 },
               line_width_ = 3.0,

               axes_line_width_ = 2.0,
               fonts_size_          = 20.0,

               marker_style_          = plt::MarkerStyle::Cross,
               marker_line_width_ = 3.0,
               marker_size_           = 12.0,

               xlim_ = { -11, 11 },
               ylim_ = { -10, 110 } );

    return 0;
  } );
}
