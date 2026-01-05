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
	
    template <typename Point, typename NumberComparisonPolicy>
    class point_angle_compare
    {
        BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

        using point_t = typename std::decay<Point>::type;
		using length_t = typename arithmetic_type_of<point_t>::type;
		using vector_t = vector<length_t, dimension_of<point_t>::value>;

        const point_t m_origin;
        const vector_t m_reference;
        NumberComparisonPolicy m_cmp;

    public:

        template <typename Point>
        point_angle_compare( const Point& origin, const NumberComparisonPolicy& cmp = NumberComparisonPolicy() )
            : m_origin( origin )
			, m_reference( constants::one<length_t>(), constants::zero<length_t>() )
            , m_cmp(cmp)
        {
            BOOST_CONCEPT_ASSERT((Point2DConcept<Point>));
        }

        template <typename Point1, typename Point2>
        bool operator()( const Point1& a, const Point2& b ) const
        {
            BOOST_CONCEPT_ASSERT((Point2DConcept<Point1>));
            BOOST_CONCEPT_ASSERT((Point2DConcept<Point2>));
            
            const vector_t da = a - m_origin, db = b - m_origin;
            const auto detb = exterior_product_area( m_reference, db );

            //! If v2 is along reference it is smallest.
			const auto zero = constants::zero<decltype( detb )>();
            if( m_cmp.equals(detb, zero) && m_cmp.greater_than_or_equal(dot_product( db, m_reference ), zero) )
                return false;
            
            const auto deta = exterior_product_area( m_reference, da );

            //! If v1 is along reference it is smallest.
            if( m_cmp.equals(deta, zero) && m_cmp.greater_than_or_equal(dot_product( da, m_reference ), zero) )
                return true;

            //! If detv1 and detv2 have the same sign, they are on the same side of reference and can be compared directly.
			if( m_cmp.greater_than_or_equal( deta * detb, decltype( deta * detb ){} ) )
            {
                //! both on same side of reference: compare to each other
                return m_cmp.greater_than(exterior_product_area( da, db ), zero);
            }

            //! At this point one of the two detvX is negative. A negative detvX means a large angle WRT reference.
            //! If v1 is positive it must be smaller than v2, else the opposite must be true.
            return m_cmp.greater_than(deta, zero);
        }
    };

}//namespace geometrix;

#endif //! GEOMETRIX_POINT_ANGLE_COMPARE_HPP
