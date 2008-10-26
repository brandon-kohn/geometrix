//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_SPHERE_HPP
#define _BOOST_GEOMETRY_SPHERE_HPP
#pragma once

#include "sphere_traits.hpp"
#include "math_functions.hpp"
#include "dimension_traits.hpp"
#include "construction_traits.hpp"
#include "numeric_sequence.hpp"
#include "detail/vector_generator.hpp"
#include "detail/sphere_generator.hpp"
#include "reference_frame_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Define access interface.
template <typename NumericType, unsigned int Dimension>
struct use_indexed_access_type< sphere< NumericType, Dimension > > 
    : boost::integral_constant< indexed_sequence_access_type,
                                BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE >{};

//! Concrete Sphere Types for some of the more common coordinate types.
typedef sphere<float, 2>       sphere_float_2d;
typedef sphere<float, 3>       sphere_float_3d;
typedef sphere<double, 2>      sphere_double_2d;
typedef sphere<double, 3>      sphere_double_3d;

typedef sphere<int, 2>         sphere_int_2d;
typedef sphere<int, 3>         sphere_int_3d;
typedef sphere<long long, 2>   sphere_int64_2d;
typedef sphere<long long, 3>   sphere_int64_3d;

BOOST_DEFINE_SPHERE_TRAITS( sphere_float_2d, neutral_reference_frame_float_2d );
BOOST_DEFINE_SPHERE_TRAITS( sphere_float_3d, neutral_reference_frame_float_3d );
BOOST_DEFINE_SPHERE_TRAITS( sphere_double_2d, neutral_reference_frame_double_2d );
BOOST_DEFINE_SPHERE_TRAITS( sphere_double_3d, neutral_reference_frame_double_3d );

BOOST_DEFINE_SPHERE_TRAITS( sphere_int_2d, neutral_reference_frame_int_2d );
BOOST_DEFINE_SPHERE_TRAITS( sphere_int_3d, neutral_reference_frame_int_3d );
BOOST_DEFINE_SPHERE_TRAITS( sphere_int64_2d, neutral_reference_frame_int64_2d );
BOOST_DEFINE_SPHERE_TRAITS( sphere_int64_3d, neutral_reference_frame_int64_3d );

//! \brief sphere access traits
//* NOTE: must be specialized for user types.
template <typename NumericType, unsigned int Dimension>
struct sphere_access_traits< sphere<NumericType,Dimension> > : public indexed_access_traits< sphere<NumericType,Dimension> >
{
    typedef sphere<NumericType,Dimension>                          sphere_type;    
    typedef typename sphere_traits< sphere_type >::coordinate_type coordinate_type;
    static inline const coordinate_type& get_radius( const segment_type& s ){ return s.get_radius(); }
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_SPHERE_HPP
