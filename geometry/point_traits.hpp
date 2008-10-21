//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_TRAITS_HPP
#define _BOOST_GEOMETRY_POINT_TRAITS_HPP
#pragma once

#include "numeric_traits.hpp"
#include "dimension_traits.hpp"
#include "indexed_access_traits.hpp"
#include "coordinate_sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
//! Default point traits struct. 
//! NOTE: must be specialized for user types.
template <typename Point>
struct point_traits : public coordinate_sequence_traits<Point>
{
	BOOST_MPL_ASSERT_MSG( 
         ( false )
		,POINT_TRAITS_NOT_DEFINED
		,(Point) );	

    typedef Point point_type;

};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_POINT_TRAITS( Point )                             \
        BOOST_DEFINE_COORDINATE_SEQUENCE_TRAITS( Point )               \
template <>                                                            \
struct point_traits< Point > : public coordinate_sequence_traits<Point>\
{                                                                      \
    typedef Point point_type;                                          \
};

//! Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_USER_POINT_TRAITS( Point, NumericType, Dimension )         \
        BOOST_DEFINE_COORDINATE_SEQUENCE_TRAITS( Point, NumericType, Dimension )\
template <>                                                                     \
struct point_traits< Point > : public coordinate_sequence_traits< Point >       \
{                                                                               \
    typedef Point point_type;                                                   \
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_TRAITS_HPP
