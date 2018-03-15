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
#include <geometrix/numeric/constants.hpp>
#include <cstdint>

namespace geometrix
{
    //! Traverse a grid using the algorithm presented in A fast voxel traversal algorithm for ray tracing J Amanatides, A Woo - Eurographics, 1987 - cse.yorku.ca
    template <typename Grid, typename Segment, typename Visitor, typename NumberComparisonPolicy>
    inline void fast_voxel_grid_traversal(const Grid& grid, const Segment& segment, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((Segment2DConcept<Segment>));
        using point_t = typename geometric_traits<Segment>::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using dimensionless_t = decltype(length_t() / length_t());

        ///First find the start and end cell index addresses.
        BOOST_AUTO(sPoint, get_start(segment));
        BOOST_AUTO(tPoint, get_end(segment));

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

		BOOST_AUTO( x1, grid.get_scaled_grid_coordinate_x( sx ) );
		BOOST_AUTO( y1, grid.get_scaled_grid_coordinate_y( sy ) );
		BOOST_AUTO( x2, grid.get_scaled_grid_coordinate_x( tx ) );
		BOOST_AUTO( y2, grid.get_scaled_grid_coordinate_y( ty ) );
                
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
                if (i < i_end)
                    ++i;
                else 
                    --i;
                visitor(i, j);
            }
            return;
        }

        //! The segment is sloped. Use the traversal from "A Fast Voxel Traversal Algorithm for Ray Tracing".
        using vector_t = vector<length_t, 2>;        
		vector_t segmentDirection = tPoint - sPoint;
        
        vector_t cellBorder;
        boost::int32_t stepI, outI;
		boost::int32_t stepJ, outJ;
        
		const auto zero = constants::zero<length_t>();
		if( cmp.greater_than( get<0>(segmentDirection), zero ) )
        {
            stepI = 1;
            outI = grid.get_width();
			set<0>(cellBorder, minX + construct<dimensionless_t>(i + 1) * grid.get_cell_size());
        }
        else
        {
            stepI = -1;
            outI = -1;
			set<0>(cellBorder, length_t(minX + construct<dimensionless_t>(i) * grid.get_cell_size()));
        }

		if( cmp.greater_than( get<1>(segmentDirection), zero ) )
        {
            stepJ = 1;
            outJ = grid.get_height();
			set<1>(cellBorder, length_t(minY + construct<dimensionless_t>(j + 1) * grid.get_cell_size()));
        }
        else
        {
            stepJ = -1;
            outJ = -1;
			set<1>(cellBorder, length_t(minY + construct<dimensionless_t>(j) * grid.get_cell_size()));
        }

		GEOMETRIX_ASSERT( get<0>(segmentDirection) != zero );
		GEOMETRIX_ASSERT( get<1>(segmentDirection) != zero );
		auto one = constants::one<length_t>();
		dimensionless_t rxr = one / get<0>(segmentDirection);
		dimensionless_t ryr = one / get<1>(segmentDirection);

		vector_t tmax( (get<0>(cellBorder) - get<0>( sPoint )) * rxr, (get<1>(cellBorder) - get<1>( sPoint )) * ryr );
        vector_t tdelta(grid.get_cell_size() * construct<dimensionless_t>(stepI) * rxr, grid.get_cell_size() * construct<dimensionless_t>(stepJ) * ryr);
        
        while(true)
        {
			if( tmax.template get<0>() < tmax.template get<1>() )
            {
                i += stepI;
                if (i == outI)
                    return;
				tmax.template get<0>() += tdelta.template get<0>();
            }
            else
            {
                j += stepJ;
                if (j == outJ)
                    return;
				tmax.template get<1>() += tdelta.template get<1>();
            }
            
            if ((stepI > 0 && i > i_end) || (stepI < 0 && i < i_end) || (stepJ > 0 && j > j_end) || (stepJ < 0 && j < j_end))
                return;

            visitor(i, j);
        }
    }

    template <typename Grid, typename Segment, typename Visitor, typename NumberComparisonPolicy>
    inline void stoppable_fast_voxel_grid_traversal(const Grid& grid, const Segment& segment, Visitor&& visitor, const NumberComparisonPolicy& cmp)
    {
        BOOST_CONCEPT_ASSERT((Segment2DConcept<Segment>));
        using point_t = typename geometric_traits<Segment>::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
		using dimensionless_t = decltype(length_t() / length_t());

        ///First find the start and end cell index addresses.
        BOOST_AUTO(sPoint, get_start(segment));
        BOOST_AUTO(tPoint, get_end(segment));

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

		BOOST_AUTO( x1, grid.get_scaled_grid_coordinate_x( sx ) );
		BOOST_AUTO( y1, grid.get_scaled_grid_coordinate_y( sy ) );
		BOOST_AUTO( x2, grid.get_scaled_grid_coordinate_x( tx ) );
		BOOST_AUTO( y2, grid.get_scaled_grid_coordinate_y( ty ) );
                
        boost::uint32_t i = static_cast<boost::uint32_t>(x1);
        boost::uint32_t j = static_cast<boost::uint32_t>(y1);

        boost::uint32_t i_end = static_cast<boost::uint32_t>(x2);
        boost::uint32_t j_end = static_cast<boost::uint32_t>(y2);

        if(!visitor(i, j))
            return;

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
                if(!visitor(i, j))
                    return;
            }
            return;
        }

        //! search along the horizontal strip
        if (j == j_end)
        {
            while (i != i_end)
            {
                if (i < i_end)
                    ++i;
                else 
                    --i;
                if(!visitor(i, j))
                    return;
            }
            return;
        }

        //! The segment is sloped. Use the traversal from "A Fast Voxel Traversal Algorithm for Ray Tracing".
        using vector_t = vector<length_t, 2>;        
		vector_t segmentDirection = tPoint - sPoint;
        
        vector_t cellBorder;
        boost::int32_t stepI, outI;
		boost::int32_t stepJ, outJ;
        
		const auto zero = constants::zero<length_t>();
		if( cmp.greater_than( get<0>(segmentDirection), zero ) )
        {
            stepI = 1;
            outI = grid.get_width();
			set<0>(cellBorder, minX + construct<dimensionless_t>(i + 1) * grid.get_cell_size());
        }
        else
        {
            stepI = -1;
            outI = -1;
			set<0>(cellBorder, length_t(minX + construct<dimensionless_t>(i) * grid.get_cell_size()));
        }

		if( cmp.greater_than( get<1>(segmentDirection), zero ) )
        {
            stepJ = 1;
            outJ = grid.get_height();
			set<1>(cellBorder, length_t(minY + construct<dimensionless_t>(j + 1) * grid.get_cell_size()));
        }
        else
        {
            stepJ = -1;
            outJ = -1;
			set<1>(cellBorder, length_t(minY + construct<dimensionless_t>(j) * grid.get_cell_size()));
        }

		GEOMETRIX_ASSERT( get<0>(segmentDirection) != zero );
		GEOMETRIX_ASSERT( get<1>(segmentDirection) != zero );
		auto one = constants::one<length_t>();
		dimensionless_t rxr = one / get<0>(segmentDirection);
		dimensionless_t ryr = one / get<1>(segmentDirection);

		vector_t tmax( (get<0>(cellBorder) - get<0>( sPoint )) * rxr, (get<1>(cellBorder) - get<1>( sPoint )) * ryr );
        vector_t tdelta(grid.get_cell_size() * construct<dimensionless_t>(stepI) * rxr, grid.get_cell_size() * construct<dimensionless_t>(stepJ) * ryr);
        
        while(true)
        {
			if( tmax.template get<0>() < tmax.template get<1>() )
            {
                i += stepI;
                if (i == outI)
                    return;
				tmax.template get<0>() += tdelta.template get<0>();
            }
            else
            {
                j += stepJ;
                if (j == outJ)
                    return;
				tmax.template get<1>() += tdelta.template get<1>();
            }
            
            if ((stepI > 0 && i > i_end) || (stepI < 0 && i < i_end) || (stepJ > 0 && j > j_end) || (stepJ < 0 && j < j_end))
                return;

            if(!visitor(i, j))
                return;
        }
    }
}//! namespace geometrix;

#endif// GEOMETRIX_FAST_VOXEL_GRID_TRAVERSAL_HPP
