//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_INTERSECTION_HPP
#define GEOMETRIX_SEGMENT_INTERSECTION_HPP

#include <geometrix/algorithm/linear_components_intersection.hpp>
#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/bounding_box_intersection.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/fusion/algorithm/query/all.hpp>

namespace geometrix {

	namespace segment_intersection_detail {

		template <typename Point, typename NumberComparisonPolicy>
		inline intersection_type parallel_intersection( const Point& A, const Point& B, const Point& C, const Point& D, Point* xPoint, const NumberComparisonPolicy& compare )
		{    
			if( !is_collinear( A, B, C, compare ) )
				return e_non_crossing;

			bool isBetweenABC = is_between( A, B, C, false, compare );
			bool isBetweenABD = is_between( A, B, D, false, compare );

			if ( isBetweenABC && isBetweenABD )
			{
				if(xPoint)
				{
					xPoint[0] = C;
					xPoint[1] = D;
				}

				return e_overlapping;
			}

			bool isBetweenCDA = is_between( C, D, A, false, compare );
			bool isBetweenCDB = is_between( C, D, B, false, compare );

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

			bool originEqualsC = numeric_sequence_equals( A, C, compare );
			bool destinationEqualsD = numeric_sequence_equals( B, D, compare );
			bool originEqualsD = numeric_sequence_equals( A, D, compare );
			bool destinationEqualsC = numeric_sequence_equals( B, C, compare );
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

		template <typename Point, typename NumberComparisonPolicy>
		inline intersection_type calculate_intersection( const Point& A, const Point& B, const Point& C, const Point& D, Point* xPoint, const NumberComparisonPolicy& compare, dimension<2> )
		{
			intersection_type iType = e_invalid_intersection;

			BOOST_AUTO( denom, get<0>( A ) * (get<1>( D ) - get<1>( C )) +
							   get<0>( B ) * (get<1>( C ) - get<1>( D )) +
							   get<0>( D ) * (get<1>( B ) - get<1>( A )) +
							   get<0>( C ) * (get<1>( A ) - get<1>( B )) );

			//If denom is zeros then segments are parallel.
			if( compare.equals( denom, 0 ) )
				return parallel_intersection( A, B, C, D, xPoint, compare );
    
			BOOST_AUTO( s, arithmetic_promote
						   (
							   get<0>( A ) * (get<1>( D ) - get<1>( C )) 
							 + get<0>( C ) * (get<1>( A ) - get<1>( D )) 
							 + get<0>( D ) * (get<1>( C ) - get<1>( A ))
						   ) / denom );

			if( compare.less_than(s, 0) || compare.greater_than(s, 1) )
				return e_non_crossing;
			else if( compare.greater_than(s, 0) && compare.less_than(s, 1) )
				iType = e_crossing;    
			else
				iType = e_endpoint;
    
			BOOST_AUTO( t, arithmetic_promote
						   (
							   -( get<0>( A ) * (get<1>( C ) - get<1>( B )) 
								+ get<0>( B ) * (get<1>( A ) - get<1>( C )) 
								+ get<0>( C ) * (get<1>( B ) - get<1>( A )) 
								)
						   ) / denom );
    
			if( compare.less_than(t, 0) || compare.greater_than(t, 1) )
				return e_non_crossing;
			else if( compare.greater_than(t, 0) && compare.less_than(t, 1) )
				iType = e_crossing;
			else
				iType = e_endpoint;

			if( xPoint )
				assign(xPoint[0], A + s * (B-A));

			return iType;
		}

		template <typename Point1, typename Point2, typename Point3, typename Point4, typename NumberComparisonPolicy>
		inline bool is_coplanar( const Point1& x1, const Point2& x2, const Point3& x3, const Point4& x4, const NumberComparisonPolicy& compare )
		{
			return compare.equals(dot_product((x3-x1),((x2-x1)^(x4-x3))), 0);
		}

		//! 3d intersection test.
		template <typename Point, typename NumberComparisonPolicy>
		inline intersection_type calculate_intersection( const Point& p1, const Point& p2, const Point& p3, const Point& p4, Point* iPoint, const NumberComparisonPolicy& compare, dimension<3> )
		{
			if( !is_coplanar( p1, p2, p3, p4, compare ) )
				return e_non_crossing;
            
			BOOST_AUTO( v1, p2-p1 );
			BOOST_AUTO( v2, p4-p3 );
    
			BOOST_AUTO( a, point_point_distance_sqrd( p1, p2 ) );
			BOOST_AUTO( b, dot_product(v1,v2) );
			BOOST_AUTO( c, point_point_distance_sqrd( p3, p4 ) );
			BOOST_AUTO( det, a * c - b * b );
    
			if( compare.greater_than(det,0.) )
			{
				BOOST_AUTO( u, p1-p3 );
				BOOST_AUTO( d, dot_product(v1,u) );
				BOOST_AUTO( e, dot_product(v2,u) );
    
				BOOST_AUTO( s, arithmetic_promote(b * e - c * d) / det );
				intersection_type iType = e_invalid_intersection;

				if( compare.less_than( s, 0. ) || compare.greater_than( s, 1. ) )
					return e_non_crossing;                
				else if( compare.greater_than( s, 0. ) && compare.less_than( s, 1. ) )
					iType = e_crossing;
				else
					iType = e_endpoint;

				BOOST_AUTO( t, arithmetic_promote(a * e - b * d) / det );
        
				if( compare.less_than( t, 0. ) || compare.greater_than( t, 1. ) )
					return e_non_crossing;                
				else if( compare.greater_than( t, 0. ) && compare.less_than( t, 1. ) )
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
							iPoint[0] = construct<Point>(p1);

						if( 1. > boost::get<1>(st) )
							iPoint[1] <<= p1 + boost::get<1>(st) * v1; 
						else
							iPoint[1] <<= p1 + v1; 

						return e_overlapping;
					}
					else
					{
						iPoint[0] = construct<Point>(p1);
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

	template <typename Point, typename NumberComparisonPolicy>
	inline intersection_type calculate_intersection( const Point& p1, const Point& p2, const Point& p3, const Point& p4, Point* iPoint, const NumberComparisonPolicy& compare )
	{
		return segment_intersection_detail::calculate_intersection( p1, p2, p3, p4, iPoint, compare, typename dimension_of<Point>::type() );
	}

	//! Compute whether the segment defined by A->B intersects the specified segment.
	template <typename Segment, typename Point, typename NumberComparisonPolicy>
	inline intersection_type calculate_intersection( const Segment& segment1, const Segment& segment2, Point* xPoints, const NumberComparisonPolicy& compare )
	{
		BOOST_CONCEPT_ASSERT(( SegmentConcept< Segment > ));

		return segment_intersection_detail::calculate_intersection( get_start( segment1 )
									 , get_end( segment1 )
									 , get_start( segment2 )
									 , get_end( segment2 )
									 , xPoints
									 , compare
									 , typename dimension_of<Segment>::type() );
	}

}//namespace geometrix;

#endif //GEOMETRIX_SEGMENT_INTERSECTION_HPP
