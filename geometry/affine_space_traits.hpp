//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_AFFINE_SPACE_TRAITS_HPP
#define _GENERATIVE_GEOMETRY_AFFINE_SPACE_TRAITS_HPP
#pragma once

#include "dimension_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{  
    //! \brief Traits struct for specializing the properties of an affine space type.

    //! Affine spaces are a concept which defines the dimension of a space and encapsulates\n
    //! a point and vector space.\n
    //! Affine spaces are created by specifying a dimension and a numeric type.\n
    //! The dimension and numeric type taken together define a numeric sequence type.\n
    //! This type is then used to describe the elements of both point and vector types.\n
    //! The point and vector types are related by the following concepts:\n
    //! An affine space of dimension \f$n\f$ defines a point space \f$P^n\f$ and vector space \f$\hat{V}^n\f$.
    //! For a point, \f$p\in P^n\f$ and vector \f$\hat{v}\in \hat{V}^n\f$ and scalar value \f$ \sigma \f$,\n
    //!
    //! Adding a vector to a point results in another point.
    //!    - \f$p_1 + \hat{v} \Rightarrow p_2 \in P^n\f$
    //! 
    //! The difference between two points is a vector.
    //!    - \f$p_1 - p_2 \Rightarrow \hat{v} \in \hat{V}^n\f$
    //! 
    //! Multiplying a vector by a scalar results in another vector.
    //!    - \f$ \sigma \hat{v_1} \Rightarrow \hat{v_2} \in \hat{V}^n\f$
    template < typename AffineSpace >
    struct affine_space_traits
    {        
        typedef typename AffineSpace::Dimension   dimension_type;        
        typedef typename AffineSpace::NumericType numeric_type;

        BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, AFFINE_SPACE_TRAITS_NOT_DEFINED
		, (AffineSpace) );

    };

    //! \brief Concept definition of an affine space.

    //! A type models an AffineSpaceConcept if it defines a dimension type and a numeric type.
    template <typename AffineSpace>
    struct AffineSpaceConcept
    {
        void constraints()
        {            
            typedef typename affine_space_traits< AffineSpace >::dimension_type dimension_type;
            typedef typename affine_space_traits< AffineSpace >::numeric_type numeric_type;
            BOOST_STATIC_ASSERT( dimension_type::value > 0 );            
        }
    };

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_AFFINE_SPACE_TRAITS_HPP
