//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_SEQUENCE_HPP
#define _BOOST_GEOMETRY_POINT_SEQUENCE_HPP
#pragma once

#include "geometric_concepts.hpp"
#include "point.hpp"
#include "point_sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_2d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_float_3d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_2d );
    BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( point_double_3d );
}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_SEQUENCE_HPP
