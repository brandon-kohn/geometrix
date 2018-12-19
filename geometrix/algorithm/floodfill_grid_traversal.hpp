//
//! Copyright © 2008-2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/fast_voxel_grid_traversal.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/utility/assert.hpp>
#include <boost/scope_exit.hpp>
#include <limits>
#include <stack>

namespace geometrix
{
    //! \class floodfill_grid_traversal
    //! This class encapsulates a color grid which can be used to perform
    //! flood-fill traversals on any positive index based grid type.( x and y >= 0)
    template <typename GridTraits>
    class floodfill_grid_traversal
    {
        using grid_traits = GridTraits;
        using grid = hash_grid_2d<std::uint32_t, grid_traits>;

    public:

		static const std::size_t BOUNDARY = static_cast<std::size_t>(-1);

        floodfill_grid_traversal(const grid_traits& gridTraits)
            : m_colorGrid(gridTraits)
            , m_height(gridTraits.get_height())
            , m_width(gridTraits.get_width())
            , m_lastColor(0)
            , m_gridTraits(gridTraits)
        {}

        //! \brief Method to perform a flood traversal.
        //! Note: The visitor class must hold a reference and do the actual cell extraction from the grid.
        //! This allows this interface to remain generic with respect to all grid access details (these become details the visitor should know however).    
        template <typename Point, typename Visitor>
        bool traversal(const Point& p, Visitor&& visitor)
        {
            return traversal(m_gridTraits.get_x_index(get<0>(p)), m_gridTraits.get_y_index(get<1>(p)), std::forward<Visitor>(visitor));
        }

        template <typename Visitor>
        bool traversal(std::uint32_t x, std::uint32_t y, Visitor&& v)
        {
            GEOMETRIX_ASSERT(x < m_width);
            GEOMETRIX_ASSERT(y < m_height);
            BOOST_SCOPE_EXIT_ALL(this) { ++m_lastColor; };

            std::ptrdiff_t y1; 
            bool spanLeft, spanRight;
            auto oldColor = get_color(x,y);
			if (oldColor == BOUNDARY)
				return false;

            auto newColor = m_lastColor + 1;//! Note: if this ever wraps...

            std::stack< std::pair<std::ptrdiff_t,std::ptrdiff_t> > stack;
            stack.emplace(x,y);

            while(!stack.empty())
            {    
                std::tie(x,y) = stack.top();
                stack.pop();

                y1 = y;
                while( y1 >= 0 && get_color(x,y1) == oldColor ) 
                {
                    y1--;
                }
                ++y1;

                spanLeft = spanRight = 0;
                while(y1 < m_height && get_color(x,y1) == oldColor)
                {
                    auto& c = get_color(x, y1);
                    if( c != BOUNDARY )
                        c = newColor;
                    
                    v(x, y1);
                    auto c_1 = get_color(x - 1, y1);
                    if(!spanLeft && x > 0 && c_1 == oldColor) 
                    {
                        stack.emplace(x - 1, y1);
                        spanLeft = 1;
                    }
                    else if(spanLeft && x > 0 && c_1 != oldColor)
                        spanLeft = 0;

                    auto cp1 = get_color(x + 1, y1);
                    if(!spanRight && x < m_width - 1 && cp1 == oldColor) 
                    {
                        stack.emplace(x + 1, y1);
                        spanRight = 1;
                    }
                    else if(spanRight && x < m_width - 1 && cp1 != oldColor)
                        spanRight = 0;
                    ++y1;
                }
            }

			return true;
        }

        template <typename PointSequence, typename NumberComparisonPolicy, typename std::enable_if<is_polyline<PointSequence>::value, int>::type = 0>
        void mark_boundary( const PointSequence& pline, const NumberComparisonPolicy& cmp)
        {
            typedef point_sequence_traits<PointSequence>::point_type point_type;
            typedef segment<point_type> segment_type;
            auto visitor = [this](std::uint32_t i, std::uint32_t j){ get_color(i, j) = BOUNDARY; }
            std::size_t numberSegments = point_sequence_traits<PointSequence>::size(pline) - 1;
            for(std::size_t i = 0; i < numberSegments; ++i)
            {
                segment_type segment(point_sequence_traits<PointSequence>::get_point(pline, i), point_sequence_traits<PointSequence>::get_point(pline, i + 1));
                fast_voxel_grid_traversal(m_gridTraits, segment, visitor, cmp);
            }
        }
        
        template <typename PointSequence, typename NumberComparisonPolicy, typename std::enable_if<is_polygon<PointSequence>::value, int>::type = 0>
        void mark_boundary(const PointSequence& pgon, const NumberComparisonPolicy& cmp)
        {
            typedef point_sequence_traits< PointSequence >::point_type point_type;
            typedef segment< point_type > segment_type;
			auto visitor = [this](std::uint32_t i, std::uint32_t j)
			{
				get_color(i, j) = BOUNDARY;
			};
            auto size = point_sequence_traits<PointSequence>::size(pgon);
            for(std::size_t i = 0, j = 1; i < size; ++i, ++j %= size)
            {
                segment_type segment(point_sequence_traits<PointSequence>::get_point(pgon, i), point_sequence_traits<PointSequence>::get_point(pgon, j));
				fast_voxel_grid_traversal(m_gridTraits, segment, visitor, cmp);
            }
        }

        template <typename Segment, typename NumberComparisonPolicy, typename std::enable_if<is_segment<Segment>::value, int>::type = 0>
        void mark_boundary(const Segment& segment, const NumberComparisonPolicy& cmp)
        {
            auto visitor = [this](std::uint32_t i, std::uint32_t j){ get_color(i, j) = BOUNDARY; }
            fast_voxel_grid_traversal(m_gridTraits, segment, visitor, cmp);
        }

        void reset_non_boundary_fills()
        {
            m_colorGrid.clear();
        }

    private:

        std::uint32_t& get_color(std::uint32_t i, std::uint32_t j)
        {
            auto pCell = m_colorGrid.find_cell(i,j);
            if(pCell)
                return *pCell;

            auto& cell = m_colorGrid.get_cell(i,j);
            cell = 0;
            return cell;
        }

        std::size_t m_lastColor;
        grid        m_colorGrid;
        grid_traits m_gridTraits;
        std::size_t m_height;
        std::size_t m_width;

    };

}//! namespace geometrix;

