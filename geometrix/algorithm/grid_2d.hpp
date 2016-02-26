//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_GRID_2D_HPP
#define GEOMETRIX_GRID_2D_HPP
#pragma once

#include <geometrix/algorithm/grid_traits.hpp>

#include <boost/multi_array.hpp>
#include <boost/functional/hash.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

namespace geometrix
{    
    template<typename Data, typename CoordinateType = double, typename GridTraits = grid_traits<CoordinateType> >
    class grid_2d
    {
    public:

        typedef Data data_type;
        typedef GridTraits traits_type;
        typedef boost::multi_array<data_type, 2> grid_type;

        grid_2d( const GridTraits& traits )
            : m_gridTraits(traits)
            , m_grid(boost::extents[traits.get_width()][traits.get_height()])
        {}
        
        template <typename Point>
        data_type const& get_cell(const Point& point) const
        {
			BOOST_CONCEPT_ASSERT( (Point2DConcept<Point>) );
			GEOMETRIX_ASSERT( is_contained( point ) );
            boost::uint32_t i = m_gridTraits.get_x_index(get<0>(point));
            boost::uint32_t j = m_gridTraits.get_y_index(get<1>(point));
            return get_cell(i,j);
        }
        
        data_type const& get_cell(boost::uint32_t i, boost::uint32_t j) const
        {            
            return m_grid[i][j];
        }
        
        template <typename Point>
        data_type& get_cell(const Point& point)
        {
			BOOST_CONCEPT_ASSERT(( Point2DConcept<Point> ));
			GEOMETRIX_ASSERT( is_contained( point ) );
            boost::uint32_t i = m_gridTraits.get_x_index(get<0>(point));
            boost::uint32_t j = m_gridTraits.get_y_index(get<1>(point));
            return get_cell(i,j);
        }
        
        data_type& get_cell(boost::uint32_t i, boost::uint32_t j)
        {            
            return m_grid[i][j];
        }
        
        const traits_type& get_traits() const { return m_gridTraits; }

		template <typename Point>
		bool is_contained( const Point& p ) const
		{
            BOOST_CONCEPT_ASSERT( (Point2DConcept<Point>) );
			return m_gridTraits.is_contained( p );
		}

    private:

		traits_type m_gridTraits;
        grid_type m_grid;

    };

}//! namespace geometrix

#endif // GEOMETRIX_GRID_2D_HPP
