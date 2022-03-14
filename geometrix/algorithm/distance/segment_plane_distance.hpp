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
		template <typename Segment, typename Plane>
		struct signed_segment_plane_distance
		{
			using length_t = typename arithmetic_type_of<Segment>::type;
			using type = length_t;
		};
	}
	
	template <typename Segment, typename Plane>
	inline typename result_of::signed_segment_plane_distance<Segment, Plane>::type signed_segment_plane_distance(const Segment& s, const Plane& l)
	{
        using access = plane_access_traits<typename std::decay<Plane>::type>;
		using length_t = typename arithmetic_type_of<Segment>::type;
        using area_t = decltype(std::declval<length_t>()*std::declval<length_t>());
       
		auto d1 = scalar_projection(get_start(s) - access::get_reference_point(l), access::get_normal_vector(l));
		auto d2 = scalar_projection(get_end(s) - access::get_reference_point(l), access::get_normal_vector(l));
        if( (d1*d2) >= constants::zero<area_t>() )
        {
		    using std::abs;
            //! both on same side. Find closest point of the closest.
            if( abs(d1) < abs(d2) )
                return d1;
            return d2;
        }

        //! It intersects.
        return constants::zero<length_t>();
	}

	namespace result_of {
		template <typename Segment, typename Plane>
		struct segment_plane_distance
		{
			using length_t = typename arithmetic_type_of<Segment>::type;
			using type = length_t;
		};
	}

	template <typename Segment, typename Plane>
	inline typename result_of::segment_plane_distance<Segment, Plane>::type segment_plane_distance(const Segment& s, const Plane& l)
	{
		using std::abs;
		return abs(signed_point_plane_distance(s, l));
	}

	namespace result_of {
		template <typename Segment, typename Plane>
		struct segment_plane_distance_sqrd
		{
			using length_t = typename arithmetic_type_of<Segment>::type;
			using type = decltype(std::declval<length_t>() * std::declval<length_t>());
		};
	}
	
    template <typename Segment, typename Plane>
	inline typename result_of::segment_plane_distance_sqrd<Segment, Plane>::type segment_plane_distance_sqrd(const Segment& s, const Plane& l)
	{
		auto l = signed_point_plane_distance(s, l);
        return l*l;
	}

}//namespace geometrix;
