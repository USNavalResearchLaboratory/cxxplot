<div align="center">
<p align="center">
  <img src="src/images/cxxplot_logo.png" width=128/>
</p>
</div>

# cxxplot: A C++ plotting library
cxxplot is a C++ plotting library meant to be used in science, engineering, and data analysis. The main objectives of the project are:
1. Provide syntax that is familiar to scientists and engineers.
2. Enable low setup times.
2. Be more performant than typical plotting libraries.
4. Operate in asynchronous mode. Therefore plot calls and user code can be combined and retain GUI interactivity. 

## Basic example
```c++
#include <cxxplot/cxxplot>
#include <vector>

namespace plt = cxxplot;

int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    
    // Generate some data
    std::vector< plt::point2d > data;

    for ( int i = 0; i <= 1000; i++ )
    {
      double x = i;
      double y = x * x;
      data.push_back( { x, y } );
    }

    // Plot the data in a window
    plt::plot( data );

    return 0;
  } );
}
```
Output:
<div align="center">
  <img src="https://raw.githubusercontent.com/USNavalResearchLaboratory/cxxplot/main/doc/images/basic_example.png" alt="Basic example">
</div>

# Dependencies & Requirements:
```
Qt5 or Qt6 and a C++20 compliant compiler
```
## Install dependencies:
### Debian/Ubuntu:
In a command line, type:
```bash
sudo apt install qtbase5-dev git cmake build-essential
````
### Windows 
Download and install a version of the Qt SDK. You can either use the MinGW compiler packaged with Qt or also install MSVC 2022.

# Compiling from source
This is an optional step.

The standard way of using cxxplot is by integrating it into a CMake project using FetchContent (see the [relevant section below](#Automatic-integration-using-CMake-FetchContent)). If, on the other hand, you plan to share the library between multiple projects, you may want to compile and install it by using the following terminal commands:
```bash
git clone https://github.com/USNavalResearchLaboratory/cxxplot.git
mkdir cxxplotbuild && cd cxxplotbuild
cmake ../cxxplot
make -j4
sudo make install
```

# Using
## Automatic integration using CMake FetchContent
This is generally how we use cxxplot, which automatically downloads, compiles, and links to the library. 

Add the following to your CMakeLists.txt file:
```cmake
include( FetchContent )

FetchContent_Declare(
  cxxplot
  GIT_REPOSITORY https://github.com/USNavalResearchLaboratory/cxxplot.git
  GIT_TAG        v0.4.1
)

FetchContent_MakeAvailable( cxxplot )

# ...

# link to it:
target_link_libraries( mytargetname PRIVATE cxxplot::cxxplot )
```

A standalone example that can be used as a starting point can be found in [101_cmake_fetchcontent](examples/101_cmake_fetchcontent).

## If installed in your system
Suppose cxxplot is installed in your system by your package manager or by following [these instructions](#Compiling-from-source). In this case, the following snippet will allow linking to cxxplot v 0.4.1 and above:
```cmake
...
find_package( cxxplot 0.4.1 REQUIRED ) # Modify the version or omit the version altogether
...
target_link_libraries( mytarget PRIVATE cxxplot::cxxplot )
```
A standalone example that can be used as a starting point can be found in [102_cmake_findpackage](examples/102_cmake_findpackage).

# Example plots
<div align="center">
  <img src="https://raw.githubusercontent.com/USNavalResearchLaboratory/cxxplot/main/doc/images/barnsley_fern.png" alt="Barnsley fern">
  
[Link to source for the image above](examples/03_barnsley_fern/main.cpp)
</div>


<div align="center">
  <img src="https://raw.githubusercontent.com/USNavalResearchLaboratory/cxxplot/main/doc/images/multiple_plots.png" alt="Multiple plots">

[Link to source for the image above](examples/04_compute_and_plot/main.cpp)
</div>

<div align="center">
  <img src="https://raw.githubusercontent.com/USNavalResearchLaboratory/cxxplot/main/doc/images/08_animation.gif" alt="Multiple plots">

[Link to source for the animation above](examples/08_animation/main.cpp)
</div>

<div align="center">
  <img src="https://raw.githubusercontent.com/USNavalResearchLaboratory/cxxplot/main/doc/images/linecolor.png" alt="Multiple plots">

[Link to source for the image  above](examples/07_line_color_order/main.cpp)
</div>

## Multiple windows dynamically updated
<div align="center">
  <img src="https://raw.githubusercontent.com/USNavalResearchLaboratory/cxxplot/main/doc/images/multiple_windows.gif" alt="Multiple windows">
</div>

# A note on the implementation
To allow for dynamic updating, non-blocking plots, and portability across the three main OSes (Windows, Linux, macOS), cxxplot default mode of operation is to run on the main thread. Certain OSes impose this restriction. 

Therefore, cxxplot requires the user code to reside in a function called by ```cxxplot::exec```. This introduces the need to instead of using the typical ```main``` form:
```c++
int main( int argc, char* argv[] )
{
      // user code goes here
    ...
    return 0;
}
```
, to use this alternative:
```c++
int main( int argc, char* argv[] )
{
  return plt::exec( argc, argv, [ & ]( ) {
    // user code goes here
    ...
    return 0;
  } );
}
```
## Windows and Linux only
cxxplot supports a less intrusive mode of operation, where an object manages the GUI thread. This functionality is supported in Windows and Linux but has not been tested to work on macOS:
```c++
int main( int argc, char* argv[] )
{
  auto t = cxxplot::experimental::gui_thread( argc, argv ); // TheGUIi thread will be alive as long as t is alive.
  // user code goes here
    ...
}
```
Instantiating the gui thread through a smart pointer allows you for safe use across multiple scopes (for example, if you need to create it in an init() function and destroy it from a close() function).

# Development state
Versions 0.x.x are alpha state versions, and the interface is expected to evolve. So, if you use cxxplot in your project, you should use explicit version designation when using find_package or FetchContent in cmake. 

cxxplot default install behavior distributes different versions in different folders, and you can have multiple versions without collisions.

# License intent
cxxplot is the work of U.S. Government employees, and the relevant source code is in the public domain. Please read the [license](license.txt) to ensure you can satisfy the authorization to use and distribute requirements.

Third-party libraries included with cxxplot have their own respective licenses. Distribution of derivative work needs to satisfy those licenses as well. If you plan to use cxxplot in a non-open-source manner, you must make arrangements with the individual copyright holders. More information can be found in the [licenses](licenses) folder.

DISTRIBUTION STATEMENT A. Approved for public release; distribution is unlimited.
