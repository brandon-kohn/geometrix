//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_POINT_HPP
#define GENERATIVE_GEOMETRY_POINT_HPP
#pragma once

#include <geometry\point_traits.hpp>
#include <geometry\math_functions.hpp>
#include <geometry\construction_traits.hpp>
#include <geometry\numeric_sequence.hpp>
#include <geometry\detail/vector_generator.hpp>
#include <geometry\detail/point_generator.hpp>
#include <geometry\neutral_reference_frame.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{

//! Define access interface.
template <typename NumericType, unsigned int Dimension>
struct indexed_access_policy< point< NumericType, Dimension > > 
    : boost::integral_constant< indexed_sequence_access_type,
                                GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE >{};

//! Concrete Point Types for some of the more common coordinate types.
typedef point<float, 2>       point_float_2d;
typedef point<float, 3>       point_float_3d;
typedef point<double, 2>      point_double_2d;
typedef point<double, 3>      point_double_3d;

typedef point<int, 2>         point_int_2d;
typedef point<int, 3>         point_int_3d;
typedef point<long long, 2>   point_int64_2d;
typedef point<long long, 3>   point_int64_3d;

GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_float_2d, neutral_reference_frame_float_2d );
GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_float_3d, neutral_reference_frame_float_3d );
GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_double_2d, neutral_reference_frame_double_2d );
GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_double_3d, neutral_reference_frame_double_3d );

GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_int_2d, neutral_reference_frame_int_2d );
GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_int_3d, neutral_reference_frame_int_3d );
GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_int64_2d, neutral_reference_frame_int64_2d );
GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS( point_int64_3d, neutral_reference_frame_int64_3d );

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_POINT_HPP
