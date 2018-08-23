//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_ANGLE_COMPARE_HPP
#define GEOMETRIX_VECTOR_ANGLE_COMPARE_HPP
#pragma once

#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/is_null.hpp>
#include <boost/concept_check.hpp>

namespace geometrix {

    //! For a given vector (which really ought to be a unit dimensionless quantity) return a value in the range of [0, 4) which is proportional to the
    template <typename Vector>
    typename dimensionless_type_of<Vector>::type pseudo_angle(const Vector& v)
    {
        GEOMETRIX_ASSERT(!is_null(v));
        using std::abs;
        using std::copysign;
        auto result = copysign(1.0 - get<0>(v) / (abs(get<0>(v))+abs(get<1>(v))), get<1>(v));
        return (get<1>(v) < 0) ? 4 + result : result;
    }

    template <typename NumberComparisonPolicy>
    class vector_angle_compare
    {
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

        const geometrix::vector_double_2d m_reference;
        NumberComparisonPolicy m_cmp;
    public:

        //! Default construct with a reference vector aligned on the X-axis.
        vector_angle_compare( const NumberComparisonPolicy& cmp = NumberComparisonPolicy() )
            : m_reference( 1., 0. )
            , m_cmp( cmp )
        {}

        //! Construct with a user specified reference vector.
        template <typename Vector>
        vector_angle_compare( const Vector& reference, const NumberComparisonPolicy& cmp = NumberComparisonPolicy() )
            : m_reference( reference )
            , m_cmp( cmp )
        {}

        template <typename Vector1, typename Vector2>
        bool operator()( const Vector1& v1, const Vector2& v2 ) const
        {
            BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector1>));
            BOOST_CONCEPT_ASSERT((Vector2DConcept<Vector2>));

            const double detv2 = exterior_product_area( m_reference, v2 );

            //! If v2 is along reference it is smallest.
            if( m_cmp.equals( detv2, 0 ) && m_cmp.greater_than_or_equal( dot_product( v2, m_reference ), 0 ) )
                return false;

            const double detv1 = exterior_product_area( m_reference, v1 );

            //! If v1 is along reference it is smallest.
            if( m_cmp.equals( detv1, 0 ) && m_cmp.greater_than_or_equal( dot_product( v1, m_reference ), 0 ) )
                return true;

            //! If detv1 and detv2 have the same sign, they are on the same side of reference and can be compared directly.
            if( m_cmp.greater_than_or_equal( detv1 * detv2, 0 ) )
            {
                //! Both are on same side of reference: compare to each other.
                return m_cmp.greater_than( exterior_product_area( v1, v2 ), 0 );
            }

            //! At this point one of the two detvX is negative. A negative detvX means a large angle WRT reference.
            //! If v1 is positive it must be smaller than v2, else the opposite must be true.
            return m_cmp.greater_than( detv1, 0 );
        }
    };
}//namespace geometrix;

#endif //! GEOMETRIX_VECTOR_ANGLE_COMPARE_HPP
