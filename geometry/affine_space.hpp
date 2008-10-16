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
#include "point.hpp"
#include "vector.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  

    template<unsigned int Dimension, typename Point, typename Vector>
    struct affine_space
    {
        typedef Dimension                                            dimension_type;
        typedef Point                                                point_type;
        typedef Vector                                               vector_type;
        typedef typename point_traits< point_type >::coordinate_type numeric_type;
    };

    //! Define the base traits of an affine space. This requires the definitions of both Points and Vectors in the space.
    BOOST_DEFINE_AFFINE_SPACE( Dimension, Point, Vector )                  \
    struct affine_space_traits< affine_space< Dimension, Point, Vector > > \
    {                                                                      \
        typedef typename affine_space< Dimension, Point, Vector >::dimension_type dimension_type;\
        typedef typename affine_space< Dimension, Point, Vector >::point_type     point_type;    \
        typedef typename affine_space< Dimension, Point, Vector >::vector_type    vector_type;   \
        typedef typename affine_space< Dimension, Point, Vector >::numeric_type   numeric_type;  \
    };

    BOOST_DEFINE_AFFINE_SPACE( 2, point_float_2d, vector_float_2d );
    BOOST_DEFINE_AFFINE_SPACE( 3, point_float_3d, vector_float_3d );
    BOOST_DEFINE_AFFINE_SPACE( 2, point_double_2d, vector_double_2d );
    BOOST_DEFINE_AFFINE_SPACE( 3, point_double_3d, vector_double_3d );

    BOOST_DEFINE_AFFINE_SPACE( 2, point_int_2d, vector_int_2d );
    BOOST_DEFINE_AFFINE_SPACE( 3, point_int_3d, vector_int_3d );
    BOOST_DEFINE_AFFINE_SPACE( 2, point_int64_2d, vector_int64_2d );
    BOOST_DEFINE_AFFINE_SPACE( 3, point_int64_3d, vector_int64_3d );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_AFFINE_SPACE_HPP
