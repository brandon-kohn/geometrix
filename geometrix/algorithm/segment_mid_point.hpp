//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_MIDPOINT_HPP
#define GEOMETRIX_SEGMENT_MIDPOINT_HPP
#pragma once

#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

namespace geometrix {

    template <typename Segment>
    inline point<typename geometric_traits<typename geometric_traits<Segment>::point_type>::arithmetic_type, dimension_of<typename geometric_traits<Segment>::point_type>::value> segment_mid_point( const Segment& seg )
    {
		return get_start(seg) + 0.5 * (get_end(seg) - get_start(seg));
    }

}//namespace geometrix;

#endif //! GEOMETRIX_SEGMENT_MIDPOINT_HPP
