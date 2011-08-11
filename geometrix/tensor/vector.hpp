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
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/detail/vector_generator.hpp>
#include <geometrix/space/neutral_reference_frame.hpp>
#include <geometrix/tensor/index_operator_vector_access_policy.hpp>

namespace geometrix {
    
//! Concrete Vector Types for some of the more common coordinate types.
typedef vector<float, 2>       vector_float_2D;
typedef vector<float, 3>       vector_float_3D;
typedef vector<float, 4>       vector_float_4D;
typedef vector<double, 2>      vector_double_2D;
typedef vector<double, 3>      vector_double_3D;
typedef vector<double, 4>      vector_double_4D;

typedef vector<int, 2>         vector_int_2D;
typedef vector<int, 3>         vector_int_3D;
typedef vector<boost::int64_t, 2>   vector_int64_2D;
typedef vector<boost::int64_t, 3>   vector_int64_3D;

}//namespace geometrix;

GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_float_2D, (float), 2, double, neutral_reference_frame_2D, index_operator_vector_access_policy<vector_float_2D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_float_3D, (float), 3, double, neutral_reference_frame_3D, index_operator_vector_access_policy<vector_float_3D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_float_4D, (float), 4, double, neutral_reference_frame<4>, index_operator_vector_access_policy<vector_float_4D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_double_2D, (double), 2, double, neutral_reference_frame_2D, index_operator_vector_access_policy<vector_double_2D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_double_3D, (double), 3, double, neutral_reference_frame_3D, index_operator_vector_access_policy<vector_double_3D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_double_4D, (double), 4, double, neutral_reference_frame<4>, index_operator_vector_access_policy<vector_double_4D> );

GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int_2D, (int), 2, double, neutral_reference_frame_2D, index_operator_vector_access_policy<vector_int_2D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int_3D, (int), 3, double, neutral_reference_frame_3D, index_operator_vector_access_policy<vector_int_3D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int64_2D, (boost::int64_t), 2, long double, neutral_reference_frame_2D, index_operator_vector_access_policy<vector_int64_2D> );
GEOMETRIX_DEFINE_VECTOR_TRAITS( geometrix::vector_int64_3D, (boost::int64_t), 3, long double, neutral_reference_frame_3D, index_operator_vector_access_policy<vector_int64_3D> );

#endif //GEOMETRIX_VECTOR_HPP
