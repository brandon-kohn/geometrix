//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_AS_TESTS_HPP
#define GEOMETRIX_AS_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

BOOST_AUTO_TEST_CASE( AsTests )
{
	using namespace geometrix;
	
	typedef point_double_2d point_2d;

	point_2d a( 1., 0. );
	point_2d b( 0., 1. );

	double dot = as<double>( as_vector( a ) * as_vector( b ) );
	BOOST_CHECK_CLOSE( dot, 0., 1e-10 );		
}

#endif//! GEOMETRIX_AS_TESTS_HPP
