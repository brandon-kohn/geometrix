//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
#define _BOOST_GEOMETRY_POLAR_ACCESS_TRAITS_HPP
#pragma once

#include "indexed_access_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Access traits for numeric sequences in a polar reference frame.
//! NOTE: must be specialized for user types.
template <typename T>
struct polar_access_traits
{
    typedef T numeric_sequence_type;

    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POLAR_ACCESS_TRAITS_NOT_DEFINED
		, (T) );	

};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_POLAR_ACCESS_TRAITS_HPP


