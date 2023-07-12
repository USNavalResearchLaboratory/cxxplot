#include <cxxplot/cxxplot>

#include <filesystem>
#include <numbers>
#include <ranges>
#include <cmath>
#include <sstream>

namespace plt = cxxplot;

std::string color_to_string( const plt::color::rgb &c )
{
  std::stringstream ss;
  ss << "RGB: [ " << int( c.r ) << ", " << int( c.g ) << ", " << int( c.b ) << " ]";

  return ss.str( );
}

int main( int argc, char *argv[] )
{
  return plt::exec( argc, argv, [&]( ) {
    using namespace plt::named_parameters;
    using namespace std::ranges;
    using namespace std::numbers;

    const int n = 500;

    // clang-format off

    auto wave_func = [ ]( double t, double f, double phi ) { return sin(2.0*pi*f*t + phi*pi/180.0); };

    auto waveform = [ n, wave_func ]( double phi )
    {
      const auto f = 1.0/(n-1);

      return
        iota_view{0, n} |
        views::transform( [f, wave_func, phi ]( double t ) { return wave_func( t, f, phi ); } );
    };

    const bool do_not_show = false; // Disable the window for now. Won't make any perceivable
                                    // difference in the particular example.

    auto w = plt::plot( do_not_show ); // Empty window

    constexpr auto k = plt::color::default_order.size( );
    for ( auto i = 0; i != k; i++ )
    {
      w.add_graph(
            waveform( -360.0 / k * i ),
            name_ = color_to_string( plt::color::default_order[ i ] ) );
    }

    w.show_legend = true;
    w.auto_redraw = true;
    w.show();

    // clang-format on

    try
    {
      auto path = std::filesystem::current_path( );
      path /= "line_color_order.pdf";

      w.save( path.string() );

      std::cout << "Plot saved in:\n" << path.string() << std::endl;
    }
    catch ( std::exception &e )
    {
      std::cerr << "cxxplot ex07: Could not save plot." << std::endl;
      ;
    }

    return 0;
  } );
}
