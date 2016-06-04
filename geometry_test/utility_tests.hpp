//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_TESTS_HPP
#define GEOMETRIX_UTILITY_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( TestUtilityOperators )
{
    using namespace geometrix;

    {
        typedef point_double_2d point_2d;
        
        point_2d p1( 0., 0. );
        point_2d p2( 1., 0. );
        point_2d p3( 2., 0. );

        //! Collinearity check.
        double dot = dot_product( as_vector(p2), as_vector(p3) );
        std::cout << "Dot: " << dot << std::endl;

        double cross = exterior_product_area( p2-p1, p3-p1 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, fraction_tolerance_comparison_policy<double>(1e-10) ) );
    }

    {
        typedef point_int_2d point_2d;
        
        point_2d p1( 0, 0 );
        point_2d p2( 1, 1 );
        point_2d p3( 2, 2 );

        //! Collinearity check.
        double dot = dot_product( as_vector(p2), as_vector(p3) );
        std::cout << "Dot: " << dot << std::endl;

        double cross = exterior_product_area( p2-p1, p3-p1 );
        std::cout << "Cross: " << cross << std::endl;

        BOOST_CHECK( is_collinear( p1, p2, p3, direct_comparison_policy() ) );   
    }

}

template<int Index>
inline void typeof_test()
{
	typedef boost::array<int, 3> tensor_type;
	typedef BOOST_TYPEOF_TPL(((tensor_type*)0)->operator[](Index)) type1;
	typedef decltype(((tensor_type*)0)->operator[](Index)) type2;
	static_assert(std::is_same<type1, int>::value, "boost typeof is fixed.");
	static_assert(std::is_same<type2, int&>::value, "compiler bug");
}

BOOST_AUTO_TEST_CASE(TypeOfTest)
{
	typeof_test<0>();
}

#include <iostream>
#include <stdexcept>

class conststr
{
	const char* p;
	std::size_t sz;
public:
	template<std::size_t N>
	constexpr conststr(const char(&a)[N]) : p(a), sz(N - 1) {}

	constexpr char operator[](std::size_t n) const
	{
		return n < sz ? p[n] : throw std::out_of_range("");
	}
	constexpr std::size_t size() const { return sz; }
};

constexpr std::size_t countlower(conststr s, std::size_t n = 0,
	std::size_t c = 0)
{
	return n == s.size() ? c :
		s[n] >= 'a' && s[n] <= 'z' ? countlower(s, n + 1, c + 1) :
		countlower(s, n + 1, c);
}

// output function that requires a compile-time constant, for testing
template<int n>
struct constN
{
	constN() { std::cout << n << '\n'; }
};

BOOST_AUTO_TEST_CASE(StdLiteralTest)
{
	static_assert(countlower("Hello, world!") == 9, "constexpr not working"); 
}

#include <geometrix/arithmetic/arithmetic.hpp>
BOOST_AUTO_TEST_CASE(ATAN2Test_DifferentArithmeticTypesConvertibleToDouble_Compiles)
{
	int a = 10;
	double b = 20.0f;
	using geometrix::atan2;
	
	auto result = atan2(a, b);


}




#endif //GEOMETRIX_UTILITY_TESTS_HPP
