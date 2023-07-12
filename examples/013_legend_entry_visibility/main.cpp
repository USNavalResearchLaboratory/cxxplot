#include <chrono>
#include <cxxplot/cxxplot>
#include <sstream>

int main( int argc, char* argv[] )
{
  return cxxplot::exec( argc, argv, [ & ]( ) {
    using namespace cxxplot::named_parameters;

    // clang-format off
    auto w = cxxplot::plot( { 0.0, 1.0, 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0 },
                            name_             = "Data 1",
                            show_legend_      = true,
                            legend_alignment_ = cxxplot::HorizontalAlignment::Left | cxxplot::VerticalAlignment::Top,
                            legend_columns_ = 2,
                            window_title_    = "Data 2 legend not visible (changing in a bit)" );
    // clang-format on

    auto& f = w.figure( 0 );

    auto& g = f.add_graph(
      std::vector< double > { { 1.0, 2.0, 2.0, 3.0, 4.0, 6.0, 9.0, 14.0, 22.0, 35.0 } },
      name_              = "Data 2",
      visible_in_legend_ = false );

    f.add_graph( std::vector< double > { { 2.0, 3.0, 3.0, 4.0, 5.0, 7.0, 10.0, 15.0, 23.0, 36.0 } },
                 name_ = "Data 3" );

    std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

    g.visible_in_legend = true;
    w.title             = "All legend entries visible";

    return 0;
  } );
}
