//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_AFFINE_SPACE_HPP
#define _BOOST_GEOMETRY_AFFINE_SPACE_HPP
#pragma once

#include "affine_space_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  

//! \brief The notion of an affine space is used to define dimension, point and vector types as well as their underlying numeric type.
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

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_AFFINE_SPACE_HPP
