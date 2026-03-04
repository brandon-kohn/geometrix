//!
//! Copyright © 2025
//! Brandon Kohn
//! Distributed under the Boost Software License, Version 1.0.
//! See accompanying file LICENSE_1_0.txt or copy at
//! http://www.boost.org/LICENSE_1_0.txt
//!
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/small_polygon.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algorithm/intersection/polygon_aabb_intersection.hpp>

#include <array>
#include <vector>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <limits>
#include <type_traits>

#define GEOMETRIX_DEBUG_CONVEX_REGION_MESH 0
namespace geometrix {

	template <typename CoordinateType, typename GridTypeGenerator>
	struct region_grid_cache
	{
		using coordinate_t = CoordinateType;
		using data_t = boost::container::flat_set<std::size_t>;
		using grid_traits_t = grid_traits<coordinate_t>;
		using grid_t = typename GridTypeGenerator::template type<data_t, grid_traits_t>;

		mutable std::optional<grid_t> m_grid;

		template <typename Points, typename Regions, typename PolygonAccessor, typename NumberComparisonPolicy>
		region_grid_cache( const Points& points,
			const Regions&               regions,
			const PolygonAccessor&       get_region_polygon,
			NumberComparisonPolicy const& cmp )
		{
			GEOMETRIX_ASSERT( !points.empty() );
			GEOMETRIX_ASSERT( !regions.empty() );

			//! Compute total bounds from all points.
			auto bounds = get_bounds( points, absolute_tolerance_comparison_policy<coordinate_t>( constants::zero<coordinate_t>() ) );

			using point_t = geometrix::point<coordinate_t, 2>;
			point_t lowerLeft( std::get<e_xmin>( bounds ), std::get<e_ymin>( bounds ) );
			point_t upperRight( std::get<e_xmax>( bounds ), std::get<e_ymax>( bounds ) );

			const auto sqrt2 = constants::sqrt_2<coordinate_t>();
			lowerLeft = lowerLeft + sqrt2 * normalize( lowerLeft - upperRight );
			upperRight = upperRight + sqrt2 * normalize( upperRight - lowerLeft );

			grid_traits_t gTraits( bounds, construct<coordinate_t>( 1.0 ) );
			m_grid.emplace( gTraits );
			auto& grid = *m_grid;

			//! Insert each region into the grid
			for( std::size_t r = 0; r < regions.size(); ++r )
			{
				const auto& poly = get_region_polygon( r ); //! poly is a sequence of point_t

				auto [xmin, xmax, ymin, ymax] = get_bounds( poly, cmp );

				auto imin = gTraits.get_x_index( xmin );
				auto imax = gTraits.get_x_index( xmax );
				auto jmin = gTraits.get_y_index( ymin );
				auto jmax = gTraits.get_y_index( ymax );

				for( auto ix = imin; ix <= imax; ++ix )
					for( auto iy = jmin; iy <= jmax; ++iy )
					{
						axis_aligned_bounding_box<point_t> box(
							gTraits.get_cell_corner0( ix, iy ),
							gTraits.get_cell_corner2( ix, iy ) );

						if( convex_polygon_aabb_intersection( poly, box, cmp ) )
							grid.get_cell( ix, iy ).insert( r );
					}
			}
		}

		template <typename Point>
		data_t* find_indices( const Point& p ) const
		{
			auto& grid = *m_grid;
			if( grid.is_contained( p ) )
				return &grid.get_cell( p );
			return nullptr;
		}
	};

	template <typename Mesh>
	struct region_adjacency_builder
	{
		using region_id_t = std::size_t;
		using vertex_index_t = std::size_t;
		using portal_t = std::pair<vertex_index_t, vertex_index_t>;
		using adjacency_t = std::vector<std::vector<region_id_t>>;
		using portal_map_t = std::vector<std::vector<portal_t>>;

		region_adjacency_builder( const Mesh& mesh,
			const std::vector<region_id_t>&   region_ids )
			: m_mesh( mesh )
			, m_region_ids( region_ids )
		{}

		void build( adjacency_t& out_adj, portal_map_t& out_portals )
		{
			const auto region_count = *std::max_element( m_region_ids.begin(), m_region_ids.end() ) + 1;

			out_adj.assign( region_count, {} );
			out_portals.assign( region_count, {} );

			struct edge_key_hash
			{
				std::size_t operator()( const std::pair<std::size_t, std::size_t>& e ) const noexcept
				{
					return std::hash<std::size_t>()( e.first ) ^ ( std::hash<std::size_t>()( e.second ) << 1 );
				}
			};

			std::unordered_map<std::pair<std::size_t, std::size_t>,
				std::pair<std::size_t, int>,
				edge_key_hash>
				edge_owner;

			const auto tri_count = m_mesh.get_number_triangles();
			for( std::size_t t = 0; t < tri_count; ++t )
			{
				const auto& tri = m_mesh.get_triangle_indices( t );
				auto        rid = m_region_ids[t];

				for( int i = 0; i < 3; ++i )
				{
					auto v0 = tri[i];
					auto v1 = tri[( i + 1 ) % 3];
					if( v1 < v0 )
						std::swap( v0, v1 );

					auto key = std::make_pair( v0, v1 );
					auto it = edge_owner.find( key );
					if( it == edge_owner.end() )
						edge_owner[key] = { rid, i };
					else
					{
						auto other = it->second.first;
						if( other == rid )
							continue;

						if( !contains( out_adj[rid], other ) )
						{
							out_adj[rid].push_back( other );
							out_portals[rid].push_back( { v0, v1 } );
						}
						if( !contains( out_adj[other], rid ) )
						{
							out_adj[other].push_back( rid );
							out_portals[other].push_back( { v0, v1 } );
						}
					}
				}
			}
		}

	private:
		template <typename T>
		static bool contains( const std::vector<T>& v, const T& x )
		{
			return std::find( v.begin(), v.end(), x ) != v.end();
		}

		const Mesh&                     m_mesh;
		const std::vector<region_id_t>& m_region_ids;
	};

	template <typename Coordinate, typename Cache = region_grid_cache<Coordinate, dense_grid_type_generator>>
	class convex_mesh_2d
	{
	public:
		using coordinate_t = Coordinate;
		using point_t = geometrix::point<coordinate_t, 2>;
		using vector_t = geometrix::vector<coordinate_t, 2>;
		using region_id_t = std::size_t;
		using vert_index_t = std::size_t;
		using segment_t = geometrix::segment<point_t>;
		using region_poly_t = geometrix::polygon<point_t>;
		using cache_t = Cache;;
		
		convex_mesh_2d(
			std::vector<point_t>&&                                            points,
			std::vector<std::vector<vert_index_t>>&&                          region_boundaries,
			std::vector<std::vector<region_id_t>>&&                           region_neighbors,
			std::vector<std::vector<std::pair<vert_index_t, vert_index_t>>>&& region_portals,
			std::vector<region_poly_t>&&                                      region_polygons,
			cache_t&&                                                         region_cache,
			std::vector<std::vector<segment_t>>&&                             portal_geometry = {}
#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
			, std::vector<region_poly_t>&& trig_polys = {}
#endif
		)
			: m_points( std::move( points ) )
			, m_region_boundaries( std::move( region_boundaries ) )
			, m_region_neighbors( std::move( region_neighbors ) )
			, m_region_portals( std::move( region_portals ) )
			, m_region_polygons( std::move( region_polygons ) )
			, m_region_cache( std::move( region_cache ) )
			, m_portal_geometry( std::move( portal_geometry ) )
#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
		    , m_trig_polys(std::move(trig_polys)) 
#endif
		{}

		const std::vector<point_t>& get_vertices() const noexcept
		{
			return m_points;
		}

		const std::vector<vert_index_t>& get_region_vertices( region_id_t r ) const noexcept
		{
			return m_region_boundaries[r];
		}

		const std::vector<region_id_t>& get_region_neighbors( region_id_t r ) const noexcept
		{
			return m_region_neighbors[r];
		}

		const std::vector<std::pair<vert_index_t, vert_index_t>>& get_region_portals( region_id_t r ) const noexcept
		{
			return m_region_portals[r];
		}

		const region_poly_t& get_region_polygon( region_id_t r ) const noexcept
		{
			return m_region_polygons[r];
		}

		const std::vector<segment_t>& get_portal_geometry( region_id_t r ) const noexcept
		{
			return m_portal_geometry[r];
		}

		std::size_t get_region_count() const noexcept
		{
			return m_region_polygons.size();
		}

		template <typename Point, typename NumberComparisonPolicy>
		std::optional<std::size_t> find_region( const Point& p, const NumberComparisonPolicy& cmp ) const
		{
			auto* bucket = m_region_cache.find_indices( p );
			if( bucket )
				for( auto r : *bucket )
				{
					if( point_in_convex_polygon( p, m_region_polygons[r], cmp ) )
						return r;
				}
			return std::nullopt;
		}

		auto get_point( vert_index_t i ) const noexcept -> const point_t&
		{
			GEOMETRIX_ASSERT( i < m_points.size() );
			return m_points[i];
		}

	private:
		std::vector<point_t>                                            m_points;
		std::vector<std::vector<vert_index_t>>                          m_region_boundaries;
		std::vector<std::vector<region_id_t>>                           m_region_neighbors;
		std::vector<std::vector<std::pair<vert_index_t, vert_index_t>>> m_region_portals;
		std::vector<region_poly_t>                                      m_region_polygons;
		region_grid_cache<coordinate_t, dense_grid_type_generator>      m_region_cache;

		//! debug-only convenience
		std::vector<std::vector<segment_t>> m_portal_geometry;
#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
		std::vector<geometrix::polygon<point_t>>                      m_trig_polys;      //! trig polys
#endif
	};

	//! Groups triangles into convex patches (regions) by checking local convexity.
	template <typename Mesh>
	class hertel_mehlhorn_decomposition
	{
		using vert_index_t = std::size_t;
		using face_ring_t = std::vector<vert_index_t>; //! CCW, no duplicated first/last

	public:
		using region_id_t = std::size_t;

		hertel_mehlhorn_decomposition( const Mesh& mesh )
			: m_mesh( mesh )
		{}

		template <typename NumberComparisonPolicy>
		void execute( const NumberComparisonPolicy& cmp )
		{
			const std::size_t N = m_mesh.get_number_triangles();
			m_region_ids.assign( N, invalid_region() );

			//! Each triangle starts as its own face
			struct face_t
			{
				std::vector<std::size_t> ring; //! CCW face boundary
				std::vector<std::size_t> tris; //! triangle indices absorbed
			};

			std::vector<face_t> faces;
			faces.reserve( N );

			std::vector<region_id_t> tri_to_face( N );

			//! Each triangle starts as its own face
			for( std::size_t t = 0; t < N; ++t )
			{
				const auto& tri = m_mesh.get_triangle_indices( t );
				face_t      f;
				f.ring = { tri[0], tri[1], tri[2] };
				f.tris = { t };
				faces.push_back( std::move( f ) );
				tri_to_face[t] = static_cast<region_id_t>( t );
			}

			//! Build candidate diagonal list
			struct diagonal_t
			{
				std::size_t ta, tb; // triangle indices
				std::size_t u, v;   // shared edge vertices
			};
			std::vector<diagonal_t> diags;
			const auto&             adj = m_mesh.get_adjacency_matrix();

			for( std::size_t a = 0; a < N; ++a )
			{
				const auto& A = m_mesh.get_triangle_indices( a );

				for( int k = 0; k < 3; ++k )
				{
					std::size_t b = adj[a][k];
					if( b == invalid_index() || b < a )
						continue;

					const auto& B = m_mesh.get_triangle_indices( b );

					for( int i = 0; i < 3; ++i )
					{
						for( int j = 0; j < 3; ++j )
						{
							if( A[i] == B[( j + 1 ) % 3] && A[( i + 1 ) % 3] == B[j] )
							{
								diags.push_back( { a, b, A[i], A[( i + 1 ) % 3] } );
							}
						}
					}
				}
			}

			//! Iteratively attempt merges
			bool merged = true;
			while( merged )
			{
				merged = false;

				for( const auto& d : diags )
				{
					auto fa = tri_to_face[d.ta];
					auto fb = tri_to_face[d.tb];

					if( fa == fb )
						continue; // already in same region

					auto& FA = faces[fa];
					auto& FB = faces[fb];

					if( FA.ring.empty() || FB.ring.empty() )
						continue; // one side dead

					auto merged_ring = merge_faces_along_edge( FA.ring, FB.ring, d.u, d.v );

					// build points for convexity check
					std::vector<typename Mesh::point_t> pts;
					pts.reserve( merged_ring.size() );
					for( auto vid : merged_ring )
						pts.push_back( m_mesh.get_point( vid ) );

					if( !is_convex_polygon_ccw( pts, cmp ) )
						continue;

					// Commit merge
					FA.ring = std::move( merged_ring );

					// Move triangles from FB to FA and remap tri_to_face
					for( auto t_idx : FB.tris )
					{
						tri_to_face[t_idx] = fa;
						FA.tris.push_back( t_idx );
					}
					FB.ring.clear();
					FB.tris.clear();

					merged = true;
				}
			}

			//! Assign region_ids
			region_id_t current = 0;
			for( auto& f : faces )
			{
				if( f.ring.empty() )
					continue;
				for( auto t : f.tris )
					m_region_ids[t] = current;
				++current;
			}

			m_region_count = current;
		}

		const std::vector<region_id_t>& get_region_ids() const noexcept { return m_region_ids; }
		std::size_t                     get_region_count() const noexcept { return m_region_count; }

	private:
		static constexpr region_id_t invalid_region() { return ( std::numeric_limits<region_id_t>::max )(); }
		static constexpr std::size_t invalid_index() { return ( std::numeric_limits<std::size_t>::max )(); }

		//! Ensure ring is CCW using signed area (shoelace).
		face_ring_t ensure_ccw( face_ring_t ring )
		{
			using coord_t = typename Mesh::coordinate_t;
			using point_t = typename Mesh::point_t;
			using area_t = decltype( coord_t{} * coord_t{} );

			auto       area = constants::zero<area_t>();
			const auto n = ring.size();
			for( std::size_t i = 0; i < n; ++i )
			{
				const auto& p0 = m_mesh.get_point( ring[i] );
				const auto& p1 = m_mesh.get_point( ring[( i + 1 ) % n] );
				area += ( p0[0] * p1[1] - p1[0] * p0[1] );
			}

			if( area < constants::zero<area_t>() )
				std::reverse( ring.begin(), ring.end() );

			return ring;
		}

		face_ring_t merge_faces_along_edge( const face_ring_t& A,
			const face_ring_t&                                 B,
			vert_index_t                                       u,
			vert_index_t                                       v )
		{
			using edge_t = std::pair<vert_index_t, vert_index_t>;

			struct EdgeHash
			{
				std::size_t operator()( const edge_t& e ) const noexcept
				{
					return std::hash<std::size_t>()( e.first ) ^ ( std::hash<std::size_t>()( e.second ) << 1 );
				}
			};

			auto make_undirected = []( vert_index_t a, vert_index_t b ) -> edge_t
			{
				return ( a < b ) ? edge_t{ a, b } : edge_t{ b, a };
			};

			//! 1) Count undirected edges over A U B.
			std::unordered_map<edge_t, int, EdgeHash> edge_count;

			auto accumulate_edges = [&]( const face_ring_t& R )
			{
				const auto n = R.size();
				for( std::size_t i = 0; i < n; ++i )
				{
					auto a = R[i];
					auto b = R[( i + 1 ) % n];
					auto key = make_undirected( a, b );
					edge_count[key]++;
				}
			};

			accumulate_edges( A );
			accumulate_edges( B );

			//! 2) Collect boundary edges: those that appear exactly once.
			std::vector<edge_t> boundary_edges;
			boundary_edges.reserve( edge_count.size() );

			for( auto const& kv : edge_count )
			{
				if( kv.second == 1 )
				{
					boundary_edges.push_back( kv.first );
				}
				else
				{
					//! Expect exactly 2 for the shared diagonal (and possibly other
					//! internal edges in more complex merges). Anything >2 would be weird.
					GEOMETRIX_ASSERT( kv.second == 2 );
				}
			}

			//! Sanity: the shared edge (u,v) should be one of the edges with count==2,
			//! i.e. *not* in boundary_edges.
#ifndef NDEBUG
			{
				auto uv = make_undirected( u, v );
				int  cnt = 0;
				auto it = edge_count.find( uv );
				if( it != edge_count.end() )
					cnt = it->second;
				GEOMETRIX_ASSERT( cnt == 2 );
			}
#endif

			//! 3) Build adjacency (undirected) from boundary edges.
			//!    This is the same pattern as your build_region_boundaries().
			std::unordered_multimap<vert_index_t, vert_index_t> adj;
			for( auto const& e : boundary_edges )
			{
				adj.emplace( e.first, e.second );
				adj.emplace( e.second, e.first );
			}

			//! 4) Walk a single ring through adjacency.
			//!    Start from the smallest vertex index for determinism.
			if( boundary_edges.empty() )
				return {};

			vert_index_t start = boundary_edges.front().first;
			for( auto const& e : boundary_edges )
			{
				start = std::min( start, std::min( e.first, e.second ) );
			}

			face_ring_t ring;
			ring.reserve( boundary_edges.size() + 1 );

			const auto   invalid_vi = ( std::numeric_limits<vert_index_t>::max )();
			vert_index_t curr = start;
			vert_index_t prev = invalid_vi;

			for( std::size_t i = 0; i < boundary_edges.size() + 5; ++i )
			{
				ring.push_back( curr );

				auto         range = adj.equal_range( curr );
				vert_index_t next = invalid_vi;

				for( auto it = range.first; it != range.second; ++it )
				{
					if( it->second != prev )
					{
						next = it->second;
						break;
					}
				}

				if( next == invalid_vi || next == start )
					break;

				prev = curr;
				curr = next;
			}

			GEOMETRIX_ASSERT( ring.size() >= 3 );

			//! 5) Ensure CCW orientation of the merged ring.
			ring = ensure_ccw( std::move( ring ) );
			return ring;
		}

		template <typename PointContainer, typename NumberComparisonPolicy>
		bool is_convex_polygon_ccw( const PointContainer& pts,
			const NumberComparisonPolicy&                 cmp )
		{
			using point_t = typename PointContainer::value_type;
			using coord_t = typename arithmetic_type_of<point_t>::type;
			using area_t = decltype( coord_t{} * coord_t{} );

			const std::size_t n = pts.size();
			if( n < 3 )
				return false;

			int sign = 0; //! +1: left turns only, -1: right turns only

			for( std::size_t i = 0; i < n; ++i )
			{
				const auto& p0 = pts[i];
				const auto& p1 = pts[( i + 1 ) % n];
				const auto& p2 = pts[( i + 2 ) % n];

				const auto v1 = p1 - p0;
				const auto v2 = p2 - p1;
				const auto cross = exterior_product_area( v1, v2 );

				if( cmp.equals( cross, area_t{} ) )
					continue; //! collinear, ignore

				const int s = cmp.greater_than( cross, area_t{} ) ? 1 : -1;
				if( sign == 0 )
				{
					sign = s;
				}
				else if( sign != s )
				{
					return false; //! turned opposite way -> non-convex
				}
			}

			//! For CCW polygons, you *expect* sign > 0; but "convex" only cares that it's consistent.
			return sign != 0;
		}

		const Mesh&              m_mesh;
		std::vector<region_id_t> m_region_ids;
		std::size_t              m_region_count = 0;
	};

	template <typename Length>
	class convex_mesh_2d_builder
	{
	public:
		using coordinate_t = Length;
		using region_id_t = std::size_t;
		using point_t = geometrix::point<coordinate_t, 2>;
		using segment_t = geometrix::segment<point_t>;
		using area_t = decltype(coordinate_t() * coordinate_t());

		convex_mesh_2d_builder() = default;

		template <typename Mesh, typename NumberComparisonPolicy>
		convex_mesh_2d<coordinate_t> build( const Mesh& mesh, const NumberComparisonPolicy& cmp )
		{
			m_points = mesh.get_vertices();

			run_region_grouping( mesh, cmp ); //! region_ids + region_count
			build_region_boundaries( mesh );  //! boundary vertex index rings
			build_region_adjacency( mesh );   //! adjacency + portal index pairs
			build_region_polygons();          //! map boundary indices -> points
			build_portal_geometry();          //! map portal index pairs -> segments

#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
			for(auto t = 0; t < mesh.get_number_triangles(); ++t)
			{
				const auto& tri_indices = mesh.get_triangle_indices(t);
				geometrix::polygon<point_t> trig_poly;
				for(auto vi : tri_indices)
				{
					trig_poly.push_back( m_points[vi] );
				}
				m_trig_polys.push_back( std::move(trig_poly) );
			}
#endif
			//! finalize and hand everything off into convex_mesh_2d
			return finalize_convex_mesh(cmp);
		}

	private:

		//! Hertel–Mehlhorn region grouping
		template <typename Mesh, typename NumberComparisonPolicy>
		void run_region_grouping( const Mesh& mesh, const NumberComparisonPolicy& cmp )
		{
			hertel_mehlhorn_decomposition<Mesh> grouping( mesh );
			grouping.execute( cmp );

			m_region_ids = grouping.get_region_ids();
			m_region_count = grouping.get_region_count();
		}

		template <typename Coordinate, typename Boundary, typename Points>
		bool is_ccw_boundary( const Boundary& ring, const Points& pts )
		{

			if( ring.size() < 3 ) [[unlikely]]
				return true;

			auto              area = area_t{};
			const std::size_t n = ring.size();

			for( std::size_t i = 0; i < n; ++i )
			{
				const auto& A = pts[ring[i]];
				const auto& B = pts[ring[( i + 1 ) % n]];
				area += ( get<0>(B) - get<0>(A) ) * ( get<1>(B) + get<1>(A) );
			}

			return area < area_t{};
		}

		template <typename Mesh>
		void build_region_boundaries( const Mesh& mesh )
		{
			using vertex_index_t = std::size_t;

			const std::size_t triCount = mesh.get_number_triangles();
			const region_id_t regionCount = *std::max_element( m_region_ids.begin(), m_region_ids.end() ) + 1;

			m_region_boundaries.resize( regionCount );

			//! For each region, find edges that appear only once
			struct EdgeHash
			{
				std::size_t operator()( const std::pair<vertex_index_t, vertex_index_t>& e ) const noexcept
				{
					return std::hash<std::size_t>()( e.first ) ^ ( std::hash<std::size_t>()( e.second ) << 1 );
				}
			};

			for( region_id_t r = 0; r < regionCount; ++r )
			{
				std::unordered_map<std::pair<vertex_index_t, vertex_index_t>, int, EdgeHash> edge_count;

				for( std::size_t t = 0; t < triCount; ++t )
				{
					if( m_region_ids[t] != r )
						continue;

					const auto&                                              tri = mesh.get_triangle_indices( t );
					std::array<std::pair<vertex_index_t, vertex_index_t>, 3> edges = {
						std::make_pair( tri[0], tri[1] ),
						std::make_pair( tri[1], tri[2] ),
						std::make_pair( tri[2], tri[0] )
					};

					for( auto& e : edges )
					{
						if( e.second < e.first )
							std::swap( e.first, e.second );
						edge_count[e]++;
					}
				}

				//! Edges appearing once are the border
				std::vector<std::pair<vertex_index_t, vertex_index_t>> border_edges;
				for( auto& kv : edge_count )
					if( kv.second == 1 )
						border_edges.push_back( kv.first );

				if( border_edges.empty() )
					continue;

				//! Chain edges into a ring
				std::unordered_multimap<vertex_index_t, vertex_index_t> adj;
				for( auto& e : border_edges )
				{
					adj.emplace( e.first, e.second );
					adj.emplace( e.second, e.first );
				}

				//! Start from the smallest vertex index
				vertex_index_t start = border_edges.front().first;
				for( auto& e : border_edges )
					start = std::min( start, std::min( e.first, e.second ) );

				std::vector<vertex_index_t> ring;
				ring.reserve( border_edges.size() + 1 );

				vertex_index_t curr = start, prev = ( std::numeric_limits<vertex_index_t>::max )();
				for( std::size_t i = 0; i < border_edges.size() + 5; ++i )
				{
					ring.push_back( curr );
					auto           range = adj.equal_range( curr );
					vertex_index_t next = ( std::numeric_limits<vertex_index_t>::max )();
					for( auto it = range.first; it != range.second; ++it )
					{
						if( it->second != prev )
						{
							next = it->second;
							break;
						}
					}
					if( next == ( std::numeric_limits<vertex_index_t>::max )() || next == start )
						break;
					prev = curr;
					curr = next;
				}

				if( ring.size() >= 3 )
				{
					//! Force CCW orientation
					if( !is_ccw_boundary<typename coordinate_t>( ring, m_points ) )
					{
						std::reverse( ring.begin(), ring.end() );
					}

					m_region_boundaries[r] = std::move( ring );
				}
			}
		}

		template <typename Mesh>
		void build_region_adjacency( const Mesh& mesh )
		{
			region_adjacency_builder<Mesh> rab( mesh, m_region_ids );
			rab.build( m_region_adjacency, m_region_portals_idx );
		}

		void build_region_polygons()
		{
			m_region_polys.clear();
			m_region_polys.resize( m_region_boundaries.size() );

			for( std::size_t rid = 0; rid < m_region_boundaries.size(); ++rid )
			{
				auto&       poly = m_region_polys[rid];
				const auto& ring = m_region_boundaries[rid];

				poly.reserve( ring.size() );
				for( auto vid : ring )
					poly.push_back( m_points[vid] );
			}
		}

		void build_portal_geometry()
		{
			m_region_portals_geom.clear();
			m_region_portals_geom.resize( m_region_portals_idx.size() );

			for( std::size_t rid = 0; rid < m_region_portals_idx.size(); ++rid )
			{
				const auto& idx_pairs = m_region_portals_idx[rid];
				auto&       geom = m_region_portals_geom[rid];

				geom.reserve( idx_pairs.size() );
				for( auto [v0, v1] : idx_pairs )
					geom.emplace_back( m_points[v0], m_points[v1] );
			}
		}

		template <typename NumberComparisonPolicy>
		convex_mesh_2d<coordinate_t> finalize_convex_mesh( NumberComparisonPolicy const& cmp )
		{
			using cache_t = region_grid_cache<coordinate_t, dense_grid_type_generator>;

			//! Build region cache over the region polygons.
			cache_t cache( m_points, m_region_polys, [this]( std::size_t r )
				{
					GEOMETRIX_ASSERT( r < m_region_polys.size() );
					return m_region_polys[r];
				}, cmp );

			return convex_mesh_2d<coordinate_t>(
				std::move( m_points ), //! points
				std::move( m_region_boundaries ),
				std::move( m_region_adjacency ),   //! neighbors
				std::move( m_region_portals_idx ), //! (vert_idx, vert_idx) pairs
				std::move( m_region_polys ),       //! region_poly_t
				std::move( cache ),                //! region_grid_cache<...>
				std::move( m_region_portals_geom ) //! optional portal geometry
#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
			  , std::move(m_trig_polys)      //! trig polys
#endif
			);
		}

	private:
	
		std::vector<point_t>                                          m_points;
		std::vector<std::size_t>                                      m_region_ids;
		std::size_t                                                   m_region_count = 0;
		std::vector<std::vector<std::size_t>>                         m_region_boundaries; //! index rings
		std::vector<geometrix::polygon<point_t>>                      m_region_polys;      //! world polys
		std::vector<std::vector<std::size_t>>                         m_region_adjacency;
		std::vector<std::vector<std::pair<std::size_t, std::size_t>>> m_region_portals_idx;
		std::vector<std::vector<segment_t>>                           m_region_portals_geom;
#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
		std::vector<geometrix::polygon<point_t>>                      m_trig_polys;      //! trig polys
#endif
	};

	template <typename Mesh, typename NumberComparisonPolicy>
	inline convex_mesh_2d<typename Mesh::coordinate_t> make_convex_mesh( Mesh const& mesh, const NumberComparisonPolicy& cmp )
	{
		using coordinate_t = typename Mesh::coordinate_t;
		convex_mesh_2d_builder<coordinate_t> builder;
		return builder.build( mesh, cmp );
	}

	//! \brief Edge “item” for region–mesh visibility search.
	//! Carries the current region, the previous region, and the angular wedge [lo,hi].
	template <typename RegionMesh>
	struct region_edge_item
	{
		using region_id_t = typename RegionMesh::region_id_t;
		using vector_t = typename RegionMesh::vector_t;
		using length_t = typename arithmetic_type_of<vector_t>::type;

		bool is_all_around() const { return get<0>( lo ) == constants::infinity<length_t>() && get<0>( hi ) == constants::negative_infinity<length_t>(); }
		bool is_from_start() const { return from == ( std::numeric_limits<std::size_t>::max )(); }
		bool is_directed() const { return !( is_from_start() || is_all_around() ); }

		region_id_t from; //! previous region, or max() if none
		region_id_t to;   //! current region
		vector_t    lo;   //! lower wedge bound (vector from origin)
		vector_t    hi;   //! upper wedge bound (vector from origin)
	};

	//! \brief Implementation of a region–mesh search, analogous to mesh_2d’s search visitor.
	//! RegionVisitor must expose:
	//!   using edge_item = region_edge_item<RegionMesh>;
	//!   bool visit(const edge_item&);
	template <typename RegionMesh, typename RegionVisitor, typename NumberComparisonPolicy>
	class region_mesh_search
	{
	public:
		using mesh_type = RegionMesh;
		using visitor_type = RegionVisitor;
		using cmp_type = NumberComparisonPolicy;
		using region_id_t = typename RegionMesh::region_id_t;
		using point_t = typename RegionMesh::point_t;
		using vector_t = typename RegionMesh::vector_t;
		using length_t = typename arithmetic_type_of<point_t>::type;
		using edge_item = region_edge_item<RegionMesh>;

		static constexpr region_id_t invalid_region() noexcept
		{
			return ( std::numeric_limits<region_id_t>::max )();
		}

		region_mesh_search( region_id_t startRegion,
			const point_t&              origin,
			const RegionMesh&           mesh,
			RegionVisitor&              visitor,
			const cmp_type&             cmp )
			: m_mesh( &mesh )
			, m_origin( origin )
			, m_visitor( &visitor )
			, m_cmp( cmp )
		{
			m_start.from = invalid_region();
			m_start.to = startRegion;
			m_start.lo = vector_t( constants::infinity<length_t>(), constants::zero<length_t>() );
			m_start.hi = vector_t( constants::negative_infinity<length_t>(), constants::zero<length_t>() );
		}

		//! \brief Starting item for the DFS/BFS stack.
		edge_item get_start() const
		{
			return m_start;
		}

		//! \brief Dispatch to the user visitor.
		bool visit( const edge_item& item )
		{
			return ( *m_visitor )( item );
		}

		//! \brief Compute traversal into neighborRegion through the portal shared
		//! between current.to and neighborRegion.
		//! Returns std::nullopt if the resulting wedge is empty.
		bool prepare_adjacent_traversal( region_id_t neighborRegion, const auto& portal, const edge_item& current, edge_item& out ) const
		{
			using area_t = decltype( std::declval<typename vector_t::value_type>() * std::declval<typename vector_t::value_type>() );

			if( neighborRegion == invalid_region() || neighborRegion == current.from )
				return false;

			const auto& verts = m_mesh->get_vertices();
			GEOMETRIX_ASSERT( portal.first < verts.size() );
			GEOMETRIX_ASSERT( portal.second < verts.size() );

			point_t pointLo = verts[portal.first];
			point_t pointHi = verts[portal.second];
			if( current.is_directed() && !is_segment_in_range_2d_direct_cmp( pointLo, pointHi, current.lo, current.hi, m_origin ) )
				return false;

			if( exterior_product_area( pointHi - pointLo, m_origin - pointLo ) < area_t{} )
				std::swap( pointLo, pointHi );

#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
			auto Su = make_segment<point_t>( m_origin, pointLo );
			auto Sv = make_segment<point_t>( m_origin, pointHi );

			auto eLo = make_segment<point_t>( m_origin, m_origin + 1000.0 * current.lo );
			auto eHi = make_segment<point_t>( m_origin, m_origin + 1000.0 * current.hi );
#endif

			vector_t                 vecLo, vecHi;
			static constexpr direct_comparison_policy cmp;
			if( !numeric_sequence_equals_2d( m_origin, pointLo, cmp ) && !numeric_sequence_equals_2d( m_origin, pointHi, cmp ) ) [[likely]]
			{
				assign( vecLo, pointLo - m_origin );
				assign( vecHi, pointHi - m_origin );

				if( current.is_directed() ) [[likely]]
				{
					vecLo = is_vector_between<false>( current.lo, current.hi, vecLo, cmp ) ? vecLo : current.lo;
					vecHi = is_vector_between<false>( current.lo, current.hi, vecHi, cmp ) ? vecHi : current.hi;
				}

				if( get_orientation( vecHi, vecLo, cmp ) == geometrix::oriented_left )
					return false;
			}
			else
			{
				assign( vecLo, constants::infinity<length_t>(), constants::zero<length_t>() );
				assign( vecHi, constants::negative_infinity<length_t>(), constants::zero<length_t>() );
			}

#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
			auto nLo = make_segment<point_t>( m_origin, m_origin + 1000.0 * vecLo );
			auto nHi = make_segment<point_t>( m_origin, m_origin + 1000.0 * vecHi );
#endif

			out.from = current.to;
			out.to = neighborRegion;
			out.lo = vecLo;
			out.hi = vecHi;
			return true;
		}

	private:
		const RegionMesh* m_mesh;
		point_t           m_origin;
		RegionVisitor*    m_visitor;
		cmp_type          m_cmp;
		edge_item         m_start;
	};

	//! \brief Convenience helper to build a search object (mirrors make_mesh_search).
	template <typename RegionMesh, typename RegionVisitor, typename NumberComparisonPolicy>
	inline auto make_region_mesh_search( typename RegionMesh::region_id_t startRegion,
		const typename RegionMesh::point_t&                               origin,
		const RegionMesh&                                                 mesh,
		RegionVisitor&                                                    visitor,
		const NumberComparisonPolicy&                                     cmp )
	{
		return region_mesh_search<RegionMesh, RegionVisitor, NumberComparisonPolicy>( startRegion, origin, mesh, visitor, cmp );
	}

	//! \brief Generic DFS driver, analogous to mesh_2d::search.
	template <typename RegionMesh, typename RegionSearch>
	inline void run_region_mesh_search( const RegionMesh& mesh, RegionSearch& search )
	{
		using edge_item = typename std::remove_reference_t<RegionSearch>::edge_item;
		using region_id_t = typename RegionMesh::region_id_t;

		constexpr region_id_t invalid = ( std::numeric_limits<region_id_t>::max )();

		std::vector<edge_item> stack;
		stack.reserve( 128 );
		stack.push_back( search.get_start() );

		while( !stack.empty() )
		{
			edge_item item = std::move( stack.back() );
			stack.pop_back();

			if( search.visit( item ) )
			{
				const auto& neighbors = mesh.get_region_neighbors( item.to );
				const auto& portals = mesh.get_region_portals( item.to );
				GEOMETRIX_ASSERT( neighbors.size() == portals.size() );

				for( std::size_t i = 0; i < neighbors.size(); ++i )
				{
					const auto nbr = neighbors[i];
					if( nbr != invalid && nbr != item.from )
					{
						edge_item nextItem;
						if( search.prepare_adjacent_traversal( nbr, portals[i], item, nextItem ) )
							stack.push_back( std::move( nextItem ) );
					}
				}
			}
		}
	}

} //! namespace geometrix
