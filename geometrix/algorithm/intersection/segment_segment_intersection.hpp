//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_INTERSECTION_SEGMENT_SEGMENT_INTERSECTION_HPP
#define GEOMETRIX_ALGORITHM_INTERSECTION_SEGMENT_SEGMENT_INTERSECTION_HPP

#include <geometrix/algorithm/linear_components_intersection.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/bounding_box_intersection.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/fusion/algorithm/query/all.hpp>

namespace geometrix {

    namespace segment_intersection_detail {

        template <typename PointA, typename PointB, typename PointC, typename PointD, typename XPoint, typename NumberComparisonPolicy>
        inline intersection_type parallel_intersection( const PointA& A, const PointB& B, const PointC& C, const PointD& D, XPoint* xPoint, const NumberComparisonPolicy& cmp )
        {    
            if( !is_collinear( A, B, C, cmp ) )
                return e_non_crossing;

            bool isBetweenABC = is_between( A, B, C, false, cmp );
            bool isBetweenABD = is_between( A, B, D, false, cmp );

            if ( isBetweenABC && isBetweenABD )
            {
                if(xPoint)
                {
                    xPoint[0] = C;
                    xPoint[1] = D;
                }

                return e_overlapping;
            }

            bool isBetweenCDA = is_between( C, D, A, false, cmp );
            bool isBetweenCDB = is_between( C, D, B, false, cmp );

            if ( isBetweenCDA && isBetweenCDB )
            {
                if(xPoint)
                {
                    xPoint[0] = A;
                    xPoint[1] = B;
                }

                return e_overlapping;
            }

            if( isBetweenABC && isBetweenCDB )
            {
                if(xPoint)
                {
                    xPoint[0] = C;
                    xPoint[1] = B;
                }

                return e_overlapping;
            }

            if( isBetweenABC && isBetweenCDA )
            {
                if(xPoint)
                {
                    xPoint[0] = C;
                    xPoint[1] = A;
                }

                return e_overlapping;
            }

            if( isBetweenABD && isBetweenCDB )
            {
                if(xPoint)
                {
                    xPoint[0] = D;
                    xPoint[1] = B;
                }

                return e_overlapping;
            }

            if( isBetweenABD && isBetweenCDA )
            {
                if(xPoint)
                {
                    xPoint[0] = D;
                    xPoint[1] = A;
                }

                return e_overlapping;
            }

            bool originEqualsC = numeric_sequence_equals( A, C, cmp );
            bool destinationEqualsD = numeric_sequence_equals( B, D, cmp );
            bool originEqualsD = numeric_sequence_equals( A, D, cmp );
            bool destinationEqualsC = numeric_sequence_equals( B, C, cmp );
            if( (originEqualsC && destinationEqualsD) || (originEqualsD && destinationEqualsC) )
            {
                if(xPoint)
                {
                    xPoint[0] = A;
                    xPoint[1] = B;
                }

                return e_overlapping;
            }

            if(originEqualsC)
            {
                if(xPoint)
                    xPoint[0] = A;
                return e_endpoint;
            }

            if(originEqualsD)
            {
                if(xPoint)
                    xPoint[0] = A;
                return e_endpoint;
            }

            if(destinationEqualsC)
            {
                if(xPoint)
                    xPoint[0] = B;
                return e_endpoint;
            }

            if(destinationEqualsD)
            {
                if(xPoint)
                    xPoint[0] = B;
                return e_endpoint;
            }

            return e_non_crossing;
        }

        template <typename PointA, typename PointB, typename PointC, typename PointD, typename XPoint, typename NumberComparisonPolicy>
        inline intersection_type segment_segment_intersection( const PointA& A, const PointB& B, const PointC& C, const PointD& D, XPoint* xPoint, const NumberComparisonPolicy& cmp, dimension<2> )
        {
            intersection_type iType = e_invalid_intersection;

            BOOST_AUTO( denom, get<0>( A ) * (get<1>( D ) - get<1>( C )) +
                               get<0>( B ) * (get<1>( C ) - get<1>( D )) +
                               get<0>( D ) * (get<1>( B ) - get<1>( A )) +
                               get<0>( C ) * (get<1>( A ) - get<1>( B )) );

            //If denom is zeros then segments are parallel.
            if( cmp.equals( denom, 0 ) )
                return parallel_intersection( A, B, C, D, xPoint, cmp );
    
            BOOST_AUTO( num, arithmetic_promote
                           (
                               get<0>( A ) * (get<1>( D ) - get<1>( C )) 
                             + get<0>( C ) * (get<1>( A ) - get<1>( D )) 
                             + get<0>( D ) * (get<1>( C ) - get<1>( A ))
                           ) );

            if( cmp.equals( num, 0 ) || cmp.equals( num, denom ) )
                iType = e_endpoint;
                        
            BOOST_AUTO( s, num / denom );
    
            num = arithmetic_promote
                  (
                    -( get<0>( A ) * (get<1>( C ) - get<1>( B )) 
                    + get<0>( B ) * (get<1>( A ) - get<1>( C )) 
                    + get<0>( C ) * (get<1>( B ) - get<1>( A )) )
                  );

            BOOST_AUTO( t, num / denom );
    
            if( cmp.less_than( 0, s ) && cmp.less_than( s, 1 ) &&
                cmp.less_than( 0, t ) && cmp.less_than( t, 1 ) )
                iType = e_crossing;
            else if( cmp.greater_than( 0, s ) || cmp.greater_than( s, 1 ) || cmp.greater_than( 0, t ) || cmp.greater_than( t, 1 ) )
                return e_non_crossing;

            if( xPoint )
                assign(xPoint[0], A + s * (B-A));

            return iType;
        }

        template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
        inline bool is_coplanar( const Point1& x1, const Point2& x2, const Point3& x3, const Point4& x4, const NumberComparisonPolicy& cmp )
        {
            return cmp.equals(dot_product((x3-x1),((x2-x1)^(x4-x3))), 0);
        }

        //! 3d intersection test.
        template <typename Point1, typename Point2, typename Point3, typename Point4, typename XPoint, typename NumberComparisonPolicy>
        inline intersection_type segment_segment_intersection( const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4, XPoint* iPoint, const NumberComparisonPolicy& cmp, dimension<3> )
        {
            if( !is_coplanar( p1, p2, p3, p4, cmp ) )
                return e_non_crossing;
            
            BOOST_AUTO( v1, p2-p1 );
            BOOST_AUTO( v2, p4-p3 );
    
            BOOST_AUTO( a, point_point_distance_sqrd( p1, p2 ) );
            BOOST_AUTO( b, dot_product(v1,v2) );
            BOOST_AUTO( c, point_point_distance_sqrd( p3, p4 ) );
            BOOST_AUTO( det, a * c - b * b );
    
            if( cmp.greater_than(det,0.) )
            {
                BOOST_AUTO( u, p1-p3 );
                BOOST_AUTO( d, dot_product(v1,u) );
                BOOST_AUTO( e, dot_product(v2,u) );
    
                BOOST_AUTO( s, arithmetic_promote(b * e - c * d) / det );
                intersection_type iType = e_invalid_intersection;

                if( cmp.less_than( s, 0. ) || cmp.greater_than( s, 1. ) )
                    return e_non_crossing;                
                else if( cmp.greater_than( s, 0. ) && cmp.less_than( s, 1. ) )
                    iType = e_crossing;
                else
                    iType = e_endpoint;

                BOOST_AUTO( t, arithmetic_promote(a * e - b * d) / det );
        
                if( cmp.less_than( t, 0. ) || cmp.greater_than( t, 1. ) )
                    return e_non_crossing;                
                else if( cmp.greater_than( t, 0. ) && cmp.less_than( t, 1. ) )
                    iType = e_crossing;
                else
                    iType = e_endpoint;
        
                iPoint[0] <<= p1 + s * v1;
                return iType;
            }
            else
            {
                //! Parallel
                if( !boost::fusion::all( v1 ^ (p3-p1), boost::lambda::_1 == 0. ) )
                    return e_non_crossing;

                //! On the same line.
                BOOST_AUTO( f, dot_product(v1,(p3-p1)));
                BOOST_AUTO( vLengthSqr, magnitude_sqrd(v1) );
                BOOST_AUTO( s0, arithmetic_promote(f) / vLengthSqr );
                BOOST_AUTO( s1, s0 + arithmetic_promote(b) / vLengthSqr );
                BOOST_AUTO( st, boost::minmax( s0, s1 ) );

                if( 1. < boost::get<0>(st) || 0. > boost::get<1>(st) )
                    return e_non_crossing;
                else if( 1. > boost::get<0>(st) )
                {
                    if( 0. < boost::get<1>(st) )
                    {
                        if( 0. < boost::get<0>(st) )
                            iPoint[0] <<= p1 + boost::get<0>(st) * v1;
                        else
                            iPoint[0] = construct<XPoint>(p1);

                        if( 1. > boost::get<1>(st) )
                            iPoint[1] <<= p1 + boost::get<1>(st) * v1; 
                        else
                            iPoint[1] <<= p1 + v1; 

                        return e_overlapping;
                    }
                    else
                    {
                        iPoint[0] = construct<XPoint>(p1);
                        return e_endpoint;
                    }
                }
                else
                {
                    iPoint[0] <<= p1 + v1;
                    return e_endpoint;
                }
            }
        }
    }//! segment_intersection_detail

    template <typename Point1, typename Point2, typename Point3, typename Point4, typename XPoint, typename NumberComparisonPolicy>
    inline intersection_type segment_segment_intersection( const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4, XPoint* iPoint, const NumberComparisonPolicy& cmp )
    {
		BOOST_CONCEPT_ASSERT( (PointConcept<Point1>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point2>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point3>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<Point4>) );
		BOOST_CONCEPT_ASSERT( (PointConcept<XPoint>) );
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );

		return segment_intersection_detail::segment_segment_intersection( p1, p2, p3, p4, iPoint, cmp, typename dimension_of<Point1>::type() );
    }

    //! Compute whether the segment defined by A->B intersects the specified segment.
    template <typename Segment1, typename Segment2, typename Point, typename NumberComparisonPolicy>
    inline intersection_type segment_segment_intersection( const Segment1& segment1, const Segment2& segment2, Point* xPoints, const NumberComparisonPolicy& cmp )
    {
		BOOST_CONCEPT_ASSERT( (SegmentConcept< Segment1 >) );
		BOOST_CONCEPT_ASSERT( (SegmentConcept< Segment2 >) );
		BOOST_CONCEPT_ASSERT( (NumberComparisonPolicyConcept<NumberComparisonPolicy>) );

        return segment_intersection_detail::segment_segment_intersection( get_start( segment1 )
                                     , get_end( segment1 )
                                     , get_start( segment2 )
                                     , get_end( segment2 )
                                     , xPoints
                                     , cmp
                                     , typename dimension_of<Segment1>::type() );
    }

}//namespace geometrix;

#endif //GEOMETRIX_ALGORITHM_INTERSECTION_SEGMENT_SEGMENT_INTERSECTION_HPP
