//
//! Copyright © 2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_VISIBLE_VERTICES_MESH_SEARCH_HPP
#define GEOMETRIX_VISIBLE_VERTICES_MESH_SEARCH_HPP
#pragma once

#include <geometrix/algorithm/mesh_2d.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algorithm/is_segment_in_range.hpp>
#include <geometrix/numeric/constants.hpp>

#include <geometrix/test/test.hpp>

#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/container/flat_set.hpp>

#define GEOMETRIX_DEBUG_VISIBLE_VERTICES_MESH_SEARCH 0

namespace geometrix
{
	template <typename CoordinateType, typename MeshType, typename NumberComparisonPolicy>
	struct visible_vertices_mesh_search_traits
	{
		using coordinate_type = CoordinateType;
		using comparison_policy = NumberComparisonPolicy;
		using mesh_t = MeshType;
	};

	template <typename Traits>
	struct visible_vertices_mesh_search
	{
		using traits = Traits;
		using coordinate_type = typename traits::coordinate_type;
		using mesh_t = typename traits::mesh_t;
		using point_t = point<coordinate_type, 2>;
		using vector_t = vector<coordinate_type, 2>;
		using comparison_policy = typename traits::comparison_policy;

		visible_vertices_mesh_search(const point_t& origin, std::size_t start, const mesh_t& mesh)
			: m_origin(origin)
			, m_mesh(mesh)
			, m_start(start)
		{}

		virtual ~visible_vertices_mesh_search()
		{}

		struct mesh_search_item
		{ 
			mesh_search_item( std::size_t from, std::size_t to, const vector_t& lo, const vector_t& hi ) 
				: from(from)
				, to(to)
				, lo( lo )
				, hi( hi )
			{}

			//! Visitor interface.
			std::size_t get_triangle_index() const { return to; }
			
			std::size_t from;
			std::size_t to;
			vector_t lo; 
			vector_t hi;
		};
		typedef mesh_search_item edge_item;

		edge_item get_start()
		{
			return edge_item( (std::numeric_limits<std::size_t>::max)(), m_start, vector_t(constants::infinity<coordinate_type>(), constants::zero<coordinate_type>() ), vector<coordinate_type,2>(constants::negative_infinity<coordinate_type>(), constants::zero<coordinate_type>() ) );
		}

		//! Visit the item and return true/false if the search should continue.
		bool visit( const edge_item& item )
		{			
			bool allAround = get<0>( item.lo ) == constants::infinity<coordinate_type>() && get<0>( item.hi ) == constants::negative_infinity<coordinate_type>();
						
			const auto& toIndices = m_mesh.get_triangle_indices( item.to );
			if( allAround )
			{
				m_vertices.push_back( toIndices[0] );
				m_vertices.push_back( toIndices[1] );
				m_vertices.push_back( toIndices[2] );
			}
			else
			{
				const auto& fromIndices = m_mesh.get_triangle_indices( item.from );
				comparison_policy cmp( 0 );
				for( std::size_t i = 0; i < 3; ++i )
				{
					if( fromIndices[0] == toIndices[i] || fromIndices[1] == toIndices[i] || fromIndices[2] == toIndices[i] )
						continue;

					const auto& point = m_mesh.get_triangle_vertices( item.to )[i];
					vector_t vPoint = point - m_origin;
					if( is_vector_between( item.lo, item.hi, vPoint, true, cmp ) )
						m_vertices.push_back( toIndices[i] );
				}
			}
			
			return true;
		}

		//! Generate a new item to visit based on the adjacent triangle at index next.
		boost::optional<edge_item> prepare_adjacent_traversal( std::size_t next, const edge_item& item )
		{
			comparison_policy cmp( 0 );
			bool allAround = get<0>( item.lo ) == constants::infinity<coordinate_type>() && get<0>( item.hi ) == constants::negative_infinity<coordinate_type>();

			const auto& fromIndices = m_mesh.get_triangle_indices( item.to );
			const auto& toIndices = m_mesh.get_triangle_indices( next );
						
			std::size_t side = get_triangle_adjacent_side( fromIndices, toIndices );
			auto pointLo = m_mesh.get_triangle_vertices( item.to )[side];
			auto pointHi = m_mesh.get_triangle_vertices( item.to )[(side + 1) % 3];

			if( exterior_product_area( pointHi - pointLo, m_origin - pointLo ) < constants::zero<decltype(std::declval<coordinate_type>() * std::declval<coordinate_type>())>() )
				std::swap( pointLo, pointHi );

			if (!is_segment_in_range_2d(make_segment(pointLo, pointHi), item.lo, item.hi, m_origin)) 
				return boost::none;

#if GEOMETRIX_TEST_ENABLED(GEOMETRIX_DEBUG_VISIBLE_VERTICES_MESH_SEARCH)
			//polygon2 pTri(mMesh.get_triangle_vertices(item.from), mMesh.get_triangle_vertices(item.from) + 3);
			typedef std::vector<point_t> polygon2;
			typedef segment<point_t> segment2;
			polygon2 cTri(m_mesh.get_triangle_vertices(item.to), m_mesh.get_triangle_vertices(item.to) + 3);
			polygon2 nTri(m_mesh.get_triangle_vertices(next), m_mesh.get_triangle_vertices(next) + 3);
			segment2 sLo{ m_origin, m_origin + item.lo };
			segment2 sHi{ m_origin, m_origin + item.hi };
			segment2 cLo{ m_origin, pointLo };
			segment2 cHi{ m_origin, pointHi };
#endif

			vector_t vecLo, vecHi;
			if( !numeric_sequence_equals_2d(m_origin, pointLo, cmp) && !numeric_sequence_equals_2d(m_origin, pointHi, cmp) )
			{
				assign( vecLo, pointLo - m_origin );
				assign( vecHi, pointHi - m_origin );
				
				if (!allAround)
				{
					vecLo = is_vector_between(item.lo, item.hi, vecLo, false, cmp) ? vecLo : item.lo;
					vecHi = is_vector_between(item.lo, item.hi, vecHi, false, cmp) ? vecHi : item.hi;
				}

				if (get_orientation(vecHi, vecLo, cmp) == geometrix::oriented_left)
					return boost::none;
			}
			else
			{
				assign( vecLo, constants::infinity<coordinate_type>(), constants::zero<coordinate_type>() );
				assign( vecHi, constants::negative_infinity<coordinate_type>(), constants::zero<coordinate_type>() );
			}
			
#if GEOMETRIX_TEST_ENABLED(GEOMETRIX_DEBUG_VISIBLE_VERTICES_MESH_SEARCH)
			segment2 nLo{ m_origin, m_origin + vecLo };
			segment2 nHi{ m_origin, m_origin + vecHi };
#endif			
			return edge_item( item.to, next, vecLo, vecHi );
		}

		const std::vector<std::size_t>& get_vertices() const { return m_vertices; }

	private:

		point_t m_origin;
		const mesh_t& m_mesh;
		std::size_t m_start;
		std::vector<std::size_t> m_vertices;
		comparison_policy m_cmp;
	};
	
}//! namespace geometrix

#endif // GEOMETRIX_VISIBLE_VERTICES_MESH_SEARCH_HPP
