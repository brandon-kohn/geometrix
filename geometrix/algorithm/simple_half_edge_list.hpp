//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
#define GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/point_angle_compare.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/edge_list.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <map>
#include <set>
#include <vector>

namespace geometrix {
	template <typename Point, typename NumberComparisonPolicy>
	class simple_half_edge_list
	{
	public:
		typedef Point                                                  point_type;
		typedef polyline<point_type>                                   polyline_type;
		typedef polygon<point_type>                                    polygon_type;
		typedef std::vector<polyline_type>                             polyline_collection;
		typedef std::vector<polygon_type>                              polygon_collection;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;

		//! Bundled vertex properties.
		struct vertex_properties
		{
			point_type position;
		};

		//! Bundled edge properties.
		struct edge_properties
		{
			arithmetic_type weight{};
			std::size_t     index{};
		};

		//! Half-edge style graph: directed edges, vertex/edge bundles.
		typedef boost::adjacency_list<
			boost::vecS,           // edge list
			boost::vecS,           // vertex list
			boost::bidirectionalS, // directed graph
			vertex_properties,     // vertex bundle (stores point)
			edge_properties        // edge bundle (weight + index)
			>
			half_edge_list;

		//! Simple undirected graph for connected components.
		typedef boost::adjacency_list<
			boost::vecS,
			boost::vecS,
			boost::undirectedS>
			component_graph;

		typedef typename boost::graph_traits<half_edge_list>::vertex_descriptor vertex_descriptor;
		typedef typename boost::graph_traits<half_edge_list>::edge_descriptor   edge_descriptor;

		typedef std::map<
			point_type,
			vertex_descriptor,
			lexicographical_comparer<NumberComparisonPolicy>>
			point_vertex_map;

		simple_half_edge_list()
		{}

		simple_half_edge_list( const NumberComparisonPolicy& compare )
			: m_pointVertexMap( compare )
			, m_compare( compare )
		{}

		simple_half_edge_list( const simple_half_edge_list& other )
			: m_pointVertexMap( other.m_pointVertexMap )
			, m_halfEdgeList( other.m_halfEdgeList )
			, m_componentGraph( other.m_componentGraph )
			, m_polygons( other.m_polygons )
			, m_polylines( other.m_polylines )
			, m_compare( other.m_compare )
		{}

		simple_half_edge_list( simple_half_edge_list&& other )
			: m_pointVertexMap( std::move( other.m_pointVertexMap ) )
			, m_halfEdgeList( std::move( other.m_halfEdgeList ) )
			, m_componentGraph( std::move( other.m_componentGraph ) )
			, m_polygons( std::move( other.m_polygons ) )
			, m_polylines( std::move( other.m_polylines ) )
			, m_compare( other.m_compare )
		{}

		template <typename Segments>
		simple_half_edge_list( const Segments& segs, const NumberComparisonPolicy& compare )
			: m_pointVertexMap( compare )
			, m_compare( compare )
		{
			for( const auto& seg : segs )
				add_edge( seg );

			calculate_point_sequences();
		}

		//! Add an edge from a segment.
		template <typename Segment>
		void add_edge( const Segment& edge )
		{
			add_edge( get_start( edge ), get_end( edge ) );
		}

		//! Add an edge from two points.
		void add_edge( const point_type& source, const point_type& target )
		{
			vertex_descriptor s = add_vertex( source );
			vertex_descriptor t = add_vertex( target );

			// Undirected connectivity for components.
			boost::add_edge( s, t, m_componentGraph );

			// Directed half-edge list (single direction as in original code).
			edge_descriptor e;
			bool            inserted;
			boost::tie( e, inserted ) = boost::add_edge( s, t, m_halfEdgeList );
			if( inserted )
			{
				m_halfEdgeList[e].weight = point_point_distance( source, target );
				// Index is optional; keep it for possible later use.
				m_halfEdgeList[e].index = m_nextEdgeIndex++;
			}
		}

		const polygon_collection& get_polygons() const
		{
			return m_polygons;
		}

		const polyline_collection& get_polylines() const
		{
			return m_polylines;
		}

		//! Recompute polygons and polylines from the current graph.
		void calculate_point_sequences()
		{
			m_polylines.clear();
			m_polygons.clear();

			std::size_t nVertices = num_vertices( m_halfEdgeList );
			if( nVertices == 0 )
				return;

			// Connected components on the undirected graph.
			std::vector<std::size_t>                 component( nVertices );
			std::size_t                              num = connected_components( m_componentGraph, &component[0] );
			std::vector<std::set<vertex_descriptor>> components( num );

			for( std::size_t i = 0; i != component.size(); ++i )
			{
				std::size_t vi = component[i];
				components[vi].insert( static_cast<vertex_descriptor>( i ) );
			}

			// Walk each connected component: either a polyline (has start)
			// or a cycle (polygon).
			for( std::set<vertex_descriptor> comp : components )
			{
				auto polylineStart = find_start( comp );
				if( polylineStart )
				{
					// --- Build polyline ---
					vertex_descriptor t = *polylineStart;
					polyline_type     polyline{ m_halfEdgeList[t].position };
					comp.erase( t );

					while( !comp.empty() )
					{
						std::size_t outDegree = boost::out_degree( t, m_halfEdgeList );
						//GEOMETRIX_ASSERT( outDegree < 2 );
						if( outDegree > 0 )
						{
							typename boost::graph_traits<half_edge_list>::out_edge_iterator
								oei,
								oei_end;
							boost::tie( oei, oei_end ) = boost::out_edges( t, m_halfEdgeList );
							t = boost::target( *oei, m_halfEdgeList );

							auto it = comp.find( t );
							if( it != comp.end() )
							{
								polyline.push_back( m_halfEdgeList[t].position );
								comp.erase( it );
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}

					if( polyline.size() > 1 )
						m_polylines.push_back( std::move( polyline ) );
				}
				else
				{
					// --- Build polygon (simple cycle) ---
					vertex_descriptor t = *comp.begin();
					polygon_type      polygon{ m_halfEdgeList[t].position };
					comp.erase( comp.begin() );

					while( !comp.empty() )
					{
						typename boost::graph_traits<half_edge_list>::out_edge_iterator
							oei,
							oei_end;
						std::size_t outDegree = boost::out_degree( t, m_halfEdgeList );
						//GEOMETRIX_ASSERT( outDegree < 2 );
						if( outDegree > 0 )
						{
							boost::tie( oei, oei_end ) = boost::out_edges( t, m_halfEdgeList );
							t = boost::target( *oei, m_halfEdgeList );

							auto it = comp.find( t );
							if( it != comp.end() )
							{
								polygon.push_back( m_halfEdgeList[t].position );
								comp.erase( it );
							}
							else
							{
								break;
							}
						}
						else
						{
							break;
						}
					}

					if( polygon.size()  > 2 )
						m_polygons.push_back( std::move( polygon ) );
				}
			}
		}

		simple_half_edge_list& operator=( simple_half_edge_list&& other )
		{
			m_pointVertexMap = std::move( other.m_pointVertexMap );
			m_halfEdgeList = std::move( other.m_halfEdgeList );
			m_componentGraph = std::move( other.m_componentGraph );
			m_polygons = std::move( other.m_polygons );
			m_polylines = std::move( other.m_polylines );
			m_compare = other.m_compare;
			m_nextEdgeIndex = other.m_nextEdgeIndex;
			return *this;
		}

		simple_half_edge_list& operator=( const simple_half_edge_list& other )
		{
			m_pointVertexMap = other.m_pointVertexMap;
			m_halfEdgeList = other.m_halfEdgeList;
			m_componentGraph = other.m_componentGraph;
			m_polygons = other.m_polygons;
			m_polylines = other.m_polylines;
			m_compare = other.m_compare;
			m_nextEdgeIndex = other.m_nextEdgeIndex;
			return *this;
		}

	private:
		//! Find a start vertex for a polyline in this component, if any.
		//! A start is a vertex with in-degree == 0 in the directed half-edge list.
		boost::optional<vertex_descriptor> find_start( const std::set<vertex_descriptor>& component ) const
		{
			for( vertex_descriptor v : component )
			{
				if( in_degree( v, m_halfEdgeList ) == 0 )
					return v;
			}

			return boost::none;
		}

		//! Lookup a vertex or add it if not present.
		vertex_descriptor add_vertex( const point_type& p )
		{
			typename point_vertex_map::iterator pvIter( m_pointVertexMap.lower_bound( p ) );
			vertex_descriptor                   v;
			if( pvIter != m_pointVertexMap.end() && !( m_pointVertexMap.key_comp()( p, pvIter->first ) ) )
			{
				v = pvIter->second;
			}
			else
			{
				v = boost::add_vertex( m_halfEdgeList );
				BOOST_VERIFY( v == boost::add_vertex( m_componentGraph ) );
				m_pointVertexMap.insert( pvIter, std::make_pair( p, v ) );
				m_halfEdgeList[v].position = p;
			}

			return v;
		}

		point_vertex_map       m_pointVertexMap;
		half_edge_list         m_halfEdgeList;
		component_graph        m_componentGraph;
		polygon_collection     m_polygons;
		polyline_collection    m_polylines;
		NumberComparisonPolicy m_compare;
		std::size_t            m_nextEdgeIndex{ 0 };
	};

	template <typename Point, typename Segments, typename NumberComparisonPolicy>
	inline simple_half_edge_list<Point, NumberComparisonPolicy>
	make_shel( const Segments& segs, const NumberComparisonPolicy& cmp )
	{
		return simple_half_edge_list<Point, NumberComparisonPolicy>( segs, cmp );
	}

} // namespace geometrix

#endif // GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
