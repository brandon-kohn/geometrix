//
//! Copyright © 2022
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
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/utility/assert.hpp>
#include <geometrix/algorithm/distance/point_plane_closest_point.hpp>
#include <geometrix/algorithm/distance/point_plane_distance.hpp>
#include <geometrix/algorithm/intersection/segment_plane_intersection.hpp>

namespace geometrix {
    namespace result_of {
        template <typename Segment, typename Plane, typename EnableIf=void>
		struct segment_plane_closest_point;

        template <typename Segment, typename Plane>
		struct segment_plane_closest_point<Segment, Plane, typename geometric_traits<Plane>::is_plane>
		{
			using length_t = typename select_arithmetic_type_from_sequences<typename geometric_traits<Plane>::point_type, typename point_type_of<Segment>::type>::type;
			using type = point<length_t, dimension_of<Segment>::value>;
		};
    }//! namespace result_of;

    template <typename Segment, typename Plane>
    inline typename result_of::segment_plane_closest_point<Segment, Plane>::type segment_plane_closest_point(const Segment& s, const Plane& pl)
    {
        using spoint_t = typename point_type_of<Segment>::type;
        using length_t = typename select_arithmetic_type_from_sequences<typename geometric_traits<Plane>::point_type, spoint_t>::type;
        using area_t = decltype(std::declval<length_t>()*std::declval<length_t>());
        using point_t = point<length_t, dimension_of<Segment>::value>;
        using dimensionless_t = typename dimensionless_type_of<point_t>::type;
        using access = plane_access_traits<Plane>;

        auto d1 = signed_point_plane_distance(get_start(s), pl);
        auto d2 = signed_point_plane_distance(get_end(s), pl);

        if( (d1*d2) >= constants::zero<area_t>() )
        {
            using std::abs;
            //! both on same side. Find closest point of the closest.
            if( abs(d1) < abs(d2) )
                return point_plane_closest_point(get_start(s), pl);
            return point_plane_closest_point(get_end(s), pl);
        }

        //! It intersects.
        auto xpoint = point_t{};
        auto t = dimensionless_t{}; 
        GEOMETRIX_VERIFY(segment_plane_intersection(get_start(s), get_end(s), pl, t, xpoint)); 
        return xpoint;
    }
    
    template <typename Segment, typename Line>
    inline typename result_of::segment_plane_closest_point<Segment, Line>::type segment_line_closest_point(const Segment& s, const Line& pl)
    {
        return segment_plane_closest_point(s, pl);
    }
}//! namespace geometrix;

