//
//! Copyright © 2026
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0.
//
#ifndef GEOMETRIX_BINNED_SAH_BUILDER_HPP
#define GEOMETRIX_BINNED_SAH_BUILDER_HPP

#include <geometrix/spatial/bounding_volume_hierarchy.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

namespace geometrix {
namespace spatial {

//! Builds a flat BVH using binned surface-area-heuristic partitioning.
//! BoundsFn(index) returns an AABB; CenterFn(index) returns a Point.
//!
//! Internal nodes store two contiguous children at first and first + 1.
//! Child slots are reserved before recursion and are filled in place so node
//! indices are stable for the lifetime of the build.
template <typename Point, typename CostFn>
class binned_sah_builder
{
public:
    using bvh_type = bounding_volume_hierarchy<Point>;
    using bounds_type = typename bvh_type::bounds_type;
    using index_type = typename bvh_type::index_type;

    explicit binned_sah_builder(CostFn cost,
                                std::size_t leaf_size = 4,
                                std::size_t bin_count = 16)
        : cost_(std::move(cost))
        , leaf_size_((std::max)(std::size_t{ 1 }, leaf_size))
        , bin_count_((std::max)(std::size_t{ 2 }, bin_count))
    {}

    template <typename BoundsFn, typename CenterFn>
    bvh_type operator()(std::size_t primitive_count,
                        BoundsFn bounds_fn,
                        CenterFn center_fn) const
    {
        bvh_type result;
        if (primitive_count == 0)
            return result;

        result.primitive_indices_.resize(primitive_count);
        std::iota(result.primitive_indices_.begin(), result.primitive_indices_.end(), index_type{ 0 });

        //! A binary BVH with N leaves has at most 2N-1 nodes. Reserving the
        //! complete upper bound also guarantees vector reallocation cannot
        //! occur while recursively writing sibling slots.
        result.nodes_.reserve(primitive_count * 2 - 1);

        auto root_bounds = range_bounds(result, 0, primitive_count, bounds_fn);
        result.nodes_.emplace_back(root_bounds);
        build_into(result, index_type{ 0 }, 0, primitive_count, bounds_fn, center_fn);
        return result;
    }

private:
    struct split_candidate
    {
        std::size_t axis = 0;
        std::size_t bin = 0;
        double cost = (std::numeric_limits<double>::max)();
        bool valid = false;
    };

    template <typename BoundsFn>
    bounds_type range_bounds(const bvh_type& bvh,
                             std::size_t first,
                             std::size_t count,
                             BoundsFn& bounds_fn) const
    {
        auto bounds = bounds_fn(bvh.primitive_indices_[first]);
        for (std::size_t i = 1; i != count; ++i)
            bounds.expand(bounds_fn(bvh.primitive_indices_[first + i]));
        return bounds;
    }

    template <typename CenterFn>
    std::pair<Point, Point> center_bounds(const bvh_type& bvh,
                                          std::size_t first,
                                          std::size_t count,
                                          CenterFn& center_fn) const
    {
        auto cmin = center_fn(bvh.primitive_indices_[first]);
        auto cmax = cmin;

        for (std::size_t i = 1; i != count; ++i)
        {
            auto c = center_fn(bvh.primitive_indices_[first + i]);
            for (std::size_t d = 0; d != dimension_of<Point>::value; ++d)
            {
                if (c[d] < cmin[d]) cmin[d] = c[d];
                if (cmax[d] < c[d]) cmax[d] = c[d];
            }
        }
        return { cmin, cmax };
    }

    template <typename BoundsFn, typename CenterFn>
    split_candidate find_split(const bvh_type& bvh,
                               std::size_t first,
                               std::size_t count,
                               const Point& cmin,
                               const Point& cmax,
                               BoundsFn& bounds_fn,
                               CenterFn& center_fn) const
    {
        split_candidate best;

        for (std::size_t axis = 0; axis != dimension_of<Point>::value; ++axis)
        {
            if (!(cmin[axis] < cmax[axis]))
                continue;

            auto extent = cmax[axis] - cmin[axis];
            auto bin_of = [&](index_type primitive)
            {
                auto x = center_fn(primitive)[axis];
                auto t = static_cast<double>((x - cmin[axis]) / extent);
                auto bin = static_cast<std::size_t>(t * static_cast<double>(bin_count_));
                return (std::min)(bin, bin_count_ - 1);
            };

            std::vector<std::size_t> counts(bin_count_, 0);
            std::vector<bounds_type> bounds;
            std::vector<bool> used(bin_count_, false);
            bounds.reserve(bin_count_);

            //! bounds_type has no default constructor. Seed each unused bin
            //! with an arbitrary valid box; used[] determines whether it
            //! contributes to prefix/suffix bounds.
            auto seed = bounds_fn(bvh.primitive_indices_[first]);
            for (std::size_t i = 0; i != bin_count_; ++i)
                bounds.emplace_back(seed);

            for (std::size_t i = 0; i != count; ++i)
            {
                auto primitive = bvh.primitive_indices_[first + i];
                auto bin = bin_of(primitive);
                ++counts[bin];

                auto primitive_bounds = bounds_fn(primitive);
                if (!used[bin])
                {
                    bounds[bin] = primitive_bounds;
                    used[bin] = true;
                }
                else
                {
                    bounds[bin].expand(primitive_bounds);
                }
            }

            std::vector<std::size_t> left_counts(bin_count_, 0);
            std::vector<std::size_t> right_counts(bin_count_, 0);
            std::vector<bounds_type> left_bounds;
            std::vector<bounds_type> right_bounds;
            std::vector<bool> left_used(bin_count_, false);
            std::vector<bool> right_used(bin_count_, false);
            left_bounds.reserve(bin_count_);
            right_bounds.reserve(bin_count_);
            for (std::size_t i = 0; i != bin_count_; ++i)
            {
                left_bounds.emplace_back(seed);
                right_bounds.emplace_back(seed);
            }

            std::size_t running_count = 0;
            bool have_bounds = false;
            bounds_type running_bounds = seed;
            for (std::size_t i = 0; i != bin_count_; ++i)
            {
                running_count += counts[i];
                if (used[i])
                {
                    if (!have_bounds)
                    {
                        running_bounds = bounds[i];
                        have_bounds = true;
                    }
                    else
                    {
                        running_bounds.expand(bounds[i]);
                    }
                }
                left_counts[i] = running_count;
                left_used[i] = have_bounds;
                if (have_bounds)
                    left_bounds[i] = running_bounds;
            }

            running_count = 0;
            have_bounds = false;
            running_bounds = seed;
            for (std::size_t i = bin_count_; i-- != 0;)
            {
                running_count += counts[i];
                if (used[i])
                {
                    if (!have_bounds)
                    {
                        running_bounds = bounds[i];
                        have_bounds = true;
                    }
                    else
                    {
                        running_bounds.expand(bounds[i]);
                    }
                }
                right_counts[i] = running_count;
                right_used[i] = have_bounds;
                if (have_bounds)
                    right_bounds[i] = running_bounds;
            }

            for (std::size_t split = 1; split != bin_count_; ++split)
            {
                auto left = split - 1;
                auto right = split;
                if (!left_used[left] || !right_used[right])
                    continue;

                auto split_cost = static_cast<double>(left_counts[left]) * cost_(left_bounds[left])
                                + static_cast<double>(right_counts[right]) * cost_(right_bounds[right]);

                if (split_cost < best.cost)
                {
                    best.axis = axis;
                    best.bin = split;
                    best.cost = split_cost;
                    best.valid = true;
                }
            }
        }

        return best;
    }

    template <typename BoundsFn, typename CenterFn>
    void build_into(bvh_type& bvh,
                    index_type node_index,
                    std::size_t first,
                    std::size_t count,
                    BoundsFn& bounds_fn,
                    CenterFn& center_fn) const
    {
        auto bounds = range_bounds(bvh, first, count, bounds_fn);
        bvh.nodes_[node_index].bounds = bounds;

        if (count <= leaf_size_)
        {
            make_leaf(bvh, node_index, first, count);
            return;
        }

        auto centers = center_bounds(bvh, first, count, center_fn);
        auto split = find_split(bvh, first, count, centers.first, centers.second,
                                bounds_fn, center_fn);
        if (!split.valid)
        {
            make_leaf(bvh, node_index, first, count);
            return;
        }

        auto extent = centers.second[split.axis] - centers.first[split.axis];
        auto bin_of = [&](index_type primitive)
        {
            auto x = center_fn(primitive)[split.axis];
            auto t = static_cast<double>((x - centers.first[split.axis]) / extent);
            auto bin = static_cast<std::size_t>(t * static_cast<double>(bin_count_));
            return (std::min)(bin, bin_count_ - 1);
        };

        auto begin = bvh.primitive_indices_.begin() + first;
        auto end = begin + count;
        auto middle = std::partition(begin, end,
            [&](index_type primitive) { return bin_of(primitive) < split.bin; });

        auto left_count = static_cast<std::size_t>(middle - begin);
        if (left_count == 0 || left_count == count)
        {
            make_leaf(bvh, node_index, first, count);
            return;
        }

        //! Siblings are deliberately contiguous. Besides compact traversal,
        //! this layout allows a later SIMD traversal to load/test both child
        //! bounding boxes together without changing the tree representation.
        auto first_child = static_cast<index_type>(bvh.nodes_.size());
        bvh.nodes_.emplace_back(bounds);
        bvh.nodes_.emplace_back(bounds);

        bvh.nodes_[node_index].first = first_child;
        bvh.nodes_[node_index].count = 0;

        build_into(bvh, first_child, first, left_count, bounds_fn, center_fn);
        build_into(bvh, first_child + 1, first + left_count, count - left_count,
                   bounds_fn, center_fn);
    }

    void make_leaf(bvh_type& bvh,
                   index_type node_index,
                   std::size_t first,
                   std::size_t count) const
    {
        bvh.nodes_[node_index].first = static_cast<index_type>(first);
        bvh.nodes_[node_index].count = static_cast<index_type>(count);
    }

    CostFn cost_;
    std::size_t leaf_size_;
    std::size_t bin_count_;
};

}//! namespace spatial
}//! namespace geometrix

#endif//! GEOMETRIX_BINNED_SAH_BUILDER_HPP
