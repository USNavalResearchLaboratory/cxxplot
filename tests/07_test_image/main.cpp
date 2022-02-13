#include <cxxplot/cxxplot>
#include <vector>

#include <QImage>
#include <QPainter>

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    auto w = plt::plot( );

    QImage source( 400, 460, QImage::Format_RGB888 );
    source.fill( Qt::red );
    {
      QPainter painter( &source );

      auto font = painter.font( );
      font.setPointSize( 24 );
      painter.setFont( font );

      painter.drawText( 20, 50, "Hello Image!" );
      painter.drawRect( 5, 5, source.width( ) - 10, source.height( ) - 10 );
    }

    auto& image = w.figure( 0 ).add_image( source.bits( ),
                                           source.width( ),
                                           source.height( ),
                                           source.bytesPerLine( ),
                                           plt::image::Format::RGB888 );

    w.figure( 0 ).axes_aspect_ratio = 1.0;

    QImage source2( 300, 380, QImage::Format_RGB888 );
    source2.fill( Qt::yellow );
    {
      QPainter painter( &source2 );

      auto font = painter.font( );
      font.setPointSize( 24 );
      painter.setFont( font );

      painter.drawEllipse( 50, 50, 40, 40 );
      painter.drawText( 20, 50, "Changed Image!" );
      painter.drawRect( 2, 2, source2.width( ) - 10, source2.height( ) - 10 );
    }

    std::this_thread::sleep_for( std::chrono::seconds( 3 ) );

    image.update( source2.bits( ),
                  source2.width( ),
                  source2.height( ),
                  source2.bytesPerLine( ),
                  plt::image::Format::RGB888 );

    return 0;
  } );
}
