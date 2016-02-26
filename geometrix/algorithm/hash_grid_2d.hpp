//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_HASH_GRID_2D_HPP
#define GEOMETRIX_HASH_GRID_2D_HPP
#pragma once

#include <geometrix/algorithm/grid_traits.hpp>
#include <boost/functional/hash.hpp>
#include <boost/cstdint.hpp>
#include <unordered_map>

namespace geometrix
{    
    template<typename Data, typename CoordinateType = double, typename GridTraits = grid_traits<CoordinateType>, typename Alloc = std::allocator<std::pair<const std::pair<boost::uint32_t, boost::uint32_t>, Data>>>
    class hash_grid_2d
    {
    public:

        typedef Data data_type;
        typedef GridTraits traits_type;
		typedef std::pair<boost::uint32_t, boost::uint32_t> key_type;
		typedef std::unordered_map<key_type, data_type, boost::hash<key_type>, std::equal_to<key_type>, Alloc> grid_type;

        hash_grid_2d( const GridTraits& traits )
            : m_gridTraits(traits)
            , m_grid()
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
			auto iter = m_grid.find( key_type( i, j ) );
			if( iter != m_grid.end() )
				return iter->second;
			else
				return m_grid.insert( iter, std::make_pair( key_type( i, j ), data_type() ) )->second;		
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
			auto iter = m_grid.find( key_type( i, j ) );
			if( iter != m_grid.end() )
				return iter->second;
			else
				return m_grid.insert( iter, std::make_pair( key_type( i, j ), data_type() ) )->second;
        }
        
        const traits_type& get_traits() const { return m_gridTraits; }

		template <typename Point>
		bool is_contained( const Point& p ) const
		{
            BOOST_CONCEPT_ASSERT( (Point2DConcept<Point>) );
			return m_gridTraits.is_contained( p );
		}

		void erase(boost::uint32_t i, boost::uint32_t j)
		{
			auto it = m_grid.find( key_type( i, j ) );
			if( it != m_grid.end() )
				m_grid.erase( it );
		}

		void clear(){ m_grid.clear(); }

    private:

		traits_type m_gridTraits;
        grid_type m_grid;

    };

}//! namespace geometrix

#endif //! GEOMETRIX_HASH_GRID_2D_HPP
