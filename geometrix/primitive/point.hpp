//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_HPP
#define GEOMETRIX_POINT_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/detail/vector_generator.hpp>
#include <geometrix/tensor/detail/point_generator.hpp>
#include <geometrix/space/neutral_reference_frame.hpp>
#include <geometrix/tensor/index_operator_vector_access_policy.hpp>

namespace geometrix {
    
//! Concrete Point Types for some of the more common coordinate types.
typedef point<float, 2>       point_float_2d;
typedef point<float, 3>       point_float_3d;
typedef point<double, 2>      point_double_2d;
typedef point<double, 3>      point_double_3d;
typedef point<double, 4>      point_double_4d;

typedef point<int, 2>         point_int_2d;
typedef point<int, 3>         point_int_3d;
typedef point<boost::int64_t, 2>   point_int64_2d;
typedef point<boost::int64_t, 3>   point_int64_3d;

}//namespace geometrix;

GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_float_2d, (float), 2, double, neutral_reference_frame_2d, index_operator_vector_access_policy<point_float_2d> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_float_3d, (float), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy<point_float_3d> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_2d, (double), 2, double, neutral_reference_frame_2d, index_operator_vector_access_policy<point_double_2d> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_3d, (double), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy<point_double_3d> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_4d, (double), 4, double, neutral_reference_frame<4>, index_operator_vector_access_policy<point_double_4d> );

GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int_2d, (int), 2, double, neutral_reference_frame_2d, index_operator_vector_access_policy<point_int_2d> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int_3d, (int), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy<point_int_3d> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int64_2d, (boost::int64_t), 2, long double, neutral_reference_frame_2d, index_operator_vector_access_policy<point_int64_2d> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int64_3d, (boost::int64_t), 3, long double, neutral_reference_frame_3d, index_operator_vector_access_policy<point_int64_3d> );

#endif //GEOMETRIX_POINT_HPP
