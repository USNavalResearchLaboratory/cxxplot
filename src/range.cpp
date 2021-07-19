#include "cxxplot/range.hpp"

#include <algorithm>

namespace cxxplot
{

range::range( range&& other ) :
  min( std::exchange( other.min, 0 ) ), max( std::exchange( other.max, 0 ) )
{
}

range& range::operator=( const range& other )
{
  if ( this != &other )
  {
    min = other.min;
    max = other.max;
  }

  return *this;
}

void range::expand( const double& x )
{
  min = std::min( min, x );
  max = std::max( max, x );
}

void range::expand( const range& other )
{
  min = std::min( min, other.min );
  max = std::max( max, other.max );
}

void range::scale(const double &s)
{
  auto mean = ( min + max ) / 2.0;
  auto half_extent = (max - min ) / 2.0;

  min = mean - half_extent * s;
  max = mean + half_extent * s;
}

}
