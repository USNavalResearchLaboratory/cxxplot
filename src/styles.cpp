#include <cxxplot/utils.hpp>

cxxplot_WARNOFF
#include "qcustomplot.h"
cxxplot_WARNON

#include "styles_p.hpp"
#include <cxxplot/styles.hpp>

namespace cxxplot
{

QCPScatterStyle::ScatterShape toQCPScatterShape( const MarkerStyle& markerStyle )
{
  switch ( markerStyle )
  {
    case MarkerStyle::None:
      return QCPScatterStyle::ssNone;

    case MarkerStyle::Dot:
      return QCPScatterStyle::ssDot;

    case MarkerStyle::Cross:
      return QCPScatterStyle::ssCross;

    case MarkerStyle::Plus:
      return QCPScatterStyle::ssPlus;

    case MarkerStyle::Circle:
      return QCPScatterStyle::ssCircle;

    case MarkerStyle::Disk:
      return QCPScatterStyle::ssDisc;

    case MarkerStyle::Square:
      return QCPScatterStyle::ssSquare;

    case MarkerStyle::Diamond:
      return QCPScatterStyle::ssDiamond;

    case MarkerStyle::Star:
      return QCPScatterStyle::ssStar;

    case MarkerStyle::Triangle:
      return QCPScatterStyle::ssTriangle;

    case MarkerStyle::InvertedTriangle:
      return QCPScatterStyle::ssTriangleInverted;

    case MarkerStyle::CrossSquare:
      return QCPScatterStyle::ssCrossSquare;

    case MarkerStyle::PlusSquare:
      return QCPScatterStyle::ssPlusSquare;

    case MarkerStyle::CrossCircle:
      return QCPScatterStyle::ssCrossCircle;

    case MarkerStyle::PlusCircle:
      return QCPScatterStyle::ssPlusCircle;

    case MarkerStyle::Peace:
      return QCPScatterStyle::ssPeace;
  }

  return QCPScatterStyle::ssNone;
}

Qt::PenStyle to_qt_pen_style( const LineStyle& line_style )
{
  switch ( line_style )
  {
    case LineStyle::None:
      return Qt::NoPen;

    case LineStyle::Solid:
      return Qt::SolidLine;

    case LineStyle::Dash:
      return Qt::DashLine;

    case LineStyle::Dot:
      return Qt::DotLine;

    case LineStyle::DashDot:
      return Qt::DashDotLine;

    case LineStyle::DashDotDot:
      return Qt::DashDotDotLine;
  }

  return Qt::NoPen;
}

}
