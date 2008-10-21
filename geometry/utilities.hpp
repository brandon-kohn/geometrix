//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_UTILITIES_HPP
#define _BOOST_GEOMETRY_UTILITIES_HPP
#pragma once

#include "geometric_concepts.hpp"
#include "point_traits.hpp"
#include "number_comparison_policy.hpp"
#include "constants.hpp"
#include "math_functions.hpp"
#include <boost/utility.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cmath>

namespace boost
{
namespace numeric
{
namespace geometry
{
	//! Function to get the angle from an origin to a target point in the 2D XY plane.
	template <typename Point>
    inline typename point_traits<Point>::coordinate_type angle_to_point( const Point& A, const Point& B, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0   )
	{
        typedef cartesian_access_traits< Point > access_traits;
        typedef typename point_traits< Point >::coordinate_type coordinate_type;
        return math_functions< coordinate_type >::atan2( access_traits::get<1>( B ) - access_traits::get<1>( A ), access_traits::get<0>( B ) - access_traits::get<0>( A ) );
	}

    //! Function to normalize an angle to within the interval [0,2*PI]
    template <typename CoordinateType>
    inline void normalize_angle( CoordinateType& angle )
    {
        //simplifies the angle to lay in the range of the interval 0 - 2*pi
        CoordinateType pi = constants<CoordinateType>::pi();        
        CoordinateType twoPI = CoordinateType( 2 ) * pi;
        if ( angle > twoPI || angle < CoordinateType( 0 ) )
        {
            CoordinateType n = math_functions<CoordinateType>::floor( angle / twoPI );
            if ( n != CoordinateType( 0 ) )
                angle -= twoPI * n;
            if ( angle > twoPI )
		        angle -= twoPI;
	        else if ( angle < CoordinateType( 0 ) )
                angle += twoPI;            
        }
    }

	//! \brief Compute the squared distance between two points A-B in the 2D Cartesian plane.
	template <typename Point>
	inline typename point_traits<Point>::coordinate_type distance_to_point_squared( const Point& A, const Point& B, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
	{
        typedef cartesian_access_traits< Point > access_traits;
		typename point_traits<Point>::coordinate_type dx = access_traits::get<0>( B ) - access_traits::get<0>( A );
		typename point_traits<Point>::coordinate_type dy = access_traits::get<1>( B ) - access_traits::get<1>( A );
		return ( dx * dx + dy * dy );		
	}

	//! \brief Compute the distance between two points A-B in the 2D Cartesian plane.
	template <typename Point>
	inline typename point_traits<Point>::coordinate_type distance_to_point( const Point& A, const Point& B, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
	{
        typedef cartesian_access_traits< Point > access_traits;        
        typedef typename point_traits< Point >::coordinate_type coordinate_type;
		coordinate_type dx = access_traits::get<0>( B ) - access_traits::get<0>( A );
		coordinate_type dy = access_traits::get<1>( B ) - access_traits::get<1>( A );
        return math_functions< coordinate_type >::sqrt( dx * dx + dy * dy );		
	}

    //! Function to determine if two points are equal to within tolerance.
    template <typename NumberComparisonPolicy, typename Point>
    inline bool equals( const Point& A, const Point& B, const NumberComparisonPolicy& compare, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
    {        
        typedef cartesian_access_traits< Point > access_traits;                
        return compare.equals( access_traits::get<0>( A ), access_traits::get<0>( B ) ) && compare.equals( access_traits::get<1>( A ), access_traits::get<1>( B ) );
    }

	//! Function to determine if 3 points are collinear in the 2D XY plane.
	//! From Computational Geometry in C by J. O'Rourke.
	template <typename NumberComparisonPolicy, typename Point>
	inline bool is_collinear( const Point& A, const Point& B, const Point& C, const NumberComparisonPolicy& compare, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
	{
        typedef cartesian_access_traits< Point > access_traits;        
    	typename point_traits<Point>::coordinate_type det = cross_product( A, B, C );
        typename point_traits<Point>::coordinate_type zero = 0;
		return compare.equals( det, zero );//Absolute tolerance checks are fine for Zero checks.
	}

	//! Function to determine if Point C is between points A-B
	//! From Computational Geometry in C by J. O'Rourke.
	template <typename NumberComparisonPolicy, typename Point>
	bool is_between( const Point& A, const Point& B, const Point& C, bool includeBounds, const NumberComparisonPolicy& compare, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0 )
	{
        typedef cartesian_access_traits< Point >        access_traits;        
        typedef typename access_traits::coordinate_type coordinate_type;
        boost::function_requires< NumberComparisonPolicyConcept<NumberComparisonPolicy, coordinate_type > >();

		if( !is_collinear( A, B, C, compare ) )
		{
			return false;
		}

		//If AB not vertical, check between on x; else on y.
        bool ABVertical = compare.equals( access_traits::get<0>( A ), access_traits::get<0>( B ) );
		if( !ABVertical )
		{
			if(includeBounds)
			{
				return ( compare.less_than_or_equals( access_traits::get<0>( A ), access_traits::get<0>( C ) ) && compare.less_than_or_equals( access_traits::get<0>( C ), access_traits::get<0>( B ) ) ||
						 compare.greater_than_or_equals( access_traits::get<0>( A ), access_traits::get<0>( C ) ) && compare.greater_than_or_equals( access_traits::get<0>( C ), access_traits::get<0>( B ) ) );
			}
			else
			{
				return ( compare.less_than( access_traits::get<0>( A ), access_traits::get<0>( C ) ) && compare.less_than( access_traits::get<0>( C ), access_traits::get<0>( B ) ) ||
						 compare.greater_than( access_traits::get<0>( A ), access_traits::get<0>( C ) ) && compare.greater_than( access_traits::get<0>( C ), access_traits::get<0>( B ) ) );
			}
		}
		else
		{
			if(includeBounds)
			{
				return ( compare.less_than_or_equals( access_traits::get<1>( A ), access_traits::get<1>( C ) ) && compare.less_than_or_equals( access_traits::get<1>( C ), access_traits::get<1>( B ) ) ||
				 		 compare.greater_than_or_equals( access_traits::get<1>( A ), access_traits::get<1>( C ) ) && compare.greater_than_or_equals( access_traits::get<1>( C ), access_traits::get<1>( B ) ) );
			}
			else
			{
				return ( compare.less_than( access_traits::get<1>( A ), access_traits::get<1>( C ) ) && compare.less_than( access_traits::get<1>( C ), access_traits::get<1>( B ) ) ||
				         compare.greater_than( access_traits::get<1>( A ), access_traits::get<1>( C ) ) && compare.greater_than( access_traits::get<1>( C ), access_traits::get<1>( B ) ) );
			}
		}
	}

    //! Orientation test to check if point C is left, collinear, or right of the line formed by A-B.
    enum orientation_type
    {
        oriented_right     = -1,
        oriented_collinear = 0,
        oriented_left      = 1
    };
    template <typename NumberComparisonPolicy, typename Point>
    orientation_type get_orientation( const Point& A, const Point& B, const Point& C, const NumberComparisonPolicy& compare )
    {
        typename point_traits<Point>::coordinate_type cross = cross_product( A, B, C );
        typename point_traits<Point>::coordinate_type zero = 0;
        if( compare.less_than( cross, zero ) )
        {
            return oriented_right;
        }
        else if( compare.greater_than( cross, zero ) )
        {
            return oriented_left;
        }
        else
        {
            return oriented_collinear;
        }
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline bool is_vertical( const Point& start, const Point& end, const NumberComparisonPolicy& compare )
    {
        return compare.equals( cartesian_access_traits< Point >::get<0>( start ), cartesian_access_traits< Point >::get<0>( end ) );
    }

    template <typename Segment, typename NumberComparisonPolicy>
    inline bool is_vertical( const Segment& s,const NumberComparisonPolicy& compare )
    {
        return is_vertical( segment_access_traits< Segment >::get_start( s ), segment_access_traits< Segment >::get_end( s ), compare );
    }

    template <typename Point, typename NumberComparisonPolicy>
    inline bool is_horizontal( const Point& start, const Point& end, const NumberComparisonPolicy& compare )
    {
        return compare.equals( cartesian_access_traits< Point >::get<1>( start ), cartesian_access_traits< Point >::get<1>( end ) );
    }

    template <typename Segment, typename NumberComparisonPolicy>
    inline bool is_horizontal( const Segment& s,const NumberComparisonPolicy& compare )
    {
        return is_horizontal( segment_access_traits< Segment >::get_start( s ), segment_access_traits< Segment >::get_end( s ), compare );
    }

    //! function to get the slope defined by two points
    template <typename Point>
    inline typename point_traits< Point >::coordinate_type get_slope( const Point& s_start, const Point& s_end )
    {
        typedef Point point_type;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        coordinate_type y0, y1, x0, x1;

        x0 = cartesian_access_traits< point_type >::get<0>( s_start );
        x1 = cartesian_access_traits< point_type >::get<0>( s_end );
        y0 = cartesian_access_traits< point_type >::get<1>( s_start );
        y1 = cartesian_access_traits< point_type >::get<1>( s_end );
                
        coordinate_type slope = (y1-y0)/(x1-x0);
        return slope;
    }

    //! function to get the slope defined by a segment.
    template <typename Segment>
    inline typename segment_traits< Segment >::coordinate_type get_slope( const Segment& s )
    {
        return get_slope< typename segment_traits< Segment >::point_type >( segment_access_traits< Segment >::get_start( s ), segment_access_traits< Segment >::get_end ( s ) );
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType y_of_x( const Point& s_start, const Point& s_end, CoordinateType x, CoordinateType& slope, const NumberComparisonPolicy& compare )
    {
        typedef Point point_type;        
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        coordinate_type y0, y1, x0, x1;

        x0 = cartesian_access_traits< point_type >::get<0>( s_start );
        x1 = cartesian_access_traits< point_type >::get<0>( s_end );
        y0 = cartesian_access_traits< point_type >::get<1>( s_start );
        y1 = cartesian_access_traits< point_type >::get<1>( s_end );

        slope = (y1-y0)/(x1-x0);

        coordinate_type y = (x - x0)*slope + y0;
        
        return y;
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType y_of_x( const Point& s_start, const Point& s_end, CoordinateType x, const NumberComparisonPolicy& compare )
    {
        CoordinateType slope;
        return y_of_x( s_start, s_end, x, slope, compare );
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType x_of_y( const Point& s_start, const Point& s_end, CoordinateType y, CoordinateType& slope, const NumberComparisonPolicy& compare )
    {
        typedef Point point_type;
        CoordinateType y0, y1, x0, x1;

        x0 = cartesian_access_traits< point_type >::get<0>( s_start );
        x1 = cartesian_access_traits< point_type >::get<0>( s_end );
        y0 = cartesian_access_traits< point_type >::get<1>( s_start );
        y1 = cartesian_access_traits< point_type >::get<1>( s_end );
                
        slope = (y1-y0)/(x1-x0);

        CoordinateType x = (y - y0)/slope + x0;     
        assert( compare.equals( y, y_of_x( s_start, s_end, x, compare )  ) );

        return x;
    }

    //! Given two points which define a (non-vertical) line segment and a coordinate X calculate Y and the slope.
    template <typename Point, typename CoordinateType, typename NumberComparisonPolicy>
    inline CoordinateType x_of_y( const Point& s_start, const Point& s_end, CoordinateType y, const NumberComparisonPolicy& compare )
    {
        CoordinateType slope;
        return x_of_y( s_start, s_end, y, slope, compare  );
    }

    //! Lexicographical compare functor for Cartesian points. Sorts first in X and then in Y (then Z).
    template <typename NumberComparisonPolicy>
    struct lexicographical_point_compare
    {        
        lexicographical_point_compare(){}
        lexicographical_point_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
        {}

        //! older compilers require disambiguation
        template <int> struct disambiguation_tag { disambiguation_tag(int) {} };

        template <typename Point>
        bool operator()( const Point& p1, const Point& p2, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0, disambiguation_tag<0> = 0 ) const
        {
            typedef cartesian_access_traits< Point > access;
            return ( m_compare.less_than( access::get<0>( p1 ), access::get<0>( p2 ) ) ) ||
                   ( m_compare.equals( access::get<0>( p1 ), access::get<0>( p2 ) ) && m_compare.less_than( access::get<1>( p1 ), access::get<1>( p2 ) ) );
        }

        template <typename Point>
        bool operator()( const Point& p1, const Point& p2, typename boost::disable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0, disambiguation_tag<1> = 0 ) const
        {
            typedef cartesian_access_traits< Point > access;
            return ( m_compare.less_than( access::get<0>( p1 ), access::get<0>( p2 ) ) )||
                   ( m_compare.equals( access::get<0>( p1 ), access::get<0>( p2 ) ) && m_compare.less_than( access::get<1>( p1 ), access::get<1>( p2 ) ) ) ||
                   ( m_compare.equals( access::get<0>( p1 ), access::get<0>( p2 ) ) && m_compare.equals( access::get<1>( p1 ), access::get<1>( p2 ) ) && m_compare.less_than( access::get<2>( p1 ), access::get<2>( p2 ) ) );
        }

        NumberComparisonPolicy m_compare;

    };

    //! Lexicographical compare functor for Cartesian points - reversed to sort in (Z), then Y then X.
    template <typename NumberComparisonPolicy>
    struct reverse_lexicographical_point_compare
    {
        reverse_lexicographical_point_compare(){}
        reverse_lexicographical_point_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
        {}

        //! older compilers require disambiguation
        template <int> struct disambiguation_tag { disambiguation_tag(int) {} };

        template <typename Point>
        bool operator()( const Point& p1, const Point& p2, typename boost::enable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0, disambiguation_tag<0> = 0 ) const
        {
            typedef cartesian_access_traits< Point > access;
            return ( m_compare.less_than( access::get<1>( p1 ), access::get<1>( p2 ) ) ) ||
                   ( m_compare.equals( access::get<1>( p1 ), access::get<1>( p2 ) ) && m_compare.less_than( access::get<0>( p1 ), access::get<0>( p2 ) ) );
        }

        template <typename Point>
        bool operator()( const Point& p1, const Point& p2, typename boost::disable_if< boost::is_same< typename point_traits<Point>::dimension_type, dimension_traits<2> > >::type* dummy = 0, disambiguation_tag<1> = 0 ) const
        {
            typedef cartesian_access_traits< Point > access;
            return ( m_compare.less_than( access::get<2>( p1 ), access::get<2>( p2 ) ) )||
                   ( m_compare.equals( access::get<2>( p1 ), access::get<2>( p2 ) ) && m_compare.less_than( access::get<1>( p1 ), access::get<1>( p2 ) ) ) ||
                   ( m_compare.equals( access::get<2>( p1 ), access::get<2>( p2 ) ) && m_compare.equals( access::get<1>( p1 ), access::get<1>( p2 ) ) && m_compare.less_than( access::get<0>( p1 ), access::get<0>( p2 ) ) );
        }

        NumberComparisonPolicy m_compare;

    };

    //! Lexicographical compare functor for Cartesian points. Sorts first in X and then in Y (then Z).
    template <typename NumberComparisonPolicy>
    struct lexicographical_segment_compare
    {
        lexicographical_segment_compare(){}
        lexicographical_segment_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
            , m_pointCompare( compare )
        {}

        template <typename Segment>
        inline bool operator()( const Segment& s1, const Segment& s2 ) const
        {
            typedef segment_access_traits< Segment > access;
            typedef segment_traits< Segment >::point_type point_type;

            const point_type& start1 = access::get_start( s1 );
            const point_type& end1   = access::get_end( s2 );
            const point_type& start2 = access::get_start( s2 );
            const point_type& end2   = access::get_end( s2 );

            //Order the segments lexicographically first.
            const point_type* lower1;
            const point_type* upper1;
            const point_type* lower2;
            const point_type* upper2;

            if( m_pointCompare( start1, end1 ) )
            {
                lower1 = &start1;
                upper1 = &end1;
            }
            else
            {
                lower1 = &end1;
                upper1 = &start1;
            }

            if( m_pointCompare( start2, end2 ) )
            {
                lower2 = &start2;
                upper2 = &end2;
            }
            else
            {
                lower2 = &end2;
                upper2 = &start2;
            }

            return m_pointCompare( *lower1, *lower2 ) || ( equals( *lower1, *lower2, m_compare ) && m_pointCompare( *upper1, *upper2 ) );
        }

        NumberComparisonPolicy                                  m_compare;
        lexicographical_point_compare< NumberComparisonPolicy > m_pointCompare;

    };

    //! Functor to compare segments lexicographically as intervals A-C !< B-C if [A-B-C].
    template <typename NumberComparisonPolicy>
    struct segment_interval_compare
    {    
        typedef lexicographical_point_compare< NumberComparisonPolicy > lex_point_compare;

        segment_interval_compare(){}
        segment_interval_compare( const NumberComparisonPolicy& compare )
            : m_lexCompare( compare ){}

        template <typename Segment>
        inline bool operator()( const Segment& lhs, const Segment& rhs ) const
        {
            typedef segment_access_traits< Segment > segment_access;
            typedef segment_access::point_type       point_type;

            const point_type* lhs_start;
            const point_type* lhs_end;
            const point_type* rhs_start;
            const point_type* rhs_end;
           
            if( m_lexCompare( segment_access::get_start( lhs ), segment_access::get_end( lhs ) ) )
            {
                lhs_start = &segment_access::get_start( lhs );
                lhs_end   = &segment_access::get_end( lhs );
            }
            else
            {
                lhs_start = &segment_access::get_end( lhs );
                lhs_end   = &segment_access::get_start( lhs );
            }

            if( m_lexCompare( segment_access::get_start( rhs ), segment_access::get_end( rhs ) ) )
            {
                rhs_start = &segment_access::get_start( rhs );
                rhs_end   = &segment_access::get_end( rhs );
            }
            else
            {
                rhs_start = &segment_access::get_end( rhs );
                rhs_end   = &segment_access::get_start( rhs );
            }

            //Now we have the segments in lexi order... we can compare the intervals.
            return m_lexCompare( *lhs_end, *rhs_start );           
        }

        lex_point_compare m_lexCompare;

    };

    //! Given a set of segments take the geometric difference of the set and the specified segments.
    //! precondition segments must all be collinear.
    template <typename Segment, typename SegmentIntervalSet, typename NumberComparisonPolicy>
    inline void collinear_segment_difference( SegmentIntervalSet& segments, const Segment& segment, const NumberComparisonPolicy& compare )
    {
        typedef lexicographical_point_compare< NumberComparisonPolicy > lex_point_compare;
        typedef segment_access_traits< Segment > segment_access;
        typedef segment_access::point_type       point_type;
        lex_point_compare lexCompare( compare );

        const point_type& C = segment_access::get_start( segment );
        const point_type& D = segment_access::get_end( segment );

        SegmentIntervalSet::iterator lb,ub;
        boost::tie( lb, ub ) = segments.equal_range( segment );

        std::vector< Segment > toInsert;
        while( lb != ub )
        {        
            const Segment& overlappedSegment = *lb;

            const point_type& A = segment_access::get_start( overlappedSegment );
            const point_type& B = segment_access::get_end( overlappedSegment );

            //! must be collinear
            assert( is_collinear( C, D, A, compare ) && is_collinear( C, D, B, compare ) );
            
            bool CAD = is_between( C, D, A, true, compare );
            bool CBD = is_between( C, D, B, true, compare );
            
            if( CAD && CBD )
            {
                //remove this one.
                segments.erase( lb++ );
                continue;
            }

            bool AEqualC = equals( A, C, compare );
            bool AEqualD = equals( A, D, compare );
            bool BEqualC = equals( B, C, compare );
            bool BEqualD = equals( B, D, compare );

            bool ACB = is_between( A, B, C, true, compare );
            bool ADB = is_between( A, B, D, true, compare );
            if( ACB && ADB )
            {
                bool CDB = is_between( C, B, D, true, compare );
                bool ADC = is_between( A, C, D, true, compare );
                if( CDB && !(BEqualC || BEqualD) )
                {
                    toInsert.push_back( construction_traits< Segment >::construct( A, C ) );
                    toInsert.push_back( construction_traits< Segment >::construct( D, B ) );
                    segments.erase( lb++ );
                    continue;
                }
                else if( ADC && !(AEqualC || AEqualD) )
                {
                    toInsert.push_back( construction_traits< Segment >::construct( A, D ) );
                    toInsert.push_back( construction_traits< Segment >::construct( C, B ) );
                    segments.erase( lb++ );
                    continue;
                }

                assert( false );
                
            }   

            if( CAD && !(AEqualC || AEqualD) )
            {         
                if( ADB && !(BEqualD) )
                {
                    toInsert.push_back( construction_traits< Segment >::construct( D, B ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
                else if( ACB && !BEqualC )
                {
                    toInsert.push_back( construction_traits< Segment >::construct( C, B ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
            }
            else if( CBD && !(BEqualC || BEqualD) )
            {
                if( ADB && !AEqualD)
                {
                    toInsert.push_back( construction_traits< Segment >::construct( A, D ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
                else if( ACB && !AEqualC )
                {
                    toInsert.push_back( construction_traits< Segment >::construct( A, C ) );
                    //remove this one.
                    segments.erase( lb++ );
                    continue;
                }
            }   

            ++lb;
        }

        segments.insert( toInsert.begin(), toInsert.end() );
    }

    //! Given a set of segments take the geometric union of the set and the specified segments.
    //! precondition segments must all be collinear.
    template <typename Segment, typename SegmentIntervalSet, typename NumberComparisonPolicy>
    inline void collinear_segment_union( SegmentIntervalSet& segments, const Segment& segment, const NumberComparisonPolicy& compare )
    {
        typedef lexicographical_point_compare< NumberComparisonPolicy > lex_point_compare;
        typedef segment_access_traits< Segment > segment_access;
        typedef segment_access::point_type       point_type;
        lex_point_compare lexCompare( compare );

        SegmentIntervalSet::iterator lb,ub;
        boost::tie( lb, ub ) = segments.equal_range( segment );

        Segment unionSegment = segment_access::construct( segment_access::get_start( segment ), segment_access::get_end( segment ) );

        while( lb != ub )
        {
            const Segment& overlappedSegment = *lb;

            const point_type& A = segment_access::get_start( overlappedSegment );
            const point_type& B = segment_access::get_end( overlappedSegment );
            const point_type& C = segment_access::get_start( unionSegment );
            const point_type& D = segment_access::get_end( unionSegment );

            //! must be collinear
            assert( is_collinear( C, D, A, compare ) && is_collinear( C, D, B, compare ) );

            bool CAD = is_between( C, D, A, true, compare );
            bool CBD = is_between( C, D, B, true, compare );

            //Already contained in the union.
            if( CAD && CBD )
            {
                //remove this one.
                segments.erase( lb++ );
                continue;
            }

            bool ACB = is_between( A, B, C, true, compare );
            bool ADB = is_between( A, B, D, true, compare );

            //New segment contains existing union.. replace.
            if( ACB && ADB )
            {
                unionSegment = segment_access::construct( A, B );
                segments.erase( lb++ );
                continue;                
            }
            
            if( CAD )
            {         
                if( ADB )
                {
                    unionSegment = segment_access::construct( C, B );
                    segments.erase( lb++ );
                    continue;
                }
                if( ACB )
                {
                    unionSegment = segment_access::construct( D, B );
                    segments.erase( lb++ );
                    continue;
                }         
            }
            else if( CBD )
            {
                if( ADB )
                {
                    unionSegment = segment_access::construct( A, C );
                    segments.erase( lb++ );
                    continue;
                }                
                if( ACB )
                {
                    unionSegment = segment_access::construct( A, D );
                    segments.erase( lb++ );
                    continue;
                } 
            }   

            ++lb;
        }

        segments.insert( unionSegment );
    }

    //! sorting compare functor to sort coordinates by less than order.
    template <typename CoordinateType, typename NumberComparisonPolicy>
    struct coordinate_less_compare
    {
        coordinate_less_compare( const NumberComparisonPolicy& compare )
            : m_compare( compare )
        {}

        inline bool operator() ( const CoordinateType& lhs, const CoordinateType& rhs ) const
        {
            return m_compare.less_than( rhs, lhs );            
        }

        NumberComparisonPolicy m_compare;

    };

    //! sorting compare functor to sort points clockwise or counter-clockwise around a point 
    template <typename Point, typename NumberComparisonPolicy>
    struct orientation_compare
    {
        typedef Point point_type;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;

        orientation_compare( const point_type& center, orientation_type winding, const NumberComparisonPolicy& compare )
            : m_winding( winding )
            , m_center( center )
            , m_compare( compare )
        {}

        inline bool operator() ( const point_type& lhs, const point_type& rhs ) const
        {            
            if( equals( lhs, rhs, m_compare ) )
                return false;

            orientation_type c_lhs = get_orientation( m_center, rhs, lhs, m_compare );

            if( c_lhs == oriented_collinear && m_winding != oriented_collinear )
            {
                //if the points are on opposite sides of the center 
                coordinate_type rhsDeltaY = cartesian_access_traits< point_type >::get<1>( rhs ) - cartesian_access_traits< point_type >::get<1>( m_center );
                coordinate_type lhsDeltaY = cartesian_access_traits< point_type >::get<1>( lhs ) - cartesian_access_traits< point_type >::get<1>( m_center );
                coordinate_type rhsDeltaX = cartesian_access_traits< point_type >::get<0>( rhs ) - cartesian_access_traits< point_type >::get<0>( m_center );
                coordinate_type lhsDeltaX = cartesian_access_traits< point_type >::get<0>( lhs ) - cartesian_access_traits< point_type >::get<0>( m_center );

                coordinate_type zero( 0 );

                //Check if opposite directions
                if( m_compare.less_than( lhsDeltaY * rhsDeltaY, zero ) || m_compare.less_than( lhsDeltaX * rhsDeltaX, zero ) )
                {
                    //opposite - compare quadrants?
                    bool rhsYNegative = m_compare.less_than( rhsDeltaY, zero );
                    bool lhsYNegative = m_compare.less_than( lhsDeltaY, zero );
                    bool rhsXNegative = m_compare.less_than( rhsDeltaX, zero );
                    bool lhsXNegative = m_compare.less_than( lhsDeltaX, zero );

                    int lhsQuadrant;
                    int rhsQuadrant;

                    if( !rhsYNegative && !rhsXNegative )
                    {
                        rhsQuadrant = 0;
                    }

                    if( !lhsYNegative && !lhsXNegative )
                    {
                        lhsQuadrant = 0;
                    }

                    if( !rhsYNegative && rhsXNegative )
                    {
                        rhsQuadrant = 1;
                    }

                    if( !lhsYNegative && lhsXNegative )
                    {
                        lhsQuadrant = 1;
                    }

                    if( rhsYNegative && rhsXNegative )
                    {
                        rhsQuadrant = 2;
                    }

                    if( lhsYNegative && lhsXNegative )
                    {
                        lhsQuadrant = 2;
                    }

                    if( rhsYNegative && !rhsXNegative )
                    {
                        rhsQuadrant = 3;
                    }

                    if( lhsYNegative && !lhsXNegative )
                    {
                        lhsQuadrant = 3;
                    }

                    return ( lhsQuadrant > rhsQuadrant && m_winding == oriented_left );
                }
                else
                {
                    return false;
                }
            }

            return ( c_lhs == m_winding );
        }

        point_type             m_center;
        orientation_type       m_winding;
        NumberComparisonPolicy m_compare;

    };
    
    template <typename Compare>
	struct pair_first_compare
	{
        pair_first_compare( const Compare& compare )
            : m_compare( compare )
        {}

        template <typename T1, typename T2>
		inline bool operator()(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2) const
		{
			return m_compare(p1.first, p2.first);
		}

        Compare m_compare;

	};

	template <typename Compare>
	struct pair_second_compare
	{
        pair_second_compare( const Compare& compare )
            : m_compare( compare )
        {}

        template <typename T1, typename T2>
		inline bool operator()(const std::pair<T1,T2>& p1, const std::pair<T1,T2>& p2) const
		{
			return m_compare(p1.second, p2.second);
		}

        Compare m_compare;

	};


}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_UTILITIES_HPP
