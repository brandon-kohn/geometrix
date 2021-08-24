//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLYLINE_TRAVERSAL_HPP
#define GEOMETRIX_POLYLINE_TRAVERSAL_HPP
#pragma once

#include <geometrix/primitive/polyline.hpp>

namespace geometrix {

    //! Visit each segment in a polyline with v(pline, i, j) -> bool where if all v returns return false the traversal stops.
    template <typename Polyline, typename ... Visitors>
    inline void polyline_traversal(const Polyline& poly, Visitors&& ... args)
    {
        using access = point_sequence_traits<Polyline>;
        auto size = access::size(poly);
        for (std::size_t i = 0, j = 1; j < size; i = j++)
        {
            bool r = false;
            ( (r |= args(poly, i, j)),...);
            if(!r)
                return;
        }
    }
}//namespace geometrix;

#endif//! GEOMETRIX_POLYLINE_TRAVERSAL_HPP
