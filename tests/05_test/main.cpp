#include <cxxplot/cxxplot>

namespace ct = cxxplot;



int main( int argc, char *argv[] )
{
  using namespace ct::named_parameters;

  auto ct_thread = ct::experimental::gui_thread();

  {
    auto w = ct::plot( { 0.0, 1.0, 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0} );
  }

  {
    auto w = ct::plot( { 0.0, 1.0, 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0}, window_title_="test" );
  }

  {
    auto w = ct::plot( {0, 1, 5, 20}, {1,-1,2,-2}, window_title_="test" );
  }

  {
    auto w = ct::plot( std::vector{ 0.0, 1.0, 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0} , window_title_="test" );
  }


  {
    auto w = ct::plot( std::vector{0, 1, 5, 20}, std::vector{1,-1,2,-2}, window_title_="test" );
  }

  {
    auto w = ct::plot( std::vector{0, 1, 5, 20}, {1.0,-1.0,2.0,-2.0}, window_title_="test", line_width_ = 2.0 );
  }

  {
    auto w = ct::plot( {0, 1, 5, 20}, std::vector{1.0,-1.0,2.0,-2.0}, window_title_="test", line_width_ = 2.0 );
  }

}
