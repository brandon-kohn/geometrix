//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_MESH_2D_HPP
#define GEOMETRIX_MESH_2D_HPP
#pragma once

#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/array_point_sequence.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/algorithm/point_in_polygon.hpp>
#include <geometrix/algorithm/grid_2d.hpp>
#include <geometrix/algorithm/hash_grid_2d.hpp>
#include <geometrix/algorithm/eberly_triangle_aabb_intersection.hpp>
#include <geometrix/numeric/constants.hpp>

#include <boost/optional.hpp>
#include <boost/utility/typed_in_place_factory.hpp>
#include <boost/container/flat_set.hpp>
#include <array>
#include <boost/limits.hpp>

namespace geometrix
{
    struct dense_grid_type_generator
    {
        template <typename Data, typename Traits>
        using type = grid_2d<Data, Traits>;
    };

    struct sparse_grid_type_generator
    {
        template <typename Data, typename Traits>
        using type = hash_grid_2d<Data, Traits>;
    };
    
    template <typename CoordinateType, typename GridTypeGenerator>
    struct triangle_grid_cache
    {
        using coordinate_t = CoordinateType;
        using data_t = boost::container::flat_set<std::size_t>;
        using grid_traits_t = grid_traits<coordinate_t>;
        using grid_t = typename GridTypeGenerator::template type<data_t, grid_traits_t>;

        template <typename Points, typename Triangles>
        triangle_grid_cache(const Points& points, const Triangles& triangles)
        {
            GEOMETRIX_ASSERT(!points.empty());
            GEOMETRIX_ASSERT(!triangles.empty());
            auto bounds = get_bounds(points, absolute_tolerance_comparison_policy<coordinate_t>(constants::zero<coordinate_t>()));
            coordinate_t xmin, xmax, ymin, ymax;

            using point_t = point<coordinate_t, 2>;
            point_t lowerLeft(boost::get<e_xmin>(bounds), boost::get<e_ymin>(bounds));
            point_t upperRight(boost::get<e_xmax>(bounds), boost::get<e_ymax>(bounds));
            const auto sqrt2 = constants::sqrt_2<coordinate_t>();
            const auto offset = sqrt2;
            lowerLeft = lowerLeft + offset * normalize(lowerLeft - upperRight);
            upperRight = upperRight + offset * normalize(upperRight - lowerLeft);
            boost::get<e_xmin>(bounds) = lowerLeft[0], boost::get<e_ymin>(bounds) = lowerLeft[1];
            boost::get<e_xmax>(bounds) = upperRight[0], boost::get<e_ymax>(bounds) = upperRight[1];

            grid_traits<coordinate_t> gTraits(bounds, construct<coordinate_t>(1.0));
            m_grid = boost::in_place<grid_t>(gTraits);
            auto& grid = *m_grid;

            //! add each triangle
            for (std::size_t i = 0; i < triangles.size(); ++i)
            {
                auto const& trig = triangles[i];
                boost::tie(xmin, ymin, xmax, ymax) = get_bounds(trig, absolute_tolerance_comparison_policy<coordinate_t>(constants::zero<coordinate_t>()));
                auto imin = gTraits.get_x_index(xmin);
                auto imax = gTraits.get_x_index(xmax);
                auto jmin = gTraits.get_y_index(ymin);
                auto jmax = gTraits.get_y_index(ymax);
                for (auto col = imin; col <= imax; ++col)
                {
                    for (auto row = jmin; row <= jmax; ++row)
                    {
                        axis_aligned_bounding_box<point<coordinate_t, 2>> box(gTraits.get_cell_corner0(col, row), gTraits.get_cell_corner2(col, row));
                        if (eberly_triangle_aabb_intersection_2d(trig[0], trig[1], trig[2], box, absolute_tolerance_comparison_policy<coordinate_t>(construct<coordinate_t>(1e-10))))
                            grid.get_cell(col, row).insert(i);
                    }
                }
            }
        }

        template <typename Point>
        boost::optional<data_t const&> find_indices(const Point& p) const 
        {
            auto& grid = *m_grid;
            if (grid.is_contained(p))
                return grid.get_cell(p);
            
            return boost::none;
        }

        mutable boost::optional<grid_t> m_grid;
    };
    
    template <typename CoordinateType>
    using default_triangle_cache = triangle_grid_cache<CoordinateType, dense_grid_type_generator>;

    template <typename TriangleCache>
    struct mesh_traits
    {
        using cache_t = TriangleCache;
    };

    template <typename CoordinateType>
    class mesh_2d_base
    {
    public:

        using coordinate_t = CoordinateType;
        using point_t = point<coordinate_t, 2>;
        using vector_t = vector<coordinate_t, 2>;

        using point_container_t = std::vector<point_t>;
        using index_container_t = std::vector<std::array<std::size_t, 3>>;
        using triangle_container_t = std::vector<std::array<point_t, 3>>;
                
        template <typename Points, typename Indices, typename NumberComparisonPolicy>
        mesh_2d_base(const Points& points, Indices indices, const NumberComparisonPolicy& cmp)
        {
            for( auto const& p : points )
                m_points.push_back( construct< point_t >( p ) );

            std::size_t numberTriangles = indices.size() / 3;

            for (std::size_t triangleIndex = 0; triangleIndex < numberTriangles; ++triangleIndex)
            {
                std::size_t i = triangleIndex * 3;
                std::size_t index0 = indices[i];
                std::size_t& index1 = indices[i + 1];
                std::size_t& index2 = indices[i + 2];

                GEOMETRIX_ASSERT( index0 < m_points.size() );
                GEOMETRIX_ASSERT( index1 < m_points.size() );
                GEOMETRIX_ASSERT( index2 < m_points.size() );

                //! Triangles should be CCW.
                if (get_orientation(m_points[index0], m_points[index1], m_points[index2], cmp) == oriented_right)
                    std::swap( index1, index2 );

                //! Triangles should not be degenerate.
                if (numeric_sequence_equals(m_points[index0], m_points[index1], cmp) ||
                    numeric_sequence_equals(m_points[index1], m_points[index2], cmp) ||
                    numeric_sequence_equals(m_points[index2], m_points[index0], cmp))
                    continue;
                                
                m_indices.push_back({ index0, index1, index2 });
                m_triangles.push_back({ m_points[index0], m_points[index1], m_points[index2] });
            }
        }

        //! Calculate a random interior position. Parameters rT, r1, and r2 should be uniformly distributed random numbers in the range of [0., 1.].
        point_t get_random_position(double rT, double r1, double r2)
        {
            GEOMETRIX_ASSERT( !m_triangles.empty() );
            GEOMETRIX_ASSERT(0. <= rT && rT <= 1.);
            GEOMETRIX_ASSERT(0. <= r1 && rT <= 1.);
            GEOMETRIX_ASSERT(0. <= r2 && rT <= 1.);

            using std::sqrt;

            std::size_t iTri = static_cast<std::size_t>(rT * (m_triangles.size() - 1));
            const auto& points = get_triangle_vertices( iTri );
            double sqrt_r1 = sqrt(r1);
            return (1 - sqrt_r1) * as_vector(points[0]) + sqrt_r1 * (1 - r2) * as_vector(points[1]) + sqrt_r1 * r2 * as_vector(points[2]);
        }

        std::size_t get_number_triangles() const { return m_triangles.size(); }
        std::size_t get_number_vertices() const { return m_points.size(); }
        const std::vector<point_t>& get_vertices() const { return m_points; }

        const std::array<std::size_t,3>& get_triangle_indices( std::size_t i ) const { return m_indices[i]; }
        const std::array<point_t, 3>& get_triangle_vertices( std::size_t i ) const { return m_triangles[i]; }
        
    protected:
        
        point_container_t m_points;
        index_container_t m_indices;
        triangle_container_t m_triangles;

    };

    template <typename Cache, typename Points, typename Triangles>
    inline Cache make_triangle_cache(const Points& pts, const Triangles& trigs)
    {
        return Cache(pts, trigs);
    }
    
    template <typename CoordinateType, typename Traits = mesh_traits<default_triangle_cache<CoordinateType>>>
    class mesh_2d : public mesh_2d_base<CoordinateType>
    {
    public:

        using base_t = mesh_2d_base<CoordinateType>;
        using traits_t = Traits;
        using cache_t = typename traits_t::cache_t;
        using adjacency_matrix_t = std::vector<std::array<std::size_t, 3>>;
        using point_container_t = typename base_t::point_container_t;
        using triangle_container_t = typename base_t::triangle_container_t;

        template <typename Points, typename Indices, typename NumberComparisonPolicy>
        mesh_2d(const Points& points, Indices indices, const NumberComparisonPolicy& cmp, const std::function<cache_t(const point_container_t&, const triangle_container_t&)>& cacheBuilder = make_triangle_cache<cache_t, point_container_t, triangle_container_t>)
            : base_t(points, indices, cmp)
            , m_cache(cacheBuilder(base_t::m_points, base_t::m_triangles))
        {}

        const adjacency_matrix_t& get_adjacency_matrix() const
        {
            if (!m_adjMatrix)
                create_adjacency_matrix();
            return *m_adjMatrix;
        }

        template <typename Point, typename NumberComparisonPolicy>
        boost::optional<std::size_t> find_triangle(const Point& p, const NumberComparisonPolicy& cmp) const
        {
            auto const& data = m_cache.find_indices(p);
            for (std::size_t ti : data)
            {
                const auto& points = base_t::get_triangle_vertices(ti);
                if (point_in_triangle(p, points[0], points[1], points[2], cmp))
                    return ti;
            }

            return boost::none;
        }

        //! search the mesh graph in a DFS fashion.
        template <typename MeshSearch >
        void search(MeshSearch&& visitor) const
        {
            using namespace boost;

            typedef typename remove_const_ref<MeshSearch>::type::edge_item edge_item;
            std::vector<edge_item> Q;
            Q.reserve(100);
            Q.push_back(visitor.get_start());

            const adjacency_matrix_t& adjMatrix = get_adjacency_matrix();

            while (!Q.empty())
            {
                edge_item item = Q.back();
                Q.pop_back();

                //! return value indicates if the search should continue.
                if (!visitor.visit(item))
                    return;

                std::size_t adjTrig = adjMatrix[item.get_triangle_index()][0];
                if (adjTrig != static_cast<std::size_t>(-1) && adjTrig != item.from)
                {
                    boost::optional<edge_item> newItem = visitor.prepare_adjacent_traversal(adjTrig, item);
                    if (newItem)
                        Q.push_back(*newItem);
                }

                adjTrig = adjMatrix[item.get_triangle_index()][1];
                if (adjTrig != static_cast<std::size_t>(-1) && adjTrig != item.from)
                {
                    boost::optional<edge_item> newItem = visitor.prepare_adjacent_traversal(adjTrig, item);
                    if (newItem)
                        Q.push_back(*newItem);
                }

                adjTrig = adjMatrix[item.get_triangle_index()][2];
                if (adjTrig != static_cast<std::size_t>(-1) && adjTrig != item.from)
                {
                    boost::optional<edge_item> newItem = visitor.prepare_adjacent_traversal(adjTrig, item);
                    if (newItem)
                        Q.push_back(*newItem);
                }
            }
        }

    private:

        void create_adjacency_matrix() const
        {
            std::array<std::size_t, 3> defaultArray = { { (std::numeric_limits<std::size_t>::max)(), (std::numeric_limits<std::size_t>::max)(), (std::numeric_limits<std::size_t>::max)() } };
            m_adjMatrix = boost::in_place<adjacency_matrix_t>(base_t::m_triangles.size(), defaultArray);
            auto& adjMatrix = *m_adjMatrix;
            enum class trig_side { zero, one, two };
            struct adj_item { std::size_t index; trig_side side; };
            std::map<std::pair<std::size_t, std::size_t>, std::vector<adj_item>> adjTriangles;

            for (std::size_t i = 0; i < base_t::m_triangles.size(); ++i)
            {
                const auto& indices = base_t::get_triangle_indices(i);
                adjTriangles[std::make_pair(indices[0], indices[1])].push_back(adj_item{ i, trig_side::zero });
                adjTriangles[std::make_pair(indices[1], indices[2])].push_back(adj_item{ i, trig_side::one });
                adjTriangles[std::make_pair(indices[2], indices[0])].push_back(adj_item{ i, trig_side::two });
            }

            for (const auto& item : adjTriangles)
            {
                const auto& key = item.first;
                const auto& itrigs = item.second;
                auto it = adjTriangles.find(std::make_pair(key.second, key.first));
                if (it != adjTriangles.end())
                {
                    //! all the trigs in it collection are adjacent to the trigs in itrigs.
                    for (auto const& i : itrigs)
                    {
                        for (auto const& j : it->second)
                        {
                            adjMatrix[i.index][static_cast<std::size_t>(i.side)] = j.index;
                            adjMatrix[j.index][static_cast<std::size_t>(j.side)] = i.index;
                        }
                    }
                }
            }
        }

        mutable boost::optional<adjacency_matrix_t> m_adjMatrix;
        cache_t m_cache;
    };

    namespace detail
    {
        template <std::size_t i, std::size_t j>
        inline bool is_adjacent_side( const std::array<std::size_t, 3>& tri1, const std::array<std::size_t, 3>& tri2 )
        {
            return tri1[i] == tri2[(j + 1) % 3] && tri1[(i + 1) % 3] == tri2[j];
        }
    }

    inline std::size_t get_triangle_adjacent_side( const std::array<std::size_t, 3>& tri1, const std::array<std::size_t, 3>& tri2 )
    {
        using namespace detail;

        if( is_adjacent_side<0, 0>( tri1, tri2 ) || is_adjacent_side<0, 1>( tri1, tri2 ) || is_adjacent_side<0, 2>( tri1, tri2 ) )
            return 0;
        if( is_adjacent_side<1, 0>( tri1, tri2 ) || is_adjacent_side<1, 1>( tri1, tri2 ) || is_adjacent_side<1, 2>( tri1, tri2 ) )
            return 1;
        if( is_adjacent_side<2, 0>( tri1, tri2 ) || is_adjacent_side<2, 1>( tri1, tri2 ) || is_adjacent_side<2, 2>( tri1, tri2 ) )
            return 2;

        GEOMETRIX_ASSERT( false );
        return (std::numeric_limits<std::size_t>::max)();
    }
}//! namespace geometrix

#endif // GEOMETRIX_MESH_2D_HPP
