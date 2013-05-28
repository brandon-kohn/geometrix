//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_HPP
#define GEOMETRIX_VECTOR_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/detail/vector_generator.hpp>
#include <geometrix/space/neutral_reference_frame.hpp>
#include <geometrix/tensor/index_operator_vector_access_policy.hpp>

namespace geometrix {
    
//! Concrete Vector Types for some of the more common coordinate types.
typedef vector<float, 2>       vector_float_2d;
typedef vector<float, 3>       vector_float_3d;
typedef vector<float, 4>       vector_float_4d;
typedef vector<double, 2>      vector_double_2d;
typedef vector<double, 3>      vector_double_3d;
typedef vector<double, 4>      vector_double_4d;

typedef vector<int, 2>         vector_int_2d;
typedef vector<int, 3>         vector_int_3d;
typedef vector<boost::int64_t, 2>   vector_int64_2d;
typedef vector<boost::int64_t, 3>   vector_int64_3d;

}//namespace geometrix;

GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_float_2d, (float), 2, double, neutral_reference_frame_2d, index_operator_vector_access_policy<vector_float_2d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_float_3d, (float), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy<vector_float_3d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_float_4d, (float), 4, double, neutral_reference_frame<4>, index_operator_vector_access_policy<vector_float_4d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_double_2d, (double), 2, double, neutral_reference_frame_2d, index_operator_vector_access_policy<vector_double_2d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_double_3d, (double), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy<vector_double_3d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_double_4d, (double), 4, double, neutral_reference_frame<4>, index_operator_vector_access_policy<vector_double_4d> );

GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int_2d, (int), 2, double, neutral_reference_frame_2d, index_operator_vector_access_policy<vector_int_2d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int_3d, (int), 3, double, neutral_reference_frame_3d, index_operator_vector_access_policy<vector_int_3d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int64_2d, (boost::int64_t), 2, long double, neutral_reference_frame_2d, index_operator_vector_access_policy<vector_int64_2d> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int64_3d, (boost::int64_t), 3, long double, neutral_reference_frame_3d, index_operator_vector_access_policy<vector_int64_3d> );

#endif //GEOMETRIX_VECTOR_HPP
