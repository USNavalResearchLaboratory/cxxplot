#include <cxxplot/cxxplot>

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    // Generate some data
    std::vector< double > x, y;

    for ( int i = 0; i <= 1000; i++ )
    {
      x.push_back( i );
      y.push_back( i * i );
    }

    // Plot the data in a window
    plt::plot( x, y );

    return 0;
  } );
}
