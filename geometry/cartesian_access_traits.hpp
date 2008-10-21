//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP
#define _BOOST_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP
#pragma once

#include "indexed_access_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief cartesian access traits for numeric sequence in cartesian reference frames.
//! NOTE: must be specialized for user types.
template <typename T>
struct cartesian_access_traits
{
    typedef T numeric_sequence_type;

    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, CARTESIAN_ACCESS_TRAITS_NOT_DEFINED
		, (T) );
};

//! \brief Macro to specialize the coordinate accessors for cartesian coords for cartesian point which model IndexedAccessConcept.
//* There is no real mechanism in place at the moment for actually constraining the type to be cartesian. This means
//* if the user defines a point type which is reference frame agnostic under both cartesian and other access types.. it will be 
//* up to the user to make certain the point passed to any given function is in the proper form.
//* Users who require compile-time tagging of point types should look at the reference_frame_tag type introduced in reference_frame_traits.
#define BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( T )                         \
template <>                                                               \
struct cartesian_access_traits< T > : public indexed_access_traits< T >   \
{                                                                         \
    typedef indexed_type coordinate_type;                                 \
};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_CARTESIAN_ACCESS_TRAITS_HPP


