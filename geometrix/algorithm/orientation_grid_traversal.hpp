//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ORIENTATION_SEGMENT_TRAVERSAL_HPP
#define GEOMETRIX_ORIENTATION_SEGMENT_TRAVERSAL_HPP
#pragma once

#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algorithm/orientation/point_segment_orientation.hpp>
#include <geometrix/algorithm/cohen_sutherland_line_clipping.hpp>
#include <geometrix/algorithm/grid_traits.hpp>
#include <geometrix/primitive/point.hpp>

#include <cstdint>

namespace geometrix
{
    //! Traverse a grid guided by a segment using the orientation of the segment to the cell corner points to determine the path.
    template <typename Grid, typename Segment, typename Visitor, typename NumberComparisonPolicy>
    inline void orientation_segment_traversal(const Grid& grid, const Segment& segment, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        typedef typename geometric_traits< Segment >::point_type point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type coordinate_type;

        ///First find the start and end cell index addresses.
        point<coordinate_type, 2> sPoint = get_start(segment);
        point<coordinate_type, 2> tPoint = get_end(segment);

        if (!lexicographically_less_than(sPoint, tPoint, cmp))
        {
            using namespace std;
            swap(sPoint, tPoint);
        }
                
        BOOST_AUTO(minX, grid.get_min_x());
        BOOST_AUTO(minY, grid.get_min_y());
        BOOST_AUTO(maxX, grid.get_max_x());
        BOOST_AUTO(maxY, grid.get_max_y());

		BOOST_AUTO( sx, get<0>( sPoint ) );
		BOOST_AUTO( sy, get<1>( sPoint ) );
		BOOST_AUTO( tx, get<0>( tPoint ) );
		BOOST_AUTO( ty, get<1>( tPoint ) );

        //! Clip the segment to the grid bounds.
        if( (sx < minX || sx > maxX ||
             sy < minY || sy > maxY ||
             tx < minX || tx > maxX ||
             ty < minY || ty > maxY) &&
			 !cohen_sutherland_line_clip( sx, sy, tx, ty, minX, maxX, minY, maxY, cmp ) )
        {
            return;
        }

        BOOST_AUTO(x1, grid.get_scaled_grid_coordinate_x(sx));
        BOOST_AUTO(y1, grid.get_scaled_grid_coordinate_y(sy));
        BOOST_AUTO(x2, grid.get_scaled_grid_coordinate_x(tx));
        BOOST_AUTO(y2, grid.get_scaled_grid_coordinate_y(ty));

        int signDeltaY = cmp.greater_than_or_equal(y2, y1) ? 1 : -1;
        
        coordinate_type x = x1;
        coordinate_type y = y1;

        std::uint32_t i = static_cast<std::uint32_t>(x);
        std::uint32_t j = static_cast<std::uint32_t>(y);

        std::uint32_t i_end = static_cast<std::uint32_t>(x2);
        std::uint32_t j_end = static_cast<std::uint32_t>(y2);

        visitor(i, j);

        //! search up the vertical strip
        if (i == i_end)
        {
            if (j == j_end)
                return;

            while (j != j_end)
            {
                ++j;
                visitor(i, j);
            }
            return;
        }

        //! search along the horizontal strip
        if (j == j_end)
        {
            while (i != i_end)
            {
                ++i;
                visitor(i, j);
            }
            return;
        }

        //! The segment is sloped. Use a Bresenham-like traversal which is guided by the segment's orientation to the cells.
        std::uint32_t old_i = i, old_j = j;
        while (true)
        {
            point<coordinate_type,2> cornerPoint;
            orientation_type orientation = oriented_collinear;

            if (signDeltaY > 0)
            {
                cornerPoint = grid.get_cell_corner2(i, j);
                orientation = get_orientation(sPoint, tPoint, cornerPoint, cmp);
            }
            else
            {
                cornerPoint = grid.get_cell_corner1(i, j);

                //! Take the mirror of this case if not collinear.
                orientation = get_orientation(sPoint, tPoint, cornerPoint, cmp);
                if (orientation == oriented_left)
                    orientation = oriented_right;
                else if (orientation == oriented_right)
                    orientation = oriented_left;
            }

            //! Check for overrun.
            coordinate_type xdiff = x2 - x;
            coordinate_type ydiff = y2 - y;
            bool x_is_over = cmp.less_than(xdiff, 0);
            bool y_is_over = signDeltaY > 0 ? cmp.less_than(ydiff, 0) : cmp.greater_than(ydiff, 0);

            if (orientation == oriented_left)
            {
                if (x_is_over || cmp.equals(xdiff, 0))
                    break;
                else
                    x += (std::min<coordinate_type>)(1, xdiff);
            }
            else if (orientation == oriented_collinear)
            {
                if ((x_is_over && y_is_over) || (cmp.equals(xdiff, 0) && cmp.equals(ydiff, 0)))
                    break;
                else
                {
                    x += (std::min<coordinate_type>)(1, xdiff);
                    y += signDeltaY > 0 ? (std::min<coordinate_type>)(signDeltaY, ydiff) : (std::max<coordinate_type>)(signDeltaY, ydiff);
                }
            }
            else if (orientation == oriented_right)
            {
                if (y_is_over || cmp.equals(ydiff, 0))
                    break;
                else
                    y += signDeltaY > 0 ? (std::min<coordinate_type>)(signDeltaY, ydiff) : (std::max<coordinate_type>)(signDeltaY, ydiff);
            }

            i = static_cast<std::uint32_t>(x);
            j = static_cast<std::uint32_t>(y);

            if (old_i != i || old_j != j)
                visitor(i, j);

            old_i = i;
            old_j = j;
        }
    }
}//! namespace geometrix;

#endif// GEOMETRIX_ORIENTATION_SEGMENT_TRAVERSAL_HPP
