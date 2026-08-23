//
//! Copyright © 2026
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0.
//
#ifndef GEOMETRIX_BOUNDING_VOLUME_HIERARCHY_HPP
#define GEOMETRIX_BOUNDING_VOLUME_HIERARCHY_HPP

#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <array>
#include <cstdint>
#include <utility>
#include <vector>

namespace geometrix {
namespace spatial {

//! A compact, immutable-after-build bounding volume hierarchy.
//! Geometry remains owned by the caller; leaves contain primitive indices.
template <typename Point>
class bounding_volume_hierarchy
{
public:
    using point_type = Point;
    using bounds_type = axis_aligned_bounding_box<point_type>;
    using index_type = std::uint32_t;

    struct node
    {
        bounds_type bounds;
        index_type first = 0;
        index_type count = 0;

        node(const bounds_type& b, index_type f = 0, index_type c = 0)
            : bounds(b), first(f), count(c)
        {}

        //! Internal nodes have count == 0 and two contiguous children at
        //! first and first + 1. Leaves address primitive_indices().
        bool is_leaf() const noexcept { return count != 0; }
    };

    bool empty() const noexcept { return nodes_.empty(); }
    std::size_t size() const noexcept { return primitive_indices_.size(); }

    const std::vector<node>& nodes() const noexcept { return nodes_; }
    const std::vector<index_type>& primitive_indices() const noexcept { return primitive_indices_; }

    void clear()
    {
        nodes_.clear();
        primitive_indices_.clear();
    }

    //! Traverse until a primitive predicate succeeds. BoundsPredicate has
    //! signature bool(Query const&, bounds_type const&); PrimitivePredicate
    //! has signature bool(Query const&, index_type).
    template <typename Query, typename BoundsPredicate, typename PrimitivePredicate>
    bool any_of(const Query& query,
                BoundsPredicate&& bounds_predicate,
                PrimitivePredicate&& primitive_predicate) const
    {
        if (nodes_.empty())
            return false;

        return any_of_impl(
            query,
            [&](const Query& q, const bounds_type& left, const bounds_type& right)
            {
                std::uint8_t mask = 0;
                if (bounds_predicate(q, left)) mask |= std::uint8_t{ 1 };
                if (bounds_predicate(q, right)) mask |= std::uint8_t{ 2 };
                return mask;
            },
            std::forward<PrimitivePredicate>(primitive_predicate),
            std::forward<BoundsPredicate>(bounds_predicate));
    }

    //! Traversal variant for predicates that test both contiguous child boxes
    //! at once. PairBoundsPredicate returns a two-bit mask: bit 0 means the
    //! left child intersects, bit 1 means the right child intersects.
    //!
    //! This is the SIMD seam: the BVH owns traversal/order, while geometry-
    //! specific code is free to evaluate the sibling pair with SSE/AVX/etc.
    template <typename Query, typename PairBoundsPredicate, typename PrimitivePredicate>
    bool any_of_pair(const Query& query,
                     PairBoundsPredicate&& pair_bounds_predicate,
                     PrimitivePredicate&& primitive_predicate) const
    {
        if (nodes_.empty())
            return false;

        //! The root has no sibling. Test it through the same pair predicate by
        //! supplying it twice; either returned bit means the root survives.
        auto root_mask = pair_bounds_predicate(query, nodes_[0].bounds, nodes_[0].bounds);
        if ((root_mask & std::uint8_t{ 3 }) == 0)
            return false;

        auto root_bounds_predicate = [](const Query&, const bounds_type&) { return true; };
        return any_of_impl(
            query,
            std::forward<PairBoundsPredicate>(pair_bounds_predicate),
            std::forward<PrimitivePredicate>(primitive_predicate),
            root_bounds_predicate);
    }

private:
    template <typename Query, typename PairBoundsPredicate,
              typename PrimitivePredicate, typename RootBoundsPredicate>
    bool any_of_impl(const Query& query,
                     PairBoundsPredicate&& pair_bounds_predicate,
                     PrimitivePredicate&& primitive_predicate,
                     RootBoundsPredicate&& root_bounds_predicate) const
    {
        constexpr std::size_t local_stack_capacity = 64;
        std::array<index_type, local_stack_capacity> local_stack;
        std::size_t stack_size = 0;
        std::vector<index_type> overflow_stack;

        auto push = [&](index_type index)
        {
            if (stack_size < local_stack_capacity)
                local_stack[stack_size++] = index;
            else
                overflow_stack.push_back(index);
        };

        auto pop = [&]()
        {
            if (!overflow_stack.empty())
            {
                auto result = overflow_stack.back();
                overflow_stack.pop_back();
                return result;
            }
            return local_stack[--stack_size];
        };

        if (!root_bounds_predicate(query, nodes_[0].bounds))
            return false;

        push(index_type{ 0 });

        while (stack_size != 0 || !overflow_stack.empty())
        {
            auto node_index = pop();
            const auto& n = nodes_[node_index];

            if (n.is_leaf())
            {
                for (index_type i = 0; i != n.count; ++i)
                {
                    auto primitive_index = primitive_indices_[n.first + i];
                    if (primitive_predicate(query, primitive_index))
                        return true;
                }
                continue;
            }

            const auto& left = nodes_[n.first];
            const auto& right = nodes_[n.first + 1];
            auto mask = static_cast<std::uint8_t>(
                pair_bounds_predicate(query, left.bounds, right.bounds));

            //! Right is pushed first so left is visited first when both hit.
            if ((mask & std::uint8_t{ 2 }) != 0)
                push(n.first + 1);
            if ((mask & std::uint8_t{ 1 }) != 0)
                push(n.first);
        }

        return false;
    }

    template <typename, typename>
    friend class binned_sah_builder;

    std::vector<node> nodes_;
    std::vector<index_type> primitive_indices_;
};

}//! namespace spatial
}//! namespace geometrix

#endif//! GEOMETRIX_BOUNDING_VOLUME_HIERARCHY_HPP
