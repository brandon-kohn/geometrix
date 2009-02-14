//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_AFFINE_SPACE_HPP
#define _GENERATIVE_GEOMETRY_AFFINE_SPACE_HPP
#pragma once

#include "affine_space_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{  

//! \brief The notion of an affine space is used to define dimension and numeric type.

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

template <typename NumericType, unsigned int Dimension>
struct affine_space
{
    typedef dimension_traits<Dimension> dimension_type;
    typedef NumericType                 numeric_type;
};

//! \brief Macro to define the traits of an affine space.
#define BOOST_DEFINE_AFFINE_SPACE( NumericType, Dimension )                       \
template <>                                                                       \
struct affine_space_traits< affine_space< NumericType, Dimension > >              \
{                                                                                 \
    typedef affine_space< NumericType, Dimension >::dimension_type dimension_type;\
    typedef affine_space< NumericType, Dimension >::numeric_type   numeric_type;  \
};

BOOST_DEFINE_AFFINE_SPACE( float, 2 );
BOOST_DEFINE_AFFINE_SPACE( float, 3 );
BOOST_DEFINE_AFFINE_SPACE( double, 2 );
BOOST_DEFINE_AFFINE_SPACE( double, 3 );
typedef affine_space< float, 2 >  affine_space_float_2d;
typedef affine_space< float, 3 >  affine_space_float_3d;
typedef affine_space< double, 2 > affine_space_double_2d;    
typedef affine_space< double, 3 > affine_space_double_3d;

BOOST_DEFINE_AFFINE_SPACE( int, 2 );
BOOST_DEFINE_AFFINE_SPACE( int, 3 );
BOOST_DEFINE_AFFINE_SPACE( long long, 2 );
BOOST_DEFINE_AFFINE_SPACE( long long, 3 );
typedef affine_space< int, 2 >       affine_space_int_2d;
typedef affine_space< int, 3 >       affine_space_int_3d;
typedef affine_space< long long, 2 > affine_space_int64_2d;    
typedef affine_space< long long, 3 > affine_space_int64_3d;    

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_AFFINE_SPACE_HPP
