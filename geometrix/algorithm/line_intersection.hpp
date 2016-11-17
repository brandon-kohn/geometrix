//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINE_INTERSECTION_HPP
#define GEOMETRIX_LINE_INTERSECTION_HPP

#include <geometrix/algorithm/linear_components_intersection.hpp>

#include <boost/typeof/typeof.hpp>

/////////////////////////////////////////////////////////////////////////////
//
// NAMESPACE
//
/////////////////////////////////////////////////////////////////////////////
namespace geometrix {

//! \brief Compute whether the line defined by A->B intersects the line defined by (C->D).
//! Currently implemented to work on types which support fractions (floating-type or rationals).
template <typename PointA, typename PointB, typename PointC, typename PointD, typename PointX, typename Dimensionless, typename NumberComparisonPolicy>
inline intersection_type line_line_intersect( const PointA& A, const PointB& B, const PointC& C, const PointD& D, PointX& xPoint, Dimensionless& s, Dimensionless& t, const NumberComparisonPolicy& cmp )
{       
    BOOST_CONCEPT_ASSERT((PointConcept<PointA>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointB>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointC>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointD>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointX>));

    using length_t = typename geometric_traits<PointX>::arithmetic_type;
    using area_t = decltype(length_t() * length_t());
    
    area_t denom = get<0>( A ) * (get<1>( D ) - get<1>( C )) +
                   get<0>( B ) * (get<1>( C ) - get<1>( D )) +
                   get<0>( D ) * (get<1>( B ) - get<1>( A )) +
                   get<0>( C ) * (get<1>( A ) - get<1>( B ));

    //! If denom is zero then lines are parallel.
	if (cmp.equals(denom, constants::zero<area_t>()))
	{
		if (is_collinear(A, B, C, cmp))
			return e_overlapping;
		else
			return e_non_crossing;
	}
    
    area_t num = get<0>( A ) * (get<1>( D ) - get<1>( C )) 
               + get<0>( C ) * (get<1>( A ) - get<1>( D )) 
               + get<0>( D ) * (get<1>( C ) - get<1>( A ));
	                        
    s = num / denom;
    
    num = get(-( get<0>( A ) * (get<1>( C ) - get<1>( B )) 
              + get<0>( B ) * (get<1>( A ) - get<1>( C )) 
              + get<0>( C ) * (get<1>( B ) - get<1>( A )) ));
	
    t = num / denom;
    
    assign(xPoint, A + s * (B-A));

    return e_crossing;
}

//! \brief Compute whether the line defined by A->B intersects the line defined by (C->D).
//! Currently implemented to work on types which support fractions (floating-type or rationals).
template <typename PointA, typename PointB, typename PointC, typename PointD, typename PointX, typename NumberComparisonPolicy>
inline intersection_type line_line_intersect( const PointA& A, const PointB& B, const PointC& C, const PointD& D, PointX& xPoint, const NumberComparisonPolicy& compare )
{       
    BOOST_CONCEPT_ASSERT((PointConcept<PointA>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointB>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointC>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointD>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointX>));
    
	using length_t = typename geometric_traits<PointX>::arithmetic_type;
	using area_t = decltype(length_t()*length_t());
	
    BOOST_AUTO( denom, get<0>( A ) * (get<1>( D ) - get<1>( C )) +
                       get<0>( B ) * (get<1>( C ) - get<1>( D )) +
                       get<0>( D ) * (get<1>( B ) - get<1>( A )) +
                       get<0>( C ) * (get<1>( A ) - get<1>( B )) );

    //If denom is zeros then segments are parallel.
    if( compare.equals( denom, constants::zero<area_t>()) )
    {
        if( is_collinear( A, B, C, compare ) )
            return e_overlapping;
        else
            return e_non_crossing;
    }        

    BOOST_AUTO( num, -( get<0>( A ) * (get<1>( C ) - get<1>( B )) +
                        get<0>( B ) * (get<1>( A ) - get<1>( C )) +
                        get<0>( C ) * (get<1>( B ) - get<1>( A )) ) );
    
    assign(xPoint, C + (num/denom) * (D-C));
    return e_crossing;
}

//! \brief Compute whether the line defined by A->B intersects the specified segment (C->D).
//! Currently implemented to work on types which support fractions (floating-type or rationals).
template <typename PointA, typename PointB, typename PointC, typename PointD, typename PointX, typename NumberComparisonPolicy>
inline intersection_type line_segment_intersect( const PointA& A, const PointB& B, const PointC& C, const PointD& D, PointX& xPoint, const NumberComparisonPolicy& compare )
{       
    BOOST_CONCEPT_ASSERT((PointConcept<PointA>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointB>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointC>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointD>));
    BOOST_CONCEPT_ASSERT((PointConcept<PointX>));
    intersection_type iType = e_invalid_intersection;

	using length_t = typename geometric_traits<PointX>::arithmetic_type;
	using area_t = decltype(length_t()*length_t());
	using dimensionless_t = typename geometric_traits<PointX>::dimensionless_type;

    BOOST_AUTO( denom, get<0>( A ) * (get<1>( D ) - get<1>( C )) +
                       get<0>( B ) * (get<1>( C ) - get<1>( D )) +
                       get<0>( D ) * (get<1>( B ) - get<1>( A )) +
                       get<0>( C ) * (get<1>( A ) - get<1>( B )) );

    //If denom is zeros then segments are parallel.
    if( compare.equals( denom, constants::zero<area_t>()) )
    {
        if( is_collinear( A, B, C, compare ) )
            return e_overlapping;
        else
            return e_non_crossing;
    }        

    BOOST_AUTO( num, get(-( get<0>( A ) * (get<1>( C ) - get<1>( B )) +
                        get<0>( B ) * (get<1>( A ) - get<1>( C )) +
                        get<0>( C ) * (get<1>( B ) - get<1>( A )) ) ) );
    
    if( compare.equals( num, constants::zero<area_t>()) || compare.equals( num, denom ) )
        iType = e_endpoint;
    
    BOOST_AUTO( t, num / denom );
    
    if (compare.less_than(t, constants::zero<dimensionless_t>()) || compare.greater_than(t, constants::one<dimensionless_t>()))
        return e_non_crossing;

    if( compare.greater_than(t, constants::zero<dimensionless_t>()) && compare.less_than(t, constants::one<dimensionless_t>()) )
        iType = e_crossing;
    
    assign(xPoint, C + t * (D-C));

    return iType;
}

//! \brief Compute whether the line defined by A->B intersects the specified segment.
//! Currently implemented to work on types which support fractions (floating-type or rationals).
template <typename Segment, typename PointA, typename PointB, typename PointX, typename NumberComparisonPolicy>
inline intersection_type line_segment_intersect( const PointA& A, const PointB& B, const Segment& segment, PointX& xPoint, const NumberComparisonPolicy& compare )
{   
    return line_segment_intersect( A, B, get_start( segment ), get_end( segment ), xPoint, compare );
}

}//namespace geometrix;

#endif //GEOMETRIX_LINE_INTERSECTION_HPP
