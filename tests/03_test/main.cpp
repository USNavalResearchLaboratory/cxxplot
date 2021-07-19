#include <cxxplot/align.hpp>
#include <cxxplot/cxxplot>

#include <QApplication>
#include <QWidget>

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

cxxplot::window_proxy createMyDefaultWindow( int i, int j )
{
  using namespace cxxplot::named_parameters;

  std::vector< cxxplot::point2d > data;
  return cxxplot::plot(
    data, window_size_ = { 320, 200 }, window_position_ = { j * 340, i * 220 } );
}

int main( int argc, char* argv[] )
{
  {
    using namespace cxxplot::named_parameters;

    auto t = cxxplot::experimental::gui_thread( argc, argv );

    auto win = createMyDefaultWindow( 0, 0 );

    win.title                                 = "Not portable test 1";
    win.figure( 0 ).axes_aspect_ratio         = 1.0;
    win.figure( 0 ).graph( 0 ).line_width = 2.0;

    constexpr double pi = 3.14159;

    for ( auto i = 0; i <= 360; i++ )
    {
      auto theta_rad = pi * i / 180.0;
      win.figure( 0 ).graph( 0 ).append_data( 0.75 * cos( theta_rad ), 0.75 * sin( theta_rad ) );
    }
  }

  {
    using namespace cxxplot::named_parameters;

    auto t = cxxplot::experimental::gui_thread( );

    auto win = createMyDefaultWindow( 0, 0 );

    win.title                                 = "Not portable test 2";
    win.figure( 0 ).axes_aspect_ratio         = 1.0;
    win.figure( 0 ).graph( 0 ).line_width = 2.0;

    constexpr double pi = 3.14159;

    for ( auto i = 0; i <= 360; i++ )
    {
      auto theta_rad = pi * i / 180.0;
      win.figure( 0 ).graph( 0 ).append_data( 0.75 * cos( theta_rad ), 0.75 * sin( theta_rad ) );
    }
  }
}
