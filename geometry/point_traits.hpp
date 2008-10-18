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

namespace boost
{
namespace numeric
{
namespace geometry
{
//! Default point traits struct. 
//! NOTE: must be specialized for user types.
template <typename Point>
struct point_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POINT_TRAITS_NOT_DEFINED
		, (Point) );	

	typedef void*                           coordinate_type;
	typedef dimension_traits<0>             dimenstion_type;
};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_POINT_TRAITS( Point )                                       \
template <>                                                                      \
struct point_traits< Point >                                                     \
{                                                                                \
	BOOST_STATIC_ASSERT( Point::dimension_type::value > 0 );                     \
    typedef Point                  point_type;                                   \
    typedef numeric_traits<Point::coordinate_type>::numeric_type coordinate_type;\
	typedef Point::dimension_type  dimension_type;                               \
};

//! Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_USER_POINT_TRAITS( Point, NumericType, Dimension )\
template <>                                                           \
struct point_traits< Point >                                          \
{                                                                     \
	BOOST_STATIC_ASSERT( Dimension > 0 );                             \
    typedef Point                                     point_type;     \
    typedef numeric_traits<NumericType>::numeric_type coordinate_type;\
	typedef dimension_traits<Dimension>               dimension_type; \
};

//! Access traits for points in cartesian coordinates
//! NOTE: must be specialized for user types.
template <typename Point>
struct cartesian_access_traits : public indexed_access_traits< Point >
{
    typedef Point point_type;

    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, CARTESIAN_ACCESS_TRAITS_NOT_DEFINED
		, (Point) );

};

//! Access traits for points in polar coordinates
//! NOTE: must be specialized for user types.
template <typename Point>
struct polar_access_traits : public indexed_access_traits< Point >
{
    typedef Point point_type;

    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POLAR_ACCESS_TRAITS_NOT_DEFINED
		, (Point) );	

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_TRAITS_HPP
