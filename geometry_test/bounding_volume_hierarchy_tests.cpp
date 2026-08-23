//
//! Copyright © 2026
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0.
//
#include "2d_kernel_fixture.hpp"

#include <geometrix/spatial/binned_sah_builder.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

namespace {

using fixture = geometry_kernel_2d_fixture;
using point2 = fixture::point2;
using segment2 = fixture::segment2;
using aabb2 = fixture::aabb2;

struct box_area
{
    double operator()(const aabb2& box) const
    {
        auto dx = box.get_upper_bound()[0] - box.get_lower_bound()[0];
        auto dy = box.get_upper_bound()[1] - box.get_lower_bound()[1];
        return dx * dy;
    }
};

aabb2 segment_bounds(const segment2& segment)
{
    auto const& a = segment.get_start();
    auto const& b = segment.get_end();

    point2 lower(
        (std::min)(a[0], b[0]),
        (std::min)(a[1], b[1]));
    point2 upper(
        (std::max)(a[0], b[0]),
        (std::max)(a[1], b[1]));
    return aabb2(lower, upper);
}

point2 segment_center(const segment2& segment)
{
    auto const& a = segment.get_start();
    auto const& b = segment.get_end();
    return point2((a[0] + b[0]) * 0.5, (a[1] + b[1]) * 0.5);
}

bool boxes_intersect(const aabb2& a, const aabb2& b)
{
    return !(a.get_upper_bound()[0] < b.get_lower_bound()[0]
          || b.get_upper_bound()[0] < a.get_lower_bound()[0]
          || a.get_upper_bound()[1] < b.get_lower_bound()[1]
          || b.get_upper_bound()[1] < a.get_lower_bound()[1]);
}

bool proper_segment_intersection(const segment2& lhs, const segment2& rhs)
{
    auto const& p = lhs.get_start();
    auto const& p2 = lhs.get_end();
    auto const& q = rhs.get_start();
    auto const& q2 = rhs.get_end();

    auto rx = p2[0] - p[0];
    auto ry = p2[1] - p[1];
    auto sx = q2[0] - q[0];
    auto sy = q2[1] - q[1];
    auto denominator = rx * sy - ry * sx;

    if (denominator == 0.0)
        return false;

    auto qpx = q[0] - p[0];
    auto qpy = q[1] - p[1];
    auto t = (qpx * sy - qpy * sx) / denominator;
    auto u = (qpx * ry - qpy * rx) / denominator;

    //! Visibility uses a proper crossing here: touching the sight-line target
    //! at an obstacle endpoint is not considered an occlusion.
    return 0.0 < t && t < 1.0 && 0.0 < u && u < 1.0;
}

std::vector<segment2> make_obstacles()
{
    return {
        segment2(point2(2.0, -1.0), point2(2.0, 1.0)),
        segment2(point2(5.0, 2.0), point2(5.0, 4.0)),
        segment2(point2(8.0, -4.0), point2(8.0, -2.0)),
        segment2(point2(11.0, -1.0), point2(11.0, 1.0)),
        segment2(point2(14.0, 2.0), point2(14.0, 4.0)),
        segment2(point2(17.0, -4.0), point2(17.0, -2.0)),
        segment2(point2(20.0, -1.0), point2(20.0, 1.0)),
        segment2(point2(23.0, 2.0), point2(23.0, 4.0))
    };
}

template <typename Bvh>
bool bvh_blocked(const Bvh& bvh,
                 const std::vector<segment2>& obstacles,
                 const segment2& query)
{
    auto query_bounds = segment_bounds(query);
    return bvh.any_of(
        query,
        [&](const segment2&, const aabb2& bounds)
        {
            return boxes_intersect(query_bounds, bounds);
        },
        [&](const segment2& sight_line, typename Bvh::index_type index)
        {
            return proper_segment_intersection(sight_line, obstacles[index]);
        });
}

bool brute_force_blocked(const std::vector<segment2>& obstacles,
                         const segment2& query)
{
    for (auto const& obstacle : obstacles)
        if (proper_segment_intersection(query, obstacle))
            return true;
    return false;
}

}//! namespace

TEST_F(geometry_kernel_2d_fixture, bvh_builds_stable_contiguous_children)
{
    auto obstacles = make_obstacles();
    geometrix::spatial::binned_sah_builder<point2, box_area> builder(box_area{}, 1, 8);

    auto bvh = builder(
        obstacles.size(),
        [&](std::uint32_t i) { return segment_bounds(obstacles[i]); },
        [&](std::uint32_t i) { return segment_center(obstacles[i]); });

    ASSERT_FALSE(bvh.empty());
    EXPECT_EQ(obstacles.size(), bvh.size());
    EXPECT_EQ(obstacles.size(), bvh.primitive_indices().size());

    std::vector<bool> seen(obstacles.size(), false);
    for (auto index : bvh.primitive_indices())
    {
        ASSERT_LT(index, obstacles.size());
        EXPECT_FALSE(seen[index]);
        seen[index] = true;
    }

    for (auto const& node : bvh.nodes())
    {
        if (node.is_leaf())
        {
            EXPECT_LE(static_cast<std::size_t>(node.first + node.count),
                      bvh.primitive_indices().size());
        }
        else
        {
            EXPECT_LT(static_cast<std::size_t>(node.first + 1), bvh.nodes().size());
        }
    }
}

TEST_F(geometry_kernel_2d_fixture, bvh_any_hit_matches_brute_force)
{
    auto obstacles = make_obstacles();
    geometrix::spatial::binned_sah_builder<point2, box_area> builder(box_area{}, 2, 8);

    auto bvh = builder(
        obstacles.size(),
        [&](std::uint32_t i) { return segment_bounds(obstacles[i]); },
        [&](std::uint32_t i) { return segment_center(obstacles[i]); });

    std::array<segment2, 8> queries = {
        segment2(point2(0.0, 0.0), point2(3.0, 0.0)),
        segment2(point2(0.0, 1.5), point2(3.0, 1.5)),
        segment2(point2(3.0, 3.0), point2(7.0, 3.0)),
        segment2(point2(6.0, 0.0), point2(10.0, 0.0)),
        segment2(point2(9.0, 0.0), point2(12.0, 0.0)),
        segment2(point2(12.0, 3.0), point2(16.0, 3.0)),
        segment2(point2(15.0, -3.0), point2(19.0, -3.0)),
        segment2(point2(21.0, 0.0), point2(24.0, 0.0))
    };

    for (auto const& query : queries)
        EXPECT_EQ(brute_force_blocked(obstacles, query),
                  bvh_blocked(bvh, obstacles, query));
}

TEST_F(geometry_kernel_2d_fixture, bvh_visibility_does_not_block_at_target_endpoint)
{
    std::vector<segment2> obstacles = {
        segment2(point2(5.0, 0.0), point2(5.0, 5.0))
    };

    geometrix::spatial::binned_sah_builder<point2, box_area> builder(box_area{}, 1, 8);
    auto bvh = builder(
        obstacles.size(),
        [&](std::uint32_t i) { return segment_bounds(obstacles[i]); },
        [&](std::uint32_t i) { return segment_center(obstacles[i]); });

    segment2 ends_at_vertex(point2(0.0, 0.0), point2(5.0, 0.0));
    EXPECT_FALSE(bvh_blocked(bvh, obstacles, ends_at_vertex));
}
