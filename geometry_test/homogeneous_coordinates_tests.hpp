//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_HOMOGENEOUS_COORDINATES_TESTS_HPP
#define GEOMETRIX_HOMOGENEOUS_COORDINATES_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/homogeneous_adaptor.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>

#include <geometrix/tensor/fusion_matrix.hpp>
#include <geometrix/tensor/fusion_vector.hpp>

#include "2d_kernel_units_fixture.hpp"

#include <iostream>

using namespace geometrix;

BOOST_FIXTURE_TEST_CASE(UnitedVectorHomogeneousCoordinateAdaptorTest, geometry_kernel_2d_units_fixture)
{
    using namespace boost::units::si;
    auto v = vector2{ 1.0 * meters, 2.0 * meters};
    
    auto hv = as_positional_homogeneous<length_t>(v);

	BOOST_CHECK(get<0>(hv).value() == 1.0);
	BOOST_CHECK(get<1>(hv).value() == 2.0);
	BOOST_CHECK(get<2>(hv).value() == 1.0);

	auto v2 = normalize(v);
	auto nv = as_normalized_homogeneous<dimensionless_t>(v2);

	BOOST_CHECK_CLOSE(get<0>(nv).value(), get<0>(v2).value(), 1e-10);
	BOOST_CHECK_CLOSE(get<1>(nv).value(), get<1>(v2).value(), 1e-10);
	BOOST_CHECK(get<2>(nv).value() == 0.0);
}


GEOMETRIX_FUSION_POD_MATRIX
(
	  xform_matrix
	, ((double, double, geometry_kernel_2d_units_fixture::length_t))
      ((double, double, geometry_kernel_2d_units_fixture::length_t))
	  ((double, double, double))
	, 3
	, 3
)

BOOST_FIXTURE_TEST_CASE(UnitedVectorHomogeneousCoordinateAdaptorMatrixMultTest, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	auto v = vector2{ 1.0 * meters, 2.0 * meters };

	auto hv = as_positional_homogeneous<length_t>(v);

	auto m = matrix3x3
	{
		1.0, 0.0, 1.0/* * meters*/
	,	0.0, 1.0, 1.0/* * meters*/
	,	0.0, 0.0, 1.0
	};

	auto result = m * hv;

	static_assert(is_vector<decltype(result)>::value, "should return a vector");
	BOOST_CHECK_CLOSE(get<0>(result).value(), 2.0, 1e-10);
	BOOST_CHECK_CLOSE(get<1>(result).value(), 3.0, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(PointHomogeneousCoordinateAdaptorMatrixMultTest, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	auto v = point2{ 1.0 * meters, 2.0 * meters };

	auto hv = as_positional_homogeneous<length_t>(v);

	auto m = matrix3x3
	{
		    1.0, 0.0, 1.0/* * meters*/
		,	0.0, 1.0, 1.0/* * meters*/
		,	0.0, 0.0, 1.0
	};

	auto result = m * hv;

	static_assert(is_point<decltype(result)>::value, "should return a point");
	BOOST_CHECK_CLOSE(get<0>(result).value(), 2.0, 1e-10);
	BOOST_CHECK_CLOSE(get<1>(result).value(), 3.0, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(PointHomogeneousCoordinateAdaptorRotationTranslationTest, geometry_kernel_2d_units_fixture)
{
	using namespace boost::units::si;
	using point3 = geometrix::point<length_t, 3>;
	auto v = point3{ 1.0 * meters, 0.0 * meters, 0.0 * meters };

	auto hv = as_positional_homogeneous<length_t>(v);

	auto t = matrix4x4
	{
		    1.0, 0.0, 0.0, 1.0/* * meters*/
		,	0.0, 1.0, 0.0, 1.0/* * meters*/
		,	0.0, 0.0, 1.0, 1.0
		,	0.0, 0.0, 0.0, 1.0
	};

	double theta = geometrix::constants::pi<double>() / 2.;
	double sint = std::sin(theta);
	double cost = std::cos(theta);
	auto rz = matrix4x4
	{
		    cost, -sint, 0.0, 0.0
		,   sint, cost, 0.0, 0.0
		,	0.0, 0.0, 1.0, 0.0
		,	0.0, 0.0, 0.0, 1.0
	};

	auto rx = matrix4x4
	{
		    1.0, 0.0, 0.0, 0.0
		,   0.0, cost, -sint, 0.0
		,	0.0, sint, cost, 0.0
		,	0.0, 0.0, 0.0, 1.0
	};

	auto ry = matrix4x4
	{
		    cost, 0.0, sint, 0.0
		,   0.0, 1.0, 0.0, 0.0
		,	-sint, 0.0, cost, 0.0
		,	0.0, 0.0, 0.0, 1.0
	};

	//! So this performs the following ops:
	//! 1) Translate by vector {1,1,1}.
	//! 2) Rotate around the x axis by theta radians.
	//! 2) Rotate around the y axis by theta radians.
	//! 2) Rotate around the z axis by theta radians.
	auto result = (t * rx * ry * rz) * hv;

	point2 p2 = result;

	static_assert(is_point<decltype(result)>::value, "should return a point");

	BOOST_CHECK_CLOSE(get<0>(result).value(), 1.0, 1e-10);
	BOOST_CHECK_CLOSE(get<1>(result).value(), -1.0, 1e-10);
}

#endif //GEOMETRIX_HOMOGENEOUS_COORDINATES_TESTS_HPP
