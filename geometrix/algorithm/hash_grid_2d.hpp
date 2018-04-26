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
    template<typename Data, typename GridTraits, typename Alloc = std::allocator<std::pair<const std::pair<boost::uint32_t, boost::uint32_t>, Data>>>
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

		hash_grid_2d(const hash_grid_2d&) = delete;
		hash_grid_2d& operator=(const hash_grid_2d&) = delete;

#if !defined(BOOST_MSVC) || BOOST_MSVC >= 1900
		hash_grid_2d(hash_grid_2d&&) = default;
		hash_grid_2d& operator=(hash_grid_2d&&) = default;
#else
		hash_grid_2d(hash_grid_2d&& o) 
            : m_gridTraits(std::move(o.m_gridTraits))
            : m_grid(std::move(o.m_grid))
        {}

		hash_grid_2d& operator=(hash_grid_2d&& o) 
        {
            m_gridTraits = std::move(o.m_gridTraits);
            m_grid = std::move(o.m_grid);
        }
#endif
        
        template <typename Point>
        data_type const* find_cell(const Point& point) const
        {
			BOOST_CONCEPT_ASSERT( (Point2DConcept<Point>) );
			GEOMETRIX_ASSERT( is_contained( point ) );
            boost::uint32_t i = m_gridTraits.get_x_index(get<0>(point));
            boost::uint32_t j = m_gridTraits.get_y_index(get<1>(point));
            return get_cell(i,j);
        }
        
        data_type const* find_cell(boost::uint32_t i, boost::uint32_t j) const
        {            
			auto iter = m_grid.find( key_type( i, j ) );
			if( iter != m_grid.end() )
				return &iter->second;
			else
				return nullptr;		
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
            auto key = key_type(i, j);
			auto iter = m_grid.find(key);
			if( iter != m_grid.end() )
				return iter->second;
			else
				return m_grid.emplace_hint(iter, key, data_type())->second;
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
