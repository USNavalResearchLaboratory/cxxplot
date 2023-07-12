#pragma once
#include <cstddef>

class QCustomPlot;
class QCPItemPixmap;

namespace cxxplot
{

class figure;

class image
{
public:
  enum class Format
  {
    RGB888,
    RGBA8888,
    Grayscale8,
    Grayscale16
  };

  image( unsigned char*     data,
         const std::size_t& width,
         const std::size_t& height,
         const std::size_t& bytes_per_line,
         const Format&      format,
         QCustomPlot*       parent_plot,
         figure*            parent );

  void update( unsigned char*     data,
               const std::size_t& width,
               const std::size_t& height,
               const std::size_t& bytes_per_line,
               const Format&      format );

private:
  QCPItemPixmap* qcp_item_pixmap_;
  figure*        parent_figure_;
};

}
