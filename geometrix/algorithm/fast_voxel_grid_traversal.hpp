//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FAST_VOXEL_GRID_TRAVERSAL_HPP
#define GEOMETRIX_FAST_VOXEL_GRID_TRAVERSAL_HPP
#pragma once

#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/algorithm/cohen_sutherland_line_clipping.hpp>
#include <geometrix/algorithm/grid_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <cstdint>

namespace geometrix
{
    //! Traverse a grid using the algorithm presented in A fast voxel traversal algorithm for ray tracing J Amanatides, A Woo - Eurographics, 1987 - cse.yorku.ca
    template <typename Grid, typename Segment, typename Visitor, typename NumberComparisonPolicy>
    inline void fast_voxel_grid_traversal(const Grid& grid, const Segment& segment, Visitor& visitor, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((Segment2DConcept<Segment>));
        typedef typename geometric_traits<Segment>::point_type point_type;
        typedef typename geometric_traits<point_type>::arithmetic_type coordinate_type;

        ///First find the start and end cell index addresses.
        BOOST_AUTO(sPoint, get_start(segment));
        BOOST_AUTO(tPoint, get_end(segment));

        BOOST_AUTO(minX, grid.get_min_x());
        BOOST_AUTO(minY, grid.get_min_y());
        BOOST_AUTO(maxX, grid.get_max_x());
        BOOST_AUTO(maxY, grid.get_max_y());

        //! Clip the segment to the grid bounds.
        if( (get<0>(sPoint) < minX || get<0>(sPoint) > maxX ||
             get<1>(sPoint) < minY || get<1>(sPoint) > maxY ||
             get<0>(tPoint) < minX || get<0>(tPoint) > maxX ||
             get<1>(tPoint) < minY || get<1>(tPoint) > maxY) &&
             !cohen_sutherland_line_clip(sPoint.get<0>(), sPoint.get<1>(), tPoint.get<0>(), tPoint.get<1>(), minX, maxX, minY, maxY, cmp) )
        {
            return;
        }

        BOOST_AUTO(x1, grid.get_scaled_grid_coordinate_x(get<0>(sPoint)));
        BOOST_AUTO(y1, grid.get_scaled_grid_coordinate_y(get<1>(sPoint)));
        BOOST_AUTO(x2, grid.get_scaled_grid_coordinate_x(get<0>(tPoint)));
        BOOST_AUTO(y2, grid.get_scaled_grid_coordinate_y(get<1>(tPoint)));
                
        boost::uint32_t i = static_cast<boost::uint32_t>(x1);
        boost::uint32_t j = static_cast<boost::uint32_t>(y1);

        boost::uint32_t i_end = static_cast<boost::uint32_t>(x2);
        boost::uint32_t j_end = static_cast<boost::uint32_t>(y2);

        visitor(i, j);

        //! search up the vertical strip
        if (i == i_end)
        {
            if (j == j_end)
                return;

            while (j != j_end)
            {
                if (j < j_end)
                    ++j;
                else
                    --j;
                visitor(i, j);
            }
            return;
        }

        //! search along the horizontal strip
        if (j == j_end)
        {
            while (i != i_end)
            {
                if ( i < i_end)
                    ++i;
                else 
                    --i;
                visitor(i, j);
            }
            return;
        }

        //! The segment is sloped. Use the traversal from "A Fast Voxel Traversal Algorithm for Ray Tracing".
        typedef vector<coordinate_type, 2> vector2;        
        vector2 segmentDirection = tPoint - sPoint;
        
        vector2 cellBorder;
        boost::int32_t stepI, outI;
		boost::int32_t stepJ, outJ;
        
        if (cmp.greater_than(segmentDirection.get<0>(), 0))
        {
            stepI = 1;
            outI = grid.get_width();
            cellBorder.get<0>() = minX + (i + 1) * grid.get_cell_size();
        }
        else
        {
            stepI = -1;
            outI = -1;
            cellBorder.get<0>() = minX + i * grid.get_cell_size();
        }

        if (cmp.greater_than(segmentDirection.get<1>(), 0))
        {
            stepJ = 1;
            outJ = grid.get_height();
            cellBorder.get<1>() = minY + (j + 1) * grid.get_cell_size();
        }
        else
        {
            stepJ = -1;
            outJ = -1;
            cellBorder.get<1>() = minY + j * grid.get_cell_size();
        }

        BOOST_ASSERT(segmentDirection.get<0>() != 0);
        BOOST_ASSERT(segmentDirection.get<1>() != 0);
        coordinate_type rxr = 1.0 / segmentDirection.get<0>();
        coordinate_type ryr = 1.0 / segmentDirection.get<1>();

        vector2 tmax((cellBorder.get<0>() - get<0>(sPoint)) * rxr, (cellBorder.get<1>() - get<1>(sPoint)) * ryr);
        vector2 tdelta(grid.get_cell_size() * stepI * rxr, grid.get_cell_size() * stepJ * ryr);
        
        while(true)
        {
            if (tmax.get<0>() < tmax.get<1>())
            {
                i += stepI;
                if (i == outI)
                    return;
                tmax.get<0>() += tdelta.get<0>();
            }
            else
            {
                j += stepJ;
                if (j == outJ)
                    return;
                tmax.get<1>() += tdelta.get<1>();
            }
            
            if (stepI > 0 && i > i_end || stepI < 0 && i < i_end || stepJ > 0 && j > j_end || stepJ < 0 && j < j_end)
                return;

            visitor(i, j);
        }
    }
}//! namespace geometrix;

#endif// GEOMETRIX_FAST_VOXEL_GRID_TRAVERSAL_HPP
