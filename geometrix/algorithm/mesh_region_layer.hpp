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

#include <array>
#include <vector>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <limits>
#include <type_traits>

#define GEOMETRIX_DEBUG_CONVEX_REGION_MESH 1

namespace geometrix {

	//! Groups triangles into convex patches (regions) by checking local convexity.
	template <typename Mesh>
	class hertel_mehlhorn_decomposition
	{
		using vert_index_t = std::size_t;
		using face_ring_t = std::vector<vert_index_t>; // CCW, no duplicated first/last

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

			// Each triangle starts as its own face
			struct face_t
			{
				std::vector<std::size_t> ring; // CCW face boundary
				std::vector<std::size_t> tris; // triangle indices absorbed
			};

			std::vector<face_t> faces;
			faces.reserve( N );

			// Create one face per triangle
			for( std::size_t t = 0; t < N; ++t )
			{
				const auto& tri = m_mesh.get_triangle_indices( t );
				faces.push_back( { std::vector<std::size_t>{ tri[0], tri[1], tri[2] },
					std::vector<std::size_t>{ t } } );
			}

			// Build candidate diagonal list
			struct diagonal_t
			{
				std::size_t fa, fb;
				std::size_t u, v;
			};
			std::vector<diagonal_t> diags;

			const auto& adj = m_mesh.get_adjacency_matrix();
			for( std::size_t a = 0; a < N; ++a )
			{
				const auto& A = m_mesh.get_triangle_indices( a );

				for( int k = 0; k < 3; ++k )
				{
					std::size_t b = adj[a][k];
					if( b == invalid_index() || b < a )
						continue;

					const auto& B = m_mesh.get_triangle_indices( b );

					// Find shared edge
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

			// Iteratively attempt merges
			bool merged = true;
			while( merged )
			{
				merged = false;

				for( auto& d : diags )
				{
					auto& FA = faces[d.fa];
					auto& FB = faces[d.fb];

					if( FA.ring.empty() || FB.ring.empty() )
						continue; // already merged

					auto merged_ring = merge_faces_along_edge( FA.ring, FB.ring, d.u, d.v );

					// Convert to points
					std::vector<typename Mesh::point_t> pts;
					pts.reserve( merged_ring.size() );
					for( auto vid : merged_ring )
						pts.push_back( m_mesh.get_point( vid ) );

					if( !is_convex_polygon_ccw( pts, cmp ) )
						continue;

					// Commit merge
					FA.ring = std::move( merged_ring );
					FA.tris.insert( FA.tris.end(), FB.tris.begin(), FB.tris.end() );
					FB.ring.clear();

					merged = true;
				}
			}

			// Assign region_ids
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

		face_ring_t merge_faces_along_edge( const face_ring_t& A,
			const face_ring_t&                                 B,
			vert_index_t                                       u,
			vert_index_t                                       v )
		{
			auto find_edge = []( const face_ring_t& R, vert_index_t a, vert_index_t b ) -> std::optional<std::size_t>
			{
				const auto n = R.size();
				for( std::size_t i = 0; i < n; ++i )
				{
					auto r0 = R[i];
					auto r1 = R[( i + 1 ) % n];
					if( ( r0 == a && r1 == b ) || ( r0 == b && r1 == a ) )
						return i;
				}
				return std::nullopt;
			};

			const auto posA = find_edge( A, u, v );
			const auto posB = find_edge( B, u, v );
			GEOMETRIX_ASSERT( posA && posB );

			const auto nA = A.size();
			const auto nB = B.size();

			const auto iA = *posA;
			const auto iB = *posB;

			auto next = []( std::size_t i, std::size_t n )
			{ return ( i + 1 ) % n; };

			// Decide orientation so that the merged polygon is CCW.
			// We want to walk A's edge [u->v] and B's edge [v->u],
			// so their traversal directions cancel.
			bool A_u_to_v = ( A[iA] == u && A[next( iA, nA )] == v );
			bool B_u_to_v = ( B[iB] == u && B[next( iB, nB )] == v );

			// We'll build: A from v -> u (excluding edge), then B from u -> v (excluding edge),
			// or the opposite depending on the above flags.

			face_ring_t R;
			R.reserve( A.size() + B.size() - 2 );

			auto append_range = [&]( const face_ring_t& src,
									std::size_t         start,
									std::size_t         end,
									bool                forward )
			{
				const auto  n = src.size();
				std::size_t i = start;
				while( true )
				{
					R.push_back( src[i] );
					if( i == end )
						break;
					if( forward )
						i = ( i + 1 ) % n;
					else
						i = ( i + n - 1 ) % n;
				}
			};

			// Case analysis on edge directions to splice correctly.
			// One consistent choice (there are several equivalent ones):

			// A: ... u -> v ...
			// B: ... v -> u ...
			if( A_u_to_v && !B_u_to_v )
			{
				// Walk A from v to u (excluding u, v)
				auto a_start = next( iA, nA );     // v
				auto a_end = ( iA + nA - 1 ) % nA; // u
				append_range( A, a_start, a_end, true );

				// Walk B from u to v (excluding u, v)
				auto b_start = next( iB, nB );     // u
				auto b_end = ( iB + nB - 1 ) % nB; // v
				append_range( B, b_start, b_end, true );
			}
			else if( !A_u_to_v && B_u_to_v )
			{
				// A: v->u, B: u->v; symmetric case
				auto a_start = next( iA, nA );     // v
				auto a_end = ( iA + nA - 1 ) % nA; // u
				append_range( A, a_start, a_end, true );

				auto b_start = next( iB, nB );     // v
				auto b_end = ( iB + nB - 1 ) % nB; // u
				append_range( B, b_start, b_end, true );
			}
			else
			{
				// Both see u->v or both see v->u; you can reverse one ring first
				// or handle these as extra cases. I'll keep it conceptual here.
				GEOMETRIX_ASSERT( false && "Handle remaining orientation cases" );
			}

			return R;
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

			int sign = 0; // +1: left turns only, -1: right turns only

			for( std::size_t i = 0; i < n; ++i )
			{
				const auto& p0 = pts[i];
				const auto& p1 = pts[( i + 1 ) % n];
				const auto& p2 = pts[( i + 2 ) % n];

				const auto v1 = p1 - p0;
				const auto v2 = p2 - p1;
				const auto cross = exterior_product_area( v1, v2 );

				if( cmp.equals( cross, area_t{} ) )
					continue; // collinear, ignore

				const int s = cmp.greater_than( cross, area_t{} ) ? 1 : -1;
				if( sign == 0 )
				{
					sign = s;
				}
				else if( sign != s )
				{
					return false; // turned opposite way -> non-convex
				}
			}

			// For CCW polygons, you *expect* sign > 0; but "convex" only cares that it's consistent.
			return sign != 0;
		}

		const Mesh&              m_mesh;
		std::vector<region_id_t> m_region_ids;
		std::size_t              m_region_count = 0;
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

	template <typename Mesh>
	inline std::vector<std::vector<std::size_t>> build_region_boundaries( const Mesh& mesh, const std::vector<std::size_t>& region_ids )
	{
		using region_id_t = std::size_t;
		using vertex_index_t = std::size_t;

		const std::size_t triCount = mesh.get_number_triangles();
		const region_id_t regionCount = *std::max_element( region_ids.begin(), region_ids.end() ) + 1;

		std::vector<std::vector<vertex_index_t>> boundaries( regionCount );

		// For each region, find edges that appear only once
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
				if( region_ids[t] != r )
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

			// Edges appearing once are the border
			std::vector<std::pair<vertex_index_t, vertex_index_t>> border_edges;
			for( auto& kv : edge_count )
				if( kv.second == 1 )
					border_edges.push_back( kv.first );

			if( border_edges.empty() )
				continue;

			// Chain edges into a ring
			std::unordered_multimap<vertex_index_t, vertex_index_t> adj;
			for( auto& e : border_edges )
			{
				adj.emplace( e.first, e.second );
				adj.emplace( e.second, e.first );
			}

			// Start from the smallest vertex index
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
				boundaries[r] = std::move( ring );
		}

		return boundaries;
	}

	template <typename CoordinateType, typename GridTypeGenerator>
	struct region_grid_cache
	{
		using coordinate_t = CoordinateType;
		using data_t = boost::container::flat_set<std::size_t>;
		using grid_traits_t = grid_traits<coordinate_t>;
		using grid_t = typename GridTypeGenerator::template type<data_t, grid_traits_t>;

		mutable std::optional<grid_t> m_grid;

		template <typename Points, typename Regions, typename PolygonAccessor>
		region_grid_cache( const Points& points,
			const Regions&               regions,
			const PolygonAccessor&       get_region_polygon )
		{
			GEOMETRIX_ASSERT( !points.empty() );
			GEOMETRIX_ASSERT( !regions.empty() );

			// Compute total bounds from all points.
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

			absolute_tolerance_comparison_policy<coordinate_t> cmp( construct<coordinate_t>( 1e-10 ) );
			auto                                               polygon_aabb_intersection = []( const auto& poly, const auto& box )
			{
				for( auto& p : poly )
					if( box.contains( p ) )
						return true;

				return false;
			};

			//! Insert each region into the grid
			for( std::size_t r = 0; r < regions.size(); ++r )
			{
				const auto& poly = get_region_polygon( r );

				// Convert to real points
				std::vector<point_t> regionPts;
				regionPts.reserve( poly.size() );
				for( auto vidx : poly )
					regionPts.push_back( points[vidx] );

				auto [xmin, xmax, ymin, ymax] = get_bounds( regionPts, cmp );

				auto imin = gTraits.get_x_index( xmin );
				auto imax = gTraits.get_x_index( xmax );
				auto jmin = gTraits.get_y_index( ymin );
				auto jmax = gTraits.get_y_index( ymax );

				for( auto ix = imin; ix <= imax; ++ix )
					for( auto iy = jmin; iy <= jmax; ++iy )
					{
						axis_aligned_bounding_box<point_t>
							box( gTraits.get_cell_corner0( ix, iy ),
								gTraits.get_cell_corner2( ix, iy ) );

						if( polygon_aabb_intersection( regionPts, box, cmp ) ) // you need this helper
							grid.get_cell( ix, iy ).insert( r );
					}
			}
		}

		template <typename Point>
		data_t find_indices( const Point& p ) const
		{
			auto& grid = *m_grid;
			if( grid.is_contained( p ) )
				return grid.get_cell( p );
			return data_t{};
		}
	};

	template <typename Coordinate>
	class convex_mesh_2d
	{
	public:
		using coordinate_t = Coordinate;
		using point_t = geometrix::point<coordinate_t, 2>;
		using vector_t = geometrix::vector<coordinate_t, 2>;
		using tri_index_t = std::size_t;
		using vert_index_t = std::size_t;
		using region_id_t = std::size_t;

		using triangle_t = std::array<vert_index_t, 3>;
		using triangle_pts_t = std::array<point_t, 3>;
		using adjacency_row = std::array<tri_index_t, 3>;
		std::vector<std::vector<tri_index_t>> m_region_triangles;

		using segment_t = geometrix::segment<point_t>;
		using triangle_poly_t = geometrix::polygon<point_t>;
		using region_poly_t = geometrix::polygon<point_t>; // dynamic size for region outline

		std::vector<triangle_poly_t> m_triangles_geom;
		std::vector<segment_t>       m_edges_geom;
		std::vector<segment_t>       m_portals_geom;
		std::vector<region_poly_t>   m_regionPolygons;

		static constexpr tri_index_t invalid_index() noexcept
		{
			return ( std::numeric_limits<tri_index_t>::max )();
		}

		convex_mesh_2d( const std::vector<point_t>&                           vertices,
			std::vector<std::vector<vert_index_t>>&&                          region_boundaries,
			std::vector<std::vector<region_id_t>>&&                           region_adjacency,
			std::vector<std::vector<std::pair<vert_index_t, vert_index_t>>>&& region_portals )
			: m_vertices( vertices )
			, m_region_boundaries( std::move( region_boundaries ) )
			, m_region_neighbors( std::move(region_adjacency) )
			, m_region_portals( std::move(region_portals) )
		{
			build();
		}

		std::size_t                 get_number_triangles() const noexcept { return m_triangles.size(); }
		std::size_t                 get_number_vertices() const noexcept { return m_vertices.size(); }
		const std::vector<point_t>& get_vertices() const noexcept { return m_vertices; }

		const triangle_t& get_triangle_indices( tri_index_t i ) const noexcept { return m_triangles[i]; }
		triangle_pts_t    get_triangle_vertices( tri_index_t i ) const noexcept
		{
			const auto& t = m_triangles[i];
			return { m_vertices[t[0]], m_vertices[t[1]], m_vertices[t[2]] };
		}

		region_id_t get_region_of_triangle( tri_index_t i ) const noexcept { return m_tri_region[i]; }

		const adjacency_row& get_adjacent_triangles( tri_index_t i ) const noexcept
		{
			GEOMETRIX_ASSERT( i < m_adj.size() );
			return m_adj[i];
		}
		struct convex_mesh_2d_diagnostics
		{
			std::vector<std::string> errors;
			std::vector<std::string> warnings;
			bool                     ok() const { return errors.empty(); }
		};

		template <typename RMV>
		convex_mesh_2d_diagnostics validate_convex_mesh_2d( const RMV& m )
		{
			using tri_t = typename RMV::tri_index_t;
			using vi_t = typename RMV::vert_index_t;
			convex_mesh_2d_diagnostics D;

			const auto nV = m.get_number_vertices();
			const auto nT = m.get_number_triangles();

			auto push_err = [&]( const char* fmt, auto... args )
			{
				char buf[256];
				std::snprintf( buf, sizeof( buf ), fmt, args... );
				D.errors.emplace_back( buf );
			};
			auto push_warn = [&]( const char* fmt, auto... args )
			{
				char buf[256];
				std::snprintf( buf, sizeof( buf ), fmt, args... );
				D.warnings.emplace_back( buf );
			};

			//! triangles: index bounds, non-degenerate, CCW
			for( tri_t t = 0; t < nT; ++t )
			{
				const auto& idx = m.get_triangle_indices( t );
				for( int k = 0; k < 3; ++k )
					if( idx[k] >= nV )
						push_err( "T%zu has out-of-range vertex index %zu", std::size_t( t ), std::size_t( idx[k] ) );

				const auto pts = m.get_triangle_vertices( t );
				using area_t = decltype( ( pts[1][0] - pts[0][0] ) * ( pts[2][1] - pts[0][1] ) );
				const auto A = exterior_product_area( pts[1] - pts[0], pts[2] - pts[0] );
				if( A == constants::zero<area_t>() )
					push_err( "T%zu is degenerate (zero area)", std::size_t( t ) );
				else if( A < constants::zero<area_t>() )
					push_err( "T%zu is CW (expected CCW)", std::size_t( t ) );
			}

			//! adjacency symmetry and edge multiplicity
			std::unordered_map<std::pair<vi_t, vi_t>, int, RMV::template undirected_edge_hash> edge_mult;
			auto                                                                               norm_edge = []( vi_t a, vi_t b )
			{ return ( a < b ) ? std::make_pair( a, b ) : std::make_pair( b, a ); };

			for( tri_t t = 0; t < nT; ++t )
			{
				const auto& idx = m.get_triangle_indices( t );
				auto        e0 = norm_edge( idx[0], idx[1] );
				auto        e1 = norm_edge( idx[1], idx[2] );
				auto        e2 = norm_edge( idx[2], idx[0] );
				edge_mult[e0]++;
				edge_mult[e1]++;
				edge_mult[e2]++;

				const auto& adj = m.get_adjacent_triangles( t );
				for( int s = 0; s < 3; ++s )
				{
					auto nb = adj[s];
					if( nb == RMV::invalid_index() )
						continue;
					if( nb >= nT )
					{
						push_err( "Adj out of range: T%zu neighbor %zu", std::size_t( t ), std::size_t( nb ) );
						continue;
					}
					// check reciprocal
					bool        back = false;
					const auto& adjnb = m.get_adjacent_triangles( nb );
					for( int ss = 0; ss < 3; ++ss )
						if( adjnb[ss] == t )
						{
							back = true;
							break;
						}
					if( !back )
						push_err( "Adj not symmetric: T%zu -> T%zu but not back", std::size_t( t ), std::size_t( nb ) );
				}
			}
			for( auto& kv : edge_mult )
			{
				if( kv.second > 2 )
					push_err( "Edge (%zu,%zu) used by %d triangles (expected <=2)", std::size_t( kv.first.first ), std::size_t( kv.first.second ), kv.second );
			}

			// If you have the portal list handy when validating, add analogous checks here.

			return D;
		}

		const std::vector<region_id_t>& get_region_neighbors( region_id_t r ) const noexcept
		{
			return m_region_neighbors[r];
		}

		const std::vector<std::pair<vert_index_t, vert_index_t>>& get_region_portals( region_id_t r ) const noexcept
		{
			return m_region_portals[r];
		}

		const std::vector<tri_index_t>& get_region_triangles( region_id_t r ) const noexcept
		{
			return m_region_triangles[r];
		}
		
		const region_poly_t& get_region_polygon( region_id_t r ) const
		{
			GEOMETRIX_ASSERT( r < m_regionPolygons.size() );
			return m_regionPolygons[r];
		}

		const std::vector<vert_index_t>& get_region_vertices( region_id_t r ) const noexcept
		{
			GEOMETRIX_ASSERT( r < m_region_boundaries.size() );
			return m_region_boundaries[r];
		}

		const point_t& get_point( vert_index_t i ) const noexcept
		{
			GEOMETRIX_ASSERT( i < m_vertices.size() );
			return m_vertices[i];
		}

		template <typename Point, typename NumberComparisonPolicy>
		std::optional<std::size_t> find_region( const Point& p, const NumberComparisonPolicy& cmp ) const
		{
			auto cellRegions = m_regionCache.find_indices( p );
			for( std::size_t r : cellRegions )
			{
				const auto& poly = m_regionPolygons[r];
				if( point_in_convex_polygon( p, poly, cmp ) )
					return r;
			}
			return std::nullopt;
		}

	private:
		struct undirected_edge_hash
		{
			std::size_t operator()( const std::pair<std::size_t, std::size_t>& e ) const noexcept
			{
				// e is already normalized (min, max)
				return std::hash<std::size_t>()( e.first ) ^ ( std::hash<std::size_t>()( e.second ) << 1 );
			}
		};

		static std::pair<vert_index_t, vert_index_t> norm_edge( vert_index_t a, vert_index_t b )
		{
			return ( a < b ) ? std::make_pair( a, b ) : std::make_pair( b, a );
		}

		struct pair_hash
		{
			std::size_t operator()( const std::pair<vert_index_t, vert_index_t>& p ) const noexcept
			{
				return std::hash<std::size_t>()( p.first ) ^ ( std::hash<std::size_t>()( p.second ) << 1 );
			}
		};

		void set_adjacent( tri_index_t src, tri_index_t nbr, vert_index_t va, vert_index_t vb )
		{
			if( src >= m_adj.size() )
				return; // safety; should not happen now
			auto& T = m_triangles[src];
			for( int s = 0; s < 3; ++s )
			{
				auto a = T[s];
				auto b = T[( s + 1 ) % 3];
				if( ( a == va && b == vb ) || ( a == vb && b == va ) )
				{
					m_adj[src][s] = nbr;
					return;
				}
			}
			// fallback: first empty slot
			for( int s = 0; s < 3; ++s )
				if( m_adj[src][s] == invalid_index() )
				{
					m_adj[src][s] = nbr;
					return;
				}
		}

		void build()
		{
			using area_t = decltype( std::declval<coordinate_t>() * std::declval<coordinate_t>() );
			m_triangles.clear();
			m_tri_region.clear();
			m_adj.clear(); // keep adjacency in sync with m_triangles as we push

			std::unordered_map<std::pair<vert_index_t, vert_index_t>, tri_index_t, pair_hash> edge_owner;
			m_region_triangles.clear();
			m_region_triangles.resize( m_region_boundaries.size() );

			for( region_id_t r = 0; r < m_region_boundaries.size(); ++r )
			{
				const auto& ring = m_region_boundaries[r];
				if( ring.size() < 3 )
					continue;

				// centroid (as a vector sum)
				geometrix::vector<coordinate_t, 2> acc{};
				for( auto vid : ring )
					acc += as_vector( m_vertices[vid] );
				point_t centroid = point_t{} + acc / static_cast<double>( ring.size() );

				// order ring CCW
				std::vector<vert_index_t> ordered = ring;
				std::sort( ordered.begin(), ordered.end(), [&]( auto a, auto b )
					{ return vector_angle( m_vertices[a] - centroid ) < vector_angle( m_vertices[b] - centroid ); } );

				// compute signed area of the polygon order
				area_t poly_area{};
				for( std::size_t i = 0; i < ordered.size(); ++i )
				{
					const auto& p0 = m_vertices[ordered[i]];
					const auto& p1 = m_vertices[ordered[( i + 1 ) % ordered.size()]];
					poly_area += p0[0] * p1[1] - p1[0] * p0[1];
				}

				// if CW, reverse the *entire* ring so it’s CCW
				if( poly_area < constants::zero<area_t>() )
				{
					std::reverse( ordered.begin(), ordered.end() );
				}

				const auto v0 = ordered[0];
				for( std::size_t i = 1; i + 1 < ordered.size(); ++i )
				{
					// build the triangle
					typename convex_mesh_2d::triangle_t tri{ v0, ordered[i], ordered[i + 1] };

					// enforce CCW per triangle (robust against local angle mis-order)
					const auto& P0 = m_vertices[tri[0]];
					const auto& P1 = m_vertices[tri[1]];
					const auto& P2 = m_vertices[tri[2]];
					using tri_area_t = decltype( std::declval<coordinate_t>() * std::declval<coordinate_t>() );
					const tri_area_t A = exterior_product_area( P1 - P0, P2 - P0 );
					if( A <= constants::zero<tri_area_t>() )
					{
						// flip to CCW
						std::swap( tri[1], tri[2] );
					}

					const tri_index_t t_idx = static_cast<tri_index_t>( m_triangles.size() );
					m_triangles.push_back( tri );
					m_tri_region.push_back( r );
					m_region_triangles[r].push_back( t_idx );

					// edge registry + adjacency hook-up (unchanged)
					const auto e0 = norm_edge( tri[0], tri[1] );
					const auto e1 = norm_edge( tri[1], tri[2] );
					const auto e2 = norm_edge( tri[2], tri[0] );
					for( auto e : { e0, e1, e2 } )
					{
						auto it = edge_owner.find( e );
						if( it == edge_owner.end() )
						{
							edge_owner.emplace( e, t_idx );
						}
						else
						{
							const tri_index_t other = it->second;
							set_adjacent( t_idx, other, e.first, e.second );
							set_adjacent( other, t_idx, e.first, e.second );
						}
					}
				}
			}

			m_adj.assign( m_triangles.size(), adjacency_row{ invalid_index(), invalid_index(), invalid_index() } );

			// stitch portals (guard map lookups)
			for( region_id_t r = 0; r < m_region_portals.size(); ++r )
			{
				for( auto [pa, pb] : m_region_portals[r] )
				{
					auto e = norm_edge( pa, pb );

					auto itA = edge_owner.find( e );
					if( itA == edge_owner.end() )
						continue; // portal edge not seen on any triangle yet

					tri_index_t tA = itA->second;
					tri_index_t tB = invalid_index();

					// find the same edge owned by a triangle of a different region
					for( const auto& kv : edge_owner )
					{
						if( kv.first == e && m_tri_region[kv.second] != m_tri_region[tA] )
						{
							tB = kv.second;
							break;
						}
					}
					if( tA != invalid_index() && tB != invalid_index() )
					{
						set_adjacent( tA, tB, pa, pb );
						set_adjacent( tB, tA, pa, pb );
					}
				}
			}

			m_triangles_geom.clear();
			m_edges_geom.clear();
			m_portals_geom.clear();

			m_triangles_geom.reserve( m_triangles.size() );
			m_edges_geom.reserve( m_triangles.size() * 3 );

			for( const auto& t : m_triangles )
			{
				const auto& v0 = m_vertices[t[0]];
				const auto& v1 = m_vertices[t[1]];
				const auto& v2 = m_vertices[t[2]];
				polygon     npgon = { v0, v1, v2 };
				m_triangles_geom.emplace_back( std::move( npgon ) );
				m_edges_geom.emplace_back( v0, v1 );
				m_edges_geom.emplace_back( v1, v2 );
				m_edges_geom.emplace_back( v2, v0 );
			}

			for( const auto& regionPorts : m_region_portals )
				for( auto [a, b] : regionPorts )
					m_portals_geom.emplace_back( m_vertices[a], m_vertices[b] );

			// build region border polygons from the triangulated mesh
			m_regionPolygons.clear();

			for( const auto& ring : m_region_boundaries )
			{
				if( ring.size() < 3 ) [[unlikely]]
					continue;

				// Convert vertex indices to points
				std::vector<point_t> pts;
				pts.reserve( ring.size() );
				for( auto vid : ring )
					pts.push_back( m_vertices[vid] );

				point_t cpoint = get_centroid( pts );

				std::sort( pts.begin(), pts.end(), [&]( const point_t& a, const point_t& b )
					{ return vector_angle( a - cpoint ) < vector_angle( b - cpoint ); } );

				area_t area{};
				for( size_t i = 0; i + 1 < pts.size(); ++i )
					area += pts[i][0] * pts[i + 1][1] - pts[i + 1][0] * pts[i][1];
				if( area < area_t{} )
					std::reverse( pts.begin() + 1, pts.end() - 1 );

				m_regionPolygons.emplace_back( pts.begin(), pts.end() );

			}

			m_regionCache = region_grid_cache<coordinate_t, dense_grid_type_generator>( m_points, m_regionPolygons, [&]( std::size_t r ) { return m_regionPolygons[r]; } );
		}

		std::vector<point_t>                                            m_vertices;
		std::vector<std::vector<vert_index_t>>                          m_region_boundaries;
		std::vector<std::vector<region_id_t>>                           m_region_neighbors;
		std::vector<std::vector<std::pair<vert_index_t, vert_index_t>>> m_region_portals;
		std::vector<triangle_t>                                         m_triangles;
		std::vector<region_id_t>                                        m_tri_region;
		std::vector<adjacency_row>                                      m_adj;
		region_grid_cache<length_t, dense_grid_type_generator>          m_regionCache;
	};

	template <typename Mesh>
	inline std::optional<std::size_t>
	find_region_entry_triangle_from_portal(
		const Mesh&                 mesh,
		typename Mesh::region_id_t  region_id,
		typename Mesh::vert_index_t v0,
		typename Mesh::vert_index_t v1 )
	{
		using tri_index_t = typename Mesh::tri_index_t;
		const auto tri_count = mesh.get_number_triangles();

		for( tri_index_t t = 0; t < tri_count; ++t )
		{
			if( mesh.get_region_of_triangle( t ) != region_id )
				continue;

			const auto& tri = mesh.get_triangle_indices( t );
			int         count = ( tri[0] == v0 || tri[1] == v0 || tri[2] == v0 )
				+ ( tri[0] == v1 || tri[1] == v1 || tri[2] == v1 );
			if( count == 2 )
				return t;
		}

		return std::nullopt;
	}

	template <typename Mesh>
	inline std::optional<std::size_t>
	find_region_entry_triangle_from_point(
		const Mesh&                   mesh,
		typename Mesh::region_id_t    region_id,
		const typename Mesh::point_t& p )
	{
		using tri_index_t = typename Mesh::tri_index_t;
		const auto               tri_count = mesh.get_number_triangles();
		direct_comparison_policy cmp;

		for( tri_index_t t = 0; t < tri_count; ++t )
		{
			if( mesh.get_region_of_triangle( t ) != region_id )
				continue;

			const auto pts = mesh.get_triangle_vertices( t );
			if( point_in_triangle( p, pts[0], pts[1], pts[2], cmp ) )
				return t;
		}

		for( tri_index_t t = 0; t < tri_count; ++t )
			if( mesh.get_region_of_triangle( t ) == region_id )
				return t;

		return std::nullopt;
	}

	template <typename Mesh, typename NumberComparisonPolicy>
	convex_mesh_2d<typename Mesh::coordinate_t> make_convex_mesh( Mesh const& mesh, const NumberComparisonPolicy& cmp )
	{
		using namespace geometrix;

		hertel_mehlhorn_decomposition grouping( mesh );
		grouping.execute( cmp );
		const auto&                                                   region_ids = grouping.get_region_ids();
		std::vector<std::vector<std::size_t>>                         region_adjacency;
		std::vector<std::vector<std::pair<std::size_t, std::size_t>>> region_portals;
		region_adjacency_builder                                      builder( mesh, region_ids );
		builder.build( region_adjacency, region_portals );
		auto region_boundaries = build_region_boundaries( mesh, region_ids );
		return convex_mesh_2d<typename Mesh::coordinate_t>( mesh.get_vertices(), std::move(region_boundaries), std::move(region_adjacency), std::move(region_portals) );
	}

	//! \brief Edge “item” for region–mesh visibility search.
	//! Carries the current region, the previous region, and the angular wedge [lo,hi].
	template <typename RegionMesh>
	struct region_edge_item
	{
		using region_id_t = typename RegionMesh::region_id_t;
		using vector_t = typename RegionMesh::vector_t;
		using length_t = typename arithmetic_type_of<vector_t>::type;
	
		bool        is_all_around() const { return get<0>( lo ) == constants::infinity<length_t>() && get<0>( hi ) == constants::negative_infinity<length_t>(); }
		bool        is_from_start() const { return from == ( std::numeric_limits<std::size_t>::max )(); }
		bool        is_directed() const { return !( is_from_start() || is_all_around() ); }
		
		region_id_t from; //! previous region, or max() if none
		region_id_t to;   //! current region
		vector_t    lo;          //! lower wedge bound (vector from origin)
		vector_t    hi;          //! upper wedge bound (vector from origin)
		bool        directed = true;
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
			m_start.from= invalid_region();
			m_start.to= startRegion;
			m_start.lo = vector_t( constants::infinity<length_t>(), constants::zero<length_t>() );
			m_start.hi = vector_t( constants::negative_infinity<length_t>(), constants::zero<length_t>() );
			m_start.directed = false;
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
		std::optional<edge_item> prepare_adjacent_traversal( region_id_t neighborRegion, const edge_item& current ) const
		{
			using area_t = decltype( std::declval<typename vector_t::value_type>() * std::declval<typename vector_t::value_type>() );

			if( neighborRegion == invalid_region() || neighborRegion == current.from )
				return std::nullopt;

			//! Locate the portal edge from current.to to neighborRegion.
			auto const& neighbors = m_mesh->get_region_neighbors( current.to );
			auto const& portals = m_mesh->get_region_portals( current.to );

			GEOMETRIX_ASSERT( neighbors.size() == portals.size() );

			std::size_t portalIndex = ( std::numeric_limits<std::size_t>::max )();
			for( std::size_t i = 0; i < neighbors.size(); ++i )
			{
				if( neighbors[i] == neighborRegion )
				{
					portalIndex = i;
					break;
				}
			}
			if( portalIndex == ( std::numeric_limits<std::size_t>::max )() )
				return std::nullopt; //! No explicit portal recorded; treat as non-adjacent for search purposes.

			const auto& portal = portals[portalIndex];
			const auto& verts = m_mesh->get_vertices();
			GEOMETRIX_ASSERT( portal.first < verts.size() );
			GEOMETRIX_ASSERT( portal.second < verts.size() );

			point_t pointLo = verts[portal.first];
			point_t pointHi = verts[portal.second];
			if( current.is_directed() && !is_segment_in_range_2d( make_segment( pointLo, pointHi ), current.lo, current.hi, m_origin ) )
				return std::nullopt;

			if( exterior_product_area( pointHi - pointLo, m_origin - pointLo ) < area_t{} )
				std::swap( pointLo, pointHi );

#if GEOMETRIX_DEBUG_CONVEX_REGION_MESH
			auto Su = make_segment<point_t>( m_origin, pointLo );
			auto Sv = make_segment<point_t>( m_origin, pointHi );

			auto eLo = make_segment<point_t>( m_origin, m_origin + 1000.0 * current.lo );
			auto eHi = make_segment<point_t>( m_origin, m_origin + 1000.0 * current.hi );
#endif

			vector_t vecLo, vecHi;
            direct_comparison_policy cmp;
			if( !numeric_sequence_equals_2d( m_origin, pointLo, cmp ) && !numeric_sequence_equals_2d( m_origin, pointHi, cmp ) )
			{
				assign( vecLo, pointLo - m_origin );
				assign( vecHi, pointHi - m_origin );

				if( current.is_directed() )
				{
					vecLo = is_vector_between( current.lo, current.hi, vecLo, false, cmp ) ? vecLo : current.lo;
					vecHi = is_vector_between( current.lo, current.hi, vecHi, false, cmp ) ? vecHi : current.hi;
				}

				if( get_orientation( vecHi, vecLo, cmp ) == geometrix::oriented_left )
					return std::nullopt;
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

			edge_item next;
			next.from = current.to;
			next.to = neighborRegion;
			next.lo = vecLo;
			next.hi = vecHi;
			return next;
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
	//! RegionSearch must expose:
	//!   using edge_item;
	//!   bool visit(const edge_item&);
	//!   std::optional<edge_item> prepare_adjacent_traversal(region_id_t, const edge_item&);
	//!   edge_item get_start() const;
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

			if( !search.visit( item ) )
				continue;

			const auto& neighbors = mesh.get_region_neighbors( item.to );
			for( region_id_t nbr : neighbors )
			{
				if( nbr == invalid || nbr == item.from )
					continue;

				if( auto nextItem = search.prepare_adjacent_traversal( nbr, item ) )
					stack.push_back( std::move( *nextItem ) );
			}
		}
	}

} // namespace geometrix
