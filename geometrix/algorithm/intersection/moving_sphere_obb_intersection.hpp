//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MOVING_SPHERE_OBB_INTERSECTION_HPP
#define GEOMETRIX_MOVING_SPHERE_OBB_INTERSECTION_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/algorithm/intersection/moving_sphere_segment_intersection.hpp>
#include <geometrix/algorithm/intersection/ray_aabb_intersection.hpp>
#include <geometrix/algorithm/distance/point_segment_closest_point.hpp>
#include <geometrix/algorithm/rotation.hpp>

namespace geometrix {

    struct moving_sphere_obb_intersection_result
    {
        typedef void (moving_sphere_obb_intersection_result::*bool_type)() const;

        moving_sphere_obb_intersection_result()
        {}

        moving_sphere_obb_intersection_result(const moving_sphere_segment_intersection_result& r)
            : result((r.is_intersecting() ? e_is_intersecting : 0) | (r.is_on_line_at_start() ? e_is_on_line_at_start : 0) | (r.is_endpoint() ? e_is_endpoint : 0) | (r.is_penetrating() ? e_is_penetrating : 0) | (r.is_moving_away() ? e_is_moving_away : 0))
        {}

        moving_sphere_obb_intersection_result(bool isIntersecting, bool isAlreadyOnLine, bool isEndpoint, bool isPenetrating, bool isMovingAway)
            : result((isIntersecting ? e_is_intersecting : 0) | (isAlreadyOnLine ? e_is_on_line_at_start : 0) | (isEndpoint ? e_is_endpoint : 0) | (isPenetrating ? e_is_penetrating : 0) | (isMovingAway ? e_is_moving_away : 0))
        {}

        bool is_intersecting() const { return (result & e_is_intersecting) != 0; }
        bool is_on_line_at_start() const { return (result & e_is_on_line_at_start) != 0; }
        bool is_endpoint() const { return (result & e_is_endpoint) != 0; }
        bool is_penetrating() const { return (result & e_is_penetrating) != 0; }
        bool is_touching() const { return is_intersecting() && !is_penetrating(); }
        bool is_moving_away() const { return (result & e_is_moving_away) != 0; }

        operator bool_type() const {
            return is_intersecting() ? &moving_sphere_obb_intersection_result::bool_type_mfn : 0;
        }

    private:

        void bool_type_mfn() const {}

        enum Flags
        {
            e_is_intersecting = 1
          , e_is_on_line_at_start = 2
          , e_is_endpoint = 4
          , e_is_penetrating = 8
          , e_is_moving_away = 16
        };

        std::uint32_t result{ 0 };

    };

    // Intersect sphere s with movement vector v with obb p. If intersecting
    // return time t of collision and point q at which sphere hits obb
    // If already intersecting, q is the closest point between the center of the sphere
    // and the obb.
    template <typename Sphere, typename Velocity, typename OBB, typename Time, typename Point, typename NumberComparisonPolicy>
    inline moving_sphere_obb_intersection_result moving_sphere_obb_intersection( const Sphere& s, const Velocity& velocity, const OBB& obb, Time& t, Point& q, const NumberComparisonPolicy& cmp )
    {
        using point_t = typename geometric_traits<Sphere>::point_type;
        using length_t = typename geometric_traits<point_t>::arithmetic_type;
        using vector_t = vector<length_t, dimension_of<point_t>::value>;
        using dimensionless_t = typename geometric_traits<point_t>::dimensionless_type;
        using unit_vector_t = vector<dimensionless_t, dimension_of<point_t>::value>;

        //! Create a reference frame for the OBB and generate an AABB and sphere in that frame.
        //! Then use the AABB test and convert any results back to the original frame.
		auto ll = obb[0];// .get_right_backward_point();
		auto lr = obb[1];// .get_right_forward_point();
		auto ur = obb[2];// .get_left_forward_point();
		auto ul = obb[3];// .get_left_backward_point();
        auto xAxis = unit_vector_t{ constants::one<dimensionless_t>(), constants::zero<dimensionless_t>() };
        auto rot = make_rotation_matrix(obb.get_axis(0), xAxis);
        auto rs = make_sphere<2>(rotate_point(get_center(s), rot, obb.get_center()), get_radius(s));
        auto rvelocity = rotate_vector(velocity, rot);

        // Compute the AABB resulting from expanding a by sphere radius r
        axis_aligned_bounding_box<Point> a(rotate_point(ll, rot, obb.get_center()), rotate_point(ur, rot, obb.get_center()));
        vector_t rv{ get_radius(s), get_radius(s) };
        axis_aligned_bounding_box<Point> e{a.get_lower_bound() - rv, a.get_upper_bound() + rv};

        // Intersect ray against expanded AABB e. Exit with no intersection if ray
        // misses e, else get intersection point p and time t as result
        Point p;
        if (!ray_aabb_intersection(get_center(rs), rvelocity, e, t, p, cmp))
            return moving_sphere_obb_intersection_result();

        // Compute which min and max faces of a the intersection point p lies
        // outside of. Note, u and v cannot have the same bits set and
        // they must have at least one bit set among them
        int u = 0, v = 0;
        if (get<0>(p) < get<0>(a.get_lower_bound())) u |= 1;
        if (get<0>(p) > get<0>(a.get_upper_bound())) v |= 1;
        if (get<1>(p) < get<1>(a.get_lower_bound())) u |= 2;
        if (get<1>(p) > get<1>(a.get_upper_bound())) v |= 2;

        // ‘Or’ all set bits together into a bit mask (note: here u + v == u | v)
        int m = u + v;

        auto corner = [&ll, &lr, &ur, &ul] (int n) -> Point
        {
            return (n & 1) ? (n & 2) ? ur : lr : (n & 2) ? ul : ll;
        };

        // If 2 bits set (m == 3) then p is in a vertex region
        if (m == 3)
        {
            // Must now intersect segment [c, c+d] against the capsules of the two
            // edges meeting at the vertex and return the best time, if one or more hit
            Time tmin = (std::numeric_limits<Time>::max)();
            moving_sphere_segment_intersection_result minResult;
            Point qmin;
            auto result = moving_sphere_segment_intersection(s, velocity, segment<Point>(corner(v), corner(v ^ 1)), t, q, cmp);
            if (result && t < tmin)
            {
                tmin = t;
                minResult = result;
                assign(qmin, q);
            }

            result = moving_sphere_segment_intersection(s, velocity, segment<Point>(corner(v), corner(v ^ 2)), t, q, cmp);
            if (result && t < tmin)
            {
                tmin = t;
                minResult = result;
                assign(qmin, q);
            }

            if (tmin == (std::numeric_limits<Time>::max)())
                return moving_sphere_obb_intersection_result(); // No intersection
            t = tmin;
            assign(q, qmin);
            return minResult; // Intersection at time t == tmin
        }

        // If only one bit set in m, then p is in a face region
        if ((m & (m - 1)) == 0)
        {
            // Do nothing. Time t from intersection with
            // expanded box is correct intersection time
            Point newCenter = construct<Point>(get_center(s) + t * velocity);
            //! Now find the intersection point on the sphere.
            assign(q, newCenter + get_radius(s) * normalize(point_segment_closest_point(newCenter, corner(u ^ 3), corner(v)) - newCenter));
            return moving_sphere_obb_intersection_result(true, false, false, true, false);
        }

        // p is in an edge region. Intersect against the capsule at the edge
        return moving_sphere_segment_intersection(s, velocity, segment<Point>(corner(u ^ 3), corner(v)), t, q, cmp);
    }
}//namespace geometrix;

#endif //GEOMETRIX_MOVING_SPHERE_OBB_INTERSECTION_HPP
