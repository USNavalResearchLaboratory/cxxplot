#include <cxxplot/cxxplot>
#include <sstream>

int main( int argc, char* argv[] )
{
  return cxxplot::exec( argc, argv, [ & ]( ) {
    using namespace cxxplot::named_parameters;

    auto w = cxxplot::plot( { 0.0, 1.0, 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0 },
                            name_        = "graph 0",
                            show_legend_ = true,
                            legend_alignment_
                            = cxxplot::HorizontalAlignment::Left | cxxplot::VerticalAlignment::Top,
                            legend_columns_ = 2 );

    auto& f = w.figure( 0 );

    for ( int i = 1; i != 15; i++ )
    {
      std::stringstream ss;
      ss << "graph " << i;

      f.add_graph( std::vector< double > { { 0.0 + i,
                                             1.0 + i,
                                             1.0 + i,
                                             2.0 + i,
                                             3.0 + i,
                                             5.0 + i,
                                             8.0 + i,
                                             13.0 + i,
                                             21.0 + i,
                                             34.0 + i } },
                   name_ = ss.str( ) );
    }

    return 0;
  } );
}
