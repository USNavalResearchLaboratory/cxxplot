#include <cxxplot/cxxplot>

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    // Generate some data
    std::vector< plt::point2d > data;

    for ( int i = 0; i <= 1000; i++ )
    {
      double x = i;
      double y = x * x;
      data.push_back( { x, y } );
    }

    // Plot the data in a window
    plt::plot( data );

    return 0;
  } );
}
