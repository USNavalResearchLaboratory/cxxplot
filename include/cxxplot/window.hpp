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

#include "figure.hpp"
#include "graph.hpp"

#include "align.hpp"
#include "execution.hpp"
#include "named_parameter.hpp"
#include "utils.hpp"

#include "gettersetter.hpp"

#include <cxxplot/concepts.hpp>

cxxplot_WARNOFF
#include <QObject>
#include <QPointer>
#include <QVector>
  cxxplot_WARNON

#include <array>
#include <sstream>
#include <type_traits>

#include <tuple>

#include <iostream>

  namespace cxxplot
{

  class widget;

  class window_proxy :
    public QObject,
    public named_parameters::supports_named_parameters< window_proxy >
  {
    Q_OBJECT

  public:
    window_proxy( const bool& show = true );

    template< BasicDataContainer Container_t >
    window_proxy( Container_t&& container ) : window_proxy( false )
    {
      init_with_one_container( std::forward< Container_t >( container ) );
    }

    template< class Arg0, typename Arg1, typename... Args >
    window_proxy( Arg0&& arg0, Arg1&& arg1, Args&&... args ) : window_proxy( false )
    {
      // To resolve overload ambiguities
      if constexpr ( BasicRealContainer< typename std::decay<
                       Arg0 >::type > && BasicRealContainer< typename std::decay< Arg1 >::type > )
      {
        init_with_two_containers( std::forward< Arg0 >( arg0 ),
                                  std::forward< Arg1 >( arg1 ),
                                  std::forward< Args >( args )... );
      }
      else
      {
        init_with_one_container( std::forward< Arg0 >( arg0 ),
                                 std::forward< Arg1 >( arg1 ),
                                 std::forward< Args >( args )... );
      }
    }

    window_proxy( QPointer< widget > plot_widget );

    window_proxy( window_proxy&& other );

    ~window_proxy( );

    // OpenGL may be slower depending on what you want to do. Benchmark before.
    void set_open_gl( const bool& opengl, const bool experimental_warning = true );

    void show( );

    bool is_plot_valid( const char* command_name );

    int                          get_min_redraw_interval( );
    void                         set_min_redraw_interval( int msec );
    detail::getter_setter< int > min_redraw_interval_ms {
      [ this ]( ) { return get_min_redraw_interval( ); },
      [ this ]( const int& ms ) { set_min_redraw_interval( ms ); }
    };

    std::string                          get_title( ) const;
    void                                 set_title( std::string title );
    detail::getter_setter< std::string > title { [ this ]( ) { return get_title( ); },
                                                 [ this ]( const std::string& title ) {
                                                   set_title( title );
                                                 } };

    std::array< int, 2 >                          get_size( ) const;
    void                                          set_size( const int& width, const int& height );
    detail::getter_setter< std::array< int, 2 > > size {
      [ this ]( ) { return get_size( ); }, [ this ]( auto ar ) { set_size( ar[ 0 ], ar[ 1 ] ); }
    };

    std::array< int, 2 >                          get_position( ) const;
    void                                          set_position( const int& x, const int& y );
    detail::getter_setter< std::array< int, 2 > > position {
      [ this ]( ) { return get_position( ); },
      [ this ]( auto ar ) { set_position( ar[ 0 ], ar[ 1 ] ); }
    };

    const bool&                   get_auto_redraw( ) const;
    void                          set_auto_redraw( const bool& yesno );
    detail::getter_setter< bool > auto_redraw { [ this ]( ) { return get_auto_redraw( ); },
                                                [ this ]( const bool& yesno ) {
                                                  set_auto_redraw( yesno );
                                                } };

    const bool&                   get_antialiasing( ) const;
    void                          set_antialiasing( const bool& yesno );
    detail::getter_setter< bool > antialiasing { [ this ]( ) { return get_antialiasing( ); },
                                                 [ this ]( const bool& yesno ) {
                                                   set_antialiasing( yesno );
                                                 } };

    bool                          get_show_legend( ) const;
    void                          set_show_legend( const bool& yesno );
    detail::getter_setter< bool > show_legend { [ this ]( ) { return get_show_legend( ); },
                                                [ this ]( const bool& yesno ) {
                                                  set_show_legend( yesno );
                                                } };

    void                          set_legend_alignment( const alignment_t& a );
    detail::setter< alignment_t > legend_alignment { [ this ]( const alignment_t& a ) {
      set_legend_alignment( a );
    } };

    const std::vector< class figure >& figures( ) const;
    std::vector< class figure >&       figures( );

    class figure& figure( const std::size_t& i );

    template< class Container_t, typename... Args >
    class graph& add_graph( Container_t&& data, Args&&... args )
    {
      if ( figures( ).size( ) == 0 )
      {
        auto& f = create_first_figure( );

        return f.add_graph( std::forward< Container_t >( data ), std::forward< Args >( args )... );
      }
      else if ( figures( ).size( ) > 1 )
      {
        std::stringstream ss;

        ss << "Window has more than one figures. Use .figure( n ).add_graph(...); with n "
              "the figure number to add a new graph. (n < "
           << figures( ).size( ) << ").";

        throw std::out_of_range( ss.str( ) );
      }
      else
      {
        return figure( 0 ).add_graph( std::forward< Container_t >( data ),
                                      std::forward< Args >( args )... );
      }
    }

    void wait_redraw_to_finish( );

    //!
    //! \brief save Saves the plot to a file (.png, .jpg, .bmp, .pdf)
    //! \param file_name Name of the file
    //! \param width Width of the resulting plot. If 0 use the window width.
    //! \param height Height of the resulting plot. If 0 use the window height.
    //! \param force_overwrite If true overwrite the file if it exists. Otherwise prompt.
    //! \return
    //!
    bool save( const std::string& file_name,
               const int&         width           = 0,
               const int&         height          = 0,
               const bool         force_overwrite = false );

    void                     set_fonts_size( const double& s );
    detail::setter< double > fonts_size { [ this ]( const double& s ) { set_fonts_size( s ); } };

  private slots:
    void redraw( );

  private:
    template< class Container_t, typename... Args >
    void init_with_one_container( Container_t&& container, Args&&... args )
    {
      auto default_auto_redraw = get_auto_redraw( );

      bool user_auto_redraw = default_auto_redraw;

      set_auto_redraw( false );

      auto& f = figure( 0 );
      f.add_graph( );

      auto& g = f.graph( 0 );

      g.set_data( std::forward< Container_t >( container ) );

      constexpr auto num_args = sizeof...( Args );
      if constexpr ( num_args != 0 )
      {
        constexpr auto used = window_proxy::dry_run_set< Args... >( )
                              + figure::dry_run_set< Args... >( )
                              + graph ::dry_run_set< Args... >( );

        static_assert(
          used == num_args,
          "Error: Window constructor was provided with an incorrect number of arguments." );

        set_auto_redraw( default_auto_redraw );

        set( std::forward< Args >( args )... );

        user_auto_redraw = get_auto_redraw( );
        set_auto_redraw( false );

        f.set( std::forward< Args >( args )... );
        g.set( std::forward< Args >( args )... );
      }

      redraw( );

      show( );
      set_auto_redraw( user_auto_redraw );
    }

    template< class Container1_t, class Container2_t, typename... Args >
    void init_with_two_containers( const Container1_t& c1, const Container2_t& c2, Args&&... args )
    {
      auto default_auto_redraw = get_auto_redraw( );

      bool user_auto_redraw = default_auto_redraw;

      set_auto_redraw( false );

      auto& f = figure( 0 );
      f.add_graph( );

      auto& g = f.graph( 0 );

      g.set_data( c1, c2 );

      constexpr auto num_args = sizeof...( Args );
      if constexpr ( num_args != 0 )
      {
        constexpr auto used = window_proxy::dry_run_set< Args... >( )
                              + figure::dry_run_set< Args... >( )
                              + graph ::dry_run_set< Args... >( );

        static_assert(
          used == num_args,
          "Error: Window constructor was provided with an incorrect number of arguments." );

        set_auto_redraw( default_auto_redraw );

        set( std::forward< Args >( args )... );

        user_auto_redraw = get_auto_redraw( );
        set_auto_redraw( false );

        f.set( std::forward< Args >( args )... );
        g.set( std::forward< Args >( args )... );
      }

      redraw( );

      show( );
      set_auto_redraw( user_auto_redraw );
    }

  private:
    class figure& create_first_figure( );

    friend class figure;

  private:
    void handle_updated_visual_items( const class figure& f );

    void handle_updated_visual_items( const graph& g );

    void valid_or_throw( ) const;

  private:
    QPointer< widget > plot_widget_;

    bool redraw_on_destroy = true;
  };

  // TODO: make named_parameter a friend of the classes providing the methods and make the methods
  // private
  namespace named_parameters
  {
  inline const named_parameter window_size_ { &window_proxy::set_size };
  inline const named_parameter window_position_ { &window_proxy::set_position };
  inline const named_parameter window_title_ { &window_proxy::set_title };
  inline const named_parameter show_legend_ { &window_proxy::set_show_legend };
  inline const named_parameter legend_alignment_ { &window_proxy::set_legend_alignment };
  inline const named_parameter fonts_size_ { &window_proxy::set_fonts_size };

  inline const named_parameter auto_redraw_ { &window_proxy::set_auto_redraw };
  inline const named_parameter antialiasing_ { &window_proxy::set_antialiasing };
  }

  template< class T, typename... Args >
  window_proxy plot( std::initializer_list< T > list, Args && ... args )
  {
    return window_proxy( std::vector< T >( list.begin( ), list.end( ) ),
                         std::forward< Args >( args )... );
  }

  template< class T1, class T2, typename... Args >
  window_proxy plot(
    std::initializer_list< T1 > l1, std::initializer_list< T2 > l2, Args && ... args )
  {
    std::vector< double > v1( l1.begin( ), l1.end( ) );
    std::vector< double > v2( l2.begin( ), l2.end( ) );
    return window_proxy( v1, v2, std::forward< Args >( args )... );
  }

  template< class C, class T, typename... Args >
  window_proxy plot( const C& c1, std::initializer_list< T > l, Args&&... args )
  {
    std::vector< double > c2( l.begin( ), l.end( ) );
    return window_proxy( c1, c2, std::forward< Args >( args )... );
  }

  template< class T, class C, typename... Args >
  window_proxy plot( std::initializer_list< T > l, const C& c2, Args&&... args )
  {
    std::vector< double > c1( l.begin( ), l.end( ) );
    return window_proxy( c1, c2, std::forward< Args >( args )... );
  }

  template< typename Arg0, typename... Args >
  window_proxy plot( Arg0 && arg0, Args && ... args )
  {
    return window_proxy( std::forward< Arg0 >( arg0 ), std::forward< Args >( args )... );
  }

  template< typename... Args >
  window_proxy plot( const bool show = true )
  {
    return window_proxy( show );
  }
}
