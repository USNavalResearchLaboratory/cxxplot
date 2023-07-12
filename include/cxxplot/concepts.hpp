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

#include <concepts>
#include <type_traits>

namespace cxxplot
{

template< typename T > // TODO: This can be simplified
concept Element_Has_Real_value_type = requires( T c ) {
  {
    typename std::decay< decltype( *( c.begin( ) ) ) >::type { }
  } -> std::convertible_to< double >;
};

template< typename T >
concept ReferenceConvertibleToDouble = std::is_reference_v< T > && std::is_convertible_v< T, double >;

template< typename T >
concept NotReferenceConvertibleToDouble = !std::is_reference_v< T > && std::is_convertible_v< T, double >;

template< typename T, typename U >
concept ConvertibleToButNotSame = std::convertible_to< T, U > && !std::same_as< T, U >;

template< typename T >
concept HasBeginEndIterators = requires( T c ) {
  {
    c.begin( )
  };
  {
    c.end( )
  };
};

template< typename T >
concept HasConstBeginEndIterators = requires( const T c ) {
  {
    c.begin( )
  };
  {
    c.end( )
  };
};

template< class T >
concept BasicContainer = HasBeginEndIterators< T > || HasConstBeginEndIterators< T >;

template< class T >
concept BasicRealContainer = BasicContainer< T > && Element_Has_Real_value_type< T >;

// Access by member function (point.x(), point.y() ) -------------------------
// TODO: Should we worry how x(), y() is returned like we do here, or just have a double get_x...
// only?
// TODO: I think we should warn if .z() or .z / .w() or .w are available
#define cxxplot_DEFINE_CONCEPT_MEMBER_FUNC_RETURN_REFERENCE_CONVERTIBLE_TO_DOUBLE( fname )                             \
  template< typename T >                                                                                               \
  concept fname##ReturnReferenceConvertibleToDouble = requires( T v ) {                                                \
    {                                                                                                                  \
      v.fname( )                                                                                                       \
    } -> ReferenceConvertibleToDouble<>;                                                                               \
  };

#define cxxplot_DEFINE_CONCEPT_MEMBER_FUNC_RETURN_NOT_REFERENCE_CONVERTIBLE_TO_DOUBLE( fname )                         \
  template< typename T >                                                                                               \
  concept fname##ReturnNotReferenceConvertibleToDouble = requires( T v ) {                                             \
    {                                                                                                                  \
      v.fname( )                                                                                                       \
    } -> NotReferenceConvertibleToDouble<>;                                                                            \
  };

#define cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS( fname )                                      \
  cxxplot_DEFINE_CONCEPT_MEMBER_FUNC_RETURN_REFERENCE_CONVERTIBLE_TO_DOUBLE( fname )                                   \
    cxxplot_DEFINE_CONCEPT_MEMBER_FUNC_RETURN_NOT_REFERENCE_CONVERTIBLE_TO_DOUBLE(                                     \
      fname ) template< fname##ReturnReferenceConvertibleToDouble V >                                                  \
    const auto& get_##fname( const V& vec )                                                                            \
  {                                                                                                                    \
    return vec.fname( );                                                                                               \
  }                                                                                                                    \
  template< fname##ReturnReferenceConvertibleToDouble V >                                                              \
  const auto& get_##fname( V& vec )                                                                                    \
  {                                                                                                                    \
    return vec.fname( );                                                                                               \
  }                                                                                                                    \
  template< fname##ReturnNotReferenceConvertibleToDouble V >                                                           \
  double get_##fname( const V& vec )                                                                                   \
  {                                                                                                                    \
    return vec.fname( );                                                                                               \
  }                                                                                                                    \
  template< fname##ReturnNotReferenceConvertibleToDouble V >                                                           \
  double get_##fname( V& vec )                                                                                         \
  {                                                                                                                    \
    return vec.fname( );                                                                                               \
  }

cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS( x )
  cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS( y )
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(getX)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(getY)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(GetX)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(GetY)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(get_x)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(get_y)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(get_X)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(get_Y)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(Get_X)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(Get_Y)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(Get_x)
// cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_FUNC_ACCESS(Get_y)

// Access by member variable (vec.x, vec.y ) -------------------------
#define cxxplot_DEFINE_CONCEPT_MEMBER_VAR_CONVERTIBLE_TO_BUT_NOT_DOUBLE( vname )                                       \
  template< typename T >                                                                                               \
  concept vname##MemberVarConvertibleToButNotDouble = requires( T v ) {                                                \
    {                                                                                                                  \
      v.vname                                                                                                          \
    } -> ConvertibleToButNotSame< double >;                                                                            \
  };

#define cxxplot_DEFINE_CONCEPT_MEMBER_VAR_IS_DOUBLE( vname )                                                           \
  template< typename T >                                                                                               \
  concept vname##MemberVarIsDouble = requires( T v ) {                                                                 \
    {                                                                                                                  \
      v.vname                                                                                                          \
    } -> std::same_as< double >;                                                                                       \
  };

#define cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS( vname )                                       \
  cxxplot_DEFINE_CONCEPT_MEMBER_VAR_CONVERTIBLE_TO_BUT_NOT_DOUBLE( vname )                                             \
    cxxplot_DEFINE_CONCEPT_MEMBER_VAR_IS_DOUBLE( vname ) template< vname##MemberVarConvertibleToButNotDouble V >       \
    double get_##vname( const V& vec )                                                                                 \
  {                                                                                                                    \
    return vec.vname;                                                                                                  \
  }                                                                                                                    \
  template< vname##MemberVarConvertibleToButNotDouble V >                                                              \
  double get_##vname( V& vec )                                                                                         \
  {                                                                                                                    \
    return vec.vname;                                                                                                  \
  }                                                                                                                    \
  template< vname##MemberVarIsDouble V >                                                                               \
  const double& get_##vname( const V& vec )                                                                            \
  {                                                                                                                    \
    return vec.vname;                                                                                                  \
  }

    cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS( x )
      cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS( y )
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(getX)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(getY)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(GetX)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(GetY)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(get_x)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(get_y)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(get_X)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(get_Y)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(Get_X)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(Get_Y)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(Get_x)
  // cxxplot_DEFINE_CONCEPTS_AND_GET_FUNCTIONS_FOR_MEMBER_VAR_ACCESS(Get_y)

  template< class T >
  concept Has_get_x_get_y = requires( T a ) {
    {
      get_x( a )
    } -> std::convertible_to< double >;
    {
      get_y( a )
    } -> std::convertible_to< double >;
  };

template< class C >
concept ElementHas_get_x_get_y = requires( C c ) { requires Has_get_x_get_y< decltype( *( c.begin( ) ) ) >; };

template< class T >
concept BasicPoint2DContainer = BasicContainer< T > && ElementHas_get_x_get_y< T >;

template< class T >
concept BasicDataContainer = BasicRealContainer< T > || BasicPoint2DContainer< T >;

}
