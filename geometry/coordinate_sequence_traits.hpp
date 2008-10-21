//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_COORDINATE_SEQUENCE_TRAITS_HPP
#define _BOOST_GEOMETRY_COORDINATE_SEQUENCE_TRAITS_HPP
#pragma once

#include "numeric_sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief traits type for a sequence of coordinates.
//* A coordinate sequence is a numeric sequence with an additional property assigned to the coordinate type.
//* In practical terms the additional property would conform to a metric type on the type of the coordinate for each dimension.
//* Examples of metric type include length for cartesian coordinates and the radius in polar coordinates, angle for the
//* angular distances expressed in the theta and phi coordinates of polar coordinates etc.
//* TODO: It remains to rationalize how units would work at this level and impact the derived types.

//! Default coordinate sequence traits struct. 
//! NOTE: must be specialized for user types.
template <typename CoordinateSequence>
struct coordinate_sequence_traits : public numeric_sequence_traits<CoordinateSequence>
{
	BOOST_MPL_ASSERT_MSG(
		 ( false )
		,COORDINATE_SEQUENCE_TRAITS_NOT_DEFINED
		,(CoordinateSequence) );	

    typedef CoordinateSequence coordinate_sequence_type;
	typedef void               coordinate_type;

};

//! Macro for coordinate sequence type with embedded traits
#define BOOST_DEFINE_COORDINATE_SEQUENCE_TRAITS( CoordinateSequence )                                     \
        BOOST_DEFINE_NUMERIC_SEQUENCE_TRAITS( CoordinateSequence )                                        \
template <>                                                                                               \
struct coordinate_sequence_traits<CoordinateSequence> : public numeric_sequence_traits<CoordinateSequence>\
{                                                                                                         \
	BOOST_STATIC_ASSERT( CoordinateSequence::dimension_type::value > 0 );                                 \
    typedef CoordinateSequence coordinate_sequence_type;                                                  \
    typedef numeric_type       coordinate_type;                                                           \
};

//! Macro for coordinate sequence type which does not have embedded traits - User Defined coordinate sequences.
#define BOOST_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( CoordinateSequence, NumericType, Dimension )     \
        BOOST_DEFINE_NUMERIC_SEQUENCE_TRAITS( CoordinateSequence, NumericType, Dimension )             \
template <>                                                                                            \
struct coordinate_sequence_traits<NumericSequence> : public numeric_sequence_traits<CoordinateSequence>\
{                                                                                                      \
	BOOST_STATIC_ASSERT( CoordinateSequence::dimension_type::value > 0 );                              \
    typedef CoordinateSequence coordinate_sequence_type;                                               \
    typedef numeric_type       coordinate_type;                                                        \
};

}}}//namespace boost::coordinate::geometry;

#endif //_BOOST_GEOMETRY_COORDINATE_SEQUENCE_TRAITS_HPP
