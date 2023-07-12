#include <cxxplot/cxxplot>

#include <iostream>

namespace plt = cxxplot;

int main( int argc, char *argv[] )
{
    return plt::exec( argc, argv, [&]()
    {
        std::cout << "Using cxxplot version: " << plt::version() << std::endl;
        // Generate some data
        std::vector< plt::point2d > data;

        for ( double x = 0; x != 1000; x++ )
        {
            double y = x * x;
            data.push_back( { x, y } );
        }

        // Plot the data in a window
        plt::plot( data );

        return 0;
    });
}
