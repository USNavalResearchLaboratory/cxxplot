#include <cxxplot/cxxplot>

#include <cmath>
#include <filesystem>
#include <numbers>
#include <ranges>
#include <sstream>

namespace plt = cxxplot;

struct my_point2d_t
{
  double my_x = 0.0;
  double my_y = 0.0;
};

// Provide an overload for get_x, get_y functions so that cxxplot can get the values.
const double &get_x( const my_point2d_t & p ) {  return p.my_x; }
const double &get_y( const my_point2d_t & p ) {  return p.my_y; }

auto rect( double xcen, double ycen, double width, double height )
{
  return std::array{my_point2d_t{xcen - width / 2.0, ycen - height / 2.0},
                      my_point2d_t{xcen + width / 2.0, ycen - height / 2.0},
                      my_point2d_t{xcen + width / 2.0, ycen + height / 2.0},
                      my_point2d_t{xcen - width / 2.0, ycen + height / 2.0},
                      my_point2d_t{xcen - width / 2.0, ycen - height / 2.0}};
}

int main( int argc, char *argv[] )
{
  return plt::exec( argc, argv, [&]( ) {
    using namespace plt::named_parameters;
    using namespace std::ranges;
    using namespace std::numbers;

    auto  w             = plt::plot( false );
    auto &f             = w.figure( 0 );
    f.x_range           = {-6.0, 6.0};
    f.y_range           = {-3.0, 3.0};
    f.axes_aspect_ratio = 1.0;

    auto &rect1 = w.figure( 0 ).add_graph( );
    rect1.set_data( rect( 0.0, 0.0, 10.0, 5.0 ) );

    for ( auto i = 0; i != 5; i++ )
    {
      f.add_graph( );
    }

    w.show( );

    double xcen = 0.0;
    double ycen = 0.0;

    for ( auto i = 0; i != 1101; i++ )
    {
      std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
      w.auto_redraw = false;
      for ( auto k = 0; k != 5; k++ )
      {
        double width  = 10.0 * sin( 2 * pi * i / 600.0 / ( k + 1 ) );
        double height = 5.0 * sin( 2 * pi * i / 600.0 / ( k + 1 ) );

        f.graph( k + 1 ).set_data( rect( xcen, ycen, width, height ) );
      }
      w.auto_redraw = true;
    }

    return 0;
  } );
}
