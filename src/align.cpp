#include "cxxplot/align.hpp"

#include <type_traits>

cxxplot::alignment_t::alignment_t( const int& v ) : val_( v )
{
}

const int& cxxplot::alignment_t::value( ) const
{
  return val_;
}

cxxplot::alignment_t operator|( const cxxplot::HorizontalAlignment& hor, const cxxplot::VerticalAlignment& ver )
{
  using namespace cxxplot;
  using ug_t = std::underlying_type< cxxplot::HorizontalAlignment >::type;

  return alignment_t { static_cast< ug_t >( hor ) | static_cast< ug_t >( ver ) };
}

cxxplot::alignment_t operator|( const cxxplot::VerticalAlignment& ver, const cxxplot::HorizontalAlignment& hor )
{
  return hor | ver;
}
