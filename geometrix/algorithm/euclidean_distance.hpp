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
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/numeric/constants.hpp>

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
                  typename geometric_traits<typename geometrix::remove_const_ref<T1>::type>::point_type
                , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
              >
        {};
                
        template <typename T1, typename T2>
        struct segment_segment_distance_sqrd
            : point_point_distance_sqrd
              <
                  typename geometric_traits<typename geometrix::remove_const_ref<T1>::type>::point_type
                , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
              >
        {};

    }//namespace result_of;

    namespace detail
    {
        template <typename Point1, typename Point2, std::size_t Index>
        struct distance_sqrd
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;

            static result_type eval( const Point1& v1, const Point2& v2 )
            {
                BOOST_AUTO( a, get<Index-1>(v2) - get<Index-1>(v1) );
                return a * a + distance_sqrd<Point1, Point2, Index-1>::eval(v1,v2);
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 0>
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
            static result_type eval( const Point1& v1, const Point2& v2 )
            {
                BOOST_AUTO( a, get<0>(v2) - get<0>(v1) );
                return a * a;
            }
        };

        template <typename Point1, typename Point2>
        struct distance_sqrd<Point1, Point2, 1>
        {
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
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
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
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
            typedef typename result_of::point_point_distance_sqrd<Point1, Point2>::type result_type;
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
	using std::sqrt;
    return sqrt( detail::distance_sqrd<CoordinateSequence1, CoordinateSequence2, dimension_of<CoordinateSequence1>::value-1>::eval(a, b) );
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
inline typename result_of::segment_segment_distance_sqrd<Segment1, Segment2>::type eberly_segment_segment_distance_sqrd( const Segment1& s1, const Segment2& s2 )
{        
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

namespace closest_point_segment_segment_detail
{
	// clamp n to lie within the range [min, max] 
	template <typename T>
	inline T clamp(T n, T min, T max )
	{
		if( n < min ) return min;
		if( n > max ) return max;
		return n;
	}
}

//! From Real-Time Collision Detection.
// Computes closest points C1 and C2 of S1(s)=P1+s*(Q1-P1) and
// S2(t)=P2+t*(Q2-P2), returning s and t. Function result is squared 
// distance between between S1(s) and S2(t)
namespace result_of
{
	template <typename Point>
	struct closest_point_segment_segment
		: point_point_distance_sqrd<Point, Point>
	{};
}//! namespace result_of
template <typename Point, typename NumberComparisonPolicy>
inline typename result_of::closest_point_segment_segment<Point>::type closest_point_segment_segment( const Point& p1, const Point& q1, const Point& p2, const Point& q2, typename geometric_traits<Point>::dimensionless_type& s, typename geometric_traits<Point>::dimensionless_type& t, Point &c1, Point &c2, const NumberComparisonPolicy& cmp )
{
	using namespace closest_point_segment_segment_detail;
	using dimensionless_t = typename geometric_traits<Point>::dimensionless_type;
	using length_t = typename geometric_traits<Point>::arithmetic_type;	
	using area_t = decltype(length_t() * length_t());
	using vector_t = vector<length_t, 2>;

	const auto zeroArea = constants::zero<area_t>();
	const auto zero = constants::zero<dimensionless_t>();
	const auto one = constants::one<dimensionless_t>();

	vector_t d1 = q1 - p1; // Direction vector of segment S1 
	vector_t d2 = q2 - p2; // Direction vector of segment S2 
	vector_t r = p1 - p2;
	area_t a = dot_product(d1, d1); 
	// Squared length of segment S1, always nonnegative 
	area_t e = dot_product(d2, d2); // Squared length of segment S2, always nonnegative 
	area_t f = dot_product(d2, r);
	// Check if either or both segments degenerate into points 
	if (cmp.less_than_or_equal(a, zeroArea) && cmp.less_than_or_equal(e, zeroArea) )
	{
		// Both segments degenerate into points 
		s = t = zero;
		c1 = p1; 
		c2 = p2;
		return dot_product( c1 - c2, c1 - c2 );
	} 

	if( cmp.less_than_or_equal(a, zeroArea) )
	{
		// First segment degenerates into a point 
		s = zero;
		t = f / e; // s = 0 => t =(b*s + f) / e = f / e 
		t = clamp<dimensionless_t>(t, zero, one);
	} 
	else 
	{ 
		area_t c = dot_product(d1, r); 
		if (cmp.less_than_or_equal(e, zeroArea))
		{
			// Second segment degenerates into a point 
			t = zero;
			s = clamp<dimensionless_t>(construct<dimensionless_t>(-c / a), zero, one); // t = 0 => s =(b*t - c) / a = -c / a 
		} 
		else 
		{ 
			// The general nondegenerate case starts here 
			area_t b = dot_product(d1, d2); 
			auto denom = get(a*e-b*b); // Always nonnegative
			// If segments not parallel, compute closest point on L1 to L2 and 
			// clamp to segment S1. Else pick arbitrary s (here 0) 
			if (denom != construct<decltype(denom)>(0))
				s = clamp<dimensionless_t>(construct<dimensionless_t>((b*f - c*e) / denom), zero, one);
			else 
				s = zero;
			// Compute point on L2 closest to S1(s) using 
			// t = dot_product((P1 + D1*s) - P2,D2) / dot_product(D2,D2) = (b*s + f) / e 
			//t = (b*s + f) / e;
			// If t in [0,1] done. Else clamp t, recompute s for the new value 
			// of t using s = dot_product((P2 + D2*t) - P1,D1) / dot_product(D1,D1)= (t*b - c) / a 
			// and clamp s to [0, 1] 
			area_t tnom = b*s + f;
			if( tnom < construct<area_t>(0))
			{ 
				t = zero;
				s = clamp<dimensionless_t>(construct<dimensionless_t>(-c / a), zero, one);
			}
			else if( tnom > e ) 
			{
				t = one;
				s = clamp<dimensionless_t>(construct<dimensionless_t>((b - c) / a), zero, one);
			}
			else 
			{
				t = tnom / e; 
			}
		}
	}

	assign(c1, p1 + d1 * s); 
	assign(c2, p2 + d2 * t); 
	return dot_product( c1 - c2, c1 - c2 );
}

template <typename Segment1, typename Segment2, typename NumberComparisonPolicy>
inline typename result_of::segment_segment_distance_sqrd<Segment1, Segment2>::type segment_segment_distance_sqrd(const Segment1& s1, const Segment2& s2, const NumberComparisonPolicy& cmp)
{
	BOOST_AUTO(p1, get_start(s1));
	BOOST_AUTO(p2, get_end(s1));
	BOOST_AUTO(p3, get_start(s2));
	BOOST_AUTO(p4, get_end(s2));

	typedef typename geometric_traits<Segment1>::point_type point_type;
	typedef typename geometric_traits<point_type>::dimensionless_type dimensionless_t;
	dimensionless_t s, t;
	point_type c1, c2;
	return closest_point_segment_segment(p1, p2, p3, p4, s, t, c1, c2, cmp);
}

template <typename Segment1, typename Segment2, typename NumberComparisonPolicy>
inline typename result_of::segment_segment_distance<Segment1, Segment2>::type segment_segment_distance( const Segment1& s1, const Segment2& s2, const NumberComparisonPolicy& cmp)
{
	BOOST_AUTO(p1, get_start(s1));
	BOOST_AUTO(p2, get_end(s1));
	BOOST_AUTO(p3, get_start(s2));
	BOOST_AUTO(p4, get_end(s2));

	typedef typename geometric_traits<Segment1>::point_type point_type;
	typedef typename geometric_traits<point_type>::dimensionless_type dimensionless_t;
	dimensionless_t s, t;
	point_type c1, c2;
	using std::sqrt;
	return sqrt(closest_point_segment_segment(p1, p2, p3, p4, s, t, c1, c2, cmp));
}

template <typename Segment, typename Point>
inline Point closest_point_on_segment(const Segment& seg, const Point& p)
{
    typedef typename select_arithmetic_type_from_sequences<typename geometric_traits<Segment>::point_type, Point>::type arithmetic_type;
    typedef vector<arithmetic_type, dimension_of<Point>::value> vector_t;
    vector_t AP = p - get_start(seg);
    vector_t AB = get_end(seg) - get_start(seg);
    auto t = dot_product(AP, AB);
	if( t <= construct<decltype(t)>(0) )
		return construct<Point>(get_start(seg));
	else
	{
		auto denom = dot_product( AB, AB );
		if( t >= denom )
			return construct<Point>( get_end( seg ) );
		else
			return construct<Point>( get_start( seg ) + (t / denom) * AB );
	}
}

namespace result_of
{
	template <typename T1, typename T2>
	struct point_segment_distance
		: point_point_distance
		<
		    T1
		  , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
		>
	{};

	template <typename T1, typename T2>
	struct point_segment_distance_sqrd
		: point_point_distance_sqrd
		<
		    T1
		  , typename geometric_traits<typename geometrix::remove_const_ref<T2>::type>::point_type
		>
	{};

}//namespace result_of;

template <typename Point, typename Segment>
inline typename result_of::point_segment_distance_sqrd<Point, Segment>::type point_segment_distance_sqrd( const Point& p, const Segment& seg )
{
	typedef typename select_arithmetic_type_from_sequences<typename geometric_traits<Segment>::point_type, Point>::type arithmetic_type;
	typedef vector<arithmetic_type, dimension_of<Point>::value> vector_t;
	vector_t ab = get_end( seg ) - get_start( seg );
	vector_t ac = p - get_start( seg );
	vector_t bc = p - get_end( seg );
	auto e = dot_product( ac, ab );
	if( e <= construct<decltype(e)>(0) )
		return dot_product( ac, ac );
	auto f = dot_product( ab, ab );
	if( e >= f )
		return dot_product( bc, bc );
	return dot_product( ac, ac ) - e * e / f;
}

template <typename Point, typename Segment>
inline typename result_of::point_segment_distance<Point, Segment>::type point_segment_distance( const Point& p, const Segment& s )
{
	using std::sqrt;
	return sqrt( point_segment_distance_sqrd( p, s ) );
}

template <typename Point, typename Polygon>
inline Point closest_point_on_polygon(const Point& p, const Polygon& poly)
{
	using access = point_sequence_traits<Polygon>;
	using segment_type = segment<typename geometric_traits<Polygon>::point_type>;
	using distance_sqrd_type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
	auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
	auto minSegment = segment_type{};
	auto size = access::size(poly);
	for (std::size_t i = 0, j = 1; i < size; ++i, j = (j + 1) % size)
	{
		auto seg = make_segment(access::get_point(poly,i), access::get_point(poly,j));
		auto ldistance = point_segment_distance_sqrd(p, seg);
		if (ldistance < distance)
		{
			distance = ldistance;
			minSegment = seg;
		}
	}

	return closest_point_on_segment(minSegment, p);
}

template <typename Point, typename Polyline>
inline Point closest_point_on_polyline(const Point& p, const Polyline& poly)
{
	using access = point_sequence_traits<Polyline>;
	using segment_type = segment<typename geometric_traits<Polyline>::point_type>;
	using distance_sqrd_type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
	auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
	auto minSegment = segment_type{};
	auto size = access::size(poly);
	for (std::size_t i = 0, j = 1; j < size; i = j++)
	{
		auto seg = make_segment(access::get_point(poly, i), access::get_point(poly, j));
		auto ldistance = point_segment_distance_sqrd(p, seg);
		if (ldistance < distance)
		{
			distance = ldistance;
			minSegment = seg;
		}
	}

	return closest_point_on_segment(minSegment, p);
}

namespace result_of{
	template <typename Point, typename Polyline>
	struct point_polyline_distance_sqrd
	{
		using segment_type = segment<typename geometric_traits<Polyline>::point_type>;
		using type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
	};
}

template <typename Point, typename Polyline>
inline typename result_of::point_polyline_distance_sqrd<Point, Polyline>::type point_polyline_distance_sqrd(const Point& p, const Polyline& poly)
{
	using access = point_sequence_traits<Polyline>;
	using segment_type = segment<typename geometric_traits<Polyline>::point_type>;
	using distance_sqrd_type = typename result_of::point_segment_distance_sqrd<Point, segment_type>::type;
	auto distance = std::numeric_limits<distance_sqrd_type>::infinity();
	auto size = access::size(poly);
	for (std::size_t i = 0, j = 1; j < size; i = j++)
		distance = (std::min)(distance, point_segment_distance_sqrd(p, make_segment(access::get_point(poly, i), access::get_point(poly, j))));
	
	return distance;
}

namespace result_of {
	template <typename Point, typename Polyline>
	struct point_polyline_distance
	{
		using segment_type = segment<typename geometric_traits<Polyline>::point_type>;
		using type = typename result_of::point_segment_distance<Point, segment_type>::type;
	};
}

template <typename Point, typename Polyline>
inline typename result_of::point_polyline_distance<Point, Polyline>::type point_polyline_distance(const Point& p, const Polyline& poly)
{
	using access = point_sequence_traits<Polyline>;
	using segment_type = segment<typename geometric_traits<Polyline>::point_type>;
	using distance_type = typename result_of::point_segment_distance<Point, segment_type>::type;
	auto distance = std::numeric_limits<distance_type>::infinity();
	auto size = access::size(poly);
	for (std::size_t i = 0, j = 1; j < size; i = j++)
		distance = (std::min)(distance, point_segment_distance(p, make_segment(access::get_point(poly, i), access::get_point(poly, j))));

	return distance;
}

}//namespace geometrix;

#endif //GEOMETRIX_EUCLIDEAN_DISTANCE_HPP
