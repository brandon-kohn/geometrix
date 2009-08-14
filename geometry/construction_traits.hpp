//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_CONSTRUCTION_TRAITS_HPP
#define GENERATIVE_GEOMETRY_CONSTRUCTION_TRAITS_HPP
#pragma once

#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! A type trait 
    template <typename T>
    struct construction_traits
    {    	
        BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, CONSTRUCTION_TRAITS_NOT_DEFINED
		, (T) );
    };

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_CONSTRUCTION_TRAITS_HPP
