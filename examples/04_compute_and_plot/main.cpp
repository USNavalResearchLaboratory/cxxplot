#include <cxxplot/cxxplot>

#include <chrono>
#include <iostream>
#include <random>

namespace plt = cxxplot;

auto tic( )
{
  return std::chrono::steady_clock::now( );
}

double toc_ms( const std::chrono::steady_clock::time_point& start )
{
  auto finish = tic( );
  return double(
           std::chrono::duration_cast< std::chrono::microseconds >( finish - start ).count( ) )
         / 1000.0;
}

class matrix
{
public:
  matrix( std::size_t rows, std::size_t columns ) :
    rows_( rows ), columns_( columns ), data( rows * columns )
  {
  }

  const std::size_t& rows( ) const
  {
    return rows_;
  }

  const std::size_t& columns( ) const
  {
    return columns_;
  }

  const double& operator( )( const std::size_t& i, const std::size_t& j ) const
  {
    return data[ i + columns_ * j ];
  }

  double& operator( )( const std::size_t& i, const std::size_t& j )
  {
    return data[ columns_ * i + j ];
  }

  matrix mult1( const matrix& other )
  {
    if ( other.rows( ) != columns( ) )
    {
      throw std::invalid_argument( "Matrices size not compliant." );
    }

    matrix m( rows( ), other.columns( ) );

    for ( std::size_t i = 0; i != rows( ); i++ )
    {
      for ( std::size_t j = 0; j != columns( ); j++ )
      {
        for ( std::size_t k = 0; k != other.columns( ); k++ )
        {
          m( i, k ) = ( *this )( i, j ) * other( j, k );
        }
      }
    }
    return m;
  }

  matrix mult2( const matrix& other )
  {
    if ( other.rows( ) != columns( ) )
    {
      throw std::invalid_argument( "Matrices size not compliant." );
    }

    matrix m( rows( ), other.columns( ) );

    for ( std::size_t j = 0; j != columns( ); j++ )
    {
      for ( std::size_t k = 0; k != other.columns( ); k++ )
      {
        for ( std::size_t i = 0; i != rows( ); i++ )
        {
          m( i, k ) = ( *this )( i, j ) * other( j, k );
        }
      }
    }
    return m;
  }

private:
  std::size_t           rows_    = 0;
  std::size_t           columns_ = 0;
  std::vector< double > data;
};

std::random_device                       rd;
std::default_random_engine               eng( rd( ) );
std::uniform_real_distribution< double > distr( 0.0, 100.0 );

matrix random_matrix( const std::size_t rows, std::size_t columns )
{
  matrix m( rows, columns );

  for ( std::size_t i = 0; i != rows; i++ )
  {
    for ( std::size_t j = 0; j != rows; j++ )
    {
      m( i, j ) = distr( eng );
    }
  }

  return m;
}

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    // Generate some data
    std::vector< plt::point2d > data;
    std::vector< plt::point2d > data2;

    using namespace plt::named_parameters;

    auto w = plt::plot(
      data,
      window_title_
      = "Performance comparison of trivial square matrix multiplication (regular PC load)",
      window_size_      = { 650, 400 },
      auto_redraw_      = true,
      line_style_       = plt::LineStyle::None,
      marker_style_     = plt::MarkerStyle::Disk,
      line_color_       = plt::color::rgb( 72, 171, 72 ),
      xlabel_           = "Matrix size",
      ylabel_           = "Execution time (ms)",
      show_legend_      = true,
      legend_alignment_ = plt::VerticalAlignment::Top | plt::HorizontalAlignment::Left );

    auto& f = w.figure( 0 );

    auto& g1 = w.add_graph( data2,
                            line_color_   = plt::color::rgb( 240, 50, 50 ),
                            line_style_   = plt::LineStyle::None,
                            marker_style_ = plt::MarkerStyle::Cross );

    g1.name = "Implementation B";

    auto& g0 = f.graph( 0 );
    g0.name  = "Implementation A";

    for ( auto i = 0; i <= 200; i++ )
    {
      auto s = 3 + i;
      for ( auto k = 0; k != 10; k++ )
      {
        auto A = random_matrix( s, s );
        auto B = random_matrix( s, s );

        {
          auto start    = tic( );
          auto C        = A.mult1( B );
          auto duration = toc_ms( start );

          g0.append_data( s, duration );
        }

        {
          auto start    = tic( );
          auto C        = A.mult2( B );
          auto duration = toc_ms( start );

          g1.append_data( s, duration );
        }
      }
    }

    return 0;
  } );
}
