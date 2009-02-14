//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_COORDINATE_TRAITS_HPP
#define _GENERATIVE_GEOMETRY_COORDINATE_TRAITS_HPP
#pragma once

namespace generative
{
namespace numeric
{
namespace geometry
{
    
//! \brief Tag to check if a type is a point type.
template <typename CoordinateType>
struct is_coordinate : boost::false_type{};

//! Default coordinate_traits.
//! NOTE: must be specialized for user types.
template <typename CoordinateType>
struct coordinate_traits
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, COORDINATE_TRAITS_NOT_DEFINED
		, (CoordinateType) );

    typedef typename CoordinateType coordinate_type;
    typedef dimension_type<0>       dimension_index_type;

};

//! Macro to define a coordinate type's traits. 
//! Coordinate types have a numeric type and a dimension index in an orthogonal basis. 
//!(Dimensions which are affine combinations of orthogonal bases are not supported.)
#define BOOST_DEFINE_COORDINATE_TRAITS( CoordinateType, DimensionIndex )\
template <> struct is_coordinate< CoordinateType > : boost::true_type{};\
template <>                                                             \
struct coordinate_traits< CoordinateType >                              \
{                                                                       \
    typedef CoordinateType                 coordinate_type;             \
    typedef dimension_type<DimensionIndex> dimension_location;          \
                                                                        \
};

}}}//namespace generative::numeric::geometry

#endif //_GENERATIVE_GEOMETRY_COORDINATE_TRAITS_HPP
