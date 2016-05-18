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
#include <boost/units/physical_dimensions.hpp>
#include <boost/units/dimensionless_quantity.hpp>
#include <boost/units/io.hpp>
#include <boost/units/pow.hpp>

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/segment.hpp>
#include <geometrix/primitive/polyline.hpp>
#include <geometrix/primitive/polygon.hpp>
#include <geometrix/primitive/axis_aligned_bounding_box.hpp>
#include <geometrix/primitive/oriented_bounding_box.hpp>
#include <geometrix/tensor/vector.hpp>
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

struct geometry_kernel_2d_units_fixture
{
	typedef boost::units::quantity<boost::units::si::dimensionless, double> dimensionless_t;
	typedef boost::units::quantity<boost::units::si::length, double> length_t;
	typedef decltype(length_t()*length_t()) area_t;
	typedef decltype(area_t()*length_t()) volume_t;
	typedef boost::units::quantity<boost::units::si::plane_angle, double> angle_t;

	typedef geometrix::point<length_t,2> point2;
	typedef geometrix::vector<length_t,2> vector2;
	using dimensionless_vector2 = geometrix::vector<dimensionless_t, 2>;
	typedef geometrix::segment<point2> segment2;
	typedef geometrix::polygon<point2> polygon2;
	typedef geometrix::polyline<point2> polyline2;
	typedef geometrix::matrix<double, 2, 2> matrix22;
	typedef geometrix::matrix<double, 3, 3> matrix33;
	typedef geometrix::matrix<double, 4, 4> matrix44;
	using aabb2 = geometrix::axis_aligned_bounding_box<point2>;
	using obb2 = geometrix::oriented_bounding_box<point2, dimensionless_vector2>;
	
	typedef geometrix::compound_comparison_policy
		<
		    geometrix::absolute_tolerance_comparison_policy<double>
		  , geometrix::absolute_tolerance_comparison_policy<dimensionless_t>
		  , geometrix::absolute_tolerance_comparison_policy<length_t>
		  , geometrix::absolute_tolerance_comparison_policy<area_t>
		  , geometrix::absolute_tolerance_comparison_policy<volume_t>
		  , geometrix::absolute_tolerance_comparison_policy<angle_t>
		> comparison_policy;

	geometry_kernel_2d_units_fixture()
		: cmp
		  (
			    geometrix::absolute_tolerance_comparison_policy<double>()
			  , geometrix::absolute_tolerance_comparison_policy<dimensionless_t>()
			  , geometrix::absolute_tolerance_comparison_policy<length_t>()
			  , geometrix::absolute_tolerance_comparison_policy<area_t>()
			  , geometrix::absolute_tolerance_comparison_policy<volume_t>()
			  , geometrix::absolute_tolerance_comparison_policy<angle_t>(angle_t(1e-6 * boost::units::si::radians))
		  )
	{}

	comparison_policy cmp;
};

GEOMETRIX_DEFINE_POINT_TRAITS(geometry_kernel_2d_units_fixture::point2, (geometry_kernel_2d_units_fixture::length_t), 2, geometry_kernel_2d_units_fixture::dimensionless_t, geometry_kernel_2d_units_fixture::length_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::point2>);
GEOMETRIX_DEFINE_VECTOR_TRAITS(geometry_kernel_2d_units_fixture::vector2, (geometry_kernel_2d_units_fixture::length_t), 2, geometry_kernel_2d_units_fixture::dimensionless_t, geometry_kernel_2d_units_fixture::length_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::vector2>);
GEOMETRIX_DEFINE_VECTOR_TRAITS(geometry_kernel_2d_units_fixture::dimensionless_vector2, (geometry_kernel_2d_units_fixture::dimensionless_t), 2, geometry_kernel_2d_units_fixture::dimensionless_t, geometry_kernel_2d_units_fixture::dimensionless_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::dimensionless_vector2>);

inline bool operator ==(const geometry_kernel_2d_units_fixture::point2& s1, const geometry_kernel_2d_units_fixture::point2& s2)
{
	using namespace geometrix;
	absolute_tolerance_comparison_policy<geometry_kernel_2d_units_fixture::length_t> cmp;
	return numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator !=(const geometry_kernel_2d_units_fixture::point2& s1, const geometry_kernel_2d_units_fixture::point2& s2)
{
	using namespace geometrix;
	absolute_tolerance_comparison_policy<geometry_kernel_2d_units_fixture::length_t> cmp;
	return !numeric_sequence_equals(s1, s2, cmp);
}

inline bool operator ==(const geometry_kernel_2d_units_fixture::segment2& s1, const geometry_kernel_2d_units_fixture::segment2& s2)
{
	return s1.get_start() == s2.get_start() && s1.get_end() == s2.get_end();
}

inline bool operator !=(const geometry_kernel_2d_units_fixture::segment2& s1, const geometry_kernel_2d_units_fixture::segment2& s2)
{
	return s1.get_start() != s2.get_start() || s1.get_end() != s2.get_end();
}

namespace geometrix
{
	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::point2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "point2{" << get<0>(p) << ", " << get<1>(p) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::vector2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "vector2{" << get<0>(p) << ", " << get<1>(p) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::segment2& s)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "segment2{" << get<0>(s.get_start()) << ", " << get<1>(s.get_start()) << ", " << get<0>(s.get_end()) << ", " << get<1>(s.get_end()) << "}";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::polygon2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "polygon2{ ";
		for (std::size_t i = 0; i < p.size(); ++i) {
			if (i)
				os << ", ";
			os << p[i];
		}
		os << " }";
		return os;
	}

	inline std::ostream& operator << (std::ostream& os, const geometry_kernel_2d_units_fixture::polyline2& p)
	{
		using namespace geometrix;
		os.precision(std::numeric_limits<double>::max_digits10);
		os << "polygon2{ ";
		for (std::size_t i = 0; i < p.size(); ++i) {
			if (i)
				os << ", ";
			os << p[i];
		}
		os << " }";
		return os;
	}
}//! namespace geometrix;

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

#endif //GEOMETRIX_UNITS_TESTS_HPP
