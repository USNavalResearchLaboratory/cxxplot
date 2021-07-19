#include <cxxplot/cxxplot>
#include <fstream>
#include <ranges>

namespace ct = cxxplot;

void test_data1( )
{
  std::ofstream f;
  f.open( "test1.csv" );
  f << "0,2,3\n";
  f << "1,3,4\n";
  f.close( );

  auto data = ct::read_csv( "test1.csv" );

  auto w = ct::plot( );
  w.add_graph( data[ 0 ], data[ 1 ] );
  w.add_graph( data[ 0 ], data[ 2 ] );
}

void test_data2( )
{
  std::ofstream f;
  f.open( "test2.csv" );
  f << "0,2;3\n";
  f << "1,3,4\n";
  f << "2,2.8, 4.6";
  f.close( );

  try
  {
    auto data = ct::read_csv( "test2.csv" );
  }
  catch ( std::exception &e )
  {
    std::cout << e.what( ) << std::endl;
    return;
  }

  throw std::runtime_error( "Test failed" );
}

void test_data3( )
{
  std::ofstream f;
  f.open( "test3.csv" );
  f << "0,2;3\n";
  f << "1,3,4\n";
  f << "2,2.8, 4.6";
  f.close( );

  auto data = ct::read_delimited( "test3.csv", ",;" );

  auto w  = ct::plot( );
  w.title = "Multiple delimeters";
  w.add_graph( data[ 0 ], data[ 1 ] );
  w.add_graph( data[ 0 ], data[ 2 ] );
}

void test_data4( )
{
  std::ofstream f;
  f.open( "test4.csv" );
  f << "0 2 3\n";
  f << "1 3 4\n";
  f << "2 2.8  \"4.6\"";
  f.close( );

  auto data = ct::read_delimited( "test3.csv", ",;" );

  auto w  = ct::plot( );
  w.title = "Spaces";
  w.add_graph( data[ 0 ], data[ 1 ] );
  w.add_graph( data[ 0 ], data[ 2 ] );
}

void test_data5( )
{
  std::ofstream f;
  f.open( "test5.csv" );
  f << "0\t2 3\n";
  f << "1 3\t4\n";
  f << "2\t2.8  4.6";
  f.close( );

  auto data = ct::read_delimited( "test3.csv", ",;" );

  auto w  = ct::plot( );
  w.title = "Mixed spaces tabs";
  w.add_graph( data[ 0 ], data[ 1 ] );
  w.add_graph( data[ 0 ], data[ 2 ] );
}

void test_data6( ) // TODO: This should not work
{
  std::ofstream f;
  f.open( "test5.csv" );
  f << "0, \"text\", 2,3\n";
  f << "1,\"text\",3,4\n";
  f << "2, \"text\",\"text\",2.8  4.6";
  f.close( );

  auto data = ct::read_delimited( "test3.csv", ",;" );

  auto w  = ct::plot( );
  w.title = "Mixed spaces tabs";
  w.add_graph( data[ 0 ], data[ 1 ] );
  w.add_graph( data[ 0 ], data[ 2 ] );
}

int main( int argc, char *argv[] )
{

  ct::exec( argc, argv, []( ) {
    using namespace ct::named_parameters;

    test_data1( );
    test_data2( );
    test_data3( );
    test_data4( );
    test_data5( );
    test_data6( );

//    int count = 0;
//    for ( auto &s : ct::split_string( "hello, world", "," ) )
//      std::cout << count++ << " " << s << std::endl;
//    std::cout << std::endl;

//    count = 0;
//    for ( auto &s : ct::split_string( ",hello, world", "," ) )
//      std::cout << count++ << " " << s << std::endl;
//    std::cout << std::endl;

//    count = 0;
//    for ( auto &s : ct::split_string( ",hello, world,", "," ) )
//      std::cout << count++ << " " << s << std::endl;
//    std::cout << std::endl;

//    count = 0;
//    for ( auto &s : ct::split_string( ",hello,,, wor,ld,", "," ) )
//      std::cout << count++ << " " << s << std::endl;
//    std::cout << std::endl;

//    count = 0;
//    for ( auto &s : ct::split_string( ",hello,,, world:all,", ",:" ) )
//      std::cout << count++ << " " << s << std::endl;
//    std::cout << std::endl;

//    count = 0;
//    for ( auto &s : ct::split_string( ",hello,,, world:all<people,", ",:<" ) )
//      std::cout << count++ << " " << s << std::endl;
//    std::cout << std::endl;

//    count = 0;
//    for ( auto &s : ct::split_string( "15+35=50", "+=-" ) )
//      std::cout << count++ << " " << s << std::endl;
//    std::cout << std::endl;

    return 0;
  } );
}
