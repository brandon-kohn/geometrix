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
#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/algorithm/grid_2d.hpp>
#include <geometrix/algorithm/eberly_triangle_aabb_intersection.hpp>

#include <boost/optional.hpp>
#include <boost/utility/typed_in_place_factory.hpp>
#include <boost/container/flat_set.hpp>
#include <array>

namespace geometrix
{
	class mesh_2d
	{
	public:

		template <typename Points, typename Indices, typename NumberComparisonPolicy>
		mesh_2d(const Points& points, Indices indices, const NumberComparisonPolicy& cmp)
			: m_numberTriangles()
		{
			std::size_t numberTriangles = indices.size() / 3;

			for (std::size_t triangleIndex = 0; triangleIndex < numberTriangles; ++triangleIndex)
			{
				std::size_t i = triangleIndex * 3;
				std::size_t index0 = indices[i];
				std::size_t& index1 = indices[i + 1];
				std::size_t& index2 = indices[i + 2];

				//! Triangles should be CCW.
				if (get_orientation(points[index0], points[index1], points[index2], cmp) == oriented_right)
					std::swap( index1, index2 );

				//! Triangles should not be degenerate.
				if (numeric_sequence_equals(points[index0], points[index1], cmp) ||
					numeric_sequence_equals(points[index1], points[index2], cmp) ||
					numeric_sequence_equals(points[index2], points[index0], cmp))
					continue;

				m_points.push_back(construct< point<double, 2> >(points[index0]));
				m_indices.push_back(index0);

				m_points.push_back(construct< point<double, 2> >(points[index1]));
				m_indices.push_back(index1);

				m_points.push_back(construct< point<double, 2> >(points[index2]));
				m_indices.push_back(index2);

				++m_numberTriangles;
			}
		}

		//! Calculate a random interior position. Parameters rT, r1, and r2 should be uniformly distributed random numbers in the range of [0., 1.].
		point<double, 2> get_random_position(double rT, double r1, double r2)
		{
			BOOST_ASSERT(0. <= rT && rT <= 1.);
			BOOST_ASSERT(0. <= r1 && rT <= 1.);
			BOOST_ASSERT(0. <= r2 && rT <= 1.);

			std::size_t index0 = static_cast<std::size_t>(rT * (m_numberTriangles - 1)) * 3;
			std::size_t index1 = index0 + 1;
			std::size_t index2 = index1 + 1;
			double sqrt_r1 = sqrt(r1);

			return (1 - sqrt_r1) * as_vector(m_points[index0]) + sqrt_r1 * (1 - r2) * as_vector(m_points[index1]) + sqrt_r1 * r2 * as_vector(m_points[index2]);
		}

		std::size_t get_number_triangles() const { return m_numberTriangles; }
		std::size_t get_number_vertices() const { return m_points.size(); }
		const std::size_t* get_triangle_indices( std::size_t i ) const { return &m_indices[i * 3]; }
		const point<double, 2>* get_triangle_vertices( std::size_t i ) const { return &m_points[i * 3]; }
		
		typedef std::vector<std::array<std::size_t, 3>> adjacency_matrix;
		const adjacency_matrix& get_adjacency_matrix() const
		{
			if( !m_adjMatrix )
				create_adjacency_matrix();
			return *m_adjMatrix;
		}

		template <typename Point, typename NumberComparisonPolicy>
		boost::optional<std::size_t> find_triangle( const Point& p, const NumberComparisonPolicy& cmp )
		{
			if( !m_grid )
				create_grid();
			auto& grid = *m_grid;
			if( grid.is_contained( p ) )
			{
				const auto& data = grid.get_cell( p );
				for( std::size_t ti : data )
				{
					const auto* points = get_triangle_vertices( ti );
					if( point_in_triangle( p, points[0], points[1], points[2], cmp ) )
						return ti;
				}
			}

			return boost::none;
		}

	private:

		void create_adjacency_matrix() const
		{
			std::array<std::size_t, 3> defaultArray = {-1, -1, -1};
			m_adjMatrix = boost::in_place<adjacency_matrix>( m_numberTriangles, defaultArray );
			auto& adjMatrix = *m_adjMatrix;
			enum class trig_side { zero, one, two };
			struct adj_item { std::size_t index; trig_side side; };
			std::map<std::pair<std::size_t, std::size_t>, std::vector<adj_item>> adjTriangles;

			for( std::size_t i = 0; i < m_numberTriangles; ++i )
			{
				const std::size_t* indices = get_triangle_indices( i );
				adjTriangles[std::make_pair( indices[0], indices[1] )].push_back( adj_item{i, trig_side::zero} );
				adjTriangles[std::make_pair( indices[1], indices[2] )].push_back( adj_item{i, trig_side::one} );
				adjTriangles[std::make_pair( indices[2], indices[0] )].push_back( adj_item{i, trig_side::two} );
			}

			for( const auto& item : adjTriangles )
			{
				const auto& key = item.first;
				const auto& itrigs = item.second;
				auto it = adjTriangles.find( std::make_pair( key.second, key.first ) );
				if( it != adjTriangles.end() )
				{
					//! all the trigs in it collection are adjacent to the trigs in itrigs.
					for( auto const& i : itrigs )
					{
						for( auto const& j : it->second )
						{
							adjMatrix[i.index][static_cast<std::size_t>(i.side)] = j.index;
							adjMatrix[j.index][static_cast<std::size_t>(j.side)] = i.index;
						}
					}
				}
			}
		}

		void create_grid()
		{
			auto bounds = get_bounds( m_points, absolute_tolerance_comparison_policy<double>(0) );
			double xmin, xmax, ymin, ymax;
			grid_traits<double> gTraits( bounds, 1.0 );
			m_grid = boost::in_place<grid_2d<boost::container::flat_set<std::size_t>>>( gTraits );
			auto& grid = *m_grid;

			//! add each triangle
			for( std::size_t i = 0; i < m_numberTriangles; ++i )
			{
				auto* points = get_triangle_vertices( i );
				std::vector<point<double, 2>> trig( points, points + 3 );
				boost::tie( xmin, ymin, xmax, ymax ) = get_bounds( trig, absolute_tolerance_comparison_policy<double>( 0 ) );
				auto imin = gTraits.get_x_index( xmin );
				auto imax = gTraits.get_x_index( xmax );
				auto jmin = gTraits.get_y_index( ymin );
				auto jmax = gTraits.get_y_index( ymax );
				for( auto col = imin; col <= imax; ++col )
					for( auto row = jmin; row <= jmax; ++row )
					{
						axis_aligned_bounding_box<point<double,2>> box( gTraits.get_cell_corner0( col, row ), gTraits.get_cell_corner2( col, row ) );
						if( eberly_triangle_aabb_intersection_2d( trig[0], trig[1], trig[2], box, absolute_tolerance_comparison_policy<double>( 1e-10 ) ) )
							grid.get_cell( col, row ).insert( i );
					}
			}
		}
		
		std::size_t m_numberTriangles;
		std::vector< point<double, 2> > m_points;
		std::vector<std::size_t> m_indices;
		mutable boost::optional<adjacency_matrix> m_adjMatrix;
		mutable boost::optional<grid_2d<boost::container::flat_set<std::size_t>>> m_grid;

	};
}//! namespace geometrix

#endif // GEOMETRIX_MESH_2D_HPP
