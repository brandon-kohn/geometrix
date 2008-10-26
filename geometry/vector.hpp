//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_VECTOR_HPP
#define _BOOST_GEOMETRY_VECTOR_HPP
#pragma once

#include "vector_traits.hpp"
#include "math_functions.hpp"
#include "dimension_traits.hpp"
#include "numeric_sequence.hpp"
#include "detail/vector_generator.hpp"
#include "reference_frame_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Define access interface
template <typename NumericType, unsigned int Dimension>
struct has_compile_time_access< vector< NumericType, Dimension > > : boost::true_type {};
template <typename NumericType, unsigned int Dimension>
struct has_run_time_access< vector< NumericType, Dimension > > : boost::true_type {};

//! Concrete Vector Types for some of the more common coordinate types.
typedef vector<float, 2>       vector_float_2d;
typedef vector<float, 3>       vector_float_3d;
typedef vector<double, 2>      vector_double_2d;
typedef vector<double, 3>      vector_double_3d;

typedef vector<int, 2>         vector_int_2d;
typedef vector<int, 3>         vector_int_3d;
typedef vector<long long, 2>   vector_int64_2d;
typedef vector<long long, 3>   vector_int64_3d;

BOOST_DEFINE_VECTOR_TRAITS( vector_float_2d, neutral_reference_frame_float_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_float_3d, neutral_reference_frame_float_3d );
BOOST_DEFINE_VECTOR_TRAITS( vector_double_2d, neutral_reference_frame_double_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_double_3d, neutral_reference_frame_double_3d );

BOOST_DEFINE_VECTOR_TRAITS( vector_int_2d, neutral_reference_frame_int_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_int_3d, neutral_reference_frame_int_3d );
BOOST_DEFINE_VECTOR_TRAITS( vector_int64_2d, neutral_reference_frame_int64_2d );
BOOST_DEFINE_VECTOR_TRAITS( vector_int64_3d, neutral_reference_frame_int64_3d );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_VECTOR_HPP
