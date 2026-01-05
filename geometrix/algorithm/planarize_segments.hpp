//
//! Copyright © 2025
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <vector>
#include <algorithm>

#include <geometrix/primitive/segment.hpp>
#include <geometrix/algorithm/grid_traits.hpp>
#include <geometrix/algorithm/fast_voxel_grid_traversal.hpp>
#include <geometrix/algorithm/hash_grid_2d.hpp>
#include <geometrix/utility/utilities.hpp>
#include <boost/container/small_vector.hpp>
#include <ranges>

namespace geometrix {

	//! Planarize a set of segments by inserting intersection vertices and splitting.
	//! Uses a hash grid + fast voxel traversal to localize segment pair tests.
	//!
	//! Requirements:
	//!  * Segment supports get_start(), get_end().
	//!  * segment_segment_intersection(...) is available and robust under cmp, eps.
	//!  * GridTraits must be compatible with fast_voxel_grid_traversal (min_x, max_x, cell_size, etc.).
	//!
	template <typename GridTraits, typename Segments, typename Visitor, typename Length, typename NumberComparisonPolicy>
	void planarize_segments_hash_grid(
		const GridTraits&             gridTraits, //! Typically a grid_traits_2d over the global bbox.
		const Segments&               segs,
		Visitor&&                     visitor,
		const Length&                 tol,
		const NumberComparisonPolicy& cmp
	)
	{
		using arithmetic_type = Length;
		using dimensionless_t = decltype( Length() / Length() );
		using point_t = geometrix::point<Length, 2>;

		const std::size_t N = segs.size();
		if( !N ) [[unlikely]]
			return;

		//! Each segment gets a list of parameter values t in [0,1].
		//! Initialize with endpoints.
		std::vector<std::vector<dimensionless_t>> tvals( N );
		for( std::size_t i = 0; i < N; ++i )
		{
			tvals[i].push_back( dimensionless_t() );
			tvals[i].push_back( constants::one<dimensionless_t>() );
		}

		//! Hash grid: each cell stores indices of segments that pass through it.
		typedef boost::container::small_vector<std::size_t, 5> cell_data_t;
		typedef hash_grid_2d<cell_data_t, GridTraits> grid_t;

		grid_t grid( gridTraits );

		//!
		//! For each segment, traverse the grid using fast_voxel_grid_traversal.
		//! At each visited cell, intersect with segments already in that cell.
		//!
		for( std::size_t i = 0; i < N; ++i )
		{
			const auto& si = segs[i];

			auto gvisitor = [&]( boost::uint32_t ci, boost::uint32_t cj )
			{
				cell_data_t& cell = grid.get_cell( ci, cj );

				//! Compare si with all segments already in this cell.
				for (std::size_t j : cell)
				{
					if (j == i)
						continue;

					const auto& sj = segs[j];

					point_t I[2];
					auto        itype = segment_segment_intersection( si, sj, I, cmp );
					const auto& A = get_start(si);
					const auto& B = get_end(si);
					const auto& C = get_start(sj);
					const auto& D = get_end(sj);

					//! Small helpers
					auto project_param = [&](const auto& P, const auto& S0, const auto& S1)
					{
						arithmetic_type dx = get<0>(S1) - get<0>(S0);
						arithmetic_type dy = get<1>(S1) - get<1>(S0);
						auto denom = dx*dx + dy*dy;
						if (cmp.equals(denom, decltype(denom){}))
							return dimensionless_t{}; //! degenerate segment
						return ((get<0>(P) - get<0>(S0))*dx + (get<1>(P) - get<1>(S0))*dy) / denom;
					};

					auto push_if_interior = [&](auto t, auto& list)
					{
						if (cmp.greater_than(t, decltype(t){}) && cmp.less_than( t, constants::one<decltype(t)>() ) )
							list.push_back(t);
					};

					//! EPS-SNAP: detect near-endpoint closeness and treat as intersection
					auto sqdist = [&](const auto& P, const auto& Q)
					{
						auto dx = get<0>( P ) - get<0>( Q );
						auto dy = get<1>( P ) - get<1>( Q );
						return dx*dx + dy*dy;
					};

					//! snap distance squared
					const auto snap2 = tol * tol;

					//! If endpoints are within eps, treat as endpoint intersection.
					bool snapped = false;

					// A near C
					if (cmp.less_than(sqdist(A,C), snap2)) { snapped = true; }
					// A near D
					else if (cmp.less_than(sqdist(A,D), snap2)) { snapped = true; }
					// B near C
					else if (cmp.less_than(sqdist(B,C), snap2)) { snapped = true; }
					// B near D
					else if (cmp.less_than(sqdist(B,D), snap2)) { snapped = true; }

					if (snapped)
					{
						//! Do NOT split either segment.
						//! The endpoint t=0 or t=1 already exists in tvals[],
						//! so topology remains correct.
						continue;
					}

					// Skip completely non-intersecting or near-endpoint cases.
					if (itype == intersection_type::e_non_crossing || itype == intersection_type::e_endpoint)
						continue;

					//! Overlapping collinear segments -> split at both overlap endpoints
					if (itype == intersection_type::e_overlapping)
					{
						// Overlap endpoints are returned in I0, I1.
						auto ti0 = project_param(I[0], A, B);
						auto ti1 = project_param(I[1], A, B);
						auto tj0 = project_param(I[0], C, D);
						auto tj1 = project_param(I[1], C, D);

						push_if_interior(ti0, tvals[i]);
						push_if_interior(ti1, tvals[i]);
						push_if_interior(tj0, tvals[j]);
						push_if_interior(tj1, tvals[j]);
						continue;
					}

					// -----------------------------------------------------------------------
					// 4. Proper crossing (including oriented L->R and R->L)
					// -----------------------------------------------------------------------
					if (itype == intersection_type::e_crossing)
					{
						// Param on si
						{
							auto dx = get<0>(B) - get<0>(A);
							auto dy = get<1>(B) - get<1>(A);
							auto denom = dx*dx + dy*dy;

							if (!cmp.equals(denom, decltype(denom){}))
							{
								auto ti = ((get<0>(I[0]) - get<0>(A))*dx + (get<1>(I[0]) - get<1>(A))*dy) / denom;

								// EPS-SNAP interior check
								if (cmp.greater_than(ti, dimensionless_t{}) &&
									cmp.less_than(ti, constants::one<dimensionless_t>()))
								{
									tvals[i].push_back(ti);
								}
							}
						}

						// Param on sj
						{
							arithmetic_type dx = get<0>(D) - get<0>(C);
							arithmetic_type dy = get<1>(D) - get<1>(C);
							auto denom = dx*dx + dy*dy;

							if (!cmp.equals(denom, decltype(denom){}))
							{
								auto tj = ((get<0>(I[0]) - get<0>(C))*dx + (get<1>(I[0]) - get<1>(C))*dy) / denom;
								if (cmp.greater_than(tj, dimensionless_t{}) &&
									cmp.less_than(tj, constants::one<dimensionless_t>()))
								{
									tvals[j].push_back(tj);
								}
							}
						}

						continue;
					}

					//! Anything else -> ignore
				}

				//! Register si in this cell so later segments can see it.
				cell.push_back(i);
			};

			fast_voxel_grid_traversal(gridTraits, si, gvisitor, cmp);
		}

		//! Split each segment according to its t-values.
		using std::abs;
		for (std::size_t i = 0; i < N; ++i)
		{
			auto& tv = tvals[i];
			if (tv.size() <= 2)
			{
				//! No intersections, keep original.
				visitor(get_start(segs[i]), get_end(segs[i]));
				continue;
			}

			//! Sort and dedup t-values under eps.
			std::sort(tv.begin(), tv.end());
			std::vector<dimensionless_t> uniq;
			uniq.reserve(tv.size());
			for (auto t : tv)
			{
				if (uniq.empty() || cmp.greater_than(abs(t - uniq.back()), dimensionless_t{}))
				{
					uniq.push_back(t);
				}
			}

			const auto& A = get_start(segs[i]);
			const auto& B = get_end(segs[i]);

			for (std::size_t k = 0; k + 1 < uniq.size(); ++k)
			{
				auto t0 = uniq[k];
				auto t1 = uniq[k + 1];

				if (cmp.less_than(abs(t1 - t0), dimensionless_t{}))
					continue;

				auto P0 = point_t{
					get<0>(A) + (get<0>(B) - get<0>(A)) * t0,
					get<1>(A) + (get<1>(B) - get<1>(A)) * t0
				};

				auto P1 = point_t{
					get<0>(A) + (get<0>(B) - get<0>(A)) * t1,
					get<1>(A) + (get<1>(B) - get<1>(A)) * t1
				};

				visitor(P0, P1);
			}
		}
	}
	
	template <typename Segments, typename NumberComparisonPolicy, typename Length, typename Visitor>
	void planarize_segments(
		const Segments&               segs,
		const Length&                 cell_size,
		const NumberComparisonPolicy& cmp,
		const Length&                 tol,
		Visitor&&                     visitor
	)
	{
		using arithmetic_type = Length;

		arithmetic_type minx = +std::numeric_limits<arithmetic_type>::infinity();
		arithmetic_type miny = +std::numeric_limits<arithmetic_type>::infinity();
		arithmetic_type maxx = -std::numeric_limits<arithmetic_type>::infinity();
		arithmetic_type maxy = -std::numeric_limits<arithmetic_type>::infinity();

		for( const auto& s : segs )
		{
			const auto& a = get_start( s );
			const auto& b = get_end( s );

			minx = std::min( minx, std::min( get<0>(a), get<0>(b) ) );
			miny = std::min( miny, std::min( get<1>(a), get<1>(b) ) );
			maxx = std::max( maxx, std::max( get<0>(a), get<0>(b) ) );
			maxy = std::max( maxy, std::max( get<1>(a), get<1>(b) ) );
		}

		return planarize_segments_hash_grid(
			grid_traits<arithmetic_type>{ minx, maxx, miny, maxy, cell_size },
			segs,
			std::forward<Visitor>( visitor ),
			tol,
			cmp );
	}
} // namespace geometrix

