#include <cxxplot/cxxplot>

#include <cmath>
#include <filesystem>
#include <numbers>
#include <ranges>
#include <sstream>

namespace plt = cxxplot;

int main( int argc, char *argv[] )
{
  return plt::exec( argc, argv, [&]( ) {
    using namespace plt::named_parameters;
    using namespace std::ranges;
    using namespace std::numbers;
    using namespace std::chrono;

    // For the default line width or larger, OpenGL has a significant advantage particularly when
    // appending data.
    auto run = []( const bool use_opengl ) {
      auto start = steady_clock::now( );
      auto w     = plt::plot( );

      w.set_open_gl( use_opengl ); // OpenGL is enabled when calling this function with true.

      w.title = use_opengl ? "OpenGL" : "Software Rendering";
      auto &f = w.figure( 0 );
      auto &g = f.add_graph( );

      for ( auto i = 0; i != 25000; i++ )
      {
        g.append_data( i, sin( 2.0 * pi * i / 100 ) + cos( 2.1 * pi * i / 100 ) );
      }

      auto finish = steady_clock::now( );

      auto duration = double( duration_cast<milliseconds>( finish - start ).count( ) ) / 1000.0;

      std::stringstream ss;
      ss << std::string( w.title ) << " total time: " << duration << " sec. Try resizing windows.";

      w.title = ss.str( );
    };

    bool use_opengl = false;
    run( use_opengl );

    use_opengl = true;
    run( use_opengl );

    return 0;
  } );
}
