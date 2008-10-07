//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_LINEAR_COMPONENTS_INTERSECTION_HPP
#define _BOOST_GEOMETRY_LINEAR_COMPONENTS_INTERSECTION_HPP
#pragma once

#include "geometric_concepts.hpp"

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace boost
{
namespace numeric
{
namespace geometry
{

enum intersection_type
{
    e_invalid_intersection = -1,
    e_non_crossing = 0,
    e_crossing = 1,
    e_overlapping = 2,
    e_endpoint = 3,
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_LINEAR_COMPONENTS_INTERSECTION_HPP
