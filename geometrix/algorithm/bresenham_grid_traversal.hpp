//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BRESENHAM_SEGMENT_TRAVERSAL_HPP
#define GEOMETRIX_BRESENHAM_SEGMENT_TRAVERSAL_HPP
#pragma once

#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

namespace geometrix
{
    template <typename Grid, typename Segment, typename Visitor>
    inline void bresenham_segment_traversal(const Grid& grid, const Segment& segment, Visitor& visitor)
    {        
        ///First find the start and end cell index addresses.
        BOOST_AUTO(const& sPoint, get_start(segment));
        BOOST_AUTO(const& tPoint, get_end(segment));

        int x1 = grid.get_x_index(get<0>(sPoint));
        int y1 = grid.get_y_index(get<1>(sPoint));
        int x2 = grid.get_x_index(get<0>(tPoint));
        int y2 = grid.get_y_index(get<1>(tPoint));
        int dx, dy, bx, by, xsign, ysign, p, const1, const2;

        bx = x1;
        by = y1;
        dx = (x2 - x1);
        dy = (y2 - y1);
        if ((dx == 0) && (dy == 0)) // line fits into one grid cell
        {
            visitor(bx, by);
        }
        else if (dy == 0) // have a horizontal line
        {
            xsign = dx / math::abs(dx);

            visitor(bx, by);
            while (bx != x2)
            {
                bx += xsign;
                visitor(bx, by);
            }
        }
        else if (dx == 0) // have vertical line
        {
            ysign = dy / math::abs(dy);
            visitor(bx, by);
            while (by != y2)
            {
                by += ysign;
                visitor(bx, by);
            }
        }
        else // use Bresenham algorithm
        {
            xsign = dx / math::abs(dx);
            ysign = dy / math::abs(dy);
            dx = math::abs(dx);
            dy = math::abs(dy);
            // set initial point on line
            visitor(bx, by);

            if (dx < dy) // line more vertical than horizontal
            {
                p = 2 * dx - dy;
                const1 = 2 * dx;
                const2 = 2 * (dx - dy);
                while (by != y2)
                {
                    by = by + ysign;
                    if (p < 0)
                    {
                        p = p + const1;
                    }
                    else
                    {
                        p = p + const2;
                        bx = bx + xsign;
                    }

                    visitor(bx, by);
                }
            }
            else // line more horizontal than vertical
            {
                p = 2 * dy - dx;
                const2 = 2 * (dy - dx);
                const1 = 2 * dy;
                while (bx != x2)
                {
                    bx = bx + xsign;
                    if (p < 0)
                    {
                        p = p + const1;
                    }
                    else
                    {
                        p = p + const2;
                        by = by + ysign;
                    }

                    visitor(bx, by);
                }
            }
        }
    }
}//! namespace geometrix;

#endif// GEOMETRIX_BRESENHAM_SEGMENT_TRAVERSAL_HPP
