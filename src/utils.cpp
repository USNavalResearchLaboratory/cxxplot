#include <cxxplot/utils.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace cxxplot
{

source_location::source_location( const char* file_name, const std::uint_least32_t& line ) :
  file_name_( file_name ), line_( line )
{
}

std::string remove_chars( std::string str, const std::string& characters_to_ignore )
{
  for ( auto c : characters_to_ignore )
  {
    str.erase( std::remove( str.begin( ), str.end( ), c ), str.end( ) );
  }

  return str;
}

std::vector< std::string > split_string( const std::string& str, const std::string& delimiters )
{
  std::vector< int > delimeter_positions;
  delimeter_positions.push_back( -1 );

  for ( auto& delim : delimiters )
  {
    auto pos = str.find( delim, 0 );

    while ( pos != std::string::npos )
    {
      delimeter_positions.push_back( int( pos ) );
      pos = str.find( delim, pos + 1 );
    }
  }

  std::sort( delimeter_positions.begin( ), delimeter_positions.end( ) );

  delimeter_positions.push_back( int( str.size( ) ) );

  std::vector< std::string > substrings;

  for ( std::size_t i = 0; i != delimeter_positions.size( ) - 1; i++ )
  {
    auto from = delimeter_positions[ i ] + 1;
    auto to   = delimeter_positions[ i + 1 ];
    substrings.push_back( str.substr( from, to - from ) );
  }

  return substrings;
}

std::vector< std::vector< double > > read_delimited( const std::string& filename,
                                                     std::string        delimeters,
                                                     bool               first_row_as_header,
                                                     const std::size_t  first_row,
                                                     const std::size_t /*first_column*/ )
{

  cxxplot_cerr_SOURCE_LOCATION_CURRENT;

  auto throw_runtime_error = [ &filename ]( const std::string& message ) {
    std::stringstream ss;
    ss << "cxxplot read_csv: File: " << filename << '\n' << message;
    std::cerr << ss.str( ) << std::endl;
    throw std::runtime_error( ss.str( ) );
  };

  cxxplot_cerr_SOURCE_LOCATION_CURRENT;

  if ( !first_row_as_header )
  {
    std::vector< std::vector< double > >      data_double;
    std::vector< std::vector< std::string > > data_string;

    cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    std::ifstream csvfile( filename );

    // Make sure the file is open
    if ( !csvfile.is_open( ) )
    {
      throw_runtime_error( "Could not open file." );
    }

    //std::size_t line_num = 0;
    std::string line;

    for ( std::size_t i = 0; i != first_row; i++ )
    {
      if ( !std::getline( csvfile, line ) )
      {
        throw_runtime_error( "Premature end of file end when skipping rows." );
      }
      //line_num++;
    }

    std::vector< double > data;
    bool                  in_quotes                = false;
    bool                  next_should_be_delimiter = false;

    auto to_double = []( const std::string& s, bool& ok ) {
      double val    = 0.0;
      bool   failed = false;

      std::size_t idx = 0;
      try
      {
        val = std::stod( s, &idx );
      }
      catch ( std::exception& )
      {
        failed = true;
      }

      if ( failed == true || idx != s.size( ) )
      {
        ok = false;
        return val;
      }

      ok = true;
      return val;
    };

    auto add = [ to_double, throw_runtime_error ]( const std::string& s,
                                                   auto&              data_double,
                                                   const std::size_t& row,
                                                   const std::size_t& column,
                                                   const std::string& line ) {
      bool ok = false;
      auto v  = to_double( s, ok );
      if ( ok )
      {
        if ( row == 0 )
        {
          data_double.push_back( std::vector< double > { v } );
        }
        else
        {
          if ( column >= data_double.size( ) )
          {
            std::stringstream ss;
            //          std::stringstream ss2;
            ss << "Line " << row << " has more elements convertible to double than previous lines. Line contents: \n"
               << line << '\n';

            throw_runtime_error( ss.str( ) );
          }
        }

        data_double[ column ].push_back( v );

        std::cout << "Line: " << row << ". Adding: " << v << std::endl;
      }
      else
      {
        std::cout << "Not adding: " << s << std::endl;
      }
    };

    std::string element;

    cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    std::size_t row = 0;

    while ( std::getline( csvfile, line ) )
    {

      std::size_t column = 0;

      for ( auto iter = line.begin( ); iter != line.end( ); iter++ )
      {
        auto& c = *iter;

        if ( next_should_be_delimiter )
        {
          if ( std::isspace( c ) )
          {
            continue;
          }

          if ( delimeters.find( c ) != std::string::npos ) // delimiter
          {
            next_should_be_delimiter = false;
          }
          continue;
        }

        if ( !in_quotes )
        {
          if ( c == '"' )
          {
            in_quotes = true;
            continue;
          }

          if ( delimeters.find( c ) == std::string::npos ) // Not a delimeter
          {
            element.push_back( c );
          }
          else // delimeter and not in quotes
          {
            add( element, data_double, row, column, line );
            element.clear( );
            column++;
            continue;
          }
        }
        else // in quotes
        {
          if ( c == '"' )
          {
            in_quotes                = false;
            next_should_be_delimiter = true;
            continue;
          }
        }
      }

      add( element, data_double, row, column, line );
      element.clear( );
      row++;
    }
    return data_double;

    //      auto parts = split_string( line, delimeters );

    //      std::cout << "Split:" << std::endl;
    //      int count = 0;
    //      for ( auto& s : parts )
    //      {
    //        std::cout << count++ << " " << s << std::endl;
    //      }

    //      std::size_t                i = 0;
    //      std::vector< std::size_t > double_columns;
    //      for ( const auto& part : parts )
    //      {
    //        double val = 0.0;

    //        try
    //        {
    //          std::size_t idx = 0;
    //          val             = std::stod( part, &idx );
    //          if ( idx == part.size( ) )
    //          {
    //            std::cout << "Double" << std::endl;
    //            data_double.push_back( std::vector { val } );
    //            double_columns.push_back( i++ );
    //            continue;
    //          }
    //        }
    //        catch ( std::exception& )
    //        {
    //        }
    //        std::cout << "String" << std::endl;
    //        data_string.push_back( std::vector { part } );
    //      }

    //      cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    //      std::stringstream ss( line );

    //      std::cout << line << std::endl;

    //      line = remove_chars( line, characters_to_ignore );

    //      double val = 0.0;
    //      while ( ss >> val )
    //      {
    //        std::cout << "  " << val << std::endl;
    //        data_double.push_back( std::vector< double >( { val } ) );

    //        std::cout << "  " << char( ss.peek( ) ) << std::endl;

    //        if ( delimeters.find( char( ss.peek( ) ) ) != std::string::npos )
    //        {
    //          cxxplot_cerr_SOURCE_LOCATION_CURRENT;
    //          ss.ignore( );
    //        }
    //      }

    //      cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    //      line_num++;
    //    }

    //    cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    //    while ( std::getline( csvfile, line ) )
    //    {
    //      std::stringstream ss( line );

    //      auto original_line = line;

    //      std::cout << line << std::endl;
    //      line.erase( 0, first_column );

    //      std::cout << "  " << line << std::endl;

    //      line = remove_chars( line, characters_to_ignore );
    //      std::cout << "  " << line << std::endl;

    //      double val = 0.0;
    //      cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    //      std::size_t col_num = 0;
    //      while ( ss >> val )
    //      {
    //        std::cout << "  " << val << std::endl;
    //        if ( col_num >= data_double.size( ) )
    //        {
    //          std::stringstream ss2;
    //          ss2 << "Line " << line_num << " has more elements than previous lines. Line
    //          contents: \n"
    //              << original_line << '\n';
    //          throw_runtime_error( ss2.str( ) );
    //        }

    //        cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    //        data_double[ col_num ].push_back( val );

    //        cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    //        // if ( std::size_t( delimeters.find( ss.peek( ) ) ) != std::string::npos )
    //        {
    //          ss.ignore( );
    //        }
    //        cxxplot_cerr_SOURCE_LOCATION_CURRENT;

    //        line_num++;
    //        col_num++;
    //      }
    //    }
    //    return data_double;
  }
  else
  {
    throw std::runtime_error( "cxxplot read_csv: first_row_as_header not implemented yet." );
  }
}

std::vector< std::vector< double > > read_csv( const std::string& filename,
                                               bool               first_row_as_header,
                                               const size_t       R0,
                                               const size_t       C1 )
{
  return read_delimited( filename, ",", first_row_as_header, R0, C1 );
}

} // namespace cxxplot
