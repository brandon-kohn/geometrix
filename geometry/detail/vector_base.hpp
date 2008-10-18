//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_VECTOR_BASE_HPP
#define _BOOST_GEOMETRY_DETAIL_VECTOR_BASE_HPP
#pragma once

#include <boost/mpl/assert.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{

template <typename NumericType, unsigned int Dimension>
class vector 
{
    	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, VECTOR_DIMENSION_NOT_DEFINED
		, (NumericType) );	
};

template <typename NumericType>
class vector<NumericType,0>
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, VECTOR_CANNOT_HAVE_ZERO_DIMENSION
		, (NumericType) );	
};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_DETAIL_VECTOR_BASE_HPP


