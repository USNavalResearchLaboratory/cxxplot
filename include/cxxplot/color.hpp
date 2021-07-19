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

#include <array>
#include <cstdint>
namespace cxxplot
{

namespace color
{

struct rgb
{
  rgb( const uint8_t &red, const uint8_t &green, const uint8_t &blue );

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
};

inline auto black   = rgb{0, 0, 0};
inline auto white   = rgb{255, 255, 255};
inline auto red     = rgb{255, 0, 0};
inline auto green   = rgb{0, 255, 0};
inline auto blue    = rgb{0, 0, 255};
inline auto magenta = rgb{255, 0, 255};
inline auto cyan    = rgb{0, 255, 255};
inline auto yellow  = rgb{255, 255, 0};

namespace navy
{
inline auto navy_blue = rgb{0, 31, 56};
inline auto blue      = rgb{31, 140, 217};
inline auto yellow    = rgb{232, 170, 0};
inline auto grey      = rgb{197, 211, 211};

} // namespace navy

namespace cxxplot
{
inline auto blue       = rgb{10, 40, 70};
inline auto light_blue = navy::blue;
inline auto yellow     = navy::yellow;
inline auto grey       = rgb{117, 131, 131};
inline auto light_grey = navy::grey;
inline auto red        = rgb{190, 31, 20};
inline auto brown      = rgb{97, 68, 12};
inline auto orange     = rgb{255, 165, 0};
inline auto green      = rgb{31, 150, 20};
} // namespace cxxplot

// clang-format off
inline std::array default_order = {
  cxxplot::light_blue,
  cxxplot::red,
  cxxplot::green,
  black,
  cxxplot::yellow,
  cxxplot::brown,
  cxxplot::grey
};
// clang-format on
} // namespace color

} // namespace cxxplot
