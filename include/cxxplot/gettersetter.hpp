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

#include <functional>
#include <memory>

namespace cxxplot
{
namespace detail
{
template< class T >
struct setter
{
  setter( ) = delete;

  setter( const setter& ) = delete;

  setter( std::function< void( const T& ) > f ) : func_( f )
  {
  }

  setter& operator=( const T& v )
  {
    func_( v );
    return *this;
  }

private:
  std::function< void( const T& ) > func_;
};

// TODO: We need to disable the members if we are inheriting
// namespace detail
//{
//    struct non_type { };

//    template <class T>
//    using inherit_if_not_fundamental = typename
//    std::conditional<
//    /*if*/      std::is_fundamental<T>::value,
//    /*then*/    non_type,
//    /*else*/    T
//    >::type;

//    template <class T >
//    class getter_setter : public detail::inherit_if_not_fundamental<T>

// Possible other options is to create a type generator instead of hacking the inheritance specifier
//}

// TODO: getter_setter should also be able to work on getter function instead of variable

// template <class T >
// class getter_setter
//{

// public:
//    using value_type = T;

//    getter_setter() = delete;

//    getter_setter( const getter_setter & other ): // If we copy-init the new object imittates T

//        this_value_{ new T( other.referenced_value_) },

//        referenced_value_{ nullptr},

//        func_{ []( const T&){} }
//    {

//    }

//    getter_setter( const T&value, std::function< void(const T&)> f ):

//        referenced_value_( &value ), // We need to keep a reference in case this is copied.

//        func_( f )
//    {
//    }

//    const value_type &value() const
//    {
//        if ( this_value_ )
//        {
//            return *this_value_;
//        }
//        else
//        {
//            return *referenced_value_;
//        }
//    }

//    getter_setter &operator=( T&&v)
//    {
//        if ( this_value_ ) // We are in a copy. Behave like we are a T.
//        {
//            *this_value_ = std::forward<T>(v);
//        }
//        else
//        {
//            func_( std::forward<T>(v) );
//        }

//        return *this;
//    }

//    getter_setter &operator=(const T&v)
//    {
//        if ( this_value_ ) // We are in a copy. Behave like we are a T.
//        {
//            *this_value_ = v;
//        }
//        else
//        {
//            func_( v );
//        }

//        return *this;
//    }

//    operator const T&() const
//    {
//        return value();
//    }

// private:
//    const T *referenced_value_;

//    std::unique_ptr<T> this_value_; // A pointer so we don't construct large object when we don't
//    need to.

//    std::function<void(const T&)> func_;
//};

template< class R >
class getter_setter
{

public:
  using value_type = typename std::decay< R >::type;

private:
  using setter_f_t = std::function< void( const value_type& ) >;

  using getter_f_t = std::function< R( ) >; // R because the getter may return a reference or a
                                            // copy. TODO: Reference is not working well now (MSVC
                                            // gives a refenerence to temporary wanring.
                                            // Test it with the get_auto_redraw function.

public:
  getter_setter( ) = delete;

  getter_setter( const getter_setter& other ) :
    // If we copy-init the new object imittates T
    this_value_ { new value_type( other.const_value( ) ) }
  {
  }

  getter_setter( getter_f_t g, setter_f_t s ) : setter_ { s }, getter_ { g }
  {
  }

  const R value( )
  {
    if ( this_value_ ) // We are in a copy.
    {
      return *this_value_;
    }

    // Make sure the return value's lifetime is the same wth the getter_setter object's lifetime
    return_value_.reset( new value_type( getter_( ) ) );

    return *return_value_;
  }

  operator R( )
  {
    return value();
  }

  getter_setter& operator=( value_type&& v )
  {
    if ( this_value_ ) // We are in a copy. Behave like we are a T.
    {
      *this_value_ = std::forward< value_type >( v );
    }
    else
    {
      setter_( std::forward< value_type >( v ) );
    }

    return *this;
  }

  getter_setter& operator=( const value_type& v )
  {
    if ( this_value_ ) // We are in a copy. Behave like we are a T.
    {
      *this_value_ = v;
    }
    else
    {
      setter_( v );
    }

    return *this;
  }

  bool is_copy( ) const
  {
    return this_value_ != nullptr;
  }

private:
  value_type const_value( ) const
  {
    if ( this_value_ ) // We are in a copy.
    {
      return *this_value_;
    }

    return getter_( );
  }

  setter_f_t setter_;

  getter_f_t getter_;

  std::unique_ptr< value_type >
    this_value_; // A pointer so we don't construct large object when we don't need to.

  std::unique_ptr< value_type > return_value_;
};

} // namespace detail

} // namespace cxxplot
