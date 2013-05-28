//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_EUCLIDEAN_DISTANCE_HPP
#define GEOMETRIX_EUCLIDEAN_DISTANCE_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

#include <boost/fusion/algorithm/query/all.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename T1, typename T2>
        struct point_point_distance
            : magnitude
              <
                  GEOMETRIX_TYPEOF_BINARY_EXPR_TPL('-', T2, T1)
              >
        {};
                
        template <typename T1, typename T2>
        struct point_point_distance_sqrd
            : magnitude_sqrd
              <
                  GEOMETRIX_TYPEOF_BINARY_EXPR_TPL('-', T2, T1)
              >
        {};

        template <typename T1, typename T2>
        struct segment_segment_distance
            : point_point_distance
              <
                  typename geometric_traits<T1>::point_type
                , typename geometric_traits<T2>::point_type
              >
        {};
                
        template <typename T1, typename T2>
        struct segment_segment_distance_sqrd
            : point_point_distance_sqrd
              <
                  typename geometric_traits<T1>::point_type
                , typename geometric_traits<T2>::point_type
              >
        {};

    }//namespace result_of;

    namespace detail
    {
        template <typename Point1, typename Point2, unsigned int Index>
        struct distance_sqrd
        {
            typedef typename result_of::point_point_distance<Point1, Point2>::type result_type;

            static result_type eval( const Point1& v1, const Point2& v2 )
            {
                BOOST_AUTO( a, get<Index-1>(v2) - get<Index-1>(v1) );
                return a * a + distance_sqrd<Point1, Point2, Index-1>::eval(v1,v2);
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 0>
        {
            typedef typename result_of::point_point_distance<Point1, Point2>::type result_type;
            static result_type eval( const Point1& v1, const Point2& v2 )
            {
                BOOST_AUTO( a, get<0>(v2) - get<0>(v1) );
                return a * a;
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 1>
        {
            typedef typename result_of::point_point_distance<Point1, Point2>::type result_type;
            static result_type eval( const Point1& v1, const Point2& v2 )
            {
                BOOST_AUTO( a, get<0>(v2) - get<0>(v1) );
                BOOST_AUTO( b, get<1>(v2) - get<1>(v1) );
                return a * a + b * b;
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 2>
        {
            typedef typename result_of::point_point_distance<Point1, Point2>::type result_type;
            static result_type eval( const Point1& v1, const Point2& v2 )
            {
                BOOST_AUTO( a, get<0>(v2) - get<0>(v1) );
                BOOST_AUTO( b, get<1>(v2) - get<1>(v1) );
                BOOST_AUTO( c, get<2>(v2) - get<2>(v1) );
                return a * a + b * b + c * c;
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 3>
        {
            typedef typename result_of::point_point_distance<Point1, Point2>::type result_type;
            static result_type eval( const Point1& v1, const Point2& v2 )
            {
                BOOST_AUTO( a, get<0>(v2) - get<0>(v1) );
                BOOST_AUTO( b, get<1>(v2) - get<1>(v1) );
                BOOST_AUTO( c, get<2>(v2) - get<2>(v1) );
                BOOST_AUTO( d, get<3>(v2) - get<3>(v1) );
                return a * a + b * b + c * c + d * d;
            }
        };        

    }//namespace detail;

template <typename CoordinateSequence1, typename CoordinateSequence2>
inline typename result_of::point_point_distance
<
    CoordinateSequence1
  , CoordinateSequence2 
>::type
point_point_distance( const CoordinateSequence1& a, const CoordinateSequence2& b )
{
    return math::sqrt( detail::distance_sqrd<CoordinateSequence1, CoordinateSequence2, dimension_of<CoordinateSequence1>::value-1>::eval(a, b) );
}

template <typename CoordinateSequence1, typename CoordinateSequence2>
inline typename result_of::point_point_distance_sqrd
<
    CoordinateSequence1
  , CoordinateSequence2 
>::type
point_point_distance_sqrd( const CoordinateSequence1& a, const CoordinateSequence2& b )
{
    return detail::distance_sqrd<CoordinateSequence1, CoordinateSequence2, dimension_of<CoordinateSequence1>::value-1>::eval(a, b);
}

//! \struct distance_compare
//! \brief A predicate to compare two numeric sequences by their distance from a fixed origin.    
template <typename CoordinateSequence, typename NumberComparisonPolicy>
struct distance_compare
{
    distance_compare( const CoordinateSequence& p, const NumberComparisonPolicy& compare )
        : m_compare( compare )
        , m_p( p )
    {}
    
    template <typename CoordinateSequence1, typename CoordinateSequence2>
    bool operator()( const CoordinateSequence1& lhs, const CoordinateSequence2& rhs ) const
    {
        typename result_of::point_point_distance_sqrd
            <
                CoordinateSequence  
              , CoordinateSequence1
            >::type distanceLHS = point_point_distance_sqrd( m_p, lhs );
        typename result_of::point_point_distance_sqrd
            <
                CoordinateSequence  
              , CoordinateSequence2
            >::type distanceRHS = point_point_distance_sqrd( m_p, rhs );
        return m_compare.less_than( distanceLHS, distanceRHS );
    }

    NumberComparisonPolicy m_compare;
    CoordinateSequence     m_p;
}; 

template <typename Segment1, typename Segment2>
inline typename result_of::segment_segment_distance_sqrd<Segment1, Segment2>::type segment_segment_distance_sqrd( const Segment1& s1, const Segment2& s2 )
{
    using namespace algebra;
        
    BOOST_AUTO( p1, get_start(s1) );
    BOOST_AUTO( p2, get_end(s1) );

    BOOST_AUTO( p3, get_start(s2) );
    BOOST_AUTO( p4, get_end(s2) );

    BOOST_AUTO( v1, p2-p1 );
    BOOST_AUTO( v2, p4-p3 );
    
    BOOST_AUTO( a, point_point_distance_sqrd( p1, p2 ) );
    BOOST_AUTO( b, dot_product(-v1,v2) );
    BOOST_AUTO( c, point_point_distance_sqrd( p3, p4 ) );
    
    BOOST_AUTO( vd, p1-p3 );
    BOOST_AUTO( d, dot_product(v1,vd) );
    BOOST_AUTO( e, dot_product(-v2,vd) );
    BOOST_AUTO( f, point_point_distance_sqrd( p3, p1 ) );    
    BOOST_AUTO( det, exterior_product_area(v1,v2) );
    BOOST_AUTO( s, b*e-c*d );
    BOOST_AUTO( t, b*d-a*e );

    if( det == 0 )
    {
        if( b > 0 )
        {
            if( d >= 0 )
            {
                s = 0;
                t = 0;
            }
            else if( -d<=a )
            {
                s = -d / a;
                t = 0;
            }
            else
            {
                s = 1;
                BOOST_AUTO( tmp, a + d );
                if( -tmp >= b )
                    t = 1;
                else
                    t = -tmp / b;
            }
        }
        else
        {
            if( -d >= a )
            {
                s = 1;
                t = 0;
            }
            else if( d <= 0 )
            {
                s = -d / a;
                t = 0;
            }
            else            
            {
                s = 0;
                BOOST_AUTO( tmp, a + d );
                if( -tmp >= b )
                    t = 1;
                else
                    t = -tmp / b;
            }
        }
    }
    else
    {
        if( s>=0 )
        {
            if( s <= det )
            {
                if( t >= 0)
                {
                    if( t <= det )
                    {
                        //! Region 0
                        BOOST_AUTO( invDet, 1.0 / det );
                        s *= invDet;
                        t *= invDet;
                    }
                    else
                    {
                        //Region 3
                        s = 1.0;
                        BOOST_AUTO( tmp, b + e );
                        if( tmp > 0 )
                            t = 0;
                        else if( -tmp > c )
                            t = 1;
                        else 
                            t = -tmp / c;
                    }
                }
                else
                {
                    //Region 7
                    s = 1.0;
                    BOOST_AUTO( tmp, b + e );
                    if( tmp > 0 )
                        t = 0;
                    else if( -tmp > c )
                        t = 1;
                    else 
                        t = -tmp / c;
                }
            }
            else
            {
                if( t >= 0)
                {
                    if( t <= det )
                    {
                        //Region 1
                        s = 1.0;
                        BOOST_AUTO( tmp, b + e );
                        if( tmp > 0 )
                            t = 0;
                        else if( -tmp > c )
                            t = 1;
                        else 
                            t = -tmp / c;
                    }
                    else
                    {
                        //Region 2
                        BOOST_AUTO( tmp, b + d );
                        if( -tmp < a )
                        {
                            t = 1;
                            if( tmp > 0 )
                                s = 0;
                            else
                                s = -tmp / a;
                        }
                        else
                        {
                            s = 1;
                            BOOST_AUTO( tmp, b + e );
                            if( -tmp < c )
                            {
                                if( tmp > 0 )
                                    t = 0;
                                else
                                    t = -tmp / c;
                            }
                            else
                                t = 1;
                        }
                    }
                }
                else
                {
                    //Region 8
                    BOOST_AUTO( tmp, b + d );
                    if( -tmp < a )
                    {
                        t = 1;
                        if( tmp > 0 )
                            s = 0;
                        else
                            s = -tmp / a;
                    }
                    else
                    {
                        s = 1;
                        BOOST_AUTO( tmp, b + e );
                        if( -tmp < c )
                        {
                            if( tmp > 0 )
                                t = 0;
                            else
                                t = -tmp / c;
                        }
                        else
                            t = 1;
                    }
                }
            }
        }
        else
        {
            if( t >= 0 )
            {
                if( t <= det )
                {
                    //Region 5
                    s = 1.0;
                    BOOST_AUTO( tmp, b + e );
                    if( tmp > 0 )
                        t = 0;
                    else if( -tmp > c )
                        t = 1;
                    else 
                        t = -tmp / c;
                }
                else
                {
                    //Region 4
                    BOOST_AUTO( tmp, b + d );
                    if( -tmp < a )
                    {
                        t = 1;
                        if( tmp > 0 )
                            s = 0;
                        else
                            s = -tmp / a;
                    }
                    else
                    {
                        s = 1;
                        BOOST_AUTO( tmp, b + e );
                        if( -tmp < c )
                        {
                            if( tmp > 0 )
                                t = 0;
                            else
                                t = -tmp / c;
                        }
                        else
                            t = 1;
                    }
                }
            }
            else
            {
                //Region 6
                BOOST_AUTO( tmp, b + d );
                if( -tmp < a )
                {
                    t = 1;
                    if( tmp > 0 )
                        s = 0;
                    else
                        s = -tmp / a;
                }
                else
                {
                    s = 1;
                    BOOST_AUTO( tmp, b + e );
                    if( -tmp < c )
                    {
                        if( tmp > 0 )
                            t = 0;
                        else
                            t = -tmp / c;
                    }
                    else
                        t = 1;
                }
            }
        }
    }
        
    if( s == 0 )
    {
        if( t == 0 )
            return magnitude_sqrd(p1 - p3);
        else if( t == 1 )
            return magnitude_sqrd(p1 - p4);
        else
            return magnitude_sqrd(p1 - (p3 + t * v2));
    }
    else if( s == 1 )
    {
        if( t == 0 )
            return magnitude_sqrd(p2 - p3);
        else if( t == 1 )
            return magnitude_sqrd(p2 - p4);
        else
            return magnitude_sqrd(p2 - (p3 + t * v2));
    }
    else if( t == 0 )
        return magnitude_sqrd(p3 - (p1 + s * v1));
    else if( t == 1 )
        return magnitude_sqrd(p4 - (p1 + s * v1));
    else
        return magnitude_sqrd((p1 + s * v1) - (p3 + t * v2));
}

template <typename Segment1, typename Segment2>
inline typename result_of::segment_segment_distance<Segment1, Segment2>::type segment_segment_distance( const Segment1& s1, const Segment2& s2 )
{
    return math::sqrt( segment_segment_distance_sqrd( s1, s2 ) );
}

}//namespace geometrix;

#endif //GEOMETRIX_EUCLIDEAN_DISTANCE_HPP
