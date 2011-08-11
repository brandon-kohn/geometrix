//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CONSTANT_TESTS_HPP
#define GEOMETRIX_CONSTANT_TESTS_HPP


#include <boost/test/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/numeric/rational_utilities.hpp>
#include <lazy_exact/rational/rational_ext.hpp>
#include <lazy_exact/rational/rational_casts.hpp>

#include <iostream>

template <typename Rational>
struct format_rational
{
    typedef typename Rational::int_type int_t;

    format_rational( const Rational& src )
        : significand( 0 )
        , exponent( 0 )
    {
        boost::get_significant_digits< geometrix::numeric_traits< int_t >::digits >( src, significand, exponent );    
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
    using namespace geometrix;
    const boost::rational< T > sPi = constants< T >::pi();
    std::cout << typeid(T).name() << ": " << format_rational< boost::rational< T > >( sPi ) << std::endl;
}

BOOST_AUTO_TEST_CASE( TestConstants )
{
    using namespace geometrix;
    print_pi<boost::int8_t>();
    print_pi<boost::int16_t>();
    print_pi<boost::int32_t>();
    print_pi<boost::int64_t>();    	    
}

#endif //GEOMETRIX_CONSTANT_TESTS_HPP


