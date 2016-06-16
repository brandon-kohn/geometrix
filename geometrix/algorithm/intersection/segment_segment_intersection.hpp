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
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/bounding_box_intersection.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/numeric/constants.hpp>

#include <boost/fusion/algorithm/query/all.hpp>

namespace geometrix {

    namespace segment_intersection_detail {

        template <typename PointA, typename PointB, typename PointC, typename PointD, typename XPoint, typename NumberComparisonPolicy>
        inline intersection_type parallel_intersection( const PointA& A, const PointB& B, const PointC& C, const PointD& D, XPoint* xPoint, const NumberComparisonPolicy& cmp )
        {    
            if( !is_collinear( A, B, C, cmp ) )
                return e_non_crossing;

            bool CisBetweenAB = is_between( A, B, C, true, cmp );
            bool DisBetweenAB = is_between( A, B, D, true, cmp );

            if ( CisBetweenAB && DisBetweenAB )
            {
                if(xPoint)
                {
                    xPoint[0] = C;
                    xPoint[1] = D;
                }

                return e_overlapping;
            }

            bool AisBetweenCD = is_between( C, D, A, true, cmp );
            bool BisBetweenCD = is_between( C, D, B, true, cmp );

            if ( AisBetweenCD && BisBetweenCD )
            {
                if(xPoint)
                {
                    xPoint[0] = A;
                    xPoint[1] = B;
                }

                return e_overlapping;
            }

            if( CisBetweenAB && BisBetweenCD )
            {
                if(xPoint)
                {
                    xPoint[0] = C;
                    xPoint[1] = B;
                }

                return e_overlapping;
            }

            if( CisBetweenAB && AisBetweenCD )
            {
                if(xPoint)
                {
                    xPoint[0] = C;
                    xPoint[1] = A;
                }

                return e_overlapping;
            }

            if( DisBetweenAB && BisBetweenCD )
            {
                if(xPoint)
                {
                    xPoint[0] = D;
                    xPoint[1] = B;
                }

                return e_overlapping;
            }

            if( DisBetweenAB && AisBetweenCD )
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
			
			using length_t = typename geometric_traits<XPoint>::arithmetic_type;
			using area_t = decltype(length_t() * length_t());
			using dimensionless_t = typename geometric_traits<XPoint>::dimensionless_type;

            area_t denom = get<0>( A ) * (get<1>( D ) - get<1>( C )) +
                           get<0>( B ) * (get<1>( C ) - get<1>( D )) +
                           get<0>( D ) * (get<1>( B ) - get<1>( A )) +
                           get<0>( C ) * (get<1>( A ) - get<1>( B ));

            //If denom is zeros then segments are parallel.
            if( cmp.equals( denom, constants::zero<area_t>() ) )
                return parallel_intersection( A, B, C, D, xPoint, cmp );
    
            area_t num = get<0>( A ) * (get<1>( D ) - get<1>( C )) 
                       + get<0>( C ) * (get<1>( A ) - get<1>( D )) 
                       + get<0>( D ) * (get<1>( C ) - get<1>( A ));

            if( cmp.equals( num, constants::zero<area_t>()) || cmp.equals( num, denom ) )
                iType = e_endpoint;
                        
            dimensionless_t s = num / denom;
    
            num = get(-( get<0>( A ) * (get<1>( C ) - get<1>( B )) 
                  + get<0>( B ) * (get<1>( A ) - get<1>( C )) 
                  + get<0>( C ) * (get<1>( B ) - get<1>( A )) ));

			if( cmp.equals( num, constants::zero<area_t>()) || cmp.equals( num, denom ) )
				iType = e_endpoint;

            dimensionless_t t = num / denom;
    
			auto zero = constants::zero<dimensionless_t>();
			auto one = constants::one<dimensionless_t>();
			if (cmp.less_than(zero, s) && cmp.less_than(s, one) &&
				cmp.less_than(zero, t) && cmp.less_than(t, one))
				iType = e_crossing;
			else if (cmp.greater_than(zero, s) || cmp.greater_than(s, one) || cmp.greater_than(zero, t) || cmp.greater_than(t, one))
				return e_non_crossing;
//             if( zero < s && s < one && zero < t && t < one )
//                 iType = e_crossing;
//             else if( zero > s || s > one || zero > t || t > one )
//                 return e_non_crossing;

            if( xPoint )
                assign(xPoint[0], A + s * (B-A));

            return iType;
        }

        template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
        inline bool is_coplanar( const Point1& x1, const Point2& x2, const Point3& x3, const Point4& x4, const NumberComparisonPolicy& cmp )
        {
			auto dot = dot_product((x3 - x1), ((x2 - x1) ^ (x4 - x3)));
            return cmp.equals(dot, constants::zero<decltype(dot)>());
        }

        //! 3d intersection test.
        template <typename Point1, typename Point2, typename Point3, typename Point4, typename XPoint, typename NumberComparisonPolicy>
        inline intersection_type segment_segment_intersection( const Point1& p1, const Point2& p2, const Point3& p3, const Point4& p4, XPoint* iPoint, const NumberComparisonPolicy& cmp, dimension<3> )
        {
            if( !is_coplanar( p1, p2, p3, p4, cmp ) )
                return e_non_crossing;

			using length_t = typename geometric_traits<XPoint>::arithmetic_type;
			using area_t = decltype(length_t() * length_t());
			using dimensionless_t = typename geometric_traits<XPoint>::dimensionless_type;
			using vector_t = vector<length_t, 3>;
            
            vector_t v1 = p2-p1 ;
            vector_t v2 = p4-p3 ;
    
            auto a = point_point_distance_sqrd( p1, p2 ) ;
            auto b = dot_product(v1, v2) ;
            auto c = point_point_distance_sqrd( p3, p4 ) ;
            area_t det = a * c - b * b ;

			auto zero = constants::zero<dimensionless_t>();
			auto one = constants::one<dimensionless_t>();
    
            if( cmp.greater_than(det, constants::zero<area_t>()) )
            {
                vector_t u = p1-p3 ;
                auto d = dot_product(v1, u) ;
                auto e = dot_product(v2, u) ;
    
                dimensionless_t s = (b * e - c * d) / det ;
                intersection_type iType = e_invalid_intersection;

                if( cmp.less_than( s, zero ) || cmp.greater_than( s, one ) )
                    return e_non_crossing;                
                else if( cmp.greater_than( s, zero ) && cmp.less_than( s, one ) )
                    iType = e_crossing;
                else
                    iType = e_endpoint;

                dimensionless_t t = (a * e - b * d) / det ;
        
                if( cmp.less_than( t, zero ) || cmp.greater_than( t, one ) )
                    return e_non_crossing;                
                else if( cmp.greater_than( t, zero ) && cmp.less_than( t, one ) )
                    iType = e_crossing;
                else
                    iType = e_endpoint;
        
                iPoint[0] <<= p1 + s * v1;
                return iType;
            }
            else
            {
                //! Parallel
                if( !boost::fusion::all( v1 ^ (p3-p1), boost::lambda::_1 == constants::zero<length_t>() ) )
                    return e_non_crossing;

                //! On the same line.
                area_t f = dot_product(v1, (p3-p1));
                area_t vLengthSqr = magnitude_sqrd(v1) ;
                dimensionless_t s0 = f / vLengthSqr ;
                dimensionless_t s1 = s0 + b / vLengthSqr ;
                auto st = boost::minmax( s0, s1 ) ;

                if( one < boost::get<0>(st) || zero > boost::get<1>(st) )
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
