//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_ANGLE_COMPARE_HPP
#define GEOMETRIX_POINT_ANGLE_COMPARE_HPP
#pragma once

#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/expression.hpp>
#include <geometrix/algebra/exterior_product.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/primitive/point.hpp>
#include <boost/concept_check.hpp>

namespace geometrix {

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

    template <typename NumberComparisonPolicy>
    class point_angle_compare
    {
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));
        
        point_double_2d m_origin;
        const vector_double_2d m_reference;
        NumberComparisonPolicy m_cmp;

    public:

        template <typename Point>
        point_angle_compare( const Point& origin, const NumberComparisonPolicy& cmp = NumberComparisonPolicy() )
            : m_origin( origin )
            , m_reference( 1., 0. )
            , m_cmp(cmp)
        {
            BOOST_CONCEPT_ASSERT((Point2DConcept<Point>));
        }

        template <typename Point1, typename Point2>
        bool operator()( const Point1& a, const Point2& b ) const
        {
            BOOST_CONCEPT_ASSERT((Point2DConcept<Point1>));
            BOOST_CONCEPT_ASSERT((Point2DConcept<Point2>));
            
            const vector_double_2d da = a - m_origin, db = b - m_origin;
            const double detb = exterior_product_area( m_reference, db );

            //! If v2 is along reference it is smallest.
            if( m_cmp.equals(detb, 0) && m_cmp.greater_than_or_equal(dot_product( db, m_reference ), 0) )
                return false;
            
            const double deta = exterior_product_area( m_reference, da );

            //! If v1 is along reference it is smallest.
            if( m_cmp.equals(deta, 0) && m_cmp.greater_than_or_equal(dot_product( da, m_reference ), 0) )
                return true;

            //! If detv1 and detv2 have the same sign, they are on the same side of reference and can be compared directly.
            if( m_cmp.greater_than_or_equal(deta * detb, 0) )
            {
                //! both on same side of reference: compare to each other
                return m_cmp.greater_than(exterior_product_area( da, db ), 0);
            }

            //! At this point one of the two detvX is negative. A negative detvX means a large angle WRT reference.
            //! If v1 is positive it must be smaller than v2, else the opposite must be true.
            return m_cmp.greater_than(deta, 0);
        }
    };

}//namespace geometrix;

#endif //! GEOMETRIX_POINT_ANGLE_COMPARE_HPP
