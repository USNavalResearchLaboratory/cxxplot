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

private:
  double x_ = 0.0;
  double y_ = 0.0;
};


int main( int argc, char* argv[] )
{
  cxxplot::exec( argc, argv, []( ) {
    namespace plt = cxxplot;

    static std::mt19937                   rand_eng { };
    static std::uniform_real_distribution rand { 0.01, 99.99 };

    static std::uniform_int_distribution rand2 { 0, 255 };

    std::vector< point2d > data;

    for ( auto i = 0; i != 100; i++ )
    {
      data.emplace_back( point2d { rand( rand_eng ), rand( rand_eng ) } );
    }

    using namespace plt::named_parameters;

    auto plot_window = plt::plot( data,
                                  window_title_    = "Random lines",
                                  window_size_     = { 320, 200 },
                                  window_position_ = { 600, 40 },
                                  auto_redraw_     = false,
                                  marker_style_    = plt::MarkerStyle::Circle,
                                  line_style_      = plt::LineStyle::Solid,
                                  xlim_            = { -100, 100 },
                                  xlabel_          = "x[m]" );

    auto& f = plot_window.figure( 0 );
    auto& g = f.graph( 0 );

    f.y_label = "rand [m]";

    {
      std::cout << "Window title (1): " << plot_window.title.value( )
                << " copy?: " << ( plot_window.title.is_copy( ) ? "true" : "false" ) << std::endl;
    }

    {
      auto t = plot_window.title;
      std::cout << "Window title (2): " << t.value( ) << std::endl;
    }

    {
      std::string t = plot_window.title;
      std::cout << "Window title (3): " << t << std::endl;
    }

    auto check_return_to_temporary = [ &data ]( ) {
      auto plot_window = plt::plot( data,
                                    window_title_    = "More random lines",
                                    window_size_     = { 320, 200 },
                                    window_position_ = { 600 + 1 * 330, 40 } );
      return plot_window.title;
    };

    {
      auto t = check_return_to_temporary( );
      std::cout << "A destroyed cxxplot::window_proxy title: " << t.value( )
                << " copy?: " << ( t.is_copy( ) ? "true" : "false" ) << std::endl;
    }

    {
      plot_window.auto_redraw = false;

      g.marker_style = plt::MarkerStyle::Circle;
      g.color        = plt::color::rgb( 0, 0, 255 );

      plot_window.auto_redraw = true;
    }

    if ( true )
    { // TODO: Big problem here need to fix this.
      plot_window.set_min_redraw_interval( 0 );
      for ( auto i = 80; i != 200; i++ )
      {
        f.x_range = { 0.0, double( i ) };
        plot_window.wait_redraw_to_finish( );
      }

      plot_window.set_min_redraw_interval( 20 );
      for ( auto i = 200; i < 500; i += 10 )
      {
        f.x_range = { 0.0, double( i ) };
        plot_window.wait_redraw_to_finish( );
      }

      for ( auto i = 200; i > 99; i-- )
      {
        f.x_range = { 0.0, double( i ) };
        // efectively will disregard many calls
      }
    }

    {
      for ( auto i = 0; i != 50; i++ )
      {
        g.append_data( rand( rand_eng ), 100 );
      }
    }

    {
      using namespace std::chrono_literals;

      for ( auto i = 0; i != 5; i++ )
      {
        std::this_thread::sleep_for( 200ms );
        g.color = { uint8_t( rand2( rand_eng ) ),
                    uint8_t( rand2( rand_eng ) ),
                    uint8_t( rand2( rand_eng ) ) };
      }
    }

    std::vector< point2d > data2;
    for ( auto i = 0; i != 100; i++ )
    {
      data2.emplace_back( point2d { 200.0 + rand( rand_eng ), 200.0 + rand( rand_eng ) } );
    }

    auto w2 = plt::plot( data,
                         window_title_    = "Random points, lines and a line",
                         window_size_     = { 320, 200 },
                         window_position_ = { 600 + 2 * 330, 40 },
                         marker_style_    = plt::MarkerStyle::Plus,
                         line_style_      = plt::LineStyle::None,
                         line_color_      = plt::color::rgb( 150, 0, 0 ) );

    w2.add_graph( data2,
                  marker_style_ = plt::MarkerStyle::Cross,
                  line_style_   = plt::LineStyle::Solid,
                  line_color_   = plt::color::rgb( 0, 0, 200 ) );

    w2.add_graph( data2,
                  marker_style_   = plt::MarkerStyle::Cross,
                  line_style_     = plt::LineStyle::None,
                  line_width_ = 3.0,
                  line_color_rgb_ = { 0, 200, 0 } );

    w2.add_graph( std::vector< point2d >( { { 10.0, 200.0 }, { 200.0, 10.0 } } ),
                  line_width_ = 3.0,
                  line_color_     = plt::color::rgb( 200, 200, 0 ) );

    {
      std::vector< point2d > data;

      double step = 2.0 * 3.14159 / 200;
      for ( int i = 0; i != 1000; i++ )
      {
        double x = step * i;
        data.emplace_back( point2d { x, std::sin( x ) } );
      }

      plt::plot( data,
                 window_size_     = { 320, 200 },
                 window_position_ = { 600 + 0 * 330, 240 + 40 },
                 line_width_  = 3.0 );
    }

    auto return_window = [ &data ]( ) {
      auto plot_window = plt::plot( data,
                                    window_title_    = "Original title. Changing soon.",
                                    window_size_     = { 320, 200 },
                                    window_position_ = { 600 + 1 * 330, 240 + 40 } );
      std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

      return plot_window;
    };

    auto win  = return_window( );
    win.title = "New title";

    {
      std::vector< point2d > data;
      auto                   win = plt::plot( data,
                            window_title_    = "Plotting 10000 points.",
                            window_size_     = { 320, 200 },
                            window_position_ = { 600 + 2 * 330, 240 + 40 } );

      auto& g = win.figure( 0 ).graph( 0 );

      const double step = 0.01;
      for ( int i = 0; i != 10000; i++ )
      {
        double x = step * i;
        g.append_data( x, std::sin( x ) );
      }
    }

    return 0;
  } );
}
