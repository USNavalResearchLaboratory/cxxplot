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
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#ifdef __WIN32
#define cxxplot_FILENAME ( strrchr( __FILE__, '\\' ) ? strrchr( __FILE__, '\\' ) + 1 : __FILE__ )
#else
#define cxxplot_FILENAME ( strrchr( __FILE__, '/' ) ? strrchr( __FILE__, '/' ) + 1 : __FILE__ )
#endif

#define cxxplot_SOURCE_LOCATION_CURRENT                                                            \
  cxxplot::source_location                                                                         \
  {                                                                                                \
    cxxplot_FILENAME, __LINE__                                                                     \
  }

#define cxxplot_cerr_SOURCE_LOCATION_CURRENT                                                       \
  std::cerr << cxxplot_SOURCE_LOCATION_CURRENT << std::endl;

#ifdef _MSC_VER

#define CXXPLOT_STRINGIFY( a ) #a
#define cxxplot_WARNOFF _Pragma( CXXPLOT_STRINGIFY( warning( push, 0 ) ) )
#define cxxplot_WARNON _Pragma( CXXPLOT_STRINGIFY( warning( pop ) ) )

#else

#define CXXPLOT_STRINGIFY( a ) #a
#define cxxplot_WARNOFF                                                                            \
  _Pragma( CXXPLOT_STRINGIFY( GCC diagnostic push ) )                                              \
    _Pragma( CXXPLOT_STRINGIFY( GCC diagnostic ignored "-Wall" ) )                                 \
      _Pragma( "GCC diagnostic ignored \"-Wdeprecated-enum-enum-conversion\"" )

#define cxxplot_WARNON _Pragma( CXXPLOT_STRINGIFY( GCC diagnostic pop ) )
#endif

namespace cxxplot
{

struct source_location
{
  source_location( const char* file_name, const std::uint_least32_t& line );

  const char* file_name( ) const noexcept
  {
    return file_name_.c_str( );
  }
  constexpr std::uint_least32_t line( ) const noexcept
  {
    return line_;
  }

private:
  std::string         file_name_;
  std::uint_least32_t line_;
};

template< class E >
struct throw_stream
{
  template< class V >
  throw_stream& operator<<( const V& v )
  {
    ss << v;
    return *this;
  }

  void throw_now( )
  {
    throw E( ss.str( ) );
  }

private:
  std::stringstream ss;
};

std::vector<std::string> split_string(const std::string &             str,
                                       const std::string &delimiters );

std::vector< std::vector< double > > read_delimited(const std::string& filename,
                                                     const std::string  delimeters          = ",",
                                                     bool               first_row_as_header = false,
                                                     const std::size_t  R0                  = 0,
                                                     const std::size_t  = 0);

std::vector< std::vector< double > > read_csv(const std::string& filename,
                                               bool               first_row_as_header  = false,
                                               const std::size_t  R0                   = 0,
                                               const std::size_t  C1                   = 0);

}

template< class Stream >
Stream& operator<<( Stream& stream, const cxxplot::source_location& sl )
{
  stream << sl.file_name( ) << " line:" << sl.line( );
  return stream;
}
