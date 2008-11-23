//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_TOLERANCE_COMPARISON_TESTS_HPP
#define _BOOST_GEOMETRY_TOLERANCE_COMPARISON_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"

#include <iostream>

BOOST_AUTO_TEST_CASE( TestFloatingPointComparison )
{   
    using namespace boost::numeric::geometry;

   //! Check the values within the specified tolerance
   {
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;

       BOOST_CHECK( boost::numeric::geometry::equals_within_absolute_tolerance( u, v, 1e-6 ) );
       BOOST_CHECK( boost::numeric::geometry::equals_within_absolute_tolerance( v, u, 1e-6 ) );
       BOOST_CHECK( (abs( u - v ) <= 1e-6 ) ); ///Sanity check against simple trival tolerance check type.    

       BOOST_CHECK( boost::numeric::geometry::equals_within_fraction_tolerance( u, v, 1e-6 ) );
       BOOST_CHECK( boost::numeric::geometry::equals_within_fraction_tolerance( v, u, 1e-6 ) );

       BOOST_CHECK( boost::numeric::geometry::equals_within_absolute_tolerance( 5., 4., 1. ) );       
       BOOST_CHECK( boost::numeric::geometry::equals_within_fraction_tolerance( 5., 4., 1. ) );

       BOOST_CHECK( !boost::numeric::geometry::equals_within_absolute_tolerance( 5., 4., 0.5 ) );       
       BOOST_CHECK( boost::numeric::geometry::equals_within_fraction_tolerance( 5., 4., 0.25 ) );

       //from: http://www.adtmag.com/joop/carticle.aspx?ID=396
       float feps = 0.00001f;

       //Thus, feps>(a1-b1) will return true, indicating that a1 and b1 are equal, as will feps>(b1-a1). The "close enough" equality test has become an inequality test against a difference. However, if you consider:
       float a2 = 0.123456f;
       float b2 = -0.123457f;
       BOOST_CHECK( !boost::numeric::geometry::equals_within_absolute_tolerance( a2, b2, feps ) );       
       BOOST_CHECK( !boost::numeric::geometry::equals_within_fraction_tolerance( a2, b2, feps ) );       
       
       //feps>(a2-b2) will return feps>0.246913, which is false, while feps>(b2-a2) will return feps> -0.246913, which is incorrectly true—oops! Equality (and inequality as well) is symmetric: if a!=b is true, then b!=a must also be true. Fix: feps>fabsf(a2-b2) will return false and so will feps>fabsf(b2-a2), at the cost of one fabsf (or your favorite work-alike) per comparison.
       //Turning a comparison into an inequality again st a difference, however, has shortcomings. Let:
       
       float big1 = 1.23456e28f;
       float big2 = 1.23457e28f;
       //FAILS: BOOST_CHECK( boost::numeric::geometry::equals_within_absolute_tolerance( big1, big2, feps ) );       
       BOOST_CHECK( boost::numeric::geometry::equals_within_fraction_tolerance( big1, big2, feps ) );

       //Are big1 and big2 equal? Their difference is a quantity of the order of 1.0e23, or, in U.S. dollars, much larger than the net worth of Bill Gates. If both are the result of computations involving thousands of floating point operations, however, big1 and big2 are most likely the same number. Yet feps>fabsf(big1-big2) will return false. How about setting feps=2.0e23? Just kidding. There's more—let:
       
       float small1 = 1.23456e-10f;
       float small2 = 3.45678e-11f;
       //FAILS: BOOST_CHECK( !boost::numeric::geometry::equals_within_absolute_tolerance( small1, small2, feps ) );       
       BOOST_CHECK( !boost::numeric::geometry::equals_within_fraction_tolerance( small1, small2, feps ) );
       
       //Here, small1 and small2 are truly different, and generously bigger than the smallest IEEE float larger than zero. However, feps>fabsf(small1-small2) returns true so that small1 and small2 incorrectly appear to be equal. The same error happens if:
       float small3 =  0.000004f;
       float small4 = -0.000004f;

       //!FAILS BOOST_CHECK( !boost::numeric::geometry::equals_within_absolute_tolerance( small3, small4, feps ) );
       BOOST_CHECK( !boost::numeric::geometry::equals_within_fraction_tolerance( small3, small4, feps ) );    

       BOOST_CHECK( equals_within_fraction_tolerance( std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon(), feps ) );

       //! Check against zero
       float zero = 0;

       BOOST_CHECK( equals_within_fraction_tolerance( small3, zero, feps ) );
       BOOST_CHECK( equals_within_fraction_tolerance( zero, small3, feps ) );
       BOOST_CHECK( equals_within_fraction_tolerance( small4, zero, feps ) );
       BOOST_CHECK( equals_within_fraction_tolerance( zero, small4, feps ) );

       BOOST_CHECK( equals_within_fraction_tolerance( 0.9999f*feps, zero, feps ) );
       BOOST_CHECK( !equals_within_fraction_tolerance( feps, zero, feps ) );
       BOOST_CHECK( !equals_within_fraction_tolerance( 2.0f*feps, zero, feps ) );
   }

   //! Less than && less than equal - absolute tolerance
   {
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;
       BOOST_CHECK( !boost::numeric::geometry::less_than_with_absolute_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( !boost::numeric::geometry::less_than_with_absolute_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( boost::numeric::geometry::less_than_or_equal_with_absolute_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( boost::numeric::geometry::less_than_or_equal_with_absolute_tolerance( u, v, 1e-6 ) );
   }

   //! Less than && less than equal - fraction tolerance
   {
       double zero = 0;
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;
       BOOST_CHECK( !boost::numeric::geometry::less_than_with_fraction_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( !boost::numeric::geometry::less_than_with_fraction_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( boost::numeric::geometry::less_than_or_equal_with_fraction_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( boost::numeric::geometry::less_than_or_equal_with_fraction_tolerance( u, v, 1e-6 ) );

       BOOST_CHECK( boost::numeric::geometry::less_than_with_fraction_tolerance( zero, std::numeric_limits<double>::epsilon(), 1e-17 ) );
       BOOST_CHECK( boost::numeric::geometry::less_than_or_equal_with_fraction_tolerance( zero, std::numeric_limits<double>::epsilon(), 1e-17 ) );
   }

   //! Greater than && Greater than equal - fraction tolerance
   {
       double zero = 0;
       double u = 1. + 1e-6;
       double v = 1. + 1e-7;
       BOOST_CHECK( !boost::numeric::geometry::greater_than_with_fraction_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( !boost::numeric::geometry::greater_than_with_fraction_tolerance( v, u, 1e-6 ) );       
       BOOST_CHECK( boost::numeric::geometry::greater_than_or_equal_with_fraction_tolerance( u, v, 1e-6 ) );       
       BOOST_CHECK( boost::numeric::geometry::greater_than_or_equal_with_fraction_tolerance( v, u, 1e-6 ) );

       BOOST_CHECK( boost::numeric::geometry::greater_than_with_fraction_tolerance( std::numeric_limits<double>::epsilon(), zero, 1e-17 ) );
       BOOST_CHECK( boost::numeric::geometry::greater_than_or_equal_with_fraction_tolerance( std::numeric_limits<double>::epsilon(), zero, 1e-17 ) );
   }

}

#endif //_BOOST_GEOMETRY_TOLERANCE_COMPARISON_TESTS_HPP



