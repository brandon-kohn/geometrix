//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEGMENT_INTERVAL_TESTS_HPP
#define GEOMETRIX_SEGMENT_INTERVAL_TESTS_HPP


#include <boost/test/included/unit_test.hpp>

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algorithm/segment_intersection.hpp>
#include <geometrix/primitive/segment.hpp>
#include <iostream>

template <typename SegmentIntervalSet>
void print_segments( const SegmentIntervalSet& segments )
{
    using namespace geometrix;

    typedef segment_access_traits< SegmentIntervalSet::value_type > segment_access;
    typedef segment_access::point_type point_type;
    typedef cartesian_access_traits< point_type > point_access;
    std::cout << "-----------------------" << std::endl;
    BOOST_FOREACH( const segment_access::segment_type& seg, segments )
    {
        const point_type& C = segment_access::get_start( seg );
        const point_type& D = segment_access::get_end( seg );
        std::cout << "( " << point_access::get<0>( C ) << "," << point_access::get<1>( C ) << " )--";
        std::cout << "( " << point_access::get<0>( D ) << "," << point_access::get<1>( D ) << " )" << std::endl;
    }
}

BOOST_AUTO_TEST_CASE( TestSegmentInterval )
{
    using namespace geometrix;

    typedef point_double_2d point_2d;
    
    point_2d p1( 0., 0. );
    point_2d p2( 10., 10. );
    point_2d p3( 1., 1. );
    point_2d p4( 3., 3. );
    point_2d p5( 2., 2. );
    point_2d p6( 4., 4. ); 
    point_2d p7( 2., 2. );
    point_2d p8( 9., 9. ); 
    
    segment<point_2d> seg1( p1, p2 );
    segment<point_2d> seg2( p3, p4 );
    segment<point_2d> seg3( p5, p6 );
    segment<point_2d> seg4( p7, p8 );
    segment<point_2d> seg5( p6, p5 );

    std::set< segment<point_2d>, segment_interval_compare< absolute_tolerance_comparison_policy<double> > > segments;
    segments.insert( seg1 );
	absolute_tolerance_comparison_policy<double> cmp(1e-10);

    collinear_segment_difference( segments, seg2, cmp );
    print_segments( segments );   

    collinear_segment_difference( segments, seg2, cmp );
    print_segments( segments );   

    collinear_segment_difference( segments, seg5, cmp );
    print_segments( segments );   

    collinear_segment_difference( segments, seg3, cmp );
    print_segments( segments );   

    collinear_segment_difference( segments, seg4, cmp );
    print_segments( segments );   

}

namespace test_collinear_segment_difference_detail {
	template <typename Segment>
	inline bool operator ==(const Segment& s1, const Segment& s2)
	{
		using namespace geometrix;
		absolute_tolerance_comparison_policy<double> cmp(1e-10);
		return numeric_sequence_equals(get_start(s1), get_start(s2), cmp) && numeric_sequence_equals(get_end(s1), get_end(s2), cmp);
	}
}

BOOST_AUTO_TEST_CASE(test_collinear_segment_difference)
{
	using namespace geometrix;
	using namespace test_collinear_segment_difference_detail;
	typedef point_double_2d point2;
	typedef segment<point2> segment2;
	typedef std::set< segment2, segment_interval_compare< absolute_tolerance_comparison_policy<double> > > clip_set;
	absolute_tolerance_comparison_policy<double> cmp(1e-10);
	{
		//! A - B-C - D disjoint.
		point2 A{ -10.0, 0.0 };
		point2 B{ 10.0, 0.0 };
		point2 C{ 20.0, 0.0 };
		point2 D{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		BOOST_CHECK(segments.size() == 1 && *segments.begin() == AB);
	}

	{
		//! A - B/C - D B==C.
		point2 A{ -10.0, 0.0 };
		point2 B{ 10.0, 0.0 };
		point2 C = B;
		point2 D{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		BOOST_CHECK(segments.size() == 1 && *segments.begin() == AB);
	}

	{
		//! A - C-B - D overlapping.
		point2 A{ -10.0, 0.0 };
		point2 C{ 10.0, 0.0 };
		point2 B{ 20.0, 0.0 };
		point2 D{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		auto it = segments.begin();
		BOOST_CHECK((segments.size() == 1 && *it == segment2{ A,C }));
	}
	
	{
		//! C - A-B - D overlapping.
		point2 C{ -10.0, 0.0 };
		point2 A{ 10.0, 0.0 };
		point2 B{ 20.0, 0.0 };
		point2 D{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		BOOST_CHECK((segments.empty()));
	}

	{
		//! A - C-D - B overlapping.
		point2 A{ -10.0, 0.0 };
		point2 C{ 10.0, 0.0 };
		point2 D{ 20.0, 0.0 };
		point2 B{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		auto it = segments.begin();
		BOOST_CHECK((segments.size() == 2 && *it == segment2{ A,C } && *++it == segment2{ D,B }));
	}

	{
		//! A - D-C - B overlapping.
		point2 A{ -10.0, 0.0 };
		point2 D{ 10.0, 0.0 };
		point2 C{ 20.0, 0.0 };
		point2 B{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		auto it = segments.begin();
		BOOST_CHECK((segments.size() == 2 && *it == segment2{ A, D } && *++it == segment2{ C, B }));
	}

	{
		//! B - C-D - A overlapping.
		point2 B{ -10.0, 0.0 };
		point2 C{ 10.0, 0.0 };
		point2 D{ 20.0, 0.0 };
		point2 A{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		auto it = segments.begin();
		BOOST_CHECK((segments.size() == 2 && *it == segment2{ C,B } && *++it == segment2{ A,D }));
	}

	{
		//! B - C/D - C==D and overlapping.
		point2 B{ -10.0, 0.0 };
		point2 C{ 10.0, 0.0 };
		point2 D = C;
		point2 A{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		//! TODO: I do nothing in this case now, but I think there's an argument for splitting at C.
		BOOST_CHECK((segments.size() == 1 && *segments.begin() == AB));
	}

	{
		//! A/B-C - D disjoint.
		point2 A{ 25.0, 0.0 };
		point2 B = A;
		point2 C{ 20.0, 0.0 };
		point2 D{ 30.0, 0.0 };
		segment2 AB{ A,B };
		segment2 CD{ C,D };
		clip_set segments{ AB };
		collinear_segment_difference(segments, CD, cmp);
		BOOST_CHECK(segments.empty());
	}
}

#endif //GEOMETRIX_SEGMENT_INTERVAL_TESTS_HPP


