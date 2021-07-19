// CXXPLOT: A C++ plotting library.
//
// AUTHORIZATION TO USE AND DISTRIBUTE
//
// By using or distributing the CXXPLOT software ("THE SOFTWARE"), you agree to the following terms
// governing the use and redistribution of THE SOFTWARE originally developed at the U.S. Naval
// Research Laboratory (NRL), Computational Multiphysics Systems Lab., Code 6394.
//
// The modules of CXXPLOT containing an attribution in their header files to the NRL have been
// authored by federal employees. To the extent that a federal employee is an author of a portion of
// this software or a derivative work thereof, no copyright is claimed by the United States
// Government, as represented by the Secretary of the Navy ("GOVERNMENT") under Title 17, U.S. Code.
// All Other Rights Reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) source code distributions retain the above copyright notice, this list of conditions, and the
// following disclaimer in its entirety,
// (2) distributions including binary code include this paragraph in its entirety in the
// documentation or other materials provided with the distribution, and
// (3) all published research using this software display the following acknowledgment:
// "This work uses the software components contained within the NRL CXXPLOT computer package written
// and developed by the U.S. Naval Research Laboratory, Computational Multiphysics Systems lab.,
// Code 6394"
//
// Neither the name of NRL or its contributors, nor any entity of the United States Government may
// be used to endorse or promote products derived from this software, nor does the inclusion of the
// NRL written and developed software directly or indirectly suggest NRL's or the United States
// Government's endorsement of this product.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR THE U.S. GOVERNMENT BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Notice of Third-Party Software Licenses
//
// This software contains open source software packages from third parties. These are available on
// an "as is" basis and subject to their individual license agreements.  Additional information can
// can be found in the provided "licenses" folder.

#pragma once

#include "color.hpp"
#include "range.hpp"
#include "styles.hpp"

#include "gettersetter.hpp"
#include "named_parameter.hpp"
#include "typetraits.hpp"

#include "utils.hpp"

#include "concepts.hpp"

cxxplot_WARNOFF
#include <QObject>
#include <QVector>
  cxxplot_WARNON

#include <initializer_list>

  class QCPGraph;

namespace cxxplot
{

class widget;
class figure;

class graph : public QObject, public named_parameters::supports_named_parameters< graph >
{
  Q_OBJECT
public:
  graph( graph&& other );

  graph( graph&& other, figure* new_parent );

  graph( widget* plot_window, figure* parent );

  void set_data( const QVector< double >& x, const QVector< double >& y );

  template <class Container1_t, class Container2_t >
  void set_data( const Container1_t &Xs, const Container2_t &Ys )
  {
    // Although we are making copies here, this is faster than not, because of the different threads.
    QVector< double > xs;
    xs.reserve( int( Xs.size( ) ) );
    for ( const auto &x: Xs)
    {
      xs.push_back( x );
    }

    QVector< double > ys;
    ys.reserve( int( Ys.size( ) ) );
    for ( const auto &y: Ys)
    {
      ys.push_back( y );
    }

    set_data( xs, ys);
  }

  template <typename T>
  void set_data( std::initializer_list<T> l )
  {
    x_range_ = range { };
    y_range_ = range { };

    QVector< double > x;
    x.reserve( int( l.size( ) ) );

    QVector< double > y;
    y.reserve( int( l.size( ) ) );

    std::size_t i = 1;
    for ( const auto& value : l )
    {
      x.push_back( i++ );
      x_range_.expand( i );

      y.push_back( value );
      y_range_.expand( value );
    }

    set_data( x, y );
  }

  template< BasicRealContainer Container_t >
  void set_data( Container_t&& container )
  {
    x_range_ = range { };
    y_range_ = range { };

    QVector< double > x;

    QVector< double > y;

    std::size_t i = 1;
    for ( const auto& value : container )
    {
      x.push_back( i++ );
      x_range_.expand( i );

      y.push_back( value );
      y_range_.expand( value );
    }

    set_data( x, y );
  }

  template< BasicPoint2DContainer Container_t >
  void set_data( Container_t&& container )
  {
    x_range_ = range { };
    y_range_ = range { };

    QVector< double > x;
    x.reserve( int( container.size( ) ) );

    QVector< double > y;
    y.reserve( int( container.size( ) ) );

    for ( const auto& point : container )
    {
      x.push_back( get_x(point) );
      x_range_.expand( get_x(point) );

      y.push_back( get_y(point) );
      y_range_.expand( get_y(point) );
    }

    set_data( x, y );
  }

  void append_data( const double& x, const double& y );

  void append_data( const QVector< double >& x, const QVector< double >& y );

  template< class Container_t >
  void append_data( const Container_t& container )
  { // TODO: Concepts here
    if constexpr ( detail::type_traits< Container_t >::isPoint( ) )
    {
      addData( detail::type_traits< Container_t >::getX( container ),
               detail::type_traits< Container_t >::getY( container ) );
    }
    else
    {
      QVector< double > x;
      x.reserve( container.size( ) );

      QVector< double > y;
      y.reserve( container.size( ) );

      for ( const auto& point : container )
      {
        x.push_back( point.x( ) );

        y.push_back( point.y( ) );
      }

      append_data( x, y );
    }
  }

  void set_line_width( qreal width );
  detail::setter< qreal > line_width { [ this ]( const qreal& t ) {
    set_line_width( t );
  } };

  void set_marker_size( qreal width );

  void set_marker_line_width( qreal width );

  void setMarkerStyle( const MarkerStyle& style );

  detail::setter< MarkerStyle > marker_style { [ this ]( const MarkerStyle& v ) {
    setMarkerStyle( v );
  } };

  void set_line_style( const LineStyle& style );

  detail::setter< LineStyle > lineStyle { [ this ]( const LineStyle& style ) {
    set_line_style( style );
  } };

  void set_color_rgb( const int& r, const int& g, const int& b );

  void set_color( const color::rgb& c );

  detail::setter< color::rgb > color { [ this ]( const color::rgb& c ) { set_color( c ); } };

  void                           set_name( std::string name );
  detail::setter< const std::string& > name { [ this ]( const std::string& n ) { set_name( n ); } };

  const struct range& x_range( ) const;

  const struct range& y_range( ) const;

  int data_size( ) const;

private:
  QPointer< widget > plot_window( );

  QCPGraph* QCPGraph_;
  figure*   parent_figure_;

  struct range x_range_;
  struct range y_range_;
};

namespace named_parameters
{

inline const named_parameter line_width_ { &graph::set_line_width };
inline const named_parameter line_style_ { &graph::set_line_style };
inline const named_parameter line_color_rgb_ { &graph::set_color_rgb };
inline const named_parameter line_color_ { &graph::set_color };

inline const named_parameter marker_size_ { &graph::set_marker_size };
inline const named_parameter marker_style_ { &graph::setMarkerStyle };
inline const named_parameter marker_line_width_ { &graph::set_marker_line_width };

inline const named_parameter name_ { &graph::set_name };

}

}
