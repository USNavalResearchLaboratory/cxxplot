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

// NOTICE OF THIRD-PARTY SOFTWARE LICENSES. This software uses open source software packages from
// third parties. These are available on an "as is" basis and subject to their individual license
// agreements. Additional information can be found in the provided "licenses" folder.

#pragma once

#include "graph.hpp"
#include "image.hpp"
#include "named_parameter.hpp"

cxxplot_WARNOFF
#include <QPointer>
  cxxplot_WARNON

#include <array>
#include <memory>
#include <vector>

  namespace cxxplot
{

  class window_proxy;
  class widget;

  enum class axis_scaling_type
  {
    linear,
    logarithmic
  };

  class figure : public named_parameters::supports_named_parameters< figure >
  {

  public:
    figure( figure&& other );

    figure( QPointer< widget > plot_widget );

    void fit_to_data( const bool& request_replot = true );

    void set_xlim( double lower, double upper );
    std::array<double, 2> get_xlim( );

    void set_ylim( double lower, double upper );
    std::array<double, 2> get_ylim( );

    std::string get_xlabel( ) const;
    void        set_xlabel( std::string new_label );

    std::string get_ylabel( ) const;
    void        set_ylabel( std::string new_label );

    void                     set_axes_line_width( const double& t );
    detail::setter< double > axes_line_width { [ this ]( const double& t ) {
      set_axes_line_width( t );
    } };

    std::vector< std::unique_ptr< class graph > > graphs_;

    std::vector< std::unique_ptr< class image > > images_;

    class graph& add_graph( );

    template< BasicDataContainer Container_t >
    class graph& add_graph( Container_t&& container )
    {
      return add_graph_with_one_container( std::forward< Container_t >( container ) );
    }

    template< class Arg0, typename Arg1, typename... Args >
    class graph& add_graph( Arg0&& arg0, Arg1&& arg1, Args&&... args )
    {
      // To resolve overload ambiguities
      if constexpr ( BasicRealContainer< typename std::decay<
                       Arg0 >::type > && BasicRealContainer< typename std::decay< Arg1 >::type > )
      {
        return add_graph_with_two_containers( std::forward< Arg0 >( arg0 ),
                                              std::forward< Arg1 >( arg1 ),
                                              std::forward< Args >( args )... );
      }
      else
      {
        return add_graph_with_one_container( std::forward< Arg0 >( arg0 ),
                                             std::forward< Arg1 >( arg1 ),
                                             std::forward< Args >( args )... );
      }
    }

    class graph& graph( const std::size_t& i );

    image& add_image(unsigned char*       data,
                      const std::size_t&   width,
                      const std::size_t&   height,
                      const std::size_t&   bytes_per_line,
                      const image::Format& format );

    detail::setter< std::array< double, 2 > > x_range { [ this ]( std::array< double, 2 > ar ) {
      set_xlim( ar[ 0 ], ar[ 1 ] );
    } };

    detail::setter< std::array< double, 2 > > y_range { [ this ]( auto ar ) {
      set_ylim( ar[ 0 ], ar[ 1 ] );
    } };

    detail::getter_setter< std::string > x_label { [ this ]( ) { return get_xlabel( ); },
                                                   [ this ]( const std::string& new_label ) {
                                                     set_xlabel( new_label );
                                                   } };

    detail::getter_setter< std::string > y_label {
      [ this ]( ) { return get_ylabel( ); },
      [ this ]( const std::string&
                  new_label ) // TODO: Fix these to make the getter setter to accept functions
      { set_ylabel( new_label ); }
    };

    void                   set_auto_fit( const bool& ar );
    detail::setter< bool > auto_fit { [ this ]( const bool& ar ) { this->set_auto_fit( ar ); } };

    void                     set_axes_aspect_ratio( const double& r );
    detail::setter< double > axes_aspect_ratio { [ this ]( const double& r ) {
      this->set_axes_aspect_ratio( r );
    } };

    void                                set_x_axis_scaling_type( const axis_scaling_type& sc );
    detail::setter< axis_scaling_type > x_axis_scaling_type {
      [ this ]( const axis_scaling_type& sc ) { this->set_x_axis_scaling_type( sc ); }
    };

    void                                set_y_axis_scaling_type( const axis_scaling_type& sc );
    detail::setter< axis_scaling_type > y_axis_scaling_type {
      [ this ]( const axis_scaling_type& sc ) { this->set_y_axis_scaling_type( sc ); }
    };

    friend class graph;
    friend class image;
    friend class widget;

  private:
    void handle_updated_visual_items( );
    void update_axes( const bool& force_match_user_ranges ); // To be called only from main thread
    void update_user_ranges( const bool& match_user_ranges );
    void match_user_ranges_to_data( );
    void update_axes_data_ratio( );
    void update_canonical_ranges( );

  private:
    template< class Container_t, typename... Args >
    class graph& add_graph_with_one_container( Container_t&& data, Args&&... args )
    {
      auto& last = add_graph( );

      last.set_data( std::forward< Container_t >( data ) );

      constexpr auto num_args = sizeof...( Args );

      constexpr auto used = graph ::dry_run_set< Args... >( );

      static_assert( used == num_args,
                     "Error: Incorrect number of arguments were provided to the graph." );

      last.set( std::forward< Args >( args )... );

      return last;
    }

    template< class Container1_t, class Container2_t, typename... Args >
    class graph& add_graph_with_two_containers( const Container1_t& c1,
                                                const Container2_t& c2,
                                                Args&&... args )
    {
      auto& g = add_graph( );

      g.set_data( c1, c2 );

      constexpr auto num_args = sizeof...( Args );
      if constexpr ( num_args != 0 )
      {
        constexpr auto used = graph ::dry_run_set< Args... >( );

        static_assert( used == num_args,
                       "Error: add_graph was provided with an incorrect number of arguments." );

        g.set( std::forward< Args >( args )... );
      }
      return g;
    }

  private:
    QPointer< widget > plot_widget( );

    QPointer< window_proxy > window_proxy_;

    bool autoFit_ = true;

    double axis_ratio_ = 0.0;

    range user_x_range_;
    range user_y_range_;
    bool  user_set_range_ = false;

    range canonical_x_range_;
    range canonical_y_range_;
  };

  namespace named_parameters
  {
  inline const named_parameter xlim_ { &figure::set_xlim };
  inline const named_parameter ylim_ { &figure::set_ylim };
  inline const named_parameter x_axis_scaling_type_ { &figure::set_x_axis_scaling_type };
  inline const named_parameter y_axis_scaling_type_ { &figure::set_y_axis_scaling_type };
  inline const named_parameter xlabel_ { &figure::set_xlabel };
  inline const named_parameter ylabel_ { &figure::set_ylabel };
  inline const named_parameter auto_fit_ { &figure::set_auto_fit };
  inline const named_parameter axes_aspect_ratio_ { &figure::set_axes_aspect_ratio };
  inline const named_parameter axes_line_width_ { &figure::set_axes_line_width };
  }
}
