//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYGON_TRAVERSAL_HPP
#define GEOMETRIX_POLYGON_TRAVERSAL_HPP
#pragma once

#include <geometrix/primitive/polygon.hpp>

namespace geometrix {

    //! Visit each segment in a polygon with v(pgon, i, j) -> bool where if all v returns return false the traversal stops.
    template <typename Polygon, typename ... Visitors>
    inline void polygon_traversal(const Polygon& poly, Visitors&& ... args)
    {
        using access = point_sequence_traits<Polygon>;
        auto size = access::size(poly);
        for (std::size_t i = size-1, j = 0; j < size; i = j++)
        {
            bool r = false;
            ( (r |= args(poly, i, j)),...);
            if(!r)
                return;
        }
    }
}//namespace geometrix;

#endif//! GEOMETRIX_POLYGON_TRAVERSAL_HPP
