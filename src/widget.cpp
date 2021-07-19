#include "widget.hpp"

#include "execution_p.hpp"

#include <iostream>
#include <thread>

namespace cxxplot
{

widget::widget( QWidget* parent ) : QCustomPlot( parent )
{
  setMinimumSize( 100, 100 );

  auto pen = this->legend->borderPen( );
  pen.setWidthF( 1.0 );
  this->legend->setBorderPen( pen );

  setContextMenuPolicy( Qt::CustomContextMenu );

  connect( this,
           SIGNAL( customContextMenuRequested( const QPoint& ) ),
           this,
           SLOT( showContextMenu( const QPoint& ) ) );

  connect( this, SIGNAL( afterReplot( ) ), this, SLOT( handle_after_replot( ) ) );

  redraw_elapsed_timer_.restart( );
}

void widget::set_open_gl( const bool& opengl, const bool& experimental_warning )
{

  this->setOpenGl( opengl );

  if ( ( experimental_warning ) && ( opengl ) && ( this->openGl( ) ) )
  {
    std::cerr << "cxxplot warning: OpenGL is an experimental feature and will probably result "
                 "in rendering artifacts when using multiple OpenGL windows."
              << std::endl;
  }

  if ( opengl && !this->openGl( ) )
  {
    std::cerr << "cxxplot warning: Could not use OpenGL acceleration. Using software rendering"
              << std::endl;
  }
}
void widget::redraw_if_auto_redraw( )
{
  if ( auto_redraw_ )
  {
    redraw( );
  }
}

void widget::redraw( )
{
  if ( !last_draw_complete_.load( ) )
  {
    return;
  }

  if ( !is_plot_valid( "redraw" ) )
  {
    return;
  }

  auto elapsed  = redraw_elapsed_timer_.elapsed( );
  auto interval = minRedrawInterval_ms.load( );

  if ( elapsed <= interval && ( interval != 0 ) )
  {
    if ( !redraw_scheduled_.load( ) )
    {
      invoke_blocking( [ this, elapsed, interval ]( ) {
        redraw_scheduled_   = true;
        last_draw_complete_ = true;
        QTimer::singleShot( int( interval - elapsed ), this, SLOT( replot( ) ) );
      } );
    }
    return;
  }

  last_draw_complete_ = false;

  invoke_nonblocking( [ this ]( ) { this->replot( ); }, this );
}

const bool& widget::get_auto_redraw( ) const
{
  return auto_redraw_;
}

void widget::set_auto_redraw( const bool& yesno )
{
  auto_redraw_ = yesno;

  redraw( );
}

const bool& widget::get_shown_once( )
{
  return shown_once_;
}

const bool& widget::get_silence_subsequent_warnings( ) const
{
  return silence_subsequent_warnings_;
}

void widget::set_silence_subsequent_warnings( const bool& sw )
{
  silence_subsequent_warnings_ = sw;
}

const bool& widget::get_antialiasing( ) const
{
  return antialiasing_;
}

void widget::set_antialiasing( const bool& yesno )
{
  if ( yesno )
  {
    this->setAntialiasedElements( QCP::AntialiasedElement::aeAll );
  }
  else
  {
    this->setNotAntialiasedElements( QCP::AntialiasedElement::aeAll );
  }

  antialiasing_ = yesno;
}

bool widget::get_show_legend( ) const
{
  return this->legend->visible( );
}
void widget::set_show_legend( const bool& yesno )
{
  this->legend->setVisible( yesno );
}

void widget::set_legend_alignment( const alignment_t& a )
{
  this->axisRect( )->insetLayout( )->setInsetAlignment( 0, Qt::AlignmentFlag( a.value( ) ) );
}

bool widget::is_plot_valid( const char* command_name )
{
  if ( isHidden( ) && get_shown_once( ) && !get_silence_subsequent_warnings( ) )
  {
    std::cerr << "Warning: Plot window was closed. Command '" << command_name
              << "' ignored. Subsequent command calls for this window will "
                 "be silenced."
              << std::endl;

    set_silence_subsequent_warnings( true );

    return false;
  }
  return true;
}

const class figure& widget::figure( const std::size_t i ) const
{
  return figures_.at( i );
}

class figure& widget::figure( const std::size_t i )
{
  return figures_.at( i );
}

const std::vector< class figure >& widget::figures( ) const
{
  return figures_;
}

std::vector< class figure >& widget::figures( )
{
  return figures_;
}

void widget::showEvent( QShowEvent* )
{
  shown_once_                  = true;
  silence_subsequent_warnings_ = false;
}

void widget::resizeEvent( QResizeEvent* event )
{
  Q_UNUSED( event )

  setViewport( rect( ) );

  // We need this every time because QCPGraph doesn't keep track of possible user defined range when
  // used in conjuction with scaleRatio.
  for ( auto& f : figures_ )
  {
    const bool force_match_ranges = false;
    f.update_axes( force_match_ranges );
  }

  replot( rpQueuedRefresh );
}

void widget::showContextMenu( const QPoint& pos )
{
  QMenu contextMenu( tr( "Context menu" ), this );

  QAction action1( "Save as", this );
  connect( &action1, SIGNAL( triggered( ) ), this, SLOT( save( ) ) );
  contextMenu.addAction( &action1 );

  contextMenu.exec( mapToGlobal( pos ) );
}

bool cxxplot::widget::save( const std::string& file_name,
                            const int&         width,
                            const int&         height,
                            const bool&        force_overwrite )
{
  bool ret = false;

  invoke_blocking( [ this, &ret, file_name, width, height, force_overwrite ] {



    auto fname = QString::fromStdString( file_name );

    QFileInfo fi( fname );

    if ( !force_overwrite && fi.exists( ) )
    {
      auto response
        = QMessageBox::warning( this,
                                tr( "cxxplot" ),
                                tr( "File %1 exists. Overwrite?" ).arg( fi.absoluteFilePath( ) ),
                                QMessageBox::Yes | QMessageBox::No );

      if ( response == QMessageBox::No )
      {
        ret = true;
        return;
      }
    }

    auto ext = fi.suffix( ).toLower( );

    if ( ext == "pdf" )
    {
      ret = savePdf( fname, width, height );
      return;
    }
    else if ( ext == "jpg" )
    {
      ret = saveJpg( fname, width, height );
      return;
    }
    else if ( ext == "bmp" )
    {
      ret = saveBmp( fname, width, height );
      return;
    }
    else if ( ext == "png" )
    {
      ret = savePng( fname, width, height );
      return;
    }

    QMessageBox::warning(
      this,
      tr( "cxxplot" ),
      tr( "Please provide the export file type extension (.pdf, .jpg, .bmp, or .png)" ),
      QMessageBox::Ok );

    ret = false;
  } );

  return ret;
}

void widget::save( )
{
  QString file_name = QFileDialog::getSaveFileName(
    this, tr( "Save File" ), QDir::currentPath( ), tr( "Save formats (*.png *.bmp *.jpg *.pdf)" ) );

  if ( file_name.isEmpty( ) )
  {
    return;
  }

  const bool overwrite = true; // The user has already been prompted.

  if ( !save( file_name.toStdString( ), 0, 0, overwrite ) )
  {
    QMessageBox::warning( this,
                          tr( "cxxplot" ),
                          tr( "Could not save to file: \n%1" ).arg( file_name ),
                          QMessageBox::Ok );
  }
}

void widget::set_min_redraw_interval( int msec )
{
  minRedrawInterval_ms = msec;
}

int widget::get_min_redraw_interval( )
{
  return minRedrawInterval_ms.load( );
}

void widget::wait_redraw_to_finish( )
{
  while ( !last_draw_complete_.load( ) || redraw_scheduled_.load( ) )
  {
    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }
}

void widget::handle_after_replot( )
{
  last_draw_complete_ = true;
  redraw_scheduled_   = false;
  redraw_elapsed_timer_.restart( );
}
}
