//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_PLANE_TRAITS_HPP
#define GEOMETRIX_PLANE_TRAITS_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>

namespace geometrix {
//! \brief Tag to check if a type is a plane
template <typename Plane, typename EnableIf=void>
struct is_plane : boost::false_type{};

//! \brief plane access traits struct
//! NOTE: must be specialized for user types.
template <typename Plane, typename EnableIf=void>
struct plane_access_traits
{
    BOOST_MPL_ASSERT_MSG(
          ( false )
        , PLANE_ACCESS_TRAITS_NOT_DEFINED
        , (Plane) );

    typedef Plane                                                    plane_type;
    typedef typename geometric_traits< plane_type >::vector_type     vector_type;
    typedef typename geometric_traits< plane_type >::point_type      point_type;
    typedef typename geometric_traits< plane_type >::dimension_type  dimension_type;

    static const point_type&       get_reference_point( const plane_type& p ){ return p.get_reference_point(); }
    static const vector_type&      get_parallel_vector( const plane_type& p ){ return p.get_parallel_vector(); }
    static const vector_type&      get_normal_vector( const plane_type& p ){ return p.get_normal_vector(); }

};

}//namespace geometrix;

//! \brief Macro for defining a plane type traits.
#define GEOMETRIX_DEFINE_PLANE_TRAITS( Point, Vector, Plane )                \
namespace geometrix {                                                        \
template <> is_plane< Plane > : boost::true_type{};                          \
template <>                                                                  \
struct geometric_traits< plane >                                             \
{                                                                            \
    using hyperplane_dimension = dimension<3>;                               \
    typedef Vector                                        vector_type;       \
    typedef Point                                         point_type;        \
    typedef Plane                                         plane_type;        \
    typedef geometric_traits< vector_type >::dimension_type  dimension_type; \
};                                                                           \
}                                                                            \
/***/

#endif //GEOMETRIX_PLANE_TRAITS_HPP
