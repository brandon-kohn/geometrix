//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINE_TRAITS_HPP
#define GEOMETRIX_LINE_TRAITS_HPP
#pragma once

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>

namespace geometrix {
//! \brief Tag to check if a type is a line
template <typename Line, typename EnableIf=void>
struct is_line : boost::false_type{};

//! \brief line access traits struct
//! NOTE: must be specialized for user types.
template <typename Line, typename EnableIf=void>
struct line_access_traits
{
    BOOST_MPL_ASSERT_MSG(
          ( false )
        , LINE_ACCESS_TRAITS_NOT_DEFINED
        , (Line) );

    typedef Line                                                    line_type;
    typedef typename geometric_traits< line_type >::vector_type     vector_type;
    typedef typename geometric_traits< line_type >::point_type      point_type;
    typedef typename geometric_traits< line_type >::dimension_type  dimension_type;
    typedef typename geometric_traits< line_type >::arithmetic_type arithmetic_type;

    static const point_type&       get_reference_point( const line_type& l ){ return l.get_reference_point(); }
    static const vector_type&      get_parallel_vector( const line_type& l ){ return l.get_parallel_vector(); }
    static const vector_type&      get_normal_vector( const line_type& l ){ return l.get_normal_vector(); }
    static arithmetic_type         get_distance_to_origin( const line_type& l ){ return l.get_distance_to_origin(); }
};

}//namespace geometrix;

//! \brief Macro for defining a line type traits.
#define GEOMETRIX_DEFINE_LINE_TRAITS( Point, Vector, Line )                  \
namespace geometrix {                                                        \
template <> is_line< Line > : boost::true_type{};                            \
template <>                                                                  \
struct geometric_traits< Line >                                              \
{                                                                            \
    using hyperplane_dimension = dimension<2>;                               \
    typedef void                                            is_line;         \
    typedef void                                            is_plane;        \
    typedef Vector                                          vector_type;     \
    typedef Point                                           point_type;      \
    typedef Line                                            line_type;       \
    typedef geometric_traits< vector_type >::dimension_type dimension_type;  \
    typedef geometric_traits< point_type >::arithmetic_type arithmetic_type; \
};                                                                           \
}                                                                            \
/***/

#endif //GEOMETRIX_LINE_TRAITS_HPP
