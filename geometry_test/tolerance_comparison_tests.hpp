//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TOLERANCE_COMPARISON_TESTS_HPP
#define GEOMETRIX_TOLERANCE_COMPARISON_TESTS_HPP


#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestFloatingPointComparison )
{   
    using namespace geometrix;

   //! Check the values within the specified tolerance
   {
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;

       BOOST_CHECK( geometrix::equals_within_absolute_tolerance( u, v, 1e-6 ) );
       BOOST_CHECK( geometrix::equals_within_absolute_tolerance( v, u, 1e-6 ) );
       BOOST_CHECK( (std::abs( u - v ) <= 1e-6 ) ); ///Sanity check against simple trival tolerance check type.    

       BOOST_CHECK( geometrix::equals_within_fraction_tolerance( u, v, 1e-6 ) );
       BOOST_CHECK( geometrix::equals_within_fraction_tolerance( v, u, 1e-6 ) );

       BOOST_CHECK( geometrix::equals_within_absolute_tolerance( 5., 4., 1. ) );       
       BOOST_CHECK( geometrix::equals_within_fraction_tolerance( 5., 4., 1. ) );

       BOOST_CHECK( !geometrix::equals_within_absolute_tolerance( 5., 4., 0.5 ) );       
       BOOST_CHECK( geometrix::equals_within_fraction_tolerance( 5., 4., 0.25 ) );

       //from: http://www.adtmag.com/joop/carticle.aspx?ID=396
       float feps = 0.00001f;

       //Thus, feps>(a1-b1) will return true, indicating that a1 and b1 are equal, as will feps>(b1-a1). The "close enough" equality test has become an inequality test against a difference. However, if you consider:
       float a2 = 0.123456f;
       float b2 = -0.123457f;
       BOOST_CHECK( !geometrix::equals_within_absolute_tolerance( a2, b2, feps ) );       
       BOOST_CHECK( !geometrix::equals_within_fraction_tolerance( a2, b2, feps ) );       
       
       //feps>(a2-b2) will return feps>0.246913, which is false, while feps>(b2-a2) will return feps> -0.246913, which is incorrectly true—oops! Equality (and inequality as well) is symmetric: if a!=b is true, then b!=a must also be true. Fix: feps>fabsf(a2-b2) will return false and so will feps>fabsf(b2-a2), at the cost of one fabsf (or your favorite work-alike) per comparison.
       //Turning a comparison into an inequality again st a difference, however, has shortcomings. Let:
       
       float big1 = 1.23456e28f;
       float big2 = 1.23457e28f;
       //FAILS: BOOST_CHECK( geometrix::equals_within_absolute_tolerance( big1, big2, feps ) );       
       BOOST_CHECK( geometrix::equals_within_fraction_tolerance( big1, big2, feps ) );

       //Are big1 and big2 equal? Their difference is a quantity of the order of 1.0e23, or, in U.S. dollars, much larger than the net worth of Bill Gates. If both are the result of computations involving thousands of floating point operations, however, big1 and big2 are most likely the same number. Yet feps>fabsf(big1-big2) will return false. How about setting feps=2.0e23? Just kidding. There's more—let:
       
       float small1 = 1.23456e-10f;
       float small2 = 3.45678e-11f;
       //FAILS: BOOST_CHECK( !geometrix::equals_within_absolute_tolerance( small1, small2, feps ) );       
       BOOST_CHECK( !geometrix::equals_within_fraction_tolerance( small1, small2, feps ) );
       
       //Here, small1 and small2 are truly different, and generously bigger than the smallest IEEE float larger than zero. However, feps>fabsf(small1-small2) returns true so that small1 and small2 incorrectly appear to be equal. The same error happens if:
       float small3 =  0.000004f;
       float small4 = -0.000004f;

       //!FAILS BOOST_CHECK( !geometrix::equals_within_absolute_tolerance( small3, small4, feps ) );
       BOOST_CHECK( !geometrix::equals_within_fraction_tolerance( small3, small4, feps ) );    

       BOOST_CHECK( equals_within_fraction_tolerance( std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon(), feps ) );

       //! Check against zero
       float zero = 0;

       BOOST_CHECK( equals_within_fraction_tolerance( small3, zero, feps ) );
       BOOST_CHECK( equals_within_fraction_tolerance( zero, small3, feps ) );
       BOOST_CHECK( equals_within_fraction_tolerance( small4, zero, feps ) );
       BOOST_CHECK( equals_within_fraction_tolerance( zero, small4, feps ) );

       BOOST_CHECK( equals_within_fraction_tolerance( 0.9999f*feps, zero, feps ) );
       BOOST_CHECK( equals_within_fraction_tolerance( feps, zero, feps ) );
       BOOST_CHECK( !equals_within_fraction_tolerance( 2.0f*feps, zero, feps ) );
   }

   //! Less than && less than equal - absolute tolerance
   {
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;
       BOOST_CHECK( !geometrix::less_than_with_absolute_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( !geometrix::less_than_with_absolute_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( geometrix::less_than_or_equal_with_absolute_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( geometrix::less_than_or_equal_with_absolute_tolerance( u, v, 1e-6 ) );
   }

   //! Less than && less than equal - fraction tolerance
   {
       double zero = 0;
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;
       BOOST_CHECK( !geometrix::less_than_with_fraction_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( !geometrix::less_than_with_fraction_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( geometrix::less_than_or_equal_with_fraction_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( geometrix::less_than_or_equal_with_fraction_tolerance( u, v, 1e-6 ) );

       BOOST_CHECK( geometrix::less_than_with_fraction_tolerance( zero, std::numeric_limits<double>::epsilon(), 1e-17 ) );
       BOOST_CHECK( geometrix::less_than_or_equal_with_fraction_tolerance( zero, std::numeric_limits<double>::epsilon(), 1e-17 ) );
   }

   //! Greater than && Greater than equal - fraction tolerance
   {
       double zero = 0;
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;
       BOOST_CHECK( !geometrix::greater_than_with_fraction_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( !geometrix::greater_than_with_fraction_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( geometrix::greater_than_or_equal_with_fraction_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( geometrix::greater_than_or_equal_with_fraction_tolerance( v, u, 1e-6 ) );

       BOOST_CHECK( geometrix::greater_than_with_fraction_tolerance( std::numeric_limits<double>::epsilon(), zero, 1e-17 ) );
       BOOST_CHECK( geometrix::greater_than_or_equal_with_fraction_tolerance( std::numeric_limits<double>::epsilon(), zero, 1e-17 ) );
   }

}

BOOST_AUTO_TEST_CASE(TestMappedComparisonPolicy)
{
	using namespace geometrix;
	mapped_tolerance_comparison_policy<absolute_tolerance_comparison_policy<double>> cmp;

	double a = 0;
	double b = 1e-8;

	auto result = cmp.equals(a, b);
	BOOST_CHECK(!result);
}

#include <boost/units/physical_dimensions.hpp>
#include <boost/units/dimensionless_quantity.hpp>
#include <boost/units/io.hpp>
#include <boost/units/pow.hpp>
#include <boost/units/limits.hpp>
#include <boost/units/systems/si.hpp>

BOOST_AUTO_TEST_CASE(TestMappedComparisonPolicyWithUnits)
{
	using namespace geometrix;
	using length_t = boost::units::quantity<boost::units::si::length, double>;
	using area_t = boost::units::quantity<boost::units::si::area, double>;
	auto cmp = make_mapped_tolerance_comparison_policy(absolute_tolerance_comparison_policy<double>(1e-8),boost::fusion::make_pair<length_t>(relative_tolerance_comparison_policy<double>(1e-3)));

	double a = 0;
	double b = 1e-2;

	auto result = cmp.equals(a, b);
	BOOST_CHECK(!result);

	length_t al = 1e-3 * boost::units::si::meters;
	result = cmp.equals(al, 0.0 * boost::units::si::meters);
	BOOST_CHECK(result);

	area_t aa = 1e-3 * boost::units::si::square_meters;
	result = cmp.equals(aa, 0.0 * boost::units::si::square_meters);
	BOOST_CHECK(!result);
	
	using comparison_policy = mapped_tolerance_comparison_policy < absolute_tolerance_comparison_policy<double>, boost::fusion::pair<length_t, relative_tolerance_comparison_policy<double>>>;

	comparison_policy item(1e-2);

	BOOST_CHECK(item.equals(al, al));
	BOOST_CHECK(!item.equals(0, 1));

	using dimensionless_t = boost::units::quantity<boost::units::si::dimensionless, double>;

	dimensionless_t d{ 0 };
	result = cmp.equals(d, a);
	BOOST_CHECK(result);

	result = cmp.equals(a, d);
	BOOST_CHECK(result);

	//cmp.equals(aa, al);
	cmp.equals(0 * boost::units::si::meters, 0.0 * boost::units::si::meters);

	comparison_policy cmp3(0);
	result = cmp3.equals(1e-9 * boost::units::si::meters, 1e-10 * boost::units::si::meters);
	BOOST_CHECK(!result);
}

#endif //GEOMETRIX_TOLERANCE_COMPARISON_TESTS_HPP
