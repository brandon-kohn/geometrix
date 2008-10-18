//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_CONSTANTS_HPP
#define _BOOST_GEOMETRY_CONSTANTS_HPP
#pragma once

#include "geometric_concepts.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

    template <typename CoordinateType>
    struct constants
    {
        
    };

    template <>
    struct constants<double>
    {
        static inline const double pi() { return 3.1415926535897931; }
    };

    template <>
    struct constants<float>
    {
        static inline const float pi() { return 3.1415926535897931f; }
    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_CONSTANTS_HPP
