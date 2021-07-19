#include <cxxplot/align.hpp>
#include <cxxplot/cxxplot>

#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

class point2d
{
public:
  using value_type = double;

  point2d( double x, double y ) : x_( x ), y_( y )
  {
  }
  double& x( )
  {
    return x_;
  }
  const double& x( ) const
  {
    return x_;
  }
  double& y( )
  {
    return y_;
  }
  const double& y( ) const
  {
    return y_;
  }
  double squaredNorm( )
  {
    return x( ) * x( ) + y( ) * y( );
  }

private:
  double x_ = 0.0;
  double y_ = 0.0;
};


cxxplot::window_proxy createMyDefaultWindow( int i, int j )
{
  using namespace cxxplot::named_parameters;

  std::vector< point2d > data;
  return cxxplot::plot(
    data, window_size_ = { 320, 200 }, window_position_ = { j * 340, i * 220 } );
}

int main( int argc, char* argv[] )
{
  cxxplot::exec( argc, argv, []( ) {
    namespace plt = cxxplot;

    auto win  = createMyDefaultWindow( 0, 0 );
    win.title = "Ax. ratio 1. User limits";
    auto win2 = std::move( win );
    // From now on win is invalid.

    win2.title = "Moved.Ax. ratio 1. User limits";

    win2.figure( 0 ).axes_aspect_ratio = 1.0;

    using namespace cxxplot::named_parameters;
    win2.figure( 0 ).x_range = { -2.0, 2.0 };
    win2.figure( 0 ).y_range = { -3.0, 3.0 };

    constexpr double pi = 3.14159;

    for ( auto i = 0; i <= 360; i++ )
    {
      auto theta_rad = pi * i / 180.0;
      win2.figure( 0 ).graph( 0 ).append_data( 0.75 * cos( theta_rad ), 0.75 * sin( theta_rad ) );
    }

    try
    {
      win.title = "original";
    }
    catch ( std::exception& e )
    {
      std::cout << e.what( ) << std::endl;
    }

    {
      auto win                          = createMyDefaultWindow( 0, 1 );
      win.title                         = "Ax. ratio 1.";
      win.figure( 0 ).axes_aspect_ratio = 1.0;

      using namespace cxxplot::named_parameters;

      constexpr double pi = 3.14159;

      for ( auto i = 0; i <= 360; i++ )
      {
        auto theta_rad = pi * i / 180.0;
        win.figure( 0 ).graph( 0 ).append_data( 0.75 * cos( theta_rad ), 0.75 * sin( theta_rad ) );
      }
    }

    {
      auto win = createMyDefaultWindow( 0, 2 );

      win.title                         = "Ax. ratio 1. No autofit";
      win.figure( 0 ).axes_aspect_ratio = 1.0;
      win.figure( 0 ).auto_fit          = false;

      using namespace cxxplot::named_parameters;

      constexpr double pi = 3.14159;

      for ( auto i = 0; i <= 360; i++ )
      {
        auto theta_rad = pi * i / 180.0;
        win.figure( 0 ).graph( 0 ).append_data( 0.75 * cos( theta_rad ), 0.75 * sin( theta_rad ) );
      }
    }

    {
      auto win                 = createMyDefaultWindow( 0, 3 );
      win.title                = "No autofit.";
      win.figure( 0 ).auto_fit = false;

      using namespace cxxplot::named_parameters;

      constexpr double pi = 3.14159;

      for ( auto i = 0; i <= 360; i++ )
      {
        auto theta_rad = pi * i / 180.0;
        win.figure( 0 ).graph( 0 ).append_data( 0.75 * cos( theta_rad ), 0.75 * sin( theta_rad ) );
      }
    }

    {
      auto win  = createMyDefaultWindow( 1, 0 );
      win.title = "Default";

      using namespace cxxplot::named_parameters;

      constexpr double pi = 3.14159;

      for ( auto i = 0; i <= 360; i++ )
      {
        auto theta_rad = pi * i / 180.0;
        win.figure( 0 ).graph( 0 ).append_data( 0.75 * cos( theta_rad ), 0.75 * sin( theta_rad ) );
      }
    }

    return 0;
  } );
}
