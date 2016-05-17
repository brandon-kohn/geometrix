//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SORTING_TESTS_HPP
#define GEOMETRIX_SORTING_TESTS_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/arithmetic/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/utility/vector_angle_compare.hpp>
#include <deque>

BOOST_AUTO_TEST_CASE( SortingTests )
{
    using namespace geometrix;

    typedef vector_double_2d vector_2d;

    std::deque<vector_2d> vecs;
    double factor = constants::pi<double>() / 180.0;
    for( int i = 0; i < 360; ++i )
    {
        double angle = i * factor;
        double cosa = cos( angle );
        double sina = sin( angle );
        matrix<double, 2, 2> rotation = {{{ cosa, -sina }
                                        , { sina,  cosa }}};
        vecs.emplace_front(rotation * vector_2d( 1.0, 0. ));
    }

    absolute_tolerance_comparison_policy<double> cmp( 1e-10 );
    std::set < vector_2d, vector_angle_compare<absolute_tolerance_comparison_policy<double>>> sorted( vecs.begin(), vecs.end(), cmp );
    
    BOOST_CHECK( vecs.size() == sorted.size() );
    auto ita = vecs.begin();
    auto itb = sorted.rbegin();
    while( ita != vecs.end() && itb != sorted.rend() )
    {
        BOOST_CHECK( numeric_sequence_equals_2d( *ita++, *itb++, cmp ) );
    }
}

#endif//! GEOMETRIX_SORTING_TESTS_HPP
