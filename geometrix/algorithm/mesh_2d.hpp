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
				std::size_t index1 = indices[i + 1];
				std::size_t index2 = indices[i + 2];

				//! Triangles should be CCW.
				if (get_orientation(points[index0], points[index1], points[index2], cmp) == oriented_right)
				{
					indices[i + 1] = index2;
					indices[i + 2] = index1;
					index1 = indices[i + 1];
					index2 = indices[i + 2];
				}

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

	private:

		std::size_t m_numberTriangles;
		std::vector< point<double, 2> > m_points;
		std::vector<std::size_t>       m_indices;

	};
}//! namespace geometrix

#endif // GEOMETRIX_MESH_2D_HPP
