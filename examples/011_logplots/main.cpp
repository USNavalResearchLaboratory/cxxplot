#include <cxxplot/cxxplot>

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {

    std::vector< plt::point2d > data;

    for ( int i = 100; i <= 1000; i++ )
    {
      double x = i;
      double y = x * x;
      data.push_back( { x, y } );
    }

    using namespace plt::named_parameters;

    plt::plot( data, y_axis_scaling_type_ = plt::axis_scaling_type::logarithmic  );

    return 0;
  } );
}
