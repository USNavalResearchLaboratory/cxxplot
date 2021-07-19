#include "execution_p.hpp"
#include <cxxplot/execution.hpp>

cxxplot_WARNOFF
#include <QDir>
#include <QThread>
#include <QWindow>
#include <QMessageBox>
  cxxplot_WARNON;

#include <iostream>
#include <thread>

#include <cstring>

class Application : public QApplication
{
public:

  Application( int &argc, char **argv ) :
    QApplication( argc, argv )
  {
  }

  bool notify(QObject *receiver_, QEvent *event_) override
  {
    QString error;

    try
    {
      return QApplication::notify(receiver_, event_);
    }
    catch ( std::exception &e )
    {
      error = e.what();
    }
    catch ( ... )
    {
      error = "Unspecified error";
    }

    if ( !error.isEmpty() )
    {
      QMessageBox msgBox;

      msgBox.setWindowTitle( applicationName() );

      msgBox.setText( QString( "Error: " ) + error + "\nClosing..."  );
      msgBox.exec();

      QApplication::quit();
    }
    return false;
  }

};

inline void initImageResources( )
{
  Q_INIT_RESOURCE( images );
}

namespace cxxplot
{

int exec( int argc, char* argv[], std::function< int( ) > f )
{
  initImageResources( );

  Application app( argc, argv );

  app.setWindowIcon( QIcon( "://cxxplot_logo" ) );

  Application::setQuitOnLastWindowClosed( false );

  QThread user_process_thread;

  QObject destroyer;

  auto user_process = new mainWorker( std::move( f ) );
  user_process->setParent( &destroyer );

  destroyer.moveToThread( &user_process_thread );
  user_process->moveToThread( &user_process_thread );

  user_process_thread.start( );

  QMetaObject::invokeMethod( user_process, "go" );

  Application::exec( );

  user_process_thread.quit( );

  user_process_thread.wait( );

  return user_process->return_value( );
}

mainWorker::mainWorker( std::function< int( ) >&& f, QObject* parent ) :

  QObject( parent ), func_( std::forward< std::function< int( ) > >( f ) )

{
}

void mainWorker::go( )
{
  ret_ = func_( );

  Application::setQuitOnLastWindowClosed( true );

  bool aWindowIsVisible = false;
  auto wins             = Application::allWindows( );
  for ( auto* win : wins )
  {
    if ( win->isVisible( ) )
    {
      aWindowIsVisible = true;
      break;
    }
  }

  if ( !aWindowIsVisible )
  {
    QMetaObject::invokeMethod( Application::instance( ), "quit", Qt::QueuedConnection );
  }

  emit finished( );
}

int mainWorker::return_value( )
{
  return ret_;
}

async_invoker::async_invoker( std::function< void( ) > f, QObject* parent ) : f_( std::move( f ) )
{
  this->moveToThread( Application::instance( )->thread( ) );

  invoke_blocking( [ this, parent ]( ) {
    this->setParent( parent );

    QMetaObject::invokeMethod( this, "process" );
  } );
}

void async_invoker::process( )
{
  f_( );

  emit finished( );

  delete this;
}

invoker::invoker( std::function< void( ) > f ) : f_( std::move( f ) )
{
}

bool invoker::event( QEvent* ev )
{
  if ( ev->type( ) == QEvent::User )
  {
    {
      std::unique_lock< std::mutex > lock( mutex );

      f_( );

      executed_ = true;
    }

    until_done.notify_all( );

    return true;
  }

  return false;
}
//!
//! \brief invoke_blocking Executes a function in the qt thread using the qt event loop and
//! waits to complete. This should not be called from the QApplication thread (main thread).
//! \param f the function to be executed
//!
void invoke_blocking( std::function< void( ) > f )
{

  if ( Application::instance( )->thread( ) == QThread::currentThread( ) )
  {
    // std::cerr << "Warning: QApplication thread calling invoke blocking." << std::endl;

    f( );

    // The following would not work because the event will be put into the queue, but we would
    // expect it to be executed right away when called
    //    auto inv = new invoker( std::move( f ) );
    //    QObject::connect( inv, SIGNAL( finished( ) ), inv, SLOT( deleteLater( ) ) );
    //    QApplication::postEvent( inv, new QEvent( QEvent::User ) );
  }
  else
  {
    auto inv = invoker( std::move( f ) );

    inv.moveToThread( Application::instance( )->thread( ) );

    std::unique_lock< std::mutex > lk( inv.mutex );

    Application::postEvent( &inv, new QEvent( QEvent::User ) );

    inv.until_done.wait(
      lk, [ &inv]( ) -> const auto& { return inv.executed( ); } );
  }
}

//!
//! \brief invoke_blocking Submits a function to the qt thread and the qt event loop and
//! lets it run asynchronously.
//! \param f the function to be executed
//!
void invoke_nonblocking( std::function< void( ) > f, QObject* parent )
{
  new async_invoker( std::move( f ), parent ); // Will destroy itself when finished
}

void cxxplot::experimental::gui_thread::init( int&        argc,
                                              char*       argv[],
                                              const bool& portability_warning )
{
  if ( portability_warning )
  {
    std::cerr << "cxxplot::warning: cxxplot::gui_thread might not be portable to macOS. "
                 "Please use cxxplot::exec if portability is desired."
              << std::endl;
  }

  int*   argc1 = &argc;
  char** argv1 = argv;

  initialized_ = false;

  std_thread_ = std::thread( [ argc1, argv1, this ]( ) {
    initImageResources( );

    Application app( *argc1, argv1 );

    app.setWindowIcon( QIcon( "://cxxplot_logo" ) );

    initialized_ = true;

    cv.notify_one( );

    app.exec( );
  } );

  std::unique_lock< std::mutex > lk( m );
  cv.wait( lk, [ this ] { return initialized_; } );
}

experimental::gui_thread::gui_thread( int& argc, char* argv[], const bool& portability_warning )
{
  init( argc, argv, portability_warning );
}

experimental::gui_thread::gui_thread( const bool& portability_warning )
{
  std::string str = "cxxplot";

  argv_ = new char[ str.length( ) + 1 ];

  cxxplot_WARNOFF;
  strncpy( argv_, str.c_str( ), str.length( ) + 1 );
  cxxplot_WARNON;

  init( argc_, &argv_, portability_warning );
}

experimental::gui_thread::~gui_thread( )
{
  // Qt complains about timers upon destruction. I think there may be an issue of how qobject
  // detects threads in qobject.cpp. They specifically say that they don't support running the GUI
  // on a separate thread, so we should not probably complain.
  std_thread_.join( );

  if ( argv_ )
  {
    delete[] argv_;
  }
}

} // namespace cxxplot
