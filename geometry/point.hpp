//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_POINT_HPP
#define _GENERATIVE_GEOMETRY_POINT_HPP
#pragma once

#include "point_traits.hpp"
#include "math_functions.hpp"
#include "construction_traits.hpp"
#include "numeric_sequence.hpp"
#include "detail/vector_generator.hpp"
#include "detail/point_generator.hpp"
#include "neutral_reference_frame.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

//! Define access interface.
template <typename NumericType, unsigned int Dimension>
struct use_indexed_access_type< point< NumericType, Dimension > > 
    : boost::integral_constant< indexed_sequence_access_type,
                                BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE >{};

//! Concrete Point Types for some of the more common coordinate types.
typedef point<float, 2>       point_float_2d;
typedef point<float, 3>       point_float_3d;
typedef point<double, 2>      point_double_2d;
typedef point<double, 3>      point_double_3d;

typedef point<int, 2>         point_int_2d;
typedef point<int, 3>         point_int_3d;
typedef point<long long, 2>   point_int64_2d;
typedef point<long long, 3>   point_int64_3d;

BOOST_DEFINE_POINT_TRAITS( point_float_2d, neutral_reference_frame_float_2d );
BOOST_DEFINE_POINT_TRAITS( point_float_3d, neutral_reference_frame_float_3d );
BOOST_DEFINE_POINT_TRAITS( point_double_2d, neutral_reference_frame_double_2d );
BOOST_DEFINE_POINT_TRAITS( point_double_3d, neutral_reference_frame_double_3d );

BOOST_DEFINE_POINT_TRAITS( point_int_2d, neutral_reference_frame_int_2d );
BOOST_DEFINE_POINT_TRAITS( point_int_3d, neutral_reference_frame_int_3d );
BOOST_DEFINE_POINT_TRAITS( point_int64_2d, neutral_reference_frame_int64_2d );
BOOST_DEFINE_POINT_TRAITS( point_int64_3d, neutral_reference_frame_int64_3d );

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_POINT_HPP
