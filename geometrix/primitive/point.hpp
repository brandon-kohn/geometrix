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
typedef point<float, 2>       point_float_2D;
typedef point<float, 3>       point_float_3D;
typedef point<double, 2>      point_double_2D;
typedef point<double, 3>      point_double_3D;
typedef point<double, 4>      point_double_4D;

typedef point<int, 2>         point_int_2D;
typedef point<int, 3>         point_int_3D;
typedef point<boost::int64_t, 2>   point_int64_2D;
typedef point<boost::int64_t, 3>   point_int64_3D;

}//namespace geometrix;

GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_float_2D, (float), 2, double, neutral_reference_frame_2D, index_operator_vector_access_policy<point_float_2D> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_float_3D, (float), 3, double, neutral_reference_frame_3D, index_operator_vector_access_policy<point_float_3D> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_2D, (double), 2, double, neutral_reference_frame_2D, index_operator_vector_access_policy<point_double_2D> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_3D, (double), 3, double, neutral_reference_frame_3D, index_operator_vector_access_policy<point_double_3D> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_double_4D, (double), 4, double, neutral_reference_frame<4>, index_operator_vector_access_policy<point_double_4D> );

GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int_2D, (int), 2, double, neutral_reference_frame_2D, index_operator_vector_access_policy<point_int_2D> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int_3D, (int), 3, double, neutral_reference_frame_3D, index_operator_vector_access_policy<point_int_3D> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int64_2D, (boost::int64_t), 2, long double, neutral_reference_frame_2D, index_operator_vector_access_policy<point_int64_2D> );
GEOMETRIX_DEFINE_POINT_TRAITS( geometrix::point_int64_3D, (boost::int64_t), 3, long double, neutral_reference_frame_3D, index_operator_vector_access_policy<point_int64_3D> );

#endif //GEOMETRIX_POINT_HPP
