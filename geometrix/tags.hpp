//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TAGS_HPP
#define GEOMETRIX_TAGS_HPP
#pragma once

#include <geometrix/geometric_traits.hpp>
#include <boost/mpl/identity.hpp>
#include <type_traits>

namespace geometrix{

    namespace geometry_tags {
        struct point_tag {};
        struct vector_tag {};
        struct segment_tag {};
        struct polygon_tag {};
        struct polygon_with_holes_tag {};
        struct polyline_tag {};
    }//! namespace geometry_tags;

    template <typename T, typename EnableIf=void>
    struct geometry_tag_of{};

    template <typename T>
    struct geometry_tag_of<T, typename geometrix::geometric_traits<typename std::decay<T>::type>::is_point>
        : boost::mpl::identity<geometry_tags::point_tag>
    {};

    template <typename T>
    struct geometry_tag_of<T, typename geometrix::geometric_traits<typename std::decay<T>::type>::is_vector>
        : boost::mpl::identity<geometry_tags::vector_tag>
    {};

    template <typename T>
    struct geometry_tag_of<T, typename geometrix::geometric_traits<typename std::decay<T>::type>::is_segment>
        : boost::mpl::identity<geometry_tags::segment_tag>
    {};

    template <typename T>
    struct geometry_tag_of<T, typename geometrix::geometric_traits<typename std::decay<T>::type>::is_polygon>
        : boost::mpl::identity<geometry_tags::polygon_tag>
    {};

    template <typename T>
    struct geometry_tag_of<T, typename geometrix::geometric_traits<typename std::decay<T>::type>::is_polygon_with_holes>
        : boost::mpl::identity<geometry_tags::polygon_with_holes_tag>
    {};

    template <typename T>
    struct geometry_tag_of<T, typename geometrix::geometric_traits<typename std::decay<T>::type>::is_polyline>
        : boost::mpl::identity<geometry_tags::polyline_tag>
    {};

}//! namespace geometrix;

#endif //! GEOMETRIX_TAGS_HPP
