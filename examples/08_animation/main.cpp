#include <cxxplot/cxxplot>

#include <cmath>
#include <filesystem>
#include <numbers>
#include <ranges>
#include <sstream>

namespace plt = cxxplot;

template <class F>
auto lazy_func_data( F &&f, const double x0, const double xn, const std::size_t num_points = 100 )
{
  using namespace std::ranges;

  double scale = ( xn - x0 ) / ( num_points - 1 );

// clang-format off
  return
      iota_view{ std::size_t( 0 ), num_points } |
      std::ranges::views::transform( [f, x0, scale]( const int &i ) { return f( x0 + scale*i ); } );
// clang-format on
}

int main( int argc, char *argv[] )
{
  return plt::exec( argc, argv, [&]( ) {
    using namespace plt::named_parameters;
    using namespace std::ranges;
    using namespace std::numbers;

    // clang-format off

    auto x = []( const double &t, const double &a ) { return  a/20.0*cos(t) + sin(2.0*t)*cos(a*t); };
    auto y = []( const double &t, const double &a ) { return 2.0*sin((60.0-a)/12.5*t) + 0.5*sin(a*t); };


    const int n = 2000;

    auto fx =[x]( const double &a ) {
        return lazy_func_data( [a,x]( auto t ) { return x( t, a ); } , 0, 2*pi, n  );
      };

    auto fy =[y]( const double &a )
    {
        return lazy_func_data( [a,y]( auto t ) { return y( t, a ); } , 0, 2*pi, n  );
    };

    const auto a = 35.0;

    auto w = plt::plot( fx( a ), fy( a ),
                        window_title_      = "cxxplot animation example",
                        line_width_        = 1.0,
                        xlim_              = {-5.0, 5.0},
                        ylim_              = {-3.0, 3.0},
                        line_color_        = plt::color::cxxplot::green,
                        axes_aspect_ratio_ = 1.0 // TODO: There is an issue if we provide an int here
        );

    auto &f = w.figure(0);
    auto &g = f.graph(0);

    std::size_t frames = 0;
    double duration = 0;
    for (auto i = 1 ; i != 10000; i++)
    {
      auto start = std::chrono::steady_clock::now();
      g.set_data( fx( a + i*0.005 ), fy( a + i*0.005 ) );
      auto finish = std::chrono::steady_clock::now();

      auto t = double(std::chrono::duration_cast<std::chrono::microseconds>( finish - start ).count()) / 1.0e6;
      duration +=t;
      frames++;
      if ( duration > 0.5 )
        {
          std::stringstream ss;
          ss << std::round( 1.0 / duration * frames ) << " updates per sec (including dropped)";
          w.set_title( ss.str() );
          duration = 0;
          frames = 0;
        }
    }


    // clang-format on

    return 0;
  } );
}
