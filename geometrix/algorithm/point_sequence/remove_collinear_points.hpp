//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_REMOVE_COLLINEAR_POINTS_HPP
#define GEOMETRIX_REMOVE_COLLINEAR_POINTS_HPP
#pragma once

#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/utility/utilities.hpp>

namespace geometrix {

    template <typename Polygon, typename NumberComparisonPolicy>
    inline Polygon remove_collinear_points_polygon( const Polygon& poly, const NumberComparisonPolicy& cmp )
    {
        typedef point_sequence_traits<Polygon> access;
                
        std::size_t size = access::size( poly );
		if (size < 4)
			return poly;
		
		//! TODO: need to generically construct polygons.
		Polygon newPoly{ poly.front(), access::get_point(poly, 1) };
		for (std::size_t i = 2; i < size; ++i)
		{
			const auto& a = access::get_point(newPoly, access::size(newPoly) - 2);
			auto& b = access::back(newPoly);
			const auto& c = poly[i];
			if (!is_collinear(a, b, c, cmp))
			{
				if( i + 1 < size || !is_collinear(b, c, access::front(poly), cmp))
					newPoly.push_back(c);				
			}
			else
				assign(b, c);//! replace b with c.		
		}

		return std::move( newPoly );
    }

	template <typename Polyline, typename NumberComparisonPolicy>
	inline Polyline remove_collinear_points_polyline(const Polyline& poly, const NumberComparisonPolicy& cmp)
	{
		typedef point_sequence_traits<Polyline> access;

		std::size_t size = access::size(poly);
		if (size < 3)
			return poly;

		//! TODO: need to generically construct polylines.
		Polyline newPoly{ poly.front(), access::get_point(poly, 1) };
		for (std::size_t i = 2; i < size; ++i)
		{
			const auto& a = access::get_point(newPoly, access::size(newPoly) - 2);
			auto& b = access::back(newPoly);
			const auto& c = poly[i];
			if (!is_collinear(a, b, c, cmp))
				newPoly.push_back(c);
			else
				assign(b, c);//! replace b with c.		
		}

		return std::move(newPoly);
	}

}//namespace geometrix;

#endif //! GEOMETRIX_REMOVE_COLLINEAR_POINTS_HPP
