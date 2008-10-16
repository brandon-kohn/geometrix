//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_AFFINE_SPACE_TRAITS_HPP
#define _BOOST_GEOMETRY_AFFINE_SPACE_TRAITS_HPP
#pragma once

#include "dimension_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    //! Define the base traits of an affine space. This requires the definitions of both Points and Vectors in the space.
    template < typename AffineSpace >
    struct affine_space_traits
    {        
        typedef typename AffineSpace::Dimension   dimension_type;
        typedef typename AffineSpace::Point       point_type;
        typedef typename AffineSpace::Vector      vector_type;
        typedef typename AffineSpace::NumericType numeric_type;

        BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, AFFINE_SPACE_TRAITS_NOT_DEFINED
		, (AffineSpace) );

    };

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_AFFINE_SPACE_TRAITS_HPP
