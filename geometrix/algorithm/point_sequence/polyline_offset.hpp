//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_POINTSEQUENCE_POLYLINEOFFSET_HPP
#define GEOMETRIX_ALGORITHM_POINTSEQUENCE_POLYLINEOFFSET_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>

#include <geometrix/algorithm/line_intersection.hpp>
#include <algorithm>

namespace geometrix {

    template <typename Polyline, typename Length, typename NumberComparisonPolicy>
	inline Polyline polyline_offset(const Polyline& poly, orientation_type side, const Length& offset, const NumberComparisonPolicy& cmp)
	{
		using access = point_sequence_traits<Polyline>;
		using point_type = typename point_sequence_traits<Polyline>::point_type;
		using vector_type = vector<Length, dimension_of<point_type>::value>;

		std::size_t size = access::size(poly);
		GEOMETRIX_ASSERT(size > 1);

		auto A = access::get_point(poly, 0);		
		auto B = access::get_point(poly, 1);
		vector_type vNormalAB = offset * (side == oriented_right ? right_normal(normalize(B - A)) : left_normal(normalize(B - A)));
		auto pOffsetA = construct<point_type>(A + vNormalAB);
		auto pOffsetB = construct<point_type>(B + vNormalAB);
		Polyline result = { pOffsetA };
		for( std::size_t d = 2; d < size; ++d )
		{
			auto& C = B;
			auto D = access::get_point(poly, d);

			auto vNormalCD = offset * (side == oriented_right ? right_normal(normalize(D - C)) : left_normal(normalize(D - C)));
			auto pOffsetC = construct<point_type>(C + vNormalCD);
			auto pOffsetD = construct<point_type>(D + vNormalCD);
			
			point_type xPoint;
			GEOMETRIX_VERIFY(line_line_intersect(pOffsetA, pOffsetB, pOffsetC, pOffsetD, xPoint, cmp) != e_non_crossing);
			result.push_back(xPoint);

			A = C;
			B = D;
			pOffsetA = pOffsetC;
			pOffsetB = pOffsetD;
			
			if (d + 1 == size)
				result.push_back(pOffsetD);			
		}
		
		return result;
    }

}//namespace geometrix;

#endif //! GEOMETRIX_ALGORITHM_POINTSEQUENCE_POLYLINEOFFSET_HPP
