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
#include <geometrix/primitive/polygon_with_holes.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/utility/assert.hpp>
#include <boost/scope_exit.hpp>
#include <boost/container/flat_set.hpp>
#include <limits>
#include <unordered_map>
#include <stack>

namespace geometrix
{
    //! \class floodfill_grid_traversal
    //! This class encapsulates a color grid which can be used to perform
    //! flood-fill traversals on convex polygons. 
    template <typename GridTraits>
    class floodfill_grid_traversal_helper
    {
        using grid_traits = GridTraits;
        using grid = hash_grid_2d<std::uint32_t, grid_traits>;
		using cell_key = std::pair<std::uint32_t, std::uint32_t>;
		using bound_map = std::unordered_map<std::uint32_t, cell_key>;
		using cell_key_set = boost::container::flat_set<cell_key>;
		using hole_bound_map = std::unordered_map<std::uint32_t, cell_key_set>;

    public:

		static const std::uint32_t BOUNDARY = static_cast<std::uint32_t>(-1);

        floodfill_grid_traversal_helper(const grid_traits& gridTraits)
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
                    
                    visit_cell(x, y1, std::forward<Visitor>(v));
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
        
        template <typename Polygon, typename Visitor, typename NumberComparisonPolicy>
        void mark_boundary(const Polygon& pgon, Visitor&& v, const NumberComparisonPolicy& cmp)
        {
            typedef point_sequence_traits<Polygon>::point_type point_type;
            typedef segment< point_type > segment_type;
			auto visitor = [&, this](std::uint32_t i, std::uint32_t j)
			{
				get_color(i, j) = BOUNDARY;
                update_bound(i,j);
                visit_cell(i, j, v);
			};
            auto size = point_sequence_traits<Polygon>::size(pgon);
            for(std::size_t i = 0, j = 1; i < size; ++i, ++j %= size)
            {
                segment_type segment(point_sequence_traits<Polygon>::get_point(pgon, i), point_sequence_traits<Polygon>::get_point(pgon, j));
				fast_voxel_grid_traversal(m_gridTraits, segment, visitor, cmp);
            }
        }
        
        template <typename Polygon, typename Visitor, typename NumberComparisonPolicy>
        void mark_hole_boundary(const Polygon& pgon, Visitor&& v, const NumberComparisonPolicy& cmp)
        {
            typedef point_sequence_traits<Polygon>::point_type point_type;
            typedef segment<point_type> segment_type;
			auto holeBoundMap = std::map<std::uint32_t, cell_key>();
			auto visitor = [&, this](std::uint32_t i, std::uint32_t j)
			{
				get_color(i, j) = BOUNDARY;
				auto it = holeBoundMap.lower_bound(i);
				if (it != std::end(holeBoundMap) && !(holeBoundMap.key_comp()(i, it->first)))
					it->second = std::make_pair((std::min)(it->second.first, j), (std::max)(it->second.second, j));
				else
					holeBoundMap.insert(it, std::make_pair(i, std::make_pair(j, j)));
                visit_cell(i, j, v);
			};
            auto size = point_sequence_traits<Polygon>::size(pgon);
            for(std::size_t i = 0, j = 1; i < size; ++i, ++j %= size)
            {
                segment_type segment(point_sequence_traits<Polygon>::get_point(pgon, i), point_sequence_traits<Polygon>::get_point(pgon, j));
				fast_voxel_grid_traversal(m_gridTraits, segment, visitor, cmp);
            }
			for (auto item : holeBoundMap)
				m_holeBoundMap[item.first].insert(item.second);
        }

        std::uint32_t& get_color(std::uint32_t i, std::uint32_t j)
        {
            auto pCell = m_colorGrid.find_cell(i,j);
            if(pCell)
                return *pCell;

            auto& cell = m_colorGrid.get_cell(i,j);
            cell = 0;
            return cell;
        }
        
		std::uint32_t get_color(std::uint32_t i, std::uint32_t j) const
        {
            auto pCell = m_colorGrid.find_cell(i,j);
            if(pCell)
                return *pCell;
            return 0;
        }

        void update_bound(std::uint32_t i, std::uint32_t j)
        {
            auto it = m_boundMap.find(i);
            if(it != m_boundMap.end())
                it->second = std::make_pair((std::min)(it->second.first, j), (std::max)(it->second.second, j));
            else
                m_boundMap.insert(it, std::make_pair(i, std::make_pair(j, j)));
        }

        boost::optional<cell_key> get_empty_interior() const
        {
            for(const auto& item : m_boundMap)
            {
                std::uint32_t i = item.first, jmin = item.second.first, jmax = item.second.second;
				auto hit = m_holeBoundMap.find(i);
				if (hit == m_holeBoundMap.end())
				{
					//! Sweep j and find first empty.
					for (auto k = jmin + 1; k < jmax; ++k)
						if (get_color(i, k) != BOUNDARY)
							return cell_key(i, k);
				}
				else
				{
					const auto& holeBounds = hit->second;
					GEOMETRIX_ASSERT(!holeBounds.empty());
					auto bit = holeBounds.begin();
					//! Sweep j and find first empty.
					for (auto k = jmin + 1; k < jmax; ) 
					{
						if (bit != holeBounds.end() && k >= bit->first && k <= bit->second)
						{
							k = bit->second + 1;
							++bit;
							continue;
						}
						if (get_color(i, k) != BOUNDARY)
							return cell_key(i, k);
						++k;
					}
				}
            }

            return boost::none;
        }

		template <typename Visitor>
        void scan(Visitor&& v)
        {
            for(const auto& item : m_boundMap)
            {
                std::uint32_t i = item.first, jmin = item.second.first, jmax = item.second.second;
				auto hit = m_holeBoundMap.find(i);
				if (hit == m_holeBoundMap.end())
				{
					//! Sweep j and find first empty.
					for (auto k = jmin + 1; k < jmax; ++k)
						if (get_color(i, k) != BOUNDARY)
							visit_cell(i, k, std::forward<Visitor>(v));
				}
				else
				{
					const auto& holeBounds = hit->second;
					GEOMETRIX_ASSERT(!holeBounds.empty());
					auto bit = holeBounds.begin();
					//! Sweep j and find first empty.
					for (auto k = jmin + 1; k < jmax; ) 
					{
						if (bit != holeBounds.end() && k >= bit->first && k <= bit->second)
						{
							k = bit->second + 1;
							++bit;
							continue;
						}
						if (get_color(i, k) != BOUNDARY)
							visit_cell(i, k, std::forward<Visitor>(v));
						++k;
					}
				}
            }
        }

		template <typename Visitor>
		void visit_cell(std::uint32_t i, std::uint32_t j, Visitor&& v)
		{
			auto key = cell_key{ i,j };
			auto it = m_visited.find(key);
			if (it == m_visited.end())
			{
				m_visited.insert(it, key);
				v(i, j);
			}
		}

        std::uint32_t      m_lastColor;
        grid               m_colorGrid;
        bound_map          m_boundMap;
		hole_bound_map     m_holeBoundMap;
        grid_traits        m_gridTraits;
        std::size_t        m_height;
        std::size_t        m_width;
		std::set<cell_key> m_visited;

    };

	//! NOTE: Incomplete.. works only with convex polygons.
    template <typename GridTraits, typename Polygon, typename Visitor, typename NumberComparisonPolicy, typename std::enable_if<is_polygon<Polygon>::value, int>::type = 0>
    inline void floodfill_grid_traversal(const GridTraits& grid, const Polygon& pgon, Visitor&& v, const NumberComparisonPolicy& cmp)
    {
       auto ff = floodfill_grid_traversal_helper<GridTraits>(grid);
       ff.mark_boundary(pgon, std::forward<Visitor>(v), cmp);
	   ff.scan(std::forward<Visitor>(v));
       //auto start = ff.get_empty_interior();
       //if(start)
       //    ff.traversal(start->first, start->second, std::forward<Visitor>(v));
    }

	//! NOTE: Incomplete.. works only with convex polygons with convex holes.
    template <typename GridTraits, typename Point, typename Visitor, typename NumberComparisonPolicy>
    inline void floodfill_grid_traversal(const GridTraits& grid, const polygon_with_holes<Point>& pgon, Visitor&& v, const NumberComparisonPolicy& cmp)
    {
       auto ff = floodfill_grid_traversal_helper<GridTraits>(grid);
       ff.mark_boundary(pgon.get_outer(), std::forward<Visitor>(v), cmp);
	   for (const auto& h : pgon.get_holes())
		   ff.mark_hole_boundary(h, std::forward<Visitor>(v), cmp);
	   ff.scan(std::forward<Visitor>(v));
       //auto start = ff.get_empty_interior();
       //if(start)
       //    ff.traversal(start->first, start->second, std::forward<Visitor>(v));
    }
}//! namespace geometrix;

