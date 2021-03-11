//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/plane.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/utilities.hpp>

namespace geometrix {
    namespace result_of
	{
        template <typename Point, typename Plane, typename T3 = void>
		struct point_plane_closest_point;

        template <typename Point, typename Plane>
		struct point_plane_closest_point<Point, Plane, typename geometric_traits<Plane>::is_plane>
		{
			using length_t = typename select_arithmetic_type_from_sequences<typename geometric_traits<Plane>::point_type, Point>::type;
			using type = point<length_t, dimension_of<Point>::value>;
		};
        
        template <typename Point, typename T1, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void, typename T6 = void, typename T7 = void>
		struct point_line_closest_point;
       
        template <typename Point, typename Line>
		struct point_line_closest_point<Point, Line, typename geometric_traits<Line>::is_line> : point_plane_closest_point<Point, Line>{};
        
        template <typename Point, typename Point1, typename Point2>
		struct point_line_closest_point<Point, Point1, Point2 , typename geometric_traits<Point>::is_point , typename geometric_traits<Point1>::is_point , typename geometric_traits<Point2>::is_point > 
        {
			using length_t = typename select_arithmetic_type_from_sequences<Point1, Point>::type;
			using type = point<length_t, dimension_of<Point>::value>;
        };
    }

    template <typename Point, typename Plane>
    inline typename result_of::point_plane_closest_point<Point, Plane>::type point_plane_closest_point(const Point& p, const Plane& pl)
    {
        using length_t = typename select_arithmetic_type_from_sequences<typename geometric_traits<Plane>::point_type, Point>::type;
        using point_t = point<length_t, dimension_of<Point>::value>;
        using access = plane_access_traits<Plane>;
        auto n = access::get_normal_vector(pl);
        auto t = (scalar_projection(as_vector(p), n) - access::get_distance_to_origin(pl)) / scalar_projection(n,n);
        return point_t{ p - t * n };
    }
    
    template <typename Point, typename Line>
    inline typename result_of::point_line_closest_point<Point, Line>::type point_line_closest_point(const Point& p, const Line& pl)
    {
        return point_plane_closest_point(p, pl);
    }
    
    template <typename Point, typename Point1, typename Point2>
    inline typename result_of::point_line_closest_point<Point, Point1, Point2>::type point_line_closest_point(const Point& p, const Point1& p1, const Point2& p2)
    {
        using length_t = typename select_arithmetic_type_from_sequences<Point1, Point>::type;
        using point_t = point<length_t, dimension_of<Point>::value>;
        auto n = left_normal(normalize(p2-p1));
        auto t = (scalar_projection(as_vector(p), n) - scalar_projection(p1, n)) / scalar_projection(n,n);
        return point_t{ p - t * n };
    }

}//! namespace geometrix;

