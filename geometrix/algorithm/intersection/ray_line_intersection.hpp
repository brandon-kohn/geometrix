//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RAY_LINE_INTERSECTION_HPP
#define GEOMETRIX_RAY_LINE_INTERSECTION_HPP

#include <geometrix/algorithm/linear_components_intersection.hpp>
#include <geometrix/primitive/line_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/utility/utilities.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

    struct ray_line_intersection_result
    {
        typedef void (ray_line_intersection_result::*bool_type)() const;

        ray_line_intersection_result() = default;

        ray_line_intersection_result(bool isIntersecting, bool isOverlapping = false)
        : result(((isIntersecting || isOverlapping) ? e_is_intersecting : 0) | (isOverlapping ? e_overlapping : 0))
        {}

        bool is_intersecting() const { return (result & e_is_intersecting) != 0; }
        bool is_overlapping() const { return (result & e_is_overlapping) != 0; }
        
        operator bool_type() const
        {
            return is_intersecting() ? &ray_line_intersection_result::bool_type_mfn : 0;
        }

        bool operator == (bool b) const { return b == is_intersecting(); }

    private:

        void bool_type_mfn() const {}

        enum Flags
        {
            e_is_intersecting = 1
          , e_is_overlapping = 2
        };

        std::uint32_t result{ 0 };

    };

    //! \brief Compute whether the ray defined by A->v intersects the specified segment (A,B).
    template <typename Point, typename UnitVector, typename Line, typename Length, typename PointX, typename NumberComparisonPolicy>
    inline ray_line_intersection_result ray_line_intersection( const Point& O, const UnitVector& v, const Line& l, Length& t, PointX& xPoint, const NumberComparisonPolicy& cmp )
    {       
        BOOST_CONCEPT_ASSERT((Point2DConcept<Point>));
        BOOST_CONCEPT_ASSERT((Vector2DConcept<UnitVector>));
        BOOST_CONCEPT_ASSERT((Point2DConcept<PointX>));
        
        using length_t = typename arithmetic_type_of<Point>::type;
        using area_t = decltype(std::declval<length_t>() * std::declval<length_t>());
        using dimensionless_t = decltype(std::declval<length_t>() / std::declval<length_t>());
        using vector_t = vector<length_t, 2>;
        using unit_vector_t = vector<dimensionless_t, 2>;
        using line_access = line_access_traits<typename std::decay<Line>::type>;
        
        auto A = line_access::get_reference_point(l);
        vector_t v1 = O - A;
        auto v2 = line_access::get_parallel_vector(l);
        auto v3 = left_normal<unit_vector_t>(v);

        auto denom = scalar_projection(v2, v3);

        //If denom is zero then ray and line are parallel.
        if( cmp.equals( denom, constants::zero<length_t>() ) )
        {
            if (get_orientation(v, v2, cmp) != oriented_collinear)
            return false;

            return ray_line_intersection_result(true,true);
        }

        t = exterior_product_area(v2, v1) / denom;
        if (cmp.less_than(t, constants::zero<Length>()))
        return false;
        
        auto t2 = scalar_projection(v1, v3) / denom;
        if (cmp.less_than(t2, constants::zero<decltype(t2)>()))
        return false;

        assign(xPoint, O + t * v);

        return true;
    }

}//namespace geometrix;

#endif //GEOMETRIX_RAY_LINE_INTERSECTION_HPP
