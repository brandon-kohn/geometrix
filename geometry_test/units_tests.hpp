//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UNITS_TESTS_HPP
#define GEOMETRIX_UNITS_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "2d_kernel_units_fixture.hpp"

#include <geometrix/utility/utilities.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/algorithm/euclidean_distance.hpp>
#include <geometrix/algorithm//intersection/segment_segment_intersection.hpp>
#include <geometrix/algorithm/distance/point_aabb_distance.hpp>
#include <geometrix/algorithm/distance/point_obb_distance.hpp>
#include <geometrix/algorithm/distance/segment_obb_distance.hpp>
#include <geometrix/algorithm/grid_2d.hpp>

#include <set>

BOOST_FIXTURE_TEST_CASE(ComparisonPolicyEquals, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	length_t a(10.0*meters), b(20.0*meters);
	auto result = cmp.equals(a, b);
	BOOST_CHECK(result==false);
}

BOOST_FIXTURE_TEST_CASE(ComparisonPolicyLessThan, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	length_t a(10.0*meters), b(20.0*meters);
	auto result = cmp.less_than(a, b);
	BOOST_CHECK(result == true);
}

BOOST_FIXTURE_TEST_CASE(ComparisonPolicyGreaterThan, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	length_t a(10.0*meters), b(20.0*meters);
	auto result = cmp.greater_than(a, b);
	BOOST_CHECK(result == false);
}

BOOST_FIXTURE_TEST_CASE(ComparisonPolicyMismatchedTypes, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units;
	using namespace boost::units::si;
	area_t a(10.0 * pow<2>(meters));
	area_t b(10.0 * pow<2>(meters));
	auto result = cmp.greater_than(a, b);
	BOOST_CHECK(result == false);
}

BOOST_FIXTURE_TEST_CASE(DotProduct_TwoVectors_ReturnsArea, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto v1 = vector2{ 10.0 * meters, 5.0 * meters };
	auto v2 = vector2{ 5.0 * meters, 10.0 * meters };

	auto result = dot_product(v1, v2);
	static_assert(std::is_same<decltype(result), area_t>::value, "should both be areas");
	BOOST_CHECK(cmp.equals(result, 100.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(ExteriorProductArea_TwoVectors_ReturnsArea, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto v1 = vector2{ 10.0 * meters, 5.0 * meters };
	auto v2 = vector2{ 5.0 * meters, 10.0 * meters };

	auto result = exterior_product_area(v1, v2);
	static_assert(std::is_same<decltype(result), area_t>::value, "should both be areas");
	BOOST_CHECK(cmp.equals(result, 75.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(NormalizeVector_ReturnsUnitVector, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;

	auto v1 = vector2{ 10.0 * meters, 5.0 * meters };

	auto result = normalize(v1);
	static_assert(std::is_same<decltype(result), dimensionless_vector2>::value, "should both be vector<dimensionless_t,2>");
	BOOST_CHECK(cmp.equals(magnitude(result), 1.0 * dimensionless()));
}

BOOST_FIXTURE_TEST_CASE(angle_from_a_to_b_CalledWithUnits_ReturnsRadians, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;

	auto a = point2{ 10.0 * meters, 5.0 * meters };
	auto b = point2{ 10.0 * meters, 5.0 * meters };

	auto result = angle_from_a_to_b(a,b);
	static_assert(std::is_same<decltype(result), angle_t>::value, "should both be angle_t");
	BOOST_CHECK(cmp.equals(result, 0.0 * radians));
}

BOOST_FIXTURE_TEST_CASE(boostMathPi_CalledForUnitsType_ReturnsRadian, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;

	auto result = constants::pi<angle_t>();
	BOOST_CHECK(cmp.equals(result, boost::math::constants::pi<double>() * radians));
}

BOOST_FIXTURE_TEST_CASE(is_vector_between_TwoUnitedVectorsIsBetween_ReturnsTrue, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto v1 = vector2{ 1.0 * meters, 0.0 * meters };
	auto v2 = vector2{ 0.0 * meters, 1.0 * meters };
	auto v3 = vector2{ 1.0 * meters, 1.0 * meters };
	auto includeBounds = true;

	auto result = is_vector_between(v1, v2, v3, includeBounds, cmp);
	
	BOOST_CHECK(result);
}

BOOST_FIXTURE_TEST_CASE(point_point_distance_sqrd_CalledWithPointsWithUnitsOfLength_ReturnsArea, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 10.0 * meters, 5.0 * meters };
	auto b = point2{ 10.0 * meters, 5.0 * meters };
	
	auto d2 = point_point_distance_sqrd(a, b);
	
	static_assert(std::is_same<decltype(d2), area_t>::value, "should both be area_t");
	BOOST_CHECK(cmp.equals(d2, 0.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(point_point_distance_CalledWithPointsWithUnitsOfLength_ReturnsLength, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;

	auto a = point2{ 10.0 * meters, 5.0 * meters };
	auto b = point2{ 10.0 * meters, 5.0 * meters };

	auto d2 = point_point_distance(a, b);

	static_assert(std::is_same<decltype(d2), length_t>::value, "should both be length_t");
	BOOST_CHECK(cmp.equals(d2, 0.0 * meters));
}

BOOST_FIXTURE_TEST_CASE(point_segment_distance_sqrd_CalledWithPointsWithUnitsOfLength_ReturnsArea, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 10.0 * meters, 5.0 * meters };
	auto b = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };

	auto d2 = point_segment_distance_sqrd(a, b);

	static_assert(std::is_same<decltype(d2), area_t>::value, "should both be area_t");
	BOOST_CHECK(cmp.equals(d2, 0.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(point_segment_distance_CalledWithPointsWithUnitsOfLength_ReturnsLength, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units::si;

	auto a = point2{ 10.0 * meters, 5.0 * meters };
	auto b = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };

	auto d2 = point_segment_distance(a, b);

	static_assert(std::is_same<decltype(d2), length_t>::value, "should both be length_t");
	BOOST_CHECK(cmp.equals(d2, 0.0 * meters));
}

BOOST_FIXTURE_TEST_CASE(closest_point_segment_segment_CalledWithPointsWithUnitsOfLength_ReturnsSquaredLengthBetweenTwoSegments, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };
	auto b = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };

	dimensionless_t s, t;
	point2 c1, c2;
	auto d2 = closest_point_segment_segment(a.get_start(), a.get_end(), b.get_start(), b.get_end(), s, t, c1, c2, cmp);

	static_assert(std::is_same<decltype(d2), area_t>::value, "should both be area_t");
	BOOST_CHECK(cmp.equals(d2, 0.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(segment_segment_distance_sqrd_CalledWithPointsWithUnitsOfLength_ReturnsSquaredLengthBetweenTwoSegments, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };
	auto b = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };

	auto d2 = segment_segment_distance_sqrd(a, b, cmp);

	static_assert(std::is_same<decltype(d2), area_t>::value, "should both be area_t");
	BOOST_CHECK(cmp.equals(d2, 0.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(segment_segment_distance_CalledWithPointsWithUnitsOfLength_ReturnsLengthBetweenTwoSegments, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };
	auto b = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };

	auto d2 = segment_segment_distance(a, b, cmp);

	static_assert(std::is_same<decltype(d2), length_t>::value, "should both be length_t");
	BOOST_CHECK(cmp.equals(d2, 0.0 * meters));
}

BOOST_FIXTURE_TEST_CASE(closest_point_on_polygon_CalledWithPointsWithUnitsOfLength_ReturnsClosestPoint, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 0.0 * meters, 0.0 * meters };
	auto b = polygon2
	{
		{0.0 * meters, 0.0 * meters}
	  , {1.0 * meters, 0.0 * meters}
	  , {1.0 * meters, 1.0 * meters}
	  , {0.0 * meters, 1.0 * meters}
	};

	auto p = closest_point_on_polygon(a, b);

	static_assert(std::is_same<decltype(p), point2>::value, "should both be point2");
	BOOST_CHECK(numeric_sequence_equals(p, point2{ 0 * meters, 0 * meters }, cmp));
}

BOOST_FIXTURE_TEST_CASE(closest_point_on_polyline_CalledWithPointsWithUnitsOfLength_ReturnsClosestPoint, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 0.0 * meters, 0.0 * meters };
	auto b = polyline2
	{
		{ 0.0 * meters, 0.0 * meters }
	  , { 1.0 * meters, 0.0 * meters }
	  , { 1.0 * meters, 1.0 * meters }
	  , { 0.0 * meters, 1.0 * meters }
	};

	auto p = closest_point_on_polyline(a, b);

	static_assert(std::is_same<decltype(p), point2>::value, "should both be point2");
	BOOST_CHECK(numeric_sequence_equals(p, point2{ 0 * meters, 0 * meters }, cmp));
}

BOOST_FIXTURE_TEST_CASE(segment_segment_intersection_CalledWithPointsWithUnitsOfLengthNoIntersection_ReturnsFalse, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = segment2{ 10.0 * meters, 5.0 * meters, 20.0 * meters, 5.0 * meters };
	auto b = segment2{ 10.0 * meters, 15.0 * meters, 20.0 * meters, 15.0 * meters };

	point2 xPoints[2];
	auto d2 = segment_segment_intersection(a, b, xPoints, cmp);

	BOOST_CHECK(d2 == e_non_crossing);
}

BOOST_FIXTURE_TEST_CASE(point_aabb_distance_sqrd_CalledWithPointsWithUnitsOfLength_ReturnsArea, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 0.0 * meters, 0.0 * meters };
	auto b = aabb2
	{
		{ 0.0 * meters, 0.0 * meters }
	  , { 1.0 * meters, 1.0 * meters }
	};

	auto p = point_aabb_distance_sqrd(a, b);

	static_assert(std::is_same<decltype(p), area_t>::value, "should both be area_t");
	BOOST_CHECK(cmp.equals(p, 0.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(point_aabb_distance_CalledWithPointsWithUnitsOfLength_ReturnsLength, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 0.0 * meters, 0.0 * meters };
	auto b = aabb2
	{
		{ 0.0 * meters, 0.0 * meters }
	  , { 1.0 * meters, 1.0 * meters }
	};

	auto p = point_aabb_distance(a, b);

	static_assert(std::is_same<decltype(p), length_t>::value, "should both be length_t");
	BOOST_CHECK(cmp.equals(p, 0.0 * meters));
}

BOOST_FIXTURE_TEST_CASE(point_obb_distance_sqrd_CalledWithPointsWithUnitsOfLength_ReturnsArea, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 0.0 * meters, 0.0 * meters };
	auto b = obb2
	{
		{ 0.0 * meters, 0.0 * meters }
	  , { 1.0 * dimensionless(), 0.0 * dimensionless() }
	  , { 0.0 * dimensionless(), 1.0 * dimensionless() }
	  , 1.0 * meters
	  , 1.0 * meters
	};

	auto p = point_obb_distance_sqrd(a, b);

	static_assert(std::is_same<decltype(p), area_t>::value, "should both be area_t");
	BOOST_CHECK(cmp.equals(p, 0.0 * pow<2>(meters)));
}

BOOST_FIXTURE_TEST_CASE(point_obb_distance_CalledWithPointsWithUnitsOfLength_ReturnsLength, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = point2{ 0.0 * meters, 0.0 * meters };
	auto b = obb2
	{
		{ 0.0 * meters, 0.0 * meters }
	  , { 1.0 * dimensionless(), 0.0 * dimensionless() }
	  , { 0.0 * dimensionless(), 1.0 * dimensionless() }
	  , 1.0 * meters
	  , 1.0 * meters
	};

	auto p = point_obb_distance(a, b);

	static_assert(std::is_same<decltype(p), length_t>::value, "should both be length_t");
	BOOST_CHECK(cmp.equals(p, 0.0 * meters));
}

BOOST_FIXTURE_TEST_CASE(segment_obb_distance_CalledWithPointsWithUnitsOfLength_ReturnsLength, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = segment2{ 0.0 * meters, 0.0 * meters, 1.0 * meters, 0.0 * meters };
	auto b = obb2
	{
		{ 0.0 * meters, 0.0 * meters }
	  , { 1.0 * dimensionless(), 0.0 * dimensionless() }
	  , { 0.0 * dimensionless(), 1.0 * dimensionless() }
	  , 1.0 * meters
	  , 1.0 * meters
	};

	auto p = segment_obb_distance(a, b, cmp);

	static_assert(std::is_same<decltype(p), length_t>::value, "should both be length_t");
	BOOST_CHECK(cmp.equals(p, 0.0 * meters));
}

BOOST_FIXTURE_TEST_CASE(segment_obb_distance_sqrd_CalledWithPointsWithUnitsOfLength_ReturnsArea, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto a = segment2{ 0.0 * meters, 0.0 * meters, 1.0 * meters, 0.0 * meters };
	auto b = obb2
	{
		{ 0.0 * meters, 0.0 * meters }
	  , { 1.0 * dimensionless(), 0.0 * dimensionless() }
	  , { 0.0 * dimensionless(), 1.0 * dimensionless() }
	  , 1.0 * meters
	  , 1.0 * meters
	};

	auto p = segment_obb_distance_sqrd(a, b, cmp);

	static_assert(std::is_same<decltype(p), area_t>::value, "should both be area_t");
	BOOST_CHECK(cmp.equals(p, 0.0 * pow<2>(meters)));
}

#include <geometrix/algorithm/grid_traits.hpp>
BOOST_FIXTURE_TEST_CASE(convertCoordinateIntoIndex_LengthUnitsCoordinateTypeInZerothCell_CompilesAndReturns0thIndices, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto traits = grid_traits<length_t>(0.0 * meters, 1.0 * meters, 0.0 * meters, 1.0 * meters, 0.1 * meters);

	auto xindex = traits.get_x_index(0.05 * meters);
	auto yindex = traits.get_y_index(0.05 * meters);
	
	BOOST_CHECK_EQUAL(xindex, 0);
	BOOST_CHECK_EQUAL(yindex, 0);
}

BOOST_FIXTURE_TEST_CASE(get_cell_centroid_LengthUnitsCoordinateTypeInZerothCell_Returns0thCellCentroid, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto traits = grid_traits<length_t>(0.0 * meters, 1.0 * meters, 0.0 * meters, 1.0 * meters, 0.1 * meters);

	auto centroid = traits.get_cell_centroid(0, 0);

	static_assert(std::is_same<decltype(centroid), point2>::value, "type of centroid should be point2");
	BOOST_CHECK(numeric_sequence_equals(centroid, point2{ 0.05 * meters, 0.05 * meters }, cmp));
}

#include <geometrix/algorithm/fast_voxel_grid_traversal.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/tuple/tuple_comparison.hpp>
BOOST_FIXTURE_TEST_CASE(grid_2d_FastVoxelGridTraversalOfSegment_VisitsExpectedCellsAlongSegmentOnceEach, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	auto traits = grid_traits<length_t>(0.0 * meters, 1.0 * meters, 0.0 * meters, 1.0 * meters, 0.1 * meters);
	auto grid = grid_2d<int, grid_traits<length_t>>{ traits };
	segment2 seg{ 0.05 * meters, 0.05 * meters, 0.95 * meters, 0.95 * meters };

	std::set<boost::tuple<std::uint32_t, std::uint32_t>> vSet, expected = { {0,0}, {0,1}, {1,1}, {1,2}, {2,2}, {2,3}, {3,3}, {3,4}, {4,4}, {4,5}, {5,5}, {5,6}, {6,6}, {6,7}, {7,7}, {7,8}, {8,8}, {8,9}, {9,9} };
	fast_voxel_grid_traversal(traits, seg, [&grid, &vSet](std::uint32_t i, std::uint32_t j) { vSet.insert(boost::make_tuple(i, j)); ++grid.get_cell(i, j); }, cmp);

	BOOST_CHECK_EQUAL_COLLECTIONS(expected.begin(), expected.end(), vSet.begin(), vSet.end());
	for (auto i : vSet)
	{
		BOOST_CHECK(grid.get_cell(boost::get<0>(i), boost::get<1>(i)) == 1);
	}
}

#include <geometrix/algorithm/segment_mid_point.hpp>
BOOST_FIXTURE_TEST_CASE(segment_mid_point_SegmentWithUnits_ReturnsMidPoint, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	segment2 seg{ 0.05 * meters, 0.05 * meters, 0.95 * meters, 0.95 * meters };

	auto result = segment_mid_point(seg);

	static_assert(std::is_same < point2, decltype(result)>::value, "result should have type point2");
	BOOST_CHECK(numeric_sequence_equals(result, point2{ 0.5 * meters, 0.5 * meters }, cmp));
}

#include <geometrix/algorithm/intersection/moving_sphere_aabb_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(ray_aabb_intersection_WithUnits_Compiles, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;

	point2 xPoint;
	length_t t;

	aabb2 bb(point2{ 1 * meters,1 * meters }, point2{ 2 * meters, 2 * meters });
	dimensionless_vector2 d = normalize(dimensionless_vector2{ 1, 1 });
	point2 a{ 0 * meters, 0 * meters };
	
	bool result = ray_aabb_intersection(a, d, bb, t, xPoint, cmp);
	
	BOOST_CHECK(result);
	BOOST_CHECK(numeric_sequence_equals(xPoint, point2{ 1 * meters, 1 * meters }, cmp));
	BOOST_CHECK(cmp.equals(t, sqrt(2.0) * meters));
}

BOOST_FIXTURE_TEST_CASE(moving_sphere_aabb_intersection_WithUnits_Compiles, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	using namespace boost::units;
	using namespace boost::units::si;
	
	time_t t;
	point2 q;

	//! General case intersecting.
	{
		circle2 circle{ point2{ 1.0 * meters, 1.0 * meters}, 0.25 * meters};
		aabb2 bb{ point2{ -1 * meters, -1 * meters }, point2{ 0 * meters, 0 * meters} };
		polygon2 box{ point2{ -1 * meters, -1 * meters }, point2{ 0 * meters, -1 * meters }, point2{ 0 * meters, 0 * meters}, point2{ -1 * meters, 0 * meters} };
		velocity2 v{ -1 * meters_per_second, -1 * meters_per_second };
		static_assert(is_scalar<time_t>::value, "time_t should be scalar.");
		static_assert(is_tensor<velocity2>::value, "velocity2 should be tensor.");

 		BOOST_CHECK(moving_sphere_aabb_intersection(circle, v, bb, t, q, cmp));
 		circle2 qr(circle.get_center() + t * v, circle.get_radius());
 		BOOST_CHECK(numeric_sequence_equals(q, point2{ 0 * meters, 0 * meters}, cmp));
	}
}

BOOST_FIXTURE_TEST_CASE(dot_product_units_not_compiling_issue, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;

	dimensionless2 v_norm{ 2.0, 2.0 };
	length_t one = 1.0 * boost::units::si::meters;
	area_t projection = dot_product(v_norm * one, v_norm * one);
}

BOOST_FIXTURE_TEST_CASE(negation_compile_issues_units, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;

	length_t one = 1.0 * boost::units::si::meters;

	auto one_expr = -one;

	static_assert(std::is_same<decltype(one_expr), length_t>::value, "Negation of boost unit should not result in a geometrix expression");

	BOOST_CHECK(cmp.equals(-1.0, one_expr.value()));
}


#include <geometrix/algorithm/intersection/ray_segment_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(ray_segment_intersection_test_with_units, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
	length_t t = 0.0 * boost::units::si::meters;
	point2 q[2];

	{
		point2 center{ 1.0 * boost::units::si::meters, 1.0 * boost::units::si::meters };
		dimensionless2 dir = normalize(vector2{ 1.0 * boost::units::si::meters, 1.0 * boost::units::si::meters });
		segment2 segment{ 2.0 * boost::units::si::meters, 0. * boost::units::si::meters, 0. * boost::units::si::meters, 2.0 * boost::units::si::meters };
		intersection_type result = ray_segment_intersection(center, dir, segment, t, q, cmp);
		BOOST_CHECK(result == e_crossing);
		BOOST_CHECK(cmp.equals(t, 0.0 * boost::units::si::meters));
		BOOST_CHECK(numeric_sequence_equals(q[0], point2{ 1.0 * boost::units::si::meters, 1.0 * boost::units::si::meters}, cmp));
	}
}

#include <geometrix/algorithm/line_intersection.hpp>
BOOST_FIXTURE_TEST_CASE(line_line_intersection_test_with_units, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;

	point2 A = { 413942.3119* boost::units::si::meters, 3698734.116* boost::units::si::meters };// , 324.0720825
	point2 B = { 413902.5075* boost::units::si::meters, 3698734.127 * boost::units::si::meters };// , 324.236908
	point2 C = { 413879.7293* boost::units::si::meters, 3698718.683 * boost::units::si::meters };// , 324.346329
	point2 D = { 413879.7235* boost::units::si::meters, 3698715.738 * boost::units::si::meters };// , 324.3687052

	auto num = -(get<0>(A) * (get<1>(C) - get<1>(B)) + get<0>(B) * (get<1>(A) - get<1>(C)) + get<0>(C) * (get<1>(B) - get<1>(A)));	
	
	BOOST_CHECK(cmp.equals(num, 614.98971378431816 * boost::units::si::square_meters));
}

#include <geometrix/arithmetic/vector/lerp.hpp>
BOOST_FIXTURE_TEST_CASE(vector_lerp_test, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;

	vector2 A = { 1.0 * boost::units::si::meters, 0.0 * boost::units::si::meters };
	vector2 B = { 0.0 * boost::units::si::meters, 1.0 * boost::units::si::meters };
	
	auto v = lerp(A, B, 0.5);

	BOOST_CHECK(numeric_sequence_equals(v, dimensionless2{ 0.70710678118654746, 0.70710678118654746 }, cmp));	
}

#endif //GEOMETRIX_UNITS_TESTS_HPP
