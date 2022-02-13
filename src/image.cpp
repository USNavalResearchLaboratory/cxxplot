#include <cxxplot/image.hpp>

#include "qcustomplot.h"

#include <cxxplot/figure.hpp>

#include <QImage>

namespace cxxplot
{
QImage::Format toQImageFormat( const image::Format& format )
{
  switch ( format )
  {
    case image::Format::RGB888:
      return QImage::Format_RGB888;
    case image::Format::RGBA8888:
      return QImage::Format_RGBA8888;
    case image::Format::Grayscale8:
      return QImage::Format_Grayscale8;
    case image::Format::Grayscale16:
      return QImage::Format_Grayscale16;
    default:
      return QImage::Format_Invalid;
  }
}

image::image( unsigned char*     data,
              const std::size_t& width,
              const std::size_t& height,
              const std::size_t& bytes_per_line,
              const Format&      format,
              QCustomPlot*       parent_plot,
              figure*            parent ) :
  qcp_item_pixmap_( new QCPItemPixmap( parent_plot ) ),
  parent_figure_( parent )
{
  qcp_item_pixmap_->setVisible( true );
  parent_plot->addLayer( "image", parent_plot->layer( "main" ), QCustomPlot::limBelow );

  parent_plot->xAxis->setRange( -50, width + 50 );
  parent_plot->yAxis->setRange( -50, height + 50 );

  update( data, width, height, bytes_per_line, format );
}

void image::update( unsigned char* data,
                    const size_t&  width,
                    const size_t&  height,
                    const size_t&  bytes_per_line,
                    const Format&  format )
{
  QImage image_view( data, width, height, bytes_per_line, toQImageFormat( format ) );

  qcp_item_pixmap_->setLayer( "image" );
  // qcp_item_pixmap_->setScaled( true );
  QPixmap pixmap      = QPixmap::fromImage( image_view );
  QSize   pixmap_size = pixmap.size( );
  qcp_item_pixmap_->setPixmap( pixmap );
  qcp_item_pixmap_->topLeft->setType( QCPItemPosition::PositionType::ptPlotCoords );
  qcp_item_pixmap_->bottomRight->setType( QCPItemPosition::PositionType::ptPlotCoords );
  qcp_item_pixmap_->topLeft->setCoords( 0, 0 );
  qcp_item_pixmap_->bottomRight->setCoords( pixmap_size.width( ), pixmap_size.height( ) );
  // TODO: Expose the options here.
  qcp_item_pixmap_->setScaled( true, Qt::KeepAspectRatio, Qt::FastTransformation );


  parent_figure_->handle_updated_visual_items( );
}

}
