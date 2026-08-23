//
//! Copyright © 2026
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0.
//
#ifndef GEOMETRIX_BOUNDING_VOLUME_HIERARCHY_HPP
#define GEOMETRIX_BOUNDING_VOLUME_HIERARCHY_HPP

#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
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

        std::vector<index_type> stack;
        stack.reserve(64);
        stack.push_back(0);

        while (!stack.empty())
        {
            auto node_index = stack.back();
            stack.pop_back();

            const auto& n = nodes_[node_index];
            if (!bounds_predicate(query, n.bounds))
                continue;

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

            stack.push_back(n.first + 1);
            stack.push_back(n.first);
        }

        return false;
    }

private:
    template <typename, typename>
    friend class binned_sah_builder;

    std::vector<node> nodes_;
    std::vector<index_type> primitive_indices_;
};

}//! namespace spatial
}//! namespace geometrix

#endif//! GEOMETRIX_BOUNDING_VOLUME_HIERARCHY_HPP
