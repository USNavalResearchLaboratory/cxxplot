#include "cxxplot/window.hpp"

#include "execution_p.hpp"
#include "widget.hpp"

#include <QDebug>
#include <QMetaObject>

#include <sstream>

namespace cxxplot
{

window_proxy::window_proxy( const bool& show )
{
  invoke_blocking( [ this, show ]( ) {
    plot_widget_ = new widget;

    plot_widget_->xAxis->setLabel( "x" );
    plot_widget_->xAxis->grid( )->setSubGridVisible( true );
    plot_widget_->xAxis->setRange( -1.0, 1.0 );

    plot_widget_->yAxis->setLabel( "y" );
    plot_widget_->yAxis->grid( )->setSubGridVisible( true );
    plot_widget_->yAxis->setRange( -1.0, 1.0 );

    plot_widget_->yAxis2->setVisible( true );
    plot_widget_->xAxis2->setVisible( true );
    plot_widget_->xAxis2->setTicks( false );
    plot_widget_->yAxis2->setTicks( false );
    plot_widget_->xAxis2->setTickLabels( false );
    plot_widget_->yAxis2->setTickLabels( false );

    figures( ).emplace_back( cxxplot::figure( plot_widget_ ) );

    plot_widget_->resize( 640, 480 );

    // TODO: Update the range after interaction
    plot_widget_->setInteraction( QCP::iRangeDrag, true );
    plot_widget_->setInteraction( QCP::iRangeZoom, true );

    if ( show )
    {
      this->show( );
    }

    connect( QApplication::instance( ), SIGNAL( aboutToQuit( ) ), plot_widget_, SLOT( deleteLater( ) ) );
  } );

  if ( show )
  {
    plot_widget_->redraw_if_auto_redraw( );
  }
}

window_proxy::window_proxy( QPointer< widget > plot_widget ) :

  plot_widget_( plot_widget )
{
}

window_proxy::window_proxy( window_proxy&& other ) : plot_widget_( std::exchange( other.plot_widget_, nullptr ) )
{
}

window_proxy::~window_proxy( )
{
  if ( plot_widget_ != nullptr && redraw_on_destroy )
  {
    // We may get some redundant redraws if we have many window_proxies. This shouldn't be a big
    // concern but need to keep it in mind
    redraw( );
  }
}

void window_proxy::set_open_gl( const bool& opengl, const bool experimental_warning )
{
  valid_or_throw( );

  invoke_blocking(
    [ this, opengl, experimental_warning ]( ) { plot_widget_->set_open_gl( opengl, experimental_warning ); } );
}

void window_proxy::show( )
{
  invoke_blocking( [ this ]( ) { plot_widget_->show( ); } );
}

bool window_proxy::is_plot_valid( const char* command_name )
{
  if ( !plot_widget_ )
  {
    return false;
  }
  return plot_widget_->is_plot_valid( command_name );
}

void window_proxy::set_min_redraw_interval( int msec )
{
  valid_or_throw( );

  plot_widget_->set_min_redraw_interval( msec );
}

int window_proxy::get_min_redraw_interval( )
{
  valid_or_throw( );

  return plot_widget_->get_min_redraw_interval( );
}

std::string window_proxy::get_title( ) const
{
  valid_or_throw( );

  return plot_widget_->windowTitle( ).toStdString( );
}

void cxxplot::window_proxy::valid_or_throw( ) const
{
  if ( !plot_widget_ )
  {
    throw std::runtime_error(
      "Invalid window_proxy. Did you possibly move the window_proxy to a different window_proxy?" );
  }
}

void window_proxy::set_title( std::string name )
{
  valid_or_throw( );

  invoke_nonblocking( [ this, name ]( ) { plot_widget_->setWindowTitle( QString::fromStdString( name ) ); },
                      plot_widget_ );
}

void window_proxy::redraw( )
{
  valid_or_throw( );

  plot_widget_->redraw( );
}

void window_proxy::wait_redraw_to_finish( )
{
  valid_or_throw( );

  plot_widget_->wait_redraw_to_finish( );
}

bool window_proxy::save( const std::string& name, const int& width, const int& height, const bool force_overwrite )
{
  valid_or_throw( );

  return plot_widget_->save( name, width, height, force_overwrite );
}

void window_proxy::set_fonts_size( const double& s )
{
  valid_or_throw( ); // TODO: enable this on all member functions of move only classes.

  auto f = plot_widget_->xAxis->tickLabelFont( );
  f.setPointSizeF( qreal( s ) );
  plot_widget_->xAxis->setTickLabelFont( f );

  f = plot_widget_->yAxis->tickLabelFont( );
  f.setPointSizeF( qreal( s ) );
  plot_widget_->yAxis->setTickLabelFont( f );

  f = plot_widget_->xAxis->labelFont( );
  f.setPointSizeF( qreal( s ) );
  plot_widget_->xAxis->setLabelFont( f );

  f = plot_widget_->yAxis->labelFont( );
  f.setPointSizeF( qreal( s ) );
  plot_widget_->yAxis->setLabelFont( f );

  if ( plot_widget_->legend )
  {
    auto font = plot_widget_->legend->font( );
    font.setPointSizeF( qreal( s ) );
    plot_widget_->legend->setFont( font );
  }
}

class figure& window_proxy::create_first_figure( )
{
  if ( figures( ).size( ) > 0 )
  {
    throw std::runtime_error( "At least one figure already exists." );
  }

  invoke_blocking( [ this ]( ) { figures( ).emplace_back( cxxplot::figure( plot_widget_ ) ); } );

  return *( figures( ).rbegin( ) );
}

std::array< int, 2 > window_proxy::get_size( ) const
{
  valid_or_throw( );

  auto s = plot_widget_->size( );
  return { s.width( ), s.height( ) };
}

void window_proxy::set_size( const int& width, const int& height )
{
  valid_or_throw( );

  invoke_blocking( [ this, &width, &height ]( ) { plot_widget_->resize( width, height ); } );
}

std::array< int, 2 > window_proxy::get_position( ) const
{
  valid_or_throw( );

  auto p = plot_widget_->pos( );
  return { p.x( ), p.y( ) };
}

void window_proxy::set_position( const int& x, const int& y )
{
  valid_or_throw( );

  invoke_blocking( [ this, &x, &y ]( ) { plot_widget_->move( x, y ); } );
}

const bool& window_proxy::get_auto_redraw( ) const
{
  valid_or_throw( );

  return plot_widget_->get_auto_redraw( );
}

void window_proxy::set_auto_redraw( const bool& yesno )
{
  valid_or_throw( );

  plot_widget_->set_auto_redraw( yesno );
}

const bool& window_proxy::get_antialiasing( ) const
{
  valid_or_throw( );

  return plot_widget_->get_antialiasing( );
}

void window_proxy::set_antialiasing( const bool& yesno )
{
  valid_or_throw( );

  plot_widget_->set_antialiasing( yesno );
}

bool window_proxy::get_show_legend( ) const
{
  valid_or_throw( );

  // TODO: shouldn't show legened be a property of the figure?
  return plot_widget_->get_show_legend( );
}

void window_proxy::set_show_legend( const bool& yesno )
{ // This is a bit of a problem because legend should be a property of the figure. QCustomplot
  // though
  // has this a bit mixed.
  valid_or_throw( );

  plot_widget_->set_show_legend( yesno );
}

void window_proxy::set_legend_alignment( const alignment_t& a )
{
  valid_or_throw( );

  plot_widget_->set_legend_alignment( a );
}

void window_proxy::set_legend_columns( const int& cols )
{
  valid_or_throw( );

  invoke_blocking( [ this, cols ]( ) {
    plot_widget_->legend->setWrap( cols );
    plot_widget_->legend->setRowSpacing( 0 );
    plot_widget_->legend->setColumnSpacing( 4 );
    plot_widget_->legend->setFillOrder( QCPLayoutGrid::FillOrder::foColumnsFirst, true );
  } );
}

const std::vector< class figure >& window_proxy::figures( ) const
{
  valid_or_throw( );

  return plot_widget_->figures( );
}

std::vector< class figure >& window_proxy::figures( )
{
  valid_or_throw( );

  return plot_widget_->figures( );
}

class figure& window_proxy::figure( const std::size_t& i )
{
  valid_or_throw( );

  if ( i >= figures( ).size( ) )
  {
    std::stringstream ss;
    ss << "You are trying to access figure " << i + 1 << " of " << figures( ).size( ) << " figures.";
    throw std::out_of_range( ss.str( ) );
  }
  return plot_widget_->figure( i );
}

void window_proxy::handle_updated_visual_items( const class figure& /*f*/ )
{
  valid_or_throw( );

  plot_widget_->redraw_if_auto_redraw( );
}

void window_proxy::handle_updated_visual_items( const graph& /*g*/ )
{
}

}
