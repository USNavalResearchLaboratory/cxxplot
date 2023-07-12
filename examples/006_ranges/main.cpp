#include <cxxplot/cxxplot>

#include <cmath>
#include <filesystem>
#include <ranges>

template< typename T >
int sgn( T val )
{
  return ( T( 0 ) < val ) - ( val < T( 0 ) );
}

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    using namespace std::ranges;
    using namespace cxxplot::named_parameters;

    const int n = 50;

    constexpr auto wave = [ n ]( auto v ) { return std::sin( 2.0 * 3.14159 * v / ( n - 1 ) ); };

    constexpr auto sin_range = iota_view { 0, n } | views::transform( wave );

    auto filtered_range = sin_range | views::filter( []( auto v ) { return std::abs( v ) < 0.9; } );

    auto transformed_range = sin_range | views::transform( []( auto v ) {
                               return std::abs( v ) < 0.9 ? v : sgn( v ) * 0.9;
                             } );

    auto w = plt::plot( filtered_range,
                        line_width_   = 2.0,
                        line_style_   = plt::LineStyle::None,
                        marker_style_ = plt::MarkerStyle::Circle );

    w.add_graph( transformed_range,
                 line_width_   = 2.0,
                 line_style_   = plt::LineStyle::None,
                 marker_style_ = plt::MarkerStyle::Cross );

    w.figure( 0 ).graph( 0 ).name = "views::filter";
    w.figure( 0 ).graph( 1 ).name = "views::transform";

    w.figure( 0 ).y_range = { -1.0, 1.0 };
    w.figure( 0 ).x_label = "Item No.";
    w.figure( 0 ).y_label = "Result";

    w.show_legend      = true;
    w.legend_alignment = plt::VerticalAlignment::Top | plt::HorizontalAlignment::Right;
    w.title            = "C++20 ranges filters and transforms";

    try
    {
      auto path = std::filesystem::current_path( );
      path /= "Filters_and_transforms.pdf";

      w.save( path.string( ) );

      std::cout << "Plot saved in: " << path << std::endl;
    }
    catch ( std::exception& e )
    {
      std::cerr << "cxxplot ex06: Could not save plot." << std::endl;
      ;
    }

    return 0;
  } );
}
