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
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/utility/utilities.hpp>

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

        typedef Point                     point_type;
		typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
        typedef std::vector< point_type > point_sequence;
        typedef std::vector< point_sequence >       point_sequence_collection;

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
			, m_compare(other.m_compare)
		{}

		doubly_connected_edge_list(doubly_connected_edge_list&& other)
			: m_pointVertexMap(std::move(other.m_pointVertexMap))
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

        const point_sequence_collection& get_point_sequences() const
        {
            return m_pointSequences;            
        }
		
		void calculate_point_sequences()
		{
			m_pointSequences.clear();

			std::size_t edge_count = 0;
			typename boost::graph_traits<half_edge_list>::edge_iterator ei, ei_end;
			for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei)
			{
				edge_descriptor e = *ei;
				vertex_descriptor s = boost::source(e, m_graph);
				vertex_descriptor t = boost::target(e, m_graph);
				boost::put(boost::edge_index, m_graph, e, edge_count++);
			}

			point_sequence currentFace;
			boost::dynamic_bitset<> visitedEdges(edge_count);
			for (boost::tie(ei, ei_end) = boost::edges(m_graph); ei != ei_end; ++ei)
			{
				edge_descriptor e = *ei;
				std::size_t eIndex = boost::get(boost::edge_index, m_graph, e);

				edge_descriptor eD = e;

				if (!visitedEdges[eIndex])
				{
					currentFace.clear();
					
					while (!visitedEdges[eIndex])
					{
						vertex_descriptor s = boost::source(e, m_graph);
						vertex_descriptor t = boost::target(e, m_graph);

						point_type sPoint = boost::get(boost::vertex_position, m_graph, s);						
						currentFace.push_back(boost::get(boost::vertex_position, m_graph, s));

						typename boost::graph_traits< half_edge_list >::out_edge_iterator oei, oei_end;
						boost::tie(oei, oei_end) = boost::out_edges(t, m_graph);
						std::size_t outDegree = boost::out_degree(t, m_graph);
						if (outDegree > 0)
						{
							if (outDegree > 1)
							{
								//find the next edges by sorting them relative to this ones target.
								typedef point_angle_compare< NumberComparisonPolicy > winding_compare; ;
								typedef std::pair< point_type, typename boost::graph_traits< half_edge_list >::out_edge_iterator > winding_pair;
								typedef std::set< winding_pair, pair_first_compare< winding_compare > > winding_sorter;
								winding_sorter windingSorter(pair_first_compare< winding_compare >(winding_compare(boost::get(boost::vertex_position, m_graph, t), m_compare)));
								for (; oei != oei_end; ++oei)
								{
									vertex_descriptor v = boost::target(*oei, m_graph);
									if (v != s)
										windingSorter.insert(std::make_pair(boost::get(boost::vertex_position, m_graph, v), oei));
								}

								typename winding_sorter::iterator sIter = windingSorter.lower_bound(std::make_pair(sPoint, oei_end));
								if (sIter != windingSorter.end())
									e = *(sIter->second);
							}
							else
								e = *oei;

							visitedEdges.set(eIndex);
							eIndex = boost::get(boost::edge_index, m_graph, e);
						}
					}

					m_pointSequences.push_back(std::move(currentFace));
				}
			}
		}

		doubly_connected_edge_list& operator = (doubly_connected_edge_list&& other)
		{
			m_pointVertexMap = std::move(other.m_pointVertexMap);
			m_compare = other.m_compare;
			return *this;
		}

		doubly_connected_edge_list& operator = (const doubly_connected_edge_list& other)
		{
			m_pointVertexMap = other.m_pointVertexMap;
			m_compare = other.m_compare;
			return *this;
		}

    private:

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
        point_sequence_collection m_pointSequences;
        NumberComparisonPolicy m_compare;

    };

	template <typename Point, typename Segments, typename NumberComparisonPolicy>
	inline doubly_connected_edge_list<Point, NumberComparisonPolicy> make_dcel(const Segments& segs, const NumberComparisonPolicy& cmp)
	{
		return doubly_connected_edge_list<Point, NumberComparisonPolicy>(segs, cmp);
	}

}//namespace geometrix;

#endif //GEOMETRIX_DOUBLY_CONNECTED_EDGE_LIST_HPP
