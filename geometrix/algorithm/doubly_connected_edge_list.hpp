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
#include <boost/dynamic_bitset.hpp>

//! \internal Install graph properties into the boost namespace.
namespace boost
{
    enum vertex_position_t 
    { 
        vertex_position = 1313//TODO: What's a good number?
    }; 
    BOOST_INSTALL_PROPERTY(vertex, position);
}

namespace geometrix
{     
    template <typename Point, typename NumberComparisonPolicy>
    class doubly_connected_edge_list
    {
    public:

        typedef Point point_type;        
		typedef polyline<point_type> polyline_type;
		typedef polygon<point_type> polygon_type;
		typedef std::vector<polyline_type> polyline_collection;
		typedef std::vector<polygon_type> polygon_collection;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;

        typedef boost::property<boost::vertex_position_t, point_type> vertex_properties;        
        typedef boost::property<boost::edge_weight_t, arithmetic_type, boost::property< boost::edge_index_t, std::size_t >> edge_properties;
    
        typedef boost::adjacency_list
        <   boost::vecS,           // edges list 
            boost::vecS,           // vertex list
            boost::bidirectionalS, // directed graph         
            vertex_properties,     // vertex properties - point 
            edge_properties        // edge properties, weight and segment edge
        > half_edge_list; 

        typedef typename boost::graph_traits< half_edge_list >::vertex_descriptor vertex_descriptor;
        typedef typename boost::graph_traits< half_edge_list >::edge_descriptor   edge_descriptor;
        
        typedef std::map< point_type, vertex_descriptor, lexicographical_comparer< NumberComparisonPolicy > > point_vertex_map;
        
        doubly_connected_edge_list()
        {}

        doubly_connected_edge_list( const NumberComparisonPolicy& compare )
            : m_pointVertexMap( compare )			
            , m_compare( compare )
        {}

		doubly_connected_edge_list(const doubly_connected_edge_list& other)
			: m_pointVertexMap(other.m_pointVertexMap)
			, m_graph(other.m_graph)
			, m_polygons(other.m_polygons)
			, m_polylines(other.m_polylines)
			, m_compare(other.m_compare)
		{}

		doubly_connected_edge_list(doubly_connected_edge_list&& other)
			: m_pointVertexMap(std::move(other.m_pointVertexMap))
			, m_graph(std::move(other.m_graph))
			, m_polygons(std::move(other.m_polygons))
			, m_polylines(std::move(other.m_polylines))
			, m_compare( other.m_compare)
		{}

		template <typename Segments>
		doubly_connected_edge_list(const Segments& segs, const NumberComparisonPolicy& compare)
			: m_pointVertexMap(compare)
			, m_compare(compare)
		{
			for (const auto& seg : segs)
				add_edge(seg);
			calculate_point_sequences();
		}

        template <typename Segment>
        void add_edge( const Segment& edge )
        {
            add_edge( get_start( edge ), get_end( edge ) );
        }

        void add_edge( const point_type& source, const point_type& target )
        {
            vertex_descriptor s = add_vertex( source );
            vertex_descriptor t = add_vertex( target );

			edge_descriptor e;
            bool inserted;
            boost::tie( e, inserted ) = boost::add_edge( s, t, m_graph );
            if( inserted )
				boost::put( boost::edge_weight, m_graph, e, point_point_distance( source, target ) );            
        }

        const polygon_collection& get_polygons() const
        {
            return m_polygons;
        }
		
		const polyline_collection& get_polylines() const
		{
			return m_polylines;
		}

		void calculate_point_sequences()
		{
			m_polylines.clear();
			m_polygons.clear();

			std::size_t nVertices = num_vertices(m_graph);
			if (nVertices == 0)
				return;

			std::vector<std::size_t> component(nVertices);
			int num = connected_components(m_graph, &component[0]);
			std::vector<std::set<vertex_descriptor>> components(num);
			for (std::size_t i = 0; i != component.size(); ++i)
			{
				std::size_t vi = component[i];
				components[vi].insert(i);
			}

			std::size_t edge_count = 0;
			typename boost::graph_traits<half_edge_list>::edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei)
			{
				edge_descriptor e = *ei;
				vertex_descriptor s = boost::source(e, m_graph);
				vertex_descriptor t = boost::target(e, m_graph);
				boost::put(boost::edge_index, m_graph, e, edge_count++);
			}
			boost::dynamic_bitset<> visitedEdges(edge_count);

			for (std::set<vertex_descriptor> comp : components)
			{
				auto polylineStart = find_start(comp);
				if (polylineStart)
				{
					vertex_descriptor t = *polylineStart;
					polyline_type polyline{boost::get(boost::vertex_position, m_graph, t)};
					comp.erase(t);

					while (!comp.empty())
					{						
						std::size_t outDegree = boost::out_degree(t, m_graph);
						GEOMETRIX_ASSERT(outDegree < 2);
						if (outDegree > 0)
						{
							typename boost::graph_traits< half_edge_list >::out_edge_iterator oei, oei_end;
							boost::tie(oei, oei_end) = boost::out_edges(t, m_graph);
							t = boost::target(*oei, m_graph);
							auto it = comp.find(t);
							if (it != comp.end())
							{
								polyline.push_back(boost::get(boost::vertex_position, m_graph, t));
								comp.erase(it);
							}
						}
						else
							break;
					}

					m_polylines.push_back(std::move(polyline));
				}
				else
				{
					vertex_descriptor t = *comp.begin();
					polygon_type polygon{ boost::get(boost::vertex_position, m_graph, t) };
					comp.erase(comp.begin());

					while (!comp.empty())
					{
						typename boost::graph_traits< half_edge_list >::out_edge_iterator oei, oei_end;						
						std::size_t outDegree = boost::out_degree(t, m_graph);
						GEOMETRIX_ASSERT(outDegree < 2);
						if (outDegree > 0)
						{
							boost::tie(oei, oei_end) = boost::out_edges(t, m_graph);
							t = boost::target(*oei, m_graph);
							auto it = comp.find(t);
							if (it != comp.end())
							{
								polygon.push_back(boost::get(boost::vertex_position, m_graph, t));
								comp.erase(it);
							}
						}
						else
							break;
					}

					m_polygons.push_back(std::move(polygon));
				}
			}
		}

		doubly_connected_edge_list& operator = (doubly_connected_edge_list&& other)
		{
			m_pointVertexMap = std::move(other.m_pointVertexMap);
			m_graph = std::move(other.m_graph);
			m_polygons = std::move(other.m_polygons);
			m_polylines = std::move(other.m_polylines);
			m_compare = other.m_compare;
			return *this;
		}

		doubly_connected_edge_list& operator = (const doubly_connected_edge_list& other)
		{
			m_pointVertexMap = other.m_pointVertexMap;
			m_graph = other.m_graph;
			m_polygons = other.m_polygons;
			m_polylines = other.m_polylines;
			m_compare = other.m_compare;
			return *this;
		}

    private:

		boost::optional<vertex_descriptor> find_start(const std::set<vertex_descriptor>& component)
		{
			//! Check if all vertices have both in and out edges.
			for (vertex_descriptor v : component)
			{
				if (in_degree(v, m_graph) == 0)
					return v;
			}

			return boost::none;
		}

        //! Method to lookup a vertex or add it if not already in place.
        vertex_descriptor add_vertex( const point_type& p )
        {
            typename point_vertex_map::iterator pvIter( m_pointVertexMap.lower_bound( p ) );
            vertex_descriptor v;
            if( pvIter != m_pointVertexMap.end() && !( m_pointVertexMap.key_comp()( p, pvIter->first ) ) )
            {
                v = pvIter->second;
            }
            else
            {
                v = boost::add_vertex( m_graph );
                m_pointVertexMap.insert( pvIter, std::make_pair( p, v ) );
                boost::put( boost::vertex_position, m_graph, v, p );
            }

            return v;
        }
		        
        point_vertex_map m_pointVertexMap;
        half_edge_list m_graph;
        polygon_collection m_polygons;
		polyline_collection m_polylines;
        NumberComparisonPolicy m_compare;

    };

	template <typename Point, typename Segments, typename NumberComparisonPolicy>
	inline doubly_connected_edge_list<Point, NumberComparisonPolicy> make_dcel(const Segments& segs, const NumberComparisonPolicy& cmp)
	{
		return doubly_connected_edge_list<Point, NumberComparisonPolicy>(segs, cmp);
	}

}//namespace geometrix;

#endif //GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
