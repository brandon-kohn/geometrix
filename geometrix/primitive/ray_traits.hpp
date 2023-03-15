//
//! Copyright © 2023
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RAY_TRAITS_HPP
#define GEOMETRIX_RAY_TRAITS_HPP
#pragma once

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>

namespace geometrix {
//! \brief Tag to check if a type is a ray
template <typename Ray, typename EnableIf=void>
struct is_ray : boost::false_type{};

//! \brief ray access traits struct
//! NOTE: must be specialized for user types.
template <typename Ray, typename EnableIf=void>
struct ray_access_traits
{
    BOOST_MPL_ASSERT_MSG(
          ( false )
        , RAY_ACCESS_TRAITS_NOT_DEFINED
        , (Ray) );

    typedef Ray                                                    ray_type;
    typedef typename geometric_traits< ray_type >::vector_type     vector_type;
    typedef typename geometric_traits< ray_type >::point_type      point_type;
    typedef typename geometric_traits< ray_type >::dimension_type  dimension_type;
    typedef typename geometric_traits< ray_type >::arithmetic_type arithmetic_type;

    static const point_type&       get_reference_point( const ray_type& l ){ return l.get_reference_point(); }
    static const vector_type&      get_parallel_vector( const ray_type& l ){ return l.get_parallel_vector(); }
    static const vector_type&      get_normal_vector( const ray_type& l ){ return l.get_normal_vector(); }
    static arithmetic_type         get_distance_to_origin( const ray_type& l ){ return l.get_distance_to_origin(); }
};

}//namespace geometrix;

//! \brief Macro for defining a ray type traits.
#define GEOMETRIX_DEFINE_RAY_TRAITS( Point, Vector, Ray )                   \
namespace geometrix {                                                       \
template <> is_ray< Ray > : boost::true_type{};                             \
template <>                                                                 \
struct geometric_traits< Ray >                                              \
{                                                                           \
    using hyperplane_dimension = dimension<2>;                              \
    typedef void                                            is_ray;         \
    typedef void                                            is_plane;       \
    typedef Vector                                          vector_type;    \
    typedef Point                                           point_type;     \
    typedef Ray                                             ray_type;       \
    typedef geometric_traits< vector_type >::dimension_type dimension_type; \
    typedef geometric_traits< point_type >::arithmetic_type arithmetic_type;\
};                                                                          \
}                                                                           \
/***/

#endif //GEOMETRIX_RAY_TRAITS_HPP
