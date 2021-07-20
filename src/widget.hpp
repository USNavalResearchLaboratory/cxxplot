// CXXPLOT: A C++ plotting library.

// AUTHORIZATION TO USE AND DISTRIBUTE. By using or distributing the CXXPLOT software
// ("THE SOFTWARE"), you agree to the following terms governing the use and redistribution of
// THE SOFTWARE originally developed at the U.S. Naval Research Laboratory ("NRL"), Computational
// Multiphysics Systems Lab., Code 6394.

// The modules of CXXPLOT containing an attribution in their header files to the NRL have been
// authored by federal employees. To the extent that a federal employee is an author of a portion of
// this software or a derivative work thereof, no copyright is claimed by the United States
// Government, as represented by the Secretary of the Navy ("GOVERNMENT") under Title 17, U.S. Code.
// All Other Rights Reserved.

// Download, redistribution and use of source and/or binary forms, with or without modification,
// constitute an acknowledgement and agreement to the following:

// (1) source code distributions retain the above notice, this list of conditions, and the following
// disclaimer in its entirety,
// (2) distributions including binary code include this paragraph in its entirety in the
// documentation or other materials provided with the distribution, and
// (3) all published research using this software display the following acknowledgment:
// "This work uses the software components contained within the NRL CXXPLOT computer package
// written and developed by the U.S. Naval Research Laboratory, Computational Multiphysics Systems
// lab., Code 6394"

// Neither the name of NRL or its contributors, nor any entity of the United States Government may
// be used to endorse or promote products derived from this software, nor does the inclusion of the
// NRL written and developed software directly or indirectly suggest NRL's or the United States
// Government's endorsement of this product.

// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR THE U.S. GOVERNMENT BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// NOTICE OF THIRD-PARTY SOFTWARE LICENSES. This software uses open source software packages from third
// parties. These are available on an "as is" basis and subject to their individual license agreements.
// Additional information can be found in the provided "licenses" folder.

#pragma once
#include <cxxplot/align.hpp>
#include <cxxplot/figure.hpp>

cxxplot_WARNOFF
#include <qcustomplot.h>
  cxxplot_WARNON

    cxxplot_WARNOFF
#include <QElapsedTimer>
      cxxplot_WARNON

  namespace cxxplot
{
  class widget : public QCustomPlot
  {
    Q_OBJECT
  public:
    widget( QWidget* parent = nullptr );

    void set_open_gl( const bool& opengl, const bool& experimental_warning = true );

    void redraw( );
    void redraw_if_auto_redraw( );

    void wait_redraw_to_finish( );

    void set_min_redraw_interval( int msec );
    int  get_min_redraw_interval( );

    const bool& get_auto_redraw( ) const;
    void        set_auto_redraw( const bool& yesno );

    const bool& get_shown_once( );

    const bool& get_silence_subsequent_warnings( ) const;
    void        set_silence_subsequent_warnings( const bool& sw );

    const bool& get_antialiasing( ) const;
    void        set_antialiasing( const bool& yesno );

    bool get_show_legend( ) const;
    void set_show_legend( const bool& yesno );

    void set_legend_alignment( const alignment_t& a );

    bool is_plot_valid( const char* command_name );

    const class figure& figure( const std::size_t i ) const;
    class figure&       figure( const std::size_t i );

    const std::vector< class figure >& figures( ) const;
    std::vector< class figure >&       figures( );

    bool save( const std::string& filename,
               const int&         width           = 0,
               const int&         height          = 0,
               const bool&        force_overwrite = false );

  protected:
    void         showEvent( QShowEvent* ) override;
    virtual void resizeEvent( QResizeEvent* event ) Q_DECL_OVERRIDE;

  private slots:
    void handle_after_replot( );
    void showContextMenu( const QPoint& pos );
    void save();

  private:
    bool auto_redraw_                 = true;
    bool shown_once_                  = false;
    bool silence_subsequent_warnings_ = false;
    bool antialiasing_                = true;

    std::atomic_bool last_draw_complete_ = true;

    QElapsedTimer      redraw_elapsed_timer_;
    std::atomic< int > minRedrawInterval_ms = 42;
    std::atomic_bool   redraw_scheduled_    = false;

    std::vector< class figure > figures_;
  };
}
