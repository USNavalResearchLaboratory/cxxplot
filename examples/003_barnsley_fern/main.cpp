#include <cxxplot/cxxplot>

#include <random>
#include <sstream>

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    std::vector< plt::point2d > data;

    using namespace plt::named_parameters;

    // TODO: There is an issue with aspect ratio here
    auto w = plt::plot( data,
                        window_title_      = "Barnsley Fern",
                        window_size_       = { 400, 600 },
                        auto_redraw_       = true,
                        line_style_        = plt::LineStyle::None,
                        marker_style_      = plt::MarkerStyle::Dot,
                        line_color_        = plt::color::cxxplot::green,
                        line_width_        = 1.0,
                        antialiasing_      = false,
                        xlim_              = { -160, 180 },
                        ylim_              = { -260, 150 },
                        axes_aspect_ratio_ = 0.65 );

    auto& graph = w.figure( 0 ).graph( 0 );

    std::random_device                       rd;
    std::default_random_engine               eng( rd( ) );
    std::uniform_real_distribution< double > distr( 0.0, 100.0 );

    double x = 0;
    double y = 0;

    for ( auto i = 0; i != 144001; i++ )
    {
      graph.append_data( 65.0 * x, 37.0 * y - 252.0 );

      auto r = distr( eng );

      auto xn = x;
      auto yn = y;

      if ( r < 1.0 )
      {
        x = 0;
        y = 0.16 * yn;
      }
      else if ( r < 86.0 )
      {
        x = 0.85 * xn + 0.04 * yn;
        y = -0.04 * xn + 0.85 * yn + 1.6;
      }
      else if ( r < 93.0 )
      {
        x = 0.20 * xn - 0.26 * yn;
        y = 0.23 * xn + 0.22 * yn + 1.6;
      }
      else
      {
        x = -0.15 * xn + 0.28 * yn;
        y = 0.26 * xn + 0.24 * yn + 0.44;
      }

      if ( i % 500 == 0 )
      {
        std::stringstream ss;
        ss << "Barnsley Fern. Dot count: " << i;
        w.title = ss.str( );
      }
    }

    return 0;
  } );
}
