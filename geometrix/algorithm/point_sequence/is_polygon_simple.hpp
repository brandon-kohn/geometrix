//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_IS_POLYGON_SIMPLE_HPP
#define GEOMETRIX_IS_POLYGON_SIMPLE_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/utility/memoize.hpp>

namespace geometrix {

	namespace is_polygon_simple_detail{

		typedef std::pair<std::size_t, std::size_t> segment_key;

		struct intersection_key
		{
			intersection_key( const std::tuple<std::size_t, std::size_t, std::size_t, std::size_t >& key )
				: a( std::get<0>( key ), std::get<1>( key ) )
				, b( std::get<2>( key ), std::get<3>( key ) )
			{
				if( a < b )
					std::swap( a, b );
			}

			bool operator <(const intersection_key& rhs) const
			{
				return (a < rhs.a) || (a == rhs.a && b < rhs.b);
			}

			segment_key a, b;

		};

	}
	
	template <typename Polygon, typename NumberComparisonPolicy>
	inline bool is_polygon_simple_memoized( const Polygon& poly, const NumberComparisonPolicy& cmp )
	{
		using namespace is_polygon_simple_detail;
		typedef point_sequence_traits<Polygon> access;
		typedef typename access::point_type point_type;
		std::map<intersection_key, bool> cache;
		auto is_intersecting = memoize( cache, [&poly, &cmp]( std::size_t i, std::size_t j, std::size_t k, std::size_t l ) -> bool
		{
			auto iType = calculate_intersection( access::get_point( poly, i ), access::get_point( poly, j ), access::get_point( poly, k ), access::get_point( poly, l ), (point_type*)nullptr, cmp );
			return iType != e_non_crossing;
		} );
		std::size_t size = access::size( poly );
		auto next = [size]( std::size_t i ){ return (i + 1) % size; };
		for( std::size_t i = size - 1, j = 0; j < size; i = j++ ){
			for( std::size_t k = next( j ), l = next( k ); l != i; k = l, l = next( l ) ){
				if( is_intersecting(i,j,k,l) )
					return false;				
			}
		}

		return true;
	}

	template <typename Polygon, typename NumberComparisonPolicy>
	inline bool is_polygon_simple( const Polygon& poly, const NumberComparisonPolicy& cmp )
	{
		using namespace is_polygon_simple_detail;
		typedef point_sequence_traits<Polygon> access;
		typedef typename access::point_type point_type;
		std::size_t size = access::size( poly );
		BOOST_ASSERT( size > 2 );

		auto next = [size]( std::size_t i ){ return (i + 1) % size; };
		auto is_intersecting = [&poly, &next, &cmp]( std::size_t i, std::size_t j ) -> bool
		{
			auto iType = calculate_intersection( access::get_point( poly, i ), access::get_point( poly, next(i) ), access::get_point( poly, j ), access::get_point( poly, next(j) ), (point_type*)nullptr, cmp );
			return iType != e_non_crossing && (iType == e_crossing || iType == e_overlapping || (iType == e_endpoint && next( i ) != j && next( j ) != i));
		};
		
		for( std::size_t i = 0; i < size - 2; ++i )
			for( std::size_t j = i + 2; j < size; ++j )
				if( is_intersecting( i, j ) )
					return false;
			
		return true;
	}

}//namespace geometrix;

#endif //! GEOMETRIX_IS_POLYGON_SIMPLE_HPP
