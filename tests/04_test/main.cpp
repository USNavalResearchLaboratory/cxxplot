// TODO: We need to define this before concepts (why?). TODO: TEST ON MSVC
#include <array>

namespace cxxplot
{
template <class T> const T &get_x( const std::array<T,2> &p ) { return p[0]; };
template <class T> const T &get_y( const std::array<T,2> &p ) { return p[1]; };
}

#include <cxxplot/concepts.hpp>

#include <vector>
#include <list>
#include <iostream>
#include <cassert>

using namespace cxxplot;

template <class T>
struct point2d1
{
  T x() const { return x_;}
  T y() const { return y_;}

  T x_;
  T y_;
};

template <class T>
struct point2d2
{
  T x;
  T y;
};

template <class T>
struct point2d3
{
  const T &x() const { return x_;}
  const T &y() const { return y_;}

  T x_;
  T y_;
};

template <class T>
struct point2d4
{
  T &x() { return x_;}
  T &y() { return y_;}

  T x_;
  T y_;
};

template <class T>
struct point2d5
{
  const T &x() const { return x_;}
  const T &y() const { return y_;}

  T &x() { return x_;}
  T &y() { return y_;}


  T x_;
  T y_;
};



template <class T>
struct point2d6
{
  const T &x() const { return x_;}
  const T &y() const { return y_;}

  T x() { return x_;}
  T y() { return y_;}


  T x_;
  T y_;
};

template <class T>
struct point2d7
{
  const T &customGetX() const { return x_;}
  const T &customGetY() const { return y_;}

  T x_;
  T y_;
};

template <class T> const T &get_x( const point2d7<T> &p ) { return p.customGetX(); };
template <class T> const T &get_y( const point2d7<T> &p ) { return p.customGetY(); };


template <BasicRealContainer C>
void check( const C &c )
{
  assert( *(c.begin() + 2) == 33 );
}

template <BasicPoint2DContainer C>
void check( const C &c )
{
  assert( get_x( *(c.begin() + 2 )) == 33 );
}

template <BasicPoint2DContainer C>
void check( C &c )
{
  assert( get_x( *(c.begin() + 2 )) == 33 );
}

template <class T>
void test ()
{
  std::vector<T> v{ 11, 22, 33 };  check( v );

  static_assert( !BasicRealContainer< std::vector<point2d1<T>> > );
  static_assert( !BasicRealContainer< std::vector<point2d2<T>> > );
  static_assert( !BasicRealContainer< std::vector<point2d3<T>> > );
  static_assert( !BasicRealContainer< std::vector<point2d4<T>> > );
  static_assert( !BasicRealContainer< std::vector<point2d5<T>> > );
  static_assert( !BasicRealContainer< std::vector<point2d6<T>> > );
  static_assert( !BasicRealContainer< std::vector<point2d7<T>> > );
  static_assert( !BasicRealContainer< std::vector<std::array<T,2>> > );
  static_assert( !BasicRealContainer< std::vector<std::array<T,2>> > );

  static_assert( BasicPoint2DContainer< std::vector<point2d1<T>> > );
  static_assert( BasicPoint2DContainer< std::vector<point2d2<T>> > );
  static_assert( BasicPoint2DContainer< std::vector<point2d3<T>> > );
  static_assert( BasicPoint2DContainer< std::vector<point2d4<T>> > );
  static_assert( BasicPoint2DContainer< std::vector<point2d5<T>> > );
  static_assert( BasicPoint2DContainer< std::vector<point2d6<T>> > );
  static_assert( BasicPoint2DContainer< std::vector<point2d7<T>> > );
  static_assert( BasicPoint2DContainer< std::vector<std::array<T,2>> > );


  point2d1<T> p{22,33};

  assert( get_x(p) == 22 );
  assert( get_y(p) == 33 );

  {std::vector< point2d1<T> > vp{{10,20}, {30,40},{33,33},{44,44}};  check( vp );}
  {std::vector< point2d2<T> > vp{{10,20}, {30,40},{33,33},{44,44}};  check( vp );}
  {std::vector< point2d3<T> > vp{{10,20}, {30,40},{33,33},{44,44}};  check( vp );}
  {std::vector< point2d4<T> > vp{{10,20}, {30,40},{33,33},{44,44}};  check( vp );}
  {std::vector< point2d5<T> > vp{{10,20}, {30,40},{33,33},{44,44}};  check( vp );}
  {std::vector< point2d6<T> > vp{{10,20}, {30,40},{33,33},{44,44}};  check( vp );}
}

int main( )
{
  std::cout << "Testing concepts...";

  static_assert( BasicRealContainer< std::vector<double> > );
  static_assert( BasicRealContainer<std::list<double> > );
  static_assert( !BasicRealContainer< std::vector<std::string> > );
  static_assert( !BasicPoint2DContainer< std::vector<double> > );

  test<std::size_t>();
  test<int>();
  test<double>();
  test<float>();

  std::cout << " Success." << std::endl;
}
