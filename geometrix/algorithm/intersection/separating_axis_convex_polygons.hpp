//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEPARATING_AXIS_CONVEC_POLYGONS_HPP
#define GEOMETRIX_SEPARATING_AXIS_CONVEC_POLYGONS_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/arithmetic/vector/perp.hpp>
#include <geometrix/algebra/dot_product.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {
	
	namespace detail_convex_polygons_intersect {

		inline std::size_t get_middle_index( std::size_t i0, std::size_t i1, std::size_t n )
		{
			if( i0 < i1 )
				return (i0 + i1) / 2;
			else
				return ((i0 + i1 + n) / 2) % n;
		}

		template <typename Polygon, typename Point>
		inline std::size_t get_extreme_index( const Polygon& c, const Point& d )
		{
			typename point_sequence_traits<Polygon> access;
			typename typename access::point_type point_type;
			typename typename geometrix<point_type>::arithmetic_type arithmetic_type;
			typedef vector<arithmetic_type, 2> vector2;

			std::size_t i0 = 0;
			std::size_t i1 = 0;
			while( true )
			{
				std::size_t mid = get_middle_index( i0, i1 );
				std::size_t next = (mid + 1) % access::size( c );
				vector2 e = access::get_point( c, next ) - access::get_point( c, mid );
				if( dot_product( as_vector( d ), e ) > 0 )
				{
					if( mid != i0 )
						i0 = mid;
					else
						return i1;
				}
				else
				{
					std::size_t prev = (mid + access::size( c ) - 1) % access::size( c );
					vector2 e = access::get_point( c, mid ) - access::get_point( c, prev );
					if( dot_product( as_vector( d ), e ) < 0 )
						i1 = mid;
					else
						return mid;
				}
			}
		}
	}

	//! \brief Compute whether the segment defined by A->B intersects the triangle defined by t0, t1, t2.
	template <typename Polygon>
	inline bool convex_polygons_intersect( const Polygon& p0, const Polygon& p1 )
	{
		using namespace detail_convex_polygons_intersect;
		typename point_sequence_traits<Polygon> access;
		typename typename access::point_type point_type;
		typename typename geometrix<point_type>::arithmetic_type arithmetic_type;
		typedef vector<arithmetic_type, 2> vector2;

		for( std::size_t i0 = 0, i1 = access::size( p0 ) - 1; i0 < access::size( p0 ); i1 = i0, ++i0 )
		{
			vector2 d = left_normal<vector2>( access::get_point( p0, i0 ) - access::get_point( p0, i1 ) );
			std::size_t min = get_extreme_index( p1, -d );
			vector2 diff = access::get_point( p1, min ) - access::get_point( p0, i0 );
			if( dot_product( d, diff ) > 0 )
				return false;
		}

		for( std::size_t i0 = 0, i1 = access::size( p1 ) - 1; i0 < access::size( p1 ); i1 = i0, ++i0 )
		{
			vector2 d = left_normal<vector2>( access::get_point( p1, i0 ) - access::get_point( p1, i1 ) );
			std::size_t min = get_extreme_index( p0, -d );
			vector2 diff = access::get_point( p0, min ) - access::get_point( p1, i0 );
			if( dot_product( d, diff ) > 0 )
				return false;
		}

		return true;
	}

}//namespace geometrix;

#endif //GEOMETRIX_SEPARATING_AXIS_CONVEC_POLYGONS_HPP
