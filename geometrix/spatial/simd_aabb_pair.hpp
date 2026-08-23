//
//! Copyright © 2026
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0.
//
#ifndef GEOMETRIX_SIMD_AABB_PAIR_HPP
#define GEOMETRIX_SIMD_AABB_PAIR_HPP

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>
#include <cstdint>
#include <type_traits>

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
#   include <emmintrin.h>
#   define GEOMETRIX_HAS_SSE2_AABB_PAIR 1
#elif defined(__SSE2__)
#   include <emmintrin.h>
#   define GEOMETRIX_HAS_SSE2_AABB_PAIR 1
#else
#   define GEOMETRIX_HAS_SSE2_AABB_PAIR 0
#endif

namespace geometrix {
namespace spatial {

//! Tests one 2D query AABB against two child AABBs and returns a two-bit hit
//! mask. The geometry may use integral coordinates, promoted arithmetic, or
//! unit-wrapped arithmetic. SIMD selection is based on the underlying numeric
//! representation of the promoted arithmetic type, not the stored coordinate
//! type itself.
template <typename Point>
class aabb_pair_overlap
{
public:
    using bounds_type = axis_aligned_bounding_box<Point>;
    using arithmetic_type = typename geometric_traits<Point>::arithmetic_type;
    using scalar_type = typename numeric_traits<arithmetic_type>::numeric_type;

    std::uint8_t operator()(const bounds_type& query,
                            const bounds_type& left,
                            const bounds_type& right) const noexcept
    {
#if GEOMETRIX_HAS_SSE2_AABB_PAIR
        return test_sse2(query, left, right);
#else
        return test_scalar(query, left, right);
#endif
    }

private:
    static std::uint8_t test_scalar(const bounds_type& query,
                                    const bounds_type& left,
                                    const bounds_type& right) noexcept
    {
        std::uint8_t mask = 0;
        if (query.intersects(left)) mask |= std::uint8_t{ 1 };
        if (query.intersects(right)) mask |= std::uint8_t{ 2 };
        return mask;
    }

#if GEOMETRIX_HAS_SSE2_AABB_PAIR
    static std::uint8_t test_sse2(const bounds_type& query,
                                  const bounds_type& left,
                                  const bounds_type& right) noexcept
    {
        return test_sse2_dispatch(query, left, right,
            std::integral_constant<bool,
                dimension_of<Point>::value == 2 &&
                std::is_same<scalar_type, double>::value>{});
    }

    static std::uint8_t test_sse2_dispatch(const bounds_type& query,
                                           const bounds_type& left,
                                           const bounds_type& right,
                                           std::false_type) noexcept
    {
        return test_scalar(query, left, right);
    }

    //! Extract the underlying numeric representation from either a plain
    //! arithmetic value or a quantity-like type exposing value(). The latter
    //! is the common Boost.Units representation. SFINAE keeps this local to
    //! the SIMD implementation rather than imposing a new Geometrix numeric
    //! concept on callers.
    template <typename T>
    static auto numeric_value_impl(const T& value, int) noexcept
        -> decltype(static_cast<scalar_type>(value.value()))
    {
        return static_cast<scalar_type>(value.value());
    }

    template <typename T>
    static auto numeric_value_impl(const T& value, long) noexcept
        -> decltype(static_cast<scalar_type>(value))
    {
        return static_cast<scalar_type>(value);
    }

    template <typename T>
    static scalar_type numeric_value(const T& value) noexcept
    {
        return numeric_value_impl(value, 0);
    }

    static std::uint8_t test_sse2_dispatch(const bounds_type& query,
                                           const bounds_type& left,
                                           const bounds_type& right,
                                           std::true_type) noexcept
    {
        auto const& qlo = query.get_lower_bound();
        auto const& qhi = query.get_upper_bound();
        auto const& llo = left.get_lower_bound();
        auto const& lhi = left.get_upper_bound();
        auto const& rlo = right.get_lower_bound();
        auto const& rhi = right.get_upper_bound();

        //! Each lane represents one sibling: low lane = left, high = right.
        auto min_x = _mm_set_pd(numeric_value(rlo[0]), numeric_value(llo[0]));
        auto max_x = _mm_set_pd(numeric_value(rhi[0]), numeric_value(lhi[0]));
        auto min_y = _mm_set_pd(numeric_value(rlo[1]), numeric_value(llo[1]));
        auto max_y = _mm_set_pd(numeric_value(rhi[1]), numeric_value(lhi[1]));

        auto qmin_x = _mm_set1_pd(numeric_value(qlo[0]));
        auto qmax_x = _mm_set1_pd(numeric_value(qhi[0]));
        auto qmin_y = _mm_set1_pd(numeric_value(qlo[1]));
        auto qmax_y = _mm_set1_pd(numeric_value(qhi[1]));

        //! Inclusive overlap: child.max >= query.min && child.min <= query.max.
        auto hit_x = _mm_and_pd(_mm_cmpge_pd(max_x, qmin_x),
                                _mm_cmple_pd(min_x, qmax_x));
        auto hit_y = _mm_and_pd(_mm_cmpge_pd(max_y, qmin_y),
                                _mm_cmple_pd(min_y, qmax_y));
        auto hit = _mm_and_pd(hit_x, hit_y);

        return static_cast<std::uint8_t>(_mm_movemask_pd(hit));
    }
#endif
};

}//! namespace spatial
}//! namespace geometrix

#endif//! GEOMETRIX_SIMD_AABB_PAIR_HPP
