//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(GEOMETRIX_ALGORITHM_INTERSECTION_SPHERE_SPHERE_INTERSECTION_HPP)
#define GEOMETRIX_ALGORITHM_INTERSECTION_SPHERE_SPHERE_INTERSECTION_HPP

#include <geometrix/numeric/constants.hpp>
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/arithmetic/vector/perp.hpp>

#include <boost/optional.hpp>

namespace geometrix {
    
    enum class sphere_intersection_state
    {
        non_intersecting//! spheres are disjoint.
      , circumscribed //! one sphere is inside the other.
      , one_intersection
      , two_intersections
      , coincident//! same spheres.
    };

    template <typename Point>
    struct sphere_sphere_intersection_result
    {
        sphere_sphere_intersection_result(sphere_intersection_state state)
            : State(state)
        {}

        sphere_sphere_intersection_result(const Point& p)
            : State(sphere_intersection_state::one_intersection)
            , IntersectionPoint0(p)
        {}

        sphere_sphere_intersection_result(const Point& p0, const Point& p1)
            : State(sphere_intersection_state::two_intersections)
            , IntersectionPoint0(p0)
            , IntersectionPoint1(p1)
        {}

        sphere_intersection_state State{ sphere_intersection_state::non_intersecting };
        boost::optional<Point>    IntersectionPoint0;
        boost::optional<Point>    IntersectionPoint1;
    };

    template <typename SphereA, typename SphereB, typename NumberComparisonPolicy>
    sphere_sphere_intersection_result<point<typename geometric_traits<SphereA>::radius_type, 2>> sphere_sphere_intersection(const SphereA& A, const SphereB& B, const NumberComparisonPolicy& cmp)
    {
        using namespace geometrix;

        using std::sqrt;
        using std::abs;

        using length_t = typename geometric_traits<SphereA>::radius_type;
        using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
        using point_t = point<length_t, 2>;
        using vector_t = vector<length_t, 2>;

        auto d = point_point_distance(get_center(A), get_center(B));
        if (cmp.greater_than(d, get_radius(A) + get_radius(B)))
            return sphere_sphere_intersection_result<point_t>(sphere_intersection_state::non_intersecting);
        else if (cmp.less_than(d, abs(get_radius(A) - get_radius(B))))
            return sphere_sphere_intersection_result<point_t>(sphere_intersection_state::circumscribed);
        else if (cmp.equals(d, constants::zero<length_t>()))
            return sphere_sphere_intersection_result<point_t>(sphere_intersection_state::coincident);

        auto r1 = get_radius(A) * get_radius(A);
        auto a = (r1 - get_radius(B) * get_radius(B) + d * d) / (constants::two<dimensionless_t>() * d);
        auto h = sqrt(r1 - a*a);

        vector_t v = get_center(B) - get_center(A);

        auto dinv = constants::one<dimensionless_t>() / d;
        point_t m = get_center(A) + (a * dinv) * v;

        auto f = h * dinv;

        point_t s1 = m + f * right_normal(v);
        if (cmp.equals(h, constants::zero<length_t>()))
            return sphere_sphere_intersection_result<point_t>(s1);
        
        point_t s2 = m + f * left_normal(v);
        return sphere_sphere_intersection_result<point_t>(s1, s2);
    }

}//! namespace geometrix;

#endif//! GEOMETRIX_ALGORITHM_INTERSECTION_SPHERE_SPHERE_INTERSECTION_HPP

