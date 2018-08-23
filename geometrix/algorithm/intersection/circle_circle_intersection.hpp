//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(GEOMETRIX_ALGORITHM_INTERSECTION_CIRCLE_CIRCLE_INTERSECTION_HPP)
#define GEOMETRIX_ALGORITHM_INTERSECTION_CIRCLE_CIRCLE_INTERSECTION_HPP

#include <geometrix/numeric/constants.hpp>
#include <geometrix/algorithm/distance/point_point_distance.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/arithmetic/vector/perp.hpp>

#include <boost/optional.hpp>

namespace geometrix {
    
    enum class circle_intersection_state
    {
        non_intersecting//! circles are disjoint.
      , circumscribed //! one circle is inside the other.
      , one_intersection
      , two_intersections
      , coincident//! same circles.
    };

    template <typename Point>
    struct circle_circle_intersection_result
    {
        circle_circle_intersection_result(circle_intersection_state state)
            : State(state)
        {}

        circle_circle_intersection_result(const Point& p)
            : State(circle_intersection_state::one_intersection)
            , IntersectionPoint0(p)
        {}

        circle_circle_intersection_result(const Point& p0, const Point& p1)
            : State(circle_intersection_state::two_intersections)
            , IntersectionPoint0(p0)
            , IntersectionPoint1(p1)
        {}

        circle_intersection_state State{ circle_intersection_state::non_intersecting };
        boost::optional<Point>    IntersectionPoint0;
        boost::optional<Point>    IntersectionPoint1;
    };

    template <typename CircleA, typename CircleB, typename NumberComparisonPolicy>
    circle_circle_intersection_result<point<typename geometric_traits<CircleA>::radius_type, 2>> circle_circle_intersection(const CircleA& A, const CircleB& B, const NumberComparisonPolicy& cmp)
    {
        using namespace geometrix;

        using std::sqrt;
        using std::abs;

        using length_t = typename geometric_traits<CircleA>::radius_type;
        using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
        using point_t = point<length_t, 2>;
        using vector_t = vector<length_t, 2>;

        auto d = point_point_distance(get_center(A), get_center(B));
        if (cmp.greater_than(d, get_radius(A) + get_radius(B)))
            return circle_circle_intersection_result<point_t>(circle_intersection_state::non_intersecting);
        else if (cmp.less_than(d, abs(get_radius(A) - get_radius(B))))
            return circle_circle_intersection_result<point_t>(circle_intersection_state::circumscribed);
        else if (cmp.equals(d, constants::zero<length_t>()))
            return circle_circle_intersection_result<point_t>(circle_intersection_state::coincident);

        auto r1 = get_radius(A)*get_radius(A);
        auto a = (r1 - get_radius(B)*get_radius(B) + d*d) / (constants::two<dimensionless_t>() * d);
        auto h = sqrt(r1 - a*a);

        vector_t v = get_center(B) - get_center(A);

        auto dinv = constants::one<dimensionless_t>() / d;
        point_t m = get_center(A) + (a *dinv) * v;

        auto f = h * dinv;

        point_t s1 = m + f * right_normal(v);
        if (cmp.equals(h, constants::zero<length_t>()))
            return circle_circle_intersection_result<point_t>(s1);
        
        point_t s2 = m + f * left_normal(v);
        return circle_circle_intersection_result<point_t>(s1, s2);
    }

}//! namespace geometrix;

#endif//! GEOMETRIX_ALGORITHM_INTERSECTION_CIRCLE_CIRCLE_INTERSECTION_HPP

