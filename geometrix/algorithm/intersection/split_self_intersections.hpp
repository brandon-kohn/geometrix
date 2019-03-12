//
//! Copyright © 2019
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algorithm/intersection/polygon_polygon_border_intersection.hpp>
#include <geometrix/algorithm/doubly_connected_edge_list.hpp>
#include <geometrix/algorithm/intersection/segment_polygon_border_intersection.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/algorithm/point_sequence/area.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/container/flat_map.hpp>

#include <bitset>

namespace geometrix{

	namespace detail {

		template <typename Point>
		class loop_graph 
		{
			using point_type = Point;

			enum class vertex_type : std::uint8_t
			{
			    node = 1
			  , intersection = 2
			  , start = 4
			};

			struct vertex_properties
			{	
				vertex_properties() = default;

				vertex_properties(const point_type& p, vertex_type type = vertex_type::node)
					: point(p)
					, flags(type)
				{}

				bool is_start() const
				{
					return flags & vertex_type::start;
				}

				void set_start()
				{
					flags |= vertex_type::start;
				}
				
				bool is_node() const
				{
					return flags & vertex_type::node;
				}
				
				void set_node()
				{
					flags |= vertex_type::node;
				}
				
				bool is_intersection() const
				{
					return flags & vertex_type::intersection;
				}
				
				void set_intersection()
				{
					flags |= vertex_type::intersection;
				}
				
				point_type     point;

				//! bits: <intersection, start, node>
				std::bitset<3> flags;
			};

			struct edge_properties
			{
				edge_properties(std::size_t marker = (std::numeric_limits<std::size_t>::max)())
					: marker(marker)
				{}

				std::size_t marker;
			};

			using graph = boost::adjacency_list
			<
			    boost::vecS
			  , boost::vecS
			  , boost::directedS
			  , vertex_properties
			  , edge_properties
			>;

			using vertex = typename graph::vertex_descriptor;
			using edge = typename graph::edge_descriptor;
			using point_vertex_map = boost::container::flat_map<point_type, vertex, geometrix::lexicographical_comparer<geometrix::absolute_tolerance_comparison_policy<units::length>>>;
		
		public:

			loop_graph() = default;

			vertex add_start_vertex(const point_type& p) 
			{
				auto it = m_pointVertexMap.lower_bound(p);
				if (it != m_pointVertexMap.end() && !m_pointVertexMap.key_comp()(p, *it))
				{
					auto v = it->second;
					m_graph[v].set_start();
					return v;
				}

				auto v = boost::add_vertex(m_graph, vertex_properties{ p, vertex_type::start });
				m_pointVertexMap.emplace(it, p, v);
				return v;
			}
			
			vertex add_intersection_vertex(const point_type& p) 
			{
				auto it = m_pointVertexMap.lower_bound(p);
				if (it != m_pointVertexMap.end() && !m_pointVertexMap.key_comp()(p, *it))
				{
					auto v = it->second;
					m_graph[v].set_intersection();
					return v;
				}

				auto v = boost::add_vertex(m_graph, vertex_properties{ p, vertex_type::intersection });
				m_pointVertexMap.emplace(it, p, v);
				return v;
			}
			
			vertex add_node_vertex(const point_type& p) 
			{
				auto it = m_pointVertexMap.lower_bound(p);
				if (it != m_pointVertexMap.end() && !m_pointVertexMap.key_comp()(p, *it))
				{
					auto v = it->second;
					m_graph[v].set_node();
					return v;
				}

				auto v = boost::add_vertex(m_graph, vertex_properties{ p, vertex_type::node });
				m_pointVertexMap.emplace(it, p, v);
				return v;
			}

			void add_edge()
			{

			}

		private:

			mutable graph            m_graph;
			mutable point_vertex_map m_pointVertexMap;

		};
	}

	template <typename Polygon, typename NumberComparisonPolicy>
	inline std::vector<polygon<typename geometric_traits<Polygon>::point_type>> split_self_intersections( const Polygon& poly, const NumberComparisonPolicy& cmp )
	{
		using geometrix::detail::loop_graph;
		typedef point_sequence_traits<Polygon> access;
		typedef typename access::point_type point_type;
		std::size_t size = access::size( poly );
		GEOMETRIX_ASSERT( size > 2 );

		std::vector<polygon<point_type>> results = { polygon<point_type>{ } };
		std::size_t iCurr = 0;

		auto next = [size]( std::size_t i ){ return (i + 1) % size; };
		auto adjacent = [&next](std::size_t i, std::size_t j) { return next(i) == j || next(j) == i; };
		auto process = [&]( std::size_t i, std::size_t j ) -> void
		{
			point_type x[2];
			auto iType = segment_segment_intersection(access::get_point(poly, i), access::get_point(poly, next(i)), access::get_point(poly, j), access::get_point(poly, next(j)), x, cmp);
			if (iType == e_non_crossing)
			{
				return;
			}
		
			if(iType == e_endpoint)
			{
				if (adjacent(i, j)) 
				{
					return;
				}
			}

			if (iType == e_crossing)
			{
				results[iCurr++].push_back(x[0]);
				results.emplace_back(1, x[0]);
			}

			if(iType == e_overlapping)
			{
			
			}
		};

	
		
		for (std::size_t i = 0; i < size - 2; ++i) 
		{
			results[iCurr].push_back(access::get_point(poly, i));
			for (std::size_t j = i + 2; j < size; ++j)
			{
				process(i, j);
			}
		}

		return true;
	}

}//! namespace geometrix;

