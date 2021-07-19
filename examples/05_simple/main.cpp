#include <cxxplot/cxxplot>

int main( int argc, char *argv[] )
{
  return cxxplot::exec( argc, argv, [&]( )
  {
    cxxplot::plot( { 0.0, 1.0, 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0 } );

    return 0;
  } );
}
