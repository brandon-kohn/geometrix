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
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>

struct geometry_kernel_2d_units_fixture
{
	typedef boost::units::quantity<boost::units::si::length, double> length_t;
	typedef decltype(length_t()*length_t()) area_t;
	typedef decltype(area_t()*length_t()) volume_t;
	typedef boost::units::quantity<boost::units::si::plane_angle, double> angle_t;

	typedef geometrix::point<length_t,2> point2;
	typedef geometrix::vector<length_t,2> vector2;
	typedef geometrix::segment<point2> segment2;
	typedef geometrix::polygon<point2> polygon2;
	typedef geometrix::polyline<point2> polyline2;
	typedef geometrix::matrix<double, 2, 2> matrix22;
	typedef geometrix::matrix<double, 3, 3> matrix33;
	typedef geometrix::matrix<double, 4, 4> matrix44;
	
	typedef compound_comparison_policy
		<
		    geometrix::absolute_tolerance_comparison_policy<double>
		  , geometrix::absolute_tolerance_comparison_policy<length_t>
		  , geometrix::absolute_tolerance_comparison_policy<area_t>
		  , geometrix::absolute_tolerance_comparison_policy<volume_t>
		  , geometrix::absolute_tolerance_comparison_policy<angle_t>
		> comparison_policy;

	geometry_kernel_2d_units_fixture()
		: cmp
		  (
			    geometrix::absolute_tolerance_comparison_policy<double>()
			  , geometrix::absolute_tolerance_comparison_policy<length_t>()
			  , geometrix::absolute_tolerance_comparison_policy<area_t>()
			  , geometrix::absolute_tolerance_comparison_policy<volume_t>()
			  , geometrix::absolute_tolerance_comparison_policy<angle_t>(angle_t(1e-6 * boost::units::si::radians))
		  )
	{}

	comparison_policy cmp;
};

GEOMETRIX_DEFINE_POINT_TRAITS(geometry_kernel_2d_units_fixture::point2, (geometry_kernel_2d_units_fixture::length_t), 2, geometry_kernel_2d_units_fixture::length_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::point2>);
GEOMETRIX_DEFINE_VECTOR_TRAITS(geometry_kernel_2d_units_fixture::vector2, (geometry_kernel_2d_units_fixture::length_t), 2, geometry_kernel_2d_units_fixture::length_t, neutral_reference_frame_2d, index_operator_vector_access_policy<geometry_kernel_2d_units_fixture::vector2>);

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
	using namespace boost::units::si;
	area_t a(10.0 * pow<2>(meters));
	area_t b(10.0 * meters * meters);
	auto result = cmp.greater_than(a, b);
	BOOST_CHECK(result == false);
}

BOOST_FIXTURE_TEST_CASE(ComparisonPolicyCompoundTypeConstruction, geometry_kernel_2d_units_fixture)
{
	using namespace geometrix;
}

#endif //GEOMETRIX_UNITS_TESTS_HPP
