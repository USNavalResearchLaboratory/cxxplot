#include "cxxplot/figure.hpp"
#include "cxxplot/window.hpp"
#include <cxxplot/color.hpp>

#include "execution_p.hpp"
#include "widget.hpp"

#include <sstream>

namespace cxxplot
{

figure::figure( figure&& other ) :
  autoFit_( std::exchange( other.autoFit_, true ) ),
  user_x_range_( std::exchange( other.user_x_range_, range( ) ) ),
  user_y_range_( std::exchange( other.user_y_range_, range( ) ) ),
  user_set_range_( std::exchange( other.user_set_range_, false ) ),
  canonical_x_range_( std::exchange( other.canonical_x_range_, range( ) ) ),
  canonical_y_range_( std::exchange( other.canonical_y_range_, range( ) ) )
{
  window_proxy_                    = new window_proxy( other.window_proxy_->plot_widget_ );
  window_proxy_->redraw_on_destroy = false;
  for ( auto& g : other.graphs_ )
  {
    graphs_.emplace_back( new cxxplot::graph( std::move( *( g.get( ) ) ), this ) );
  }
}

figure::figure( QPointer< widget > plot_widget )
{
  window_proxy_                    = new window_proxy( plot_widget );
  window_proxy_->redraw_on_destroy = false;

  set_axes_line_width( 1.0 );

  auto pen = plot_widget->xAxis->grid( )->pen( );
  pen.setWidthF( 0.5 );
  plot_widget->xAxis->grid( )->setPen( pen );

  pen = plot_widget->yAxis->grid( )->pen( );
  pen.setWidthF( 0.5 );
  plot_widget->yAxis->grid( )->setPen( pen );

  //  auto col = color::default_order[graphs_.size() % color::default_order.size()];

  //  graphs_.emplace_back( new cxxplot::graph( window_proxy_->plot_widget_, this ) );

  //  graphs_.rbegin()->get()->set_color( col );
}

void figure::fit_to_data( const bool& request_replot )
{
  invoke_blocking( [ this ]( ) {
    // parent_window_proxy_->plot_window_->rescaleAxes(); //TODO: Why the present line doesn't
    // work?
    const bool force_match_user_ranges = true;
    update_axes( force_match_user_ranges );
  } );

  if ( request_replot )
  {
    window_proxy_->handle_updated_visual_items( *this );
  }
}

void figure::set_xlim( double lower, double upper )
{
  if ( !window_proxy_->is_plot_valid( "set_xlim" ) )
  {
    return;
  }

  autoFit_ = false;

  user_x_range_.min = lower;
  user_x_range_.max = upper;
  user_set_range_   = true;

  update_canonical_ranges( );

  invoke_blocking(
    [ this, &lower, &upper ]( ) { window_proxy_->plot_widget_->xAxis->setRange( lower, upper ); } );

  window_proxy_->handle_updated_visual_items( *this );
}

void figure::set_ylim( double lower, double upper )
{
  if ( !window_proxy_->is_plot_valid( "set_ylim" ) )
  {
    return;
  }

  autoFit_ = false;

  user_y_range_.min = lower;
  user_y_range_.max = upper;
  user_set_range_   = true;

  update_canonical_ranges( );

  invoke_blocking(
    [ this, &lower, &upper ]( ) { window_proxy_->plot_widget_->yAxis->setRange( lower, upper ); } );

  window_proxy_->handle_updated_visual_items( *this );
}

class graph& figure::add_graph( )
{
  // TODO: The reference here may get invalidated if one adds a graph after getting a refernce to
  // a previusly inserted graph. Should graphs allow a copy and have the semantics of
  // a smart pointer ?
  invoke_blocking( [ this ]( ) {
    auto col = color::default_order[ graphs_.size( ) % color::default_order.size( ) ];

    graphs_.emplace_back( new cxxplot::graph( window_proxy_->plot_widget_, this ) );

    graphs_.rbegin( )->get( )->set_color( col );
  } );

  return *( *( graphs_.rbegin( ) ) );
}

graph& figure::graph( const std::size_t& i )
{
  if ( i >= graphs_.size( ) )
  {
    std::stringstream ss;
    ss << "You are trying to access graph " << i + 1 << " of " << graphs_.size( ) << " graphs.";
    throw std::out_of_range( ss.str( ) );
  }
  return *( ( graphs_[ i ] ).get( ) );
}

void figure::set_auto_fit( const bool& ar )
{
  autoFit_ = ar;
}

void figure::set_axes_aspect_ratio( const double& r )
{
  if ( !window_proxy_->is_plot_valid( "set_axis_ratio" ) )
  {
    return;
  }

  axis_ratio_ = r;

  if ( user_set_range_ )
  {
    canonical_x_range_ = user_x_range_;
    canonical_y_range_ = user_y_range_;
  }
  else
  {
    canonical_x_range_.min = window_proxy_->plot_widget_->xAxis->range( ).lower;
    canonical_x_range_.max = window_proxy_->plot_widget_->xAxis->range( ).upper;
    canonical_y_range_.min = window_proxy_->plot_widget_->yAxis->range( ).lower;
    canonical_y_range_.max = window_proxy_->plot_widget_->yAxis->range( ).upper;
  }

  window_proxy_->plot_widget_->yAxis->setScaleRatio( window_proxy_->plot_widget_->xAxis,
                                                     axis_ratio_ );

  window_proxy_->handle_updated_visual_items( *this );
}

void figure::set_x_axis_scaling_type( const axis_scaling_type& sc )
{
  if ( sc == axis_scaling_type::linear )
  {
    window_proxy_->plot_widget_->xAxis->setScaleType( QCPAxis::stLinear );
    QSharedPointer< QCPAxisTicker > ticker(
      new QCPAxisTicker ); // TODO: Reverting to linear has not been tested
    window_proxy_->plot_widget_->xAxis->setTicker( ticker );
    window_proxy_->plot_widget_->xAxis->setNumberFormat( "gb" );
    window_proxy_->plot_widget_->xAxis->setNumberPrecision( 6 );
  }
  else
  {
    window_proxy_->plot_widget_->xAxis->setScaleType( QCPAxis::stLogarithmic );
    QSharedPointer< QCPAxisTickerLog > logTicker( new QCPAxisTickerLog );
    window_proxy_->plot_widget_->xAxis->setTicker( logTicker );
    window_proxy_->plot_widget_->xAxis->setNumberFormat( "eb" );
    window_proxy_->plot_widget_->xAxis->setNumberPrecision( 1 );
  }
}

void figure::set_y_axis_scaling_type( const axis_scaling_type& sc )
{
  if ( sc == axis_scaling_type::linear )
  {
    window_proxy_->plot_widget_->yAxis->setScaleType( QCPAxis::stLinear );
    QSharedPointer< QCPAxisTicker > ticker(
      new QCPAxisTicker ); // TODO: Reverting to linear has not been tested
    window_proxy_->plot_widget_->yAxis->setTicker( ticker );
    window_proxy_->plot_widget_->yAxis->setNumberFormat( "gb" );
    window_proxy_->plot_widget_->yAxis->setNumberPrecision( 6 );
  }
  else
  {
    window_proxy_->plot_widget_->yAxis->setScaleType( QCPAxis::stLogarithmic );
    QSharedPointer< QCPAxisTickerLog > logTicker( new QCPAxisTickerLog );
    window_proxy_->plot_widget_->yAxis->setTicker( logTicker );
    window_proxy_->plot_widget_->yAxis->setNumberFormat( "eb" );
    window_proxy_->plot_widget_->yAxis->setNumberPrecision( 0 );
  }
}

QPointer< widget > figure::plot_widget( )
{
  return window_proxy_->plot_widget_;
}

void figure::handle_updated_visual_items( )
{
  if ( autoFit_ )
  {
    fit_to_data( );
  }
  else
  {
    window_proxy_->handle_updated_visual_items( *this );
  }
}

void cxxplot::figure::update_canonical_ranges( )
{
  if ( user_set_range_ == true )
  {
    canonical_x_range_ = user_x_range_;
    canonical_y_range_ = user_y_range_;
  }
  else
  {
    canonical_x_range_.min = window_proxy_->plot_widget_->xAxis->range( ).lower;
    canonical_x_range_.max = window_proxy_->plot_widget_->xAxis->range( ).upper;
    canonical_y_range_.min = window_proxy_->plot_widget_->yAxis->range( ).lower;
    canonical_y_range_.max = window_proxy_->plot_widget_->yAxis->range( ).upper;
  }
}

void cxxplot::figure::match_user_ranges_to_data( )
{
  auto reset_ranges = false;
  for ( const auto& g : graphs_ )
  {
    if ( g->data_size( ) != 0 )
    {
      reset_ranges = true;
    }
  }

  if ( reset_ranges )
  {
    user_x_range_ = range( );
    user_y_range_ = range( );

    for ( const auto& g : graphs_ )
    {
      if ( g->data_size( ) != 0 )
      {
        user_x_range_.expand( g->x_range( ) );
        user_y_range_.expand( g->y_range( ) );
      }
    }

    user_set_range_ = true;
  }
}

void cxxplot::figure::update_user_ranges( const bool& match_user_ranges )
{
  if ( match_user_ranges )
  {
    match_user_ranges_to_data( );
    update_canonical_ranges( );
  }
}

void figure::update_axes( const bool& force_match_user_ranges )
{
  update_user_ranges( autoFit_ || force_match_user_ranges );

  window_proxy_->plot_widget_->xAxis->setRange( canonical_x_range_.min, canonical_x_range_.max );
  window_proxy_->plot_widget_->yAxis->setRange( canonical_y_range_.min, canonical_y_range_.max );

  // We need this otherwise QCustomPlot's ranges will be confined in the user ranges.
  if ( axis_ratio_ != 0.0 )
  {
    plot_widget( )->yAxis->setScaleRatio( plot_widget( )->xAxis, axis_ratio_ );
  }
}

std::string figure::get_xlabel( ) const
{
  if ( !window_proxy_->is_plot_valid( "get_xlabel" ) )
  {
    return "";
  }

  return window_proxy_->plot_widget_->xAxis->label( ).toStdString( );
}

void figure::set_xlabel( std::string new_label )
{
  if ( !window_proxy_->is_plot_valid( "set_xlabel" ) )
  {
    return;
  }

  window_proxy_->plot_widget_->xAxis->setLabel( QString::fromStdString( new_label ) );

  window_proxy_->handle_updated_visual_items( *this );
}

std::string figure::get_ylabel( ) const
{
  if ( !window_proxy_->is_plot_valid( "get_ylabel" ) )
  {
    return "";
  }

  return window_proxy_->plot_widget_->yAxis->label( ).toStdString( );
}

void figure::set_ylabel( std::string new_label )
{
  if ( !window_proxy_->is_plot_valid( "set_ylabel" ) )
  {
    return;
  }

  window_proxy_->plot_widget_->yAxis->setLabel( QString::fromStdString( new_label ) );

  window_proxy_->handle_updated_visual_items( *this );
}

void figure::set_axes_line_width( const double& t )
{
  invoke_blocking( [ this, t ]( ) {
    {
      auto pen = window_proxy_->plot_widget_->xAxis->tickPen( );
      pen.setWidthF( t );
      window_proxy_->plot_widget_->xAxis->setTickPen( pen );
    }

    {
      auto pen = window_proxy_->plot_widget_->xAxis->basePen( );
      pen.setWidthF( t );
      window_proxy_->plot_widget_->xAxis->setBasePen( pen );
    }

    {
      auto pen = window_proxy_->plot_widget_->xAxis2->basePen( );
      pen.setWidthF( t );
      window_proxy_->plot_widget_->xAxis2->setBasePen( pen );
    }

    {
      auto pen = window_proxy_->plot_widget_->yAxis->tickPen( );
      pen.setWidthF( t );
      window_proxy_->plot_widget_->yAxis->setTickPen( pen );
    }

    {
      auto pen = window_proxy_->plot_widget_->yAxis->basePen( );
      pen.setWidthF( t );
      window_proxy_->plot_widget_->yAxis->setBasePen( pen );
    }
    {
      auto pen = window_proxy_->plot_widget_->yAxis2->basePen( );
      pen.setWidthF( t );
      window_proxy_->plot_widget_->yAxis2->setBasePen( pen );
    }
  } );
}
}
