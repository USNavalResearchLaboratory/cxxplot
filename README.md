<div align="center">
<p align="center">
  <img src="src/images/cxxplot_logo.png" width=128/>
</p>
</div>

# cxxplot: A C++ plotting library
cxxplot is a C++ plotting library meant to be used in science, engineering and data analysis. The main objectives of the project are:
1. Provide syntax that is familiar to scientists and engineers.
2. Enable low setup times.
2. Be more performant than typical plotting libraries.
4. Operate in asynchronous mode. Therefore plot calls and user code can be combined and still retain GUI interactivity. 

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

## Compiling from source
# Dependencies & Requirements:
```
Qt5 or Qt6 and a C++20 compliant compiler
```
# Ubuntu Linux
## Install dependencies:
In a command line type:
```bash
sudo apt install qt5base-dev git cmake build-essential
````
## (Optional): Clone git repository, compile and install
```bash
git clone https://github.com/USNavalResearchLaboratory/cxxplot.git
mkdir cxxplotbuild && cd cxxplotbuild
cmake ../cxxplot
make -j4
sudo make install
```

# Using
## If installed in your system
The following snippet will allow linking to cxxplot v 0.2.1 and above:
```cmake
...
find_package( cxxplot 0.2.1 REQUIRED ) # Modify the version or omit the version altegther
...
target_link_libraries( mytarget PRIVATE cxxplot::cxxplot )
```
A full standalone example, that can be also used as a starting point can be found in [101_cmake_setup1](examples/101_cmake_setup1).

## Automatic integration using CMake's FetchContent (will download and compile on demand)
Add the following to your cmake file:
```cmake
FetchContent_Declare(
  cxxplot
  GIT_REPOSITORY https://github.com/USNavalResearchLaboratory/cxxplot.git
  GIT_TAG        v0.2.1
)
FetchContent_MakeAvailable(cxxplot)
...
# link to it:
target_link_libraries( mytargetname PRIVATE cxxplot::cxxplot )
```
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

## Multiple windows dynamically, updated
<div align="center">
  <img src="https://raw.githubusercontent.com/USNavalResearchLaboratory/cxxplot/main/doc/images/multiple_windows.gif" alt="Multiple windows">
</div>

# A note on the implementation
To allow for dynamic updating, non-blocking plots, and portability across the three main OSes (Windows, Linux, macOS), cxxplot default mode of operation is to run on the main thread. This restriction is imposed by certain OSes. 

Therefore, cxxplot requires the user code to reside in a function called by ```cxxplot::exec```. This introduces the need to instead of using the typical ```main``` form:
```c++
int main( int argc, char* argv[] )
{
      // user code goes here
    ...
    return 0;
}
```
to use this alternative:
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
## Windows and linux only
cxxplot supports a less intrusive mode of operation, where the gui thread is managed by an object. This functinonality is supported in Windows and Linux but has not been tested to work on macOS:
```c++
int main( int argc, char* argv[] )
{
  auto t = cxxplot::experimental::gui_thread( argc, argv ); // The gui thread will be alive as long as t is alive.
  // user code goes here
    ...
}
```
Instantiating the gui thread through a smart pointer, allows you for safe use accross multiple scopes (for example if you need to create it in an init() function and destroy it from a close() function).

# Development state
Versions 0.x.x are alpha state versions and the interface is expected to be be evolving. Therefore if you use cxxplot in your project you are advised to use explicit version designation when using find_package or FetchContent in cmake. 

cxxplot default install behaviour, distributes different versions in different folders and you can have multiple versions without collisions.

# License intent
cxxplot is the work of the U.S. Government and the relevant source code is in the public domain. Please read the [license](license.txt) to ensure you can satisfy the authorization to use and distribute requirements.

Third party libraries included with cxxplot have their own respective licenses. Distribution of derivative work need to satisfy those licenses as well. If you plan to use cxxplot in a non open-source manner, you need to make arrangements with the individual copyright holders. More information can be found in the [licenses](licenses) folder.

DISTRIBUTION STATEMENT A. Approved for public release; distribution is unlimited.
