//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DIMENSION_TRAITS_HPP
#define _BOOST_GEOMETRY_DIMENSION_TRAITS_HPP
#pragma once

#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{
    //! A type trait specialized on dimension.
    template <unsigned int Dimension>
    struct dimension_traits
    {
    	static const unsigned int value = Dimension;
    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_DIMENSION_TRAITS_HPP
