//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_CONSTANT_TESTS_HPP
#define _GENERATIVE_GEOMETRY_CONSTANT_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/constants.hpp"
#include "../geometry/rational_utilities.hpp"
#include "../lazy_exact/rational/rational_ext.hpp"
#include "../lazy_exact/rational/rational_casts.hpp"

#include <iostream>

template <typename Rational>
struct format_rational
{
    typedef typename Rational::int_type int_t;

    format_rational( const Rational& src )
        : significand( 0 )
        , exponent( 0 )
    {
        boost::get_significant_digits< generative::numeric::geometry::numeric_traits< int_t >::precision >( src, significand, exponent );    
    }        

    int_t significand;
    int_t exponent;
};

template <typename Rational>
std::ostream& operator << ( std::ostream& os, format_rational< Rational >& rat )
{
    os << rat.significand << "E" << rat.exponent;
    return os;
}

template <typename T>
void print_pi()
{
    using namespace generative::numeric::geometry;
    const boost::rational< T > sPi = constants< T >::pi();
    std::cout << typeid(T).name() << ": " << format_rational< boost::rational< T > >( sPi ) << std::endl;
}

BOOST_AUTO_TEST_CASE( TestConstants )
{
	using namespace generative::numeric::geometry;
    print_pi<char>();
    print_pi<short>();
    print_pi<int>();
    print_pi<long>();
    print_pi<long long>();    	    
}

#endif //_GENERATIVE_GEOMETRY_CONSTANT_TESTS_HPP


