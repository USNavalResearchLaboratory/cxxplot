// CXXPLOT: A C++ plotting library.
//
// AUTHORIZATION TO USE AND DISTRIBUTE
//
// By using or distributing the CXXPLOT software ("THE SOFTWARE"), you agree to the following terms
// governing the use and redistribution of THE SOFTWARE originally developed at the U.S. Naval
// Research Laboratory (NRL), Computational Multiphysics Systems Lab., Code 6394.
//
// The modules of CXXPLOT containing an attribution in their header files to the NRL have been
// authored by federal employees. To the extent that a federal employee is an author of a portion of
// this software or a derivative work thereof, no copyright is claimed by the United States
// Government, as represented by the Secretary of the Navy ("GOVERNMENT") under Title 17, U.S. Code.
// All Other Rights Reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) source code distributions retain the above copyright notice, this list of conditions, and the
// following disclaimer in its entirety,
// (2) distributions including binary code include this paragraph in its entirety in the
// documentation or other materials provided with the distribution, and
// (3) all published research using this software display the following acknowledgment:
// "This work uses the software components contained within the NRL CXXPLOT computer package written
// and developed by the U.S. Naval Research Laboratory, Computational Multiphysics Systems lab.,
// Code 6394"
//
// Neither the name of NRL or its contributors, nor any entity of the United States Government may
// be used to endorse or promote products derived from this software, nor does the inclusion of the
// NRL written and developed software directly or indirectly suggest NRL's or the United States
// Government's endorsement of this product.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR THE U.S. GOVERNMENT BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Notice of Third-Party Software Licenses
//
// This software contains open source software packages from third parties. These are available on
// an "as is" basis and subject to their individual license agreements.  Additional information can
// can be found in the provided "licenses" folder.

#pragma once

#include <tuple>

namespace cxxplot
{

namespace named_parameters
{

template< class T >
struct supports_named_parameters
{
  template< typename... Args >
  static constexpr std::size_t dry_run_set( )
  {
    if constexpr ( sizeof...( Args ) == 0 )
    {
      return 0;
    }
    else
    {
      return dry_run_set_impl< 0, Args... >( );
    }
  }

  template< std::size_t ArgumentsUsed, typename FirstArg, typename... OtherArgs >
  static constexpr std::size_t dry_run_set_impl( )
  {
    constexpr bool applies_to_class
      = std::is_same< T, typename std::decay_t< FirstArg >::Class >::value;

    if constexpr ( applies_to_class )
    {
      if constexpr ( sizeof...( OtherArgs ) != 0 )
      {
        return dry_run_set_impl< ArgumentsUsed + 1, OtherArgs... >( );
      }
      else
      {
        return ArgumentsUsed + 1;
      }
    }
    else if constexpr ( sizeof...( OtherArgs ) != 0 )
    {
      return dry_run_set_impl< ArgumentsUsed, OtherArgs... >( );
    }
    return ArgumentsUsed;
  }

  template< typename... Args >
  void set( Args&&... args )
  {
    if constexpr ( sizeof...( Args ) == 0 )
    {
      return;
    }
    else
    {
      set_impl( std::forward< Args >( args )... );
    }
  }

  template< typename FirstArg, typename... OtherArgs >
  void set_impl( FirstArg&& first_arg, OtherArgs&&... args )
  {
    constexpr bool applies_to_class
      = std::is_same< T, typename std::decay_t< FirstArg >::Class >::value;

    if constexpr ( applies_to_class )
    {
      first_arg.apply_set( static_cast< T* >( this ) );
    }

    if constexpr ( sizeof...( OtherArgs ) != 0 )
    {
      set_impl( std::forward< OtherArgs >( args )... );
    }
  }
};

// TODO: There is a problem with the implicit conversions of T.
template< class ClassType, class... T >
struct named_parameter
{
  using Tuple = std::tuple< T... >;

  named_parameter( void ( ClassType::*member_function_pointer )( T... ) ) :
    fp( member_function_pointer )
  {
  }

  struct argument
  {
    using Class = ClassType;
    argument( void ( ClassType::*member_function_pointer )( T... ), const Tuple& t ) :
      fp( member_function_pointer ), values( t )
    {
    }

    argument( ) = default;

    argument( argument const& ) = delete;

    argument( argument&& ) = delete;

    argument& operator=( argument const& ) = delete;

    argument& operator=( argument&& ) = delete;

    void apply_set( ClassType* caller ) const
    {
      apply_set_impl( caller, std::index_sequence_for< T... >( ) );
    }

  private:
    template< std::size_t... Is >
    void apply_set_impl( ClassType* caller, std::index_sequence< Is... > ) const
    {
      ( caller->*fp )( std::get< Is >( values )... );
    }

    void ( ClassType::*fp )( T... );

    const Tuple& values; // TODO: Should this not be a reference?
  };

  argument operator=( const Tuple& t ) const
  {
    return argument { fp, t };
  }

private:
  void ( ClassType::*fp )( T... );
};

}

}
