//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_MESH_SEARCH_HPP
#define GEOMETRIX_MESH_SEARCH_HPP
#pragma once

#include <geometrix/algorithm/mesh_2d.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/algorithm/orientation/vector_vector_orientation.hpp>
#include <geometrix/algorithm/is_segment_in_range.hpp>
#include <geometrix/numeric/constants.hpp>

#include <geometrix/test/test.hpp>

#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/container/flat_set.hpp>
#include <tuple>

namespace geometrix
{
    template <typename Point, typename Mesh>
    struct visible_vertices_visitor
    {
        visible_vertices_visitor( const Point& origin, const Mesh& mesh )
			: m_vertices()
			, m_origin( origin )
			, m_mesh( &mesh )
		{}

        template <typename MeshEdge>
        bool operator()( const MeshEdge& edge )
        {
            const auto& toIndices = m_mesh->get_triangle_indices( edge.to );
            if( !edge.is_all_around() )
            {
                const auto& fromIndices = m_mesh->get_triangle_indices( edge.from );
                direct_comparison_policy cmp;
                for( std::size_t i = 0; i < 3; ++i )
                {
                    if( fromIndices[0] == toIndices[i] || fromIndices[1] == toIndices[i] || fromIndices[2] == toIndices[i] )
                        continue;

                    const auto& point = m_mesh->get_triangle_vertices( edge.to )[i];
                    if( is_vector_between( edge.lo, edge.hi, point-m_origin, true, cmp ) )
                        m_vertices.push_back( toIndices[i] );
                }
            }
            else
            {
                m_vertices.push_back( toIndices[0] );
                m_vertices.push_back( toIndices[1] );
                m_vertices.push_back( toIndices[2] );
            }

            return true;
        }

        std::vector<std::size_t> const& get_vertices() const { return m_vertices; }

    private:

		Point                    m_origin;
		const Mesh*              m_mesh;
        std::vector<std::size_t> m_vertices;

    };

    template <typename Point, typename Mesh, typename ... Visitors>
    struct mesh_search
    {
        using length_t = typename arithmetic_type_of<Point>::type;
        using dimensionless_t = typename dimensionless_type_of<Point>::type;
        using vector_t = vector<length_t, 2>;
        using direction_t = vector<dimensionless_t, 2>;
        using visitor_tuple = std::tuple<Visitors...>;

        mesh_search(const vector_t& lo, const vector_t& hi, std::size_t start, const Point& origin, const Mesh& mesh, Visitors&&... a)
            : m_lo(lo)
            , m_hi(hi)
            , m_origin(origin)
            , m_mesh(mesh)
            , m_start(start)
            , m_visitors(std::forward<Visitors>(a)...)
        {}
        
        mesh_search(std::size_t start, const Point& origin, const Mesh& mesh, Visitors&&... a)
            : mesh_search(
                vector_t(constants::infinity<length_t>(), constants::zero<length_t>() )
              , vector<length_t,2>(constants::negative_infinity<length_t>(), constants::zero<length_t>())
              , start
              , origin
              , mesh
              , std::forward<Visitors>(a)...
            )
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
            std::size_t get_to_triangle() const { return to; }
            std::size_t get_from_triangle() const { return from; }
            bool        is_all_around() const { return get<0>( lo ) == constants::infinity<length_t>() && get<0>( hi ) == constants::negative_infinity<length_t>(); }

            bool operator<( const mesh_search_item& rhs ) const
            {
                return geometrix::lexicographical_compare( from, rhs.from, to, rhs.to, lo, rhs.lo, hi, rhs.hi );
            }

            std::size_t from;
            std::size_t to;
            vector_t lo;
            vector_t hi;
        };
        typedef mesh_search_item edge_item;

        edge_item get_start()
        {
            return edge_item( (std::numeric_limits<std::size_t>::max)(), m_start, m_lo, m_hi );
        }

        //! Visit the item and return true/false if the search should continue.
        bool visit( const edge_item& item )
        {
            return std::apply( [&]( auto& ... vs )
                    {
                        bool r = false;
                        ( (r |= vs(item)),...);
                        return r;
                    }, m_visitors );
        }

        //! Generate a new item to visit based on the adjacent triangle at index next.
        boost::optional<edge_item> prepare_adjacent_traversal( std::size_t next, const edge_item& item )
        {
            direct_comparison_policy cmp;
            using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
            const auto& fromIndices = m_mesh.get_triangle_indices( item.to );
            const auto& toIndices = m_mesh.get_triangle_indices( next );

            std::size_t side = get_triangle_adjacent_side( fromIndices, toIndices );
            auto pointLo = m_mesh.get_triangle_vertices( item.to )[side];
			constexpr int next_vertex[3] = { 1, 2, 0 };
            auto pointHi = m_mesh.get_triangle_vertices( item.to )[next_vertex[side]];

            if( exterior_product_area( pointHi - pointLo, m_origin - pointLo ) < constants::zero<area_t>() )
                std::swap( pointLo, pointHi );

            if (!item.is_all_around() && !is_segment_in_range_2d(make_segment(pointLo, pointHi), item.lo, item.hi, m_origin))
                return boost::none;

            vector_t vecLo, vecHi;
            if( !numeric_sequence_equals_2d(m_origin, pointLo, cmp) && !numeric_sequence_equals_2d(m_origin, pointHi, cmp) )
            {
                assign( vecLo, pointLo - m_origin );
                assign( vecHi, pointHi - m_origin );

                if (!item.is_all_around())
                {
                    vecLo = is_vector_between(item.lo, item.hi, vecLo, false, cmp) ? vecLo : item.lo;
                    vecHi = is_vector_between(item.lo, item.hi, vecHi, false, cmp) ? vecHi : item.hi;
                }

                if (get_orientation(vecHi, vecLo, cmp) == geometrix::oriented_left)
                    return boost::none;
            }
            else
            {
                assign( vecLo, constants::infinity<length_t>(), constants::zero<length_t>() );
                assign( vecHi, constants::negative_infinity<length_t>(), constants::zero<length_t>() );
            }

            auto nItem = edge_item( item.to, next, vecLo, vecHi );
			if( auto it = m_visited.lower_bound( nItem ); it == m_visited.end() || m_visited.key_comp()( nItem, *it ) ) 
            {
				m_visited.insert( it, nItem );
				return nItem;
            }

            return boost::none;
        }

    private:
		vector_t                              m_lo;
		vector_t                              m_hi;
		Point                                 m_origin;
		const Mesh&                           m_mesh;
		std::size_t                           m_start;
		std::tuple<Visitors...>               m_visitors;
		boost::container::flat_set<edge_item> m_visited;

    };

    template <typename Point, typename Mesh, typename ... Visitors>
    inline mesh_search<Point, Mesh, Visitors...> make_mesh_search(std::size_t startTrig, const Point& origin, const Mesh& mesh, Visitors&&... vs)
    {
		return mesh_search<Point, Mesh, Visitors...>( startTrig, origin, mesh, std::forward<Visitors>(vs)... );
    }
    
    template <typename Vector1, typename Vector2, typename Point, typename Mesh, typename ... Visitors>
    inline mesh_search<Point, Mesh, Visitors...> make_mesh_search(const Vector1& lo, const Vector2& hi, std::size_t startTrig, const Point& origin, const Mesh& mesh, Visitors&&... vs)
    {
		return mesh_search<Point, Mesh, Visitors...>( lo, hi, startTrig, origin, mesh, std::forward<Visitors>(vs)... );
    }

}//! namespace geometrix;

#endif // GEOMETRIX_MESH_SEARCH_HPP
