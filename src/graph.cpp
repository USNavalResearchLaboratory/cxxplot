#include "cxxplot/graph.hpp"
#include "cxxplot/figure.hpp"
#include "cxxplot/utils.hpp"
#include "cxxplot/window.hpp"

#include "execution_p.hpp"
#include "styles_p.hpp"
#include "widget.hpp"

cxxplot_WARNOFF
#include "qcustomplot.h"
  cxxplot_WARNON

#include <iostream>

  namespace cxxplot
{

  graph::graph( graph && other ) :

    QCPGraph_( std::exchange( other.QCPGraph_, nullptr ) ),

    parent_figure_( std::exchange( other.parent_figure_, nullptr ) ),

    x_range_( std::move( other.x_range_ ) ),

    y_range_( std::move( other.y_range_ ) )
  {
  }

  graph::graph( graph && other, figure * new_parent ) :
    QCPGraph_( std::exchange( other.QCPGraph_, nullptr ) ), parent_figure_( new_parent ),
    x_range_( std::move( other.x_range_ ) ), y_range_( std::move( other.y_range_ ) )
  {
  }

  graph::graph( widget * customPlot, figure * parent ) : parent_figure_( parent )
  {
    QCPGraph_ = customPlot->addGraph( );

    QCPGraph_->setAdaptiveSampling( false ); // TODO: Expose this to the user

    set_line_width( 2.0 );
  }

  void graph::set_data( const QVector< double >& x, const QVector< double >& y )
  {
    // TODO: Non-blocking? Tested and didn't see any benefits. Check again
    invoke_blocking( [ this, &x, &y ]( ) {
      auto doNotSort = true;

      QCPGraph_->setData( x, y, doNotSort ); // If this supported move we could speed things up

      for ( const auto& xv : x )
      {
        x_range_.expand( xv );
      }

      for ( const auto& yv : y )
      {
        y_range_.expand( yv );
      }
    } );

    parent_figure_->handle_updated_visual_items( );
  }

  void graph::append_data( const double& x, const double& y )
  {
    x_range_.expand( x );

    y_range_.expand( y );

    invoke_blocking( [ this, x, y ]( ) { QCPGraph_->appendData( x, y ); } );

    parent_figure_->handle_updated_visual_items( );
  }

  void graph::append_data( const QVector< double >& x, const QVector< double >& y )
  {
    // TODO: range exapand or pick for QCPGraph?
    invoke_blocking( [ this, &x, &y ]( ) // TODO: We probably can pass by reference here instead of by value. Test
                     {
                       auto doNotSort = true;
                       QCPGraph_->addData( x, y, doNotSort );
                     } );

    parent_figure_->handle_updated_visual_items( );
  }

  void graph::set_line_width( qreal width )
  {
    invoke_blocking( [ this, width ]( ) {
      QPen pen = QCPGraph_->pen( );
      pen.setWidthF( width );
      QCPGraph_->setPen( pen );
    } );
  }

  void graph::set_marker_size( double width )
  {
    auto ss = QCPGraph_->scatterStyle( );
    ss.setSize( width );
    QCPGraph_->setScatterStyle( ss );
  }

  void graph::set_marker_line_width( qreal width )
  {
    invoke_blocking( [ this, width ]( ) {
      auto ss  = QCPGraph_->scatterStyle( );
      QPen pen = ss.pen( );
      pen.setColor( QCPGraph_->pen( ).color( ) );
      pen.setStyle( Qt::SolidLine );
      pen.setWidthF( width );
      ss.setPen( pen );
      QCPGraph_->setScatterStyle( ss );
    } );
  }

  void graph::setMarkerStyle( const MarkerStyle& style )
  {
    invoke_blocking( [ this, style ]( ) {
      auto ss = QCPGraph_->scatterStyle( );
      ss.setShape( toQCPScatterShape( style ) );
      QCPGraph_->setScatterStyle( ss );
    } );

    parent_figure_->handle_updated_visual_items( );
  }

  void graph::set_line_style( const LineStyle& style )
  {
    invoke_blocking( [ this, style ]( ) {
      if ( style == LineStyle::None )
      {
        QCPGraph_->setLineStyle( QCPGraph::lsNone );
      }
      else
      {
        QPen pen = QCPGraph_->pen( );

        pen.setStyle( to_qt_pen_style( style ) );

        QCPGraph_->setPen( pen );
      }
    } );

    parent_figure_->handle_updated_visual_items( );
  }

  void graph::set_color_rgb( const int& r, const int& g, const int& b )
  {
    auto throwIfOutOfRange( []( const int& v ) {
      if ( v < 0 || v > 255 )
      {
        throw_stream< std::out_of_range > ts;

        ts << "Color component out of range. Requested value: " << v << ". Allowable range: [0,255].";

        ts.throw_now( );
      }
    } );

    throwIfOutOfRange( r );
    throwIfOutOfRange( g );
    throwIfOutOfRange( b );

    set_color( color::rgb( uint8_t( r ), uint8_t( g ), uint8_t( b ) ) );
  }

  void graph::set_color( const color::rgb& c )
  {
    invoke_blocking( [ this, c ]( ) {
      QPen pen = QCPGraph_->pen( );

      pen.setColor( QColor( c.r, c.g, c.b ) );

      QCPGraph_->setPen( pen );
    } );

    parent_figure_->handle_updated_visual_items( );
  }

  void graph::set_name( std::string n ) // TODO: We need a copy here. Check out why
  {
    auto str = QString::fromStdString( n );
    invoke_blocking( [ this, str ]( ) { QCPGraph_->setName( str ); } );
  }

  void graph::set_visible_in_legend( const bool& visible_in_legend )
  {
    invoke_blocking( [ this, visible_in_legend ]( ) {
      if ( visible_in_legend )
      {
        QCPGraph_->addToLegend( );
      }
      else
      {
        QCPGraph_->removeFromLegend( );
      }
    } );
  }

  const struct range& graph::x_range( ) const
  {
    return x_range_;
  }

  const struct range& graph::y_range( ) const
  {
    return y_range_;
  }

  int graph::data_size( ) const
  {
    return QCPGraph_->data( )->size( );
  }

  QPointer< widget > graph::plot_window( )
  {
    return parent_figure_->plot_widget( );
  }
}
