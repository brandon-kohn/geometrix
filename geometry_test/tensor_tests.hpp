//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TENSOR_TESTS_HPP
#define GEOMETRIX_TENSOR_TESTS_HPP


#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <geometrix/space/cartesian_access_traits.hpp>
#include <geometrix/space/cartesian_polar_transformation.hpp>
#include <geometrix/space/polar_cartesian_transformation.hpp>
#include <geometrix/space/polar_access_traits.hpp>
#include <geometrix/space/affine_space_traits.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>
#include <geometrix/utility/array.hpp>

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/numeric/constants.hpp>

#include "tuple_kernal.hpp"
#include "vector_kernal.hpp"

BOOST_AUTO_TEST_CASE( TestIndexedSequence )
{
    using namespace geometrix;    
    
    //double_3 sequence(
    typedef boost::array<double,3> array3;

    GEOMETRIX_STATIC_ASSERT(( is_point<geometrix::expr<boost::proto::exprns_::basic_expr<boost::proto::tag::terminal,boost::proto::argsns_::term<point_tuple_3 &>,0> > >::value ));
    GEOMETRIX_STATIC_ASSERT(( is_point<geometrix::expr<boost::proto::exprns_::basic_expr<boost::proto::tag::terminal,boost::proto::argsns_::term<point_tuple_3 &>,0> > >::value ));

    array3 ar = { 0., 1., 2. };
    
    set<1>( ar, 5. );
    set<1>( ar, 1. );
    
    //run-time access.
    {
        point_vector_3 a( 1., 2., 0. );
        vector_vector_3 b( 1., 2., 0. );

        //! Check addition
        {
            point_vector_3 c = a;
            c <<= ( a + b );
            BOOST_CHECK_CLOSE( get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 4., 1e-10 );
        }

        //! Check addition
        {
            point_vector_3 c = a;
            c <<= ( c + b );
            BOOST_CHECK_CLOSE( get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 4., 1e-10 );
        }

        //! Check subtraction
        {
            point_vector_3 c = a;
            c <<= ( c - b );
            BOOST_CHECK_CLOSE( get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 0., 1e-10 );
        }

        //! Check subtraction
        {
            point_vector_3 c = a;
            c <<= ( a - b );
            BOOST_CHECK_CLOSE( get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 0., 1e-10 );
        }

        //! Check subtraction
        {
            vector_vector_3 c;
            c <<= ( a - a );
            BOOST_CHECK_CLOSE( get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 0., 1e-10 );
        }

        //! Check scalar multiplication... 
        {
            vector_vector_3 c = b;
            c <<= ( 4.0 * c );
            BOOST_CHECK_CLOSE( get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 8., 1e-10 );
        }

        //! Check scalar multiplication
        {
            vector_vector_3 c = b;
            c <<= 4.0 * c;
            BOOST_CHECK_CLOSE( get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 8., 1e-10 );
        }

        //! Check scalar division
        {
            vector_vector_3 c = b;
            c <<= ( c / 4.0 );
            BOOST_CHECK_CLOSE( get<0>( c ), .25, 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), .5, 1e-10 );
        }
    }
    
    //! compile-time access.
    {
        point_tuple_3 a( construct<point_tuple_3>( 1., 2., 0. ) );
        vector_tuple_3 b( construct<vector_tuple_3>( 1., 2., 0. ) );
        
        //! Check addition
        {
            point_tuple_3 c = a;
            c <<= ( c + b );
            BOOST_CHECK_CLOSE( get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 4., 1e-10 );
        }

        //! Check subtraction
        {
            point_tuple_3 c = a;
            c = construct<point_tuple_3>( c - b );
            BOOST_CHECK_CLOSE( get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 0., 1e-10 );
        }

        //! Check scalar multiplication
        {
            vector_tuple_3 c = b;
            double v = 4.0;
            c <<= ( v * c );
            BOOST_CHECK_CLOSE( get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 8., 1e-10 );
        }

        //! Check scalar division
        {
            vector_tuple_3 c = b;
            c <<= ( c / 4.0 );
            BOOST_CHECK_CLOSE( get<0>( c ), .25, 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), .5, 1e-10 );
        }
    }

    //! mixed access.
    {
        point_tuple_3 a( construct<point_tuple_3>( 1., 2., 0. ) );
        vector_tuple_3 b( construct<vector_tuple_3>( 1., 2., 0. ) );
        
        //! Check addition
        {
            point_vector_3 c( 1., 2., 0. );
            c <<= ( c + b );
            BOOST_CHECK_CLOSE( get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 4., 1e-10 );
        }

        //! Check addition
        {
            point_vector_3 c( 1., 2., 0. );
            c <<= ( c + b );
            BOOST_CHECK_CLOSE( get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 4., 1e-10 );
        }

        //! Check subtraction
        {
            point_vector_3 c( 1., 2., 0. );
            c <<= ( a - b );
            BOOST_CHECK_CLOSE( get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 0., 1e-10 );
        }

        //! Check subtraction
        {
            vector_vector_3 c( construct<vector_vector_3>( 1., 2., 0. ) );
            c <<= ( c - b );
            BOOST_CHECK_CLOSE( get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( c ), 0., 1e-10 );
        }

        //! Check addition
        {
            vector_vector_3 c( construct<vector_vector_3>( 1., 2., 0. ) );
            vector_tuple_3 a( construct<vector_tuple_3>( 1., 2., 0. ) );
            a <<= ( a + c );
            BOOST_CHECK_CLOSE( get<0>( a ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( a ), 4., 1e-10 );
        }

        //! Check addition
        {
            vector_vector_3 c( construct<vector_vector_3>( 1., 2., 0. ) );
            vector_tuple_3 a( construct<vector_tuple_3>( 1., 2., 0. ) );
            a <<= ( a + c );
            BOOST_CHECK_CLOSE( get<0>( a ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( a ), 4., 1e-10 );
        }

        //! Check subtraction
        {
            vector_vector_3 c( construct<vector_vector_3>( 1., 2., 0. ) );
            vector_tuple_3 a( construct<vector_tuple_3>( 1., 2., 0. ) );
            a <<= ( a - c );
            BOOST_CHECK_CLOSE( get<0>( a ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( get<1>( a ), 0., 1e-10 );
        }
    }
}

#endif //GEOMETRIX_TENSOR_TESTS_HPP
