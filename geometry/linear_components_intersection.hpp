//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_LINEAR_COMPONENTS_INTERSECTION_HPP
#define GENERATIVE_GEOMETRY_LINEAR_COMPONENTS_INTERSECTION_HPP
#pragma once

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace generative
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

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_LINEAR_COMPONENTS_INTERSECTION_HPP
