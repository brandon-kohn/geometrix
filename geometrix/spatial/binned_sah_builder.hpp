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
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <vector>

namespace geometrix {
namespace spatial {

//! Builds a flat BVH using binned surface-area-heuristic partitioning.
//! BoundsFn(index) returns an AABB; CenterFn(index) returns a Point.
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
        , leaf_size_(leaf_size)
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

        result.nodes_.reserve(primitive_count * 2);
        build_node(result, 0, primitive_count, bounds_fn, center_fn);
        return result;
    }

private:
    template <typename BoundsFn, typename CenterFn>
    index_type build_node(bvh_type& bvh,
                          std::size_t first,
                          std::size_t count,
                          BoundsFn& bounds_fn,
                          CenterFn& center_fn) const
    {
        auto bounds = bounds_fn(bvh.primitive_indices_[first]);
        for (std::size_t i = 1; i != count; ++i)
            bounds.expand(bounds_fn(bvh.primitive_indices_[first + i]));

        auto node_index = static_cast<index_type>(bvh.nodes_.size());
        bvh.nodes_.emplace_back(bounds);

        if (count <= leaf_size_)
        {
            bvh.nodes_[node_index].first = static_cast<index_type>(first);
            bvh.nodes_[node_index].count = static_cast<index_type>(count);
            return node_index;
        }

        //! Keep the first implementation deliberately simple: choose the
        //! longest centroid axis, then bin centroids and evaluate SAH splits.
        auto c0 = center_fn(bvh.primitive_indices_[first]);
        auto cmin = c0;
        auto cmax = c0;
        for (std::size_t i = 1; i != count; ++i)
        {
            auto c = center_fn(bvh.primitive_indices_[first + i]);
            for (std::size_t d = 0; d != dimension_of<Point>::value; ++d)
            {
                if (c[d] < cmin[d]) cmin[d] = c[d];
                if (cmax[d] < c[d]) cmax[d] = c[d];
            }
        }

        std::size_t axis = 0;
        for (std::size_t d = 1; d != dimension_of<Point>::value; ++d)
            if ((cmax[axis] - cmin[axis]) < (cmax[d] - cmin[d])) axis = d;

        if (!(cmin[axis] < cmax[axis]))
            return make_leaf(bvh, node_index, first, count);

        auto extent = cmax[axis] - cmin[axis];
        auto bin_of = [&](index_type primitive)
        {
            auto x = center_fn(primitive)[axis];
            auto t = static_cast<double>((x - cmin[axis]) / extent);
            auto b = static_cast<std::size_t>(t * static_cast<double>(bin_count_));
            return (std::min)(b, bin_count_ - 1);
        };

        std::vector<std::size_t> bin_counts(bin_count_, 0);
        std::vector<bounds_type> bin_bounds;
        std::vector<bool> bin_used(bin_count_, false);
        bin_bounds.reserve(bin_count_);
        for (std::size_t i = 0; i != bin_count_; ++i)
            bin_bounds.emplace_back(bounds);

        for (std::size_t i = 0; i != count; ++i)
        {
            auto primitive = bvh.primitive_indices_[first + i];
            auto b = bin_of(primitive);
            ++bin_counts[b];
            if (!bin_used[b])
            {
                bin_bounds[b] = bounds_fn(primitive);
                bin_used[b] = true;
            }
            else
                bin_bounds[b].expand(bounds_fn(primitive));
        }

        double best_cost = (std::numeric_limits<double>::max)();
        std::size_t best_split = 0;
        bool found = false;

        for (std::size_t split = 1; split != bin_count_; ++split)
        {
            std::size_t left_count = 0, right_count = 0;
            bool have_left = false, have_right = false;
            bounds_type left_bounds = bounds;
            bounds_type right_bounds = bounds;

            for (std::size_t b = 0; b != split; ++b)
                if (bin_used[b])
                {
                    left_count += bin_counts[b];
                    if (!have_left) { left_bounds = bin_bounds[b]; have_left = true; }
                    else left_bounds.expand(bin_bounds[b]);
                }
            for (std::size_t b = split; b != bin_count_; ++b)
                if (bin_used[b])
                {
                    right_count += bin_counts[b];
                    if (!have_right) { right_bounds = bin_bounds[b]; have_right = true; }
                    else right_bounds.expand(bin_bounds[b]);
                }

            if (!have_left || !have_right)
                continue;

            auto split_cost = static_cast<double>(left_count) * cost_(left_bounds)
                            + static_cast<double>(right_count) * cost_(right_bounds);
            if (split_cost < best_cost)
            {
                best_cost = split_cost;
                best_split = split;
                found = true;
            }
        }

        if (!found)
            return make_leaf(bvh, node_index, first, count);

        auto begin = bvh.primitive_indices_.begin() + first;
        auto end = begin + count;
        auto middle = std::partition(begin, end, [&](index_type primitive) { return bin_of(primitive) < best_split; });
        auto left_count = static_cast<std::size_t>(middle - begin);
        if (left_count == 0 || left_count == count)
            return make_leaf(bvh, node_index, first, count);

        //! Reserve two contiguous child slots before recursively filling them.
        auto child = static_cast<index_type>(bvh.nodes_.size());
        bvh.nodes_.emplace_back(bounds);
        bvh.nodes_.emplace_back(bounds);
        bvh.nodes_[node_index].first = child;
        bvh.nodes_[node_index].count = 0;

        build_into(bvh, child, first, left_count, bounds_fn, center_fn);
        build_into(bvh, child + 1, first + left_count, count - left_count, bounds_fn, center_fn);
        return node_index;
    }

    template <typename BoundsFn, typename CenterFn>
    void build_into(bvh_type& bvh,
                    index_type slot,
                    std::size_t first,
                    std::size_t count,
                    BoundsFn& bounds_fn,
                    CenterFn& center_fn) const
    {
        auto temporary_index = build_node(bvh, first, count, bounds_fn, center_fn);
        if (temporary_index == slot)
            return;
        bvh.nodes_[slot] = bvh.nodes_[temporary_index];
        bvh.nodes_.erase(bvh.nodes_.begin() + temporary_index);
        //! This simple bootstrap builder intentionally favors clarity. The
        //! next revision should build directly into reserved slots so node
        //! indices never require relocation.
    }

    index_type make_leaf(bvh_type& bvh, index_type node_index,
                         std::size_t first, std::size_t count) const
    {
        bvh.nodes_[node_index].first = static_cast<index_type>(first);
        bvh.nodes_[node_index].count = static_cast<index_type>(count);
        return node_index;
    }

    CostFn cost_;
    std::size_t leaf_size_;
    std::size_t bin_count_;
};

}//! namespace spatial
}//! namespace geometrix

#endif//! GEOMETRIX_BINNED_SAH_BUILDER_HPP
