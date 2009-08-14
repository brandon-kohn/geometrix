//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
#define GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "../geometry/cartesian_access_traits.hpp"
#include "../geometry/cartesian_polar_transformation.hpp"
#include "../geometry/polar_cartesian_transformation.hpp"
#include "../geometry/polar_access_traits.hpp"
#include "../geometry/affine_space_traits.hpp"
#include "../geometry/numeric_sequence.hpp"
#include "../geometry/point.hpp"
#include "../geometry/vector.hpp"
#include "../geometry/indexed_sequence_traversal.hpp"
#include "../geometry/indexed_sequence_operators.hpp"
#include "../geometry/number_comparison_policy.hpp"
#include "../geometry/constants.hpp"
#include "tuple_kernal.hpp"
#include "vector_kernal.hpp"

BOOST_AUTO_TEST_CASE( TestIndexedSequence )
{
    using namespace generative::numeric::geometry;
    using namespace generative::numeric::geometry::detail;
    using namespace boost::lambda;

    typedef numeric_sequence< double, 3 > double_3;
    typedef point< double, 3 > point_3;
    typedef vector< double, 3 > vector_3;
    typedef double numeric_type;

    //double_3 sequence(
    typedef boost::array<double,3> array3;

    array3 ar = { 0., 1., 2. };

    //run-time access.
    {
        point_vector_3 a( 1., 2., 0. );
        vector_vector_3 b( 1., 2., 0. );

	    //! Check addition
	    {
            point_vector_3 c = a;
            //indexed_sequence_traversal::for_each( c, a, _1 += _2 );
            c = a + b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<1>( c ), 4., 1e-10 );
	    }

        //! Check addition
	    {
            point_vector_3 c = a;
            c = c + b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<1>( c ), 4., 1e-10 );
	    }

	    //! Check subtraction
	    {
		    point_vector_3 c = a;
            //indexed_sequence_traversal::for_each( c, a, _1 -= _2 );
            c = c - b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<1>( c ), 0., 1e-10 );
	    }

        //! Check subtraction
	    {
		    point_vector_3 c = a;
            c = a - b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<1>( c ), 0., 1e-10 );
	    }

	    //! Check scalar multiplication... 
	    {
            vector_vector_3 c = b;
            //indexed_sequence_traversal::for_each( c, _1 *= 4.0 );
            c = 4.0 * c;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<1>( c ), 8., 1e-10 );
	    }

        //! Check scalar multiplication
	    {
		    vector_vector_3 c = b;
            c = c * 4.0;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<1>( c ), 8., 1e-10 );
	    }

	    //! Check scalar division
	    {
            vector_vector_3 c = b;
            //indexed_sequence_traversal::for_each( c, _1 /= 4.0 );
            c = c / 4.0;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<0>( c ), .25, 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<1>( c ), .5, 1e-10 );
	    }
    }

    //! compile-time access.
    {
        point_tuple_3 a( construction_traits<point_tuple_3>::construct( 1., 2., 0. ) );
	    vector_tuple_3 b( construction_traits<vector_tuple_3>::construct( 1., 2., 0. ) );
        
        //! Check addition
	    {
            point_tuple_3 c = a;
            c = c + b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_tuple_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_tuple_3>::get<1>( c ), 4., 1e-10 );
	    }

        //! Check subtraction
	    {
		    point_tuple_3 c = a;
            c = c - b;            
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_tuple_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_tuple_3>::get<1>( c ), 0., 1e-10 );
	    }

        //! Check scalar multiplication
	    {
		    vector_tuple_3 c = b;
            double v = 4.0;
            c = c * v;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<1>( c ), 8., 1e-10 );
	    }

        //! Check scalar division
	    {
		    vector_tuple_3 c = b;
            c = c / 4.0;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<0>( c ), .25, 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<1>( c ), .5, 1e-10 );
	    }
    }

    //! mixed access.
    {
        point_tuple_3 a( construction_traits<point_tuple_3>::construct( 1., 2., 0. ) );
	    vector_tuple_3 b( construction_traits<vector_tuple_3>::construct( 1., 2., 0. ) );
        
	    //! Check addition
	    {
            point_vector_3 c( 1., 2., 0. );
            //indexed_sequence_traversal::for_each( c, b, _1 += _2 );
            c = c + b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<1>( c ), 4., 1e-10 );
	    }

        //! Check addition
	    {
            point_vector_3 c( 1., 2., 0. );
            c = c + b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<1>( c ), 4., 1e-10 );
	    }

	    //! Check subtraction
	    {
		    point_vector_3 c( 1., 2., 0. );
            //indexed_sequence_traversal::for_each( c, a, _1 -= _2 );
            c = construction_traits< point_vector_3 >::construct( a - b );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_vector_3>::get<1>( c ), 0., 1e-10 );
	    }

        //! Check subtraction
	    {
		    vector_vector_3 c( construction_traits<vector_vector_3>::construct( 1., 2., 0. ) );
            c = c - b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_vector_3>::get<1>( c ), 0., 1e-10 );
	    }

        //! Check addition
	    {
            vector_vector_3 c( construction_traits<vector_vector_3>::construct( 1., 2., 0. ) );
            vector_tuple_3 a( construction_traits<vector_tuple_3>::construct( 1., 2., 0. ) );
            //indexed_sequence_traversal::for_each( a, c, _1 += _2 );
            a = a + c;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<0>( a ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<1>( a ), 4., 1e-10 );
	    }

        //! Check addition
	    {
            vector_vector_3 c( construction_traits<vector_vector_3>::construct( 1., 2., 0. ) );
            vector_tuple_3 a( construction_traits<vector_tuple_3>::construct( 1., 2., 0. ) );
            a = a + b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<0>( a ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<1>( a ), 4., 1e-10 );
	    }

	    //! Check subtraction
	    {
            vector_vector_3 c( construction_traits<vector_vector_3>::construct( 1., 2., 0. ) );
            vector_tuple_3 a( construction_traits<vector_tuple_3>::construct( 1., 2., 0. ) );
            a = a - c;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<0>( a ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_tuple_3>::get<1>( a ), 0., 1e-10 );
	    }
    }

	typedef cartesian_reference_frame_double_3d                       cartesian_frame_3d;
    typedef reference_frame_tag< point_tuple_3, cartesian_frame_3d >  cartesian_point_3d;
    typedef reference_frame_tag< vector_tuple_3, cartesian_frame_3d > cartesian_vector_3d;
    typedef polar_reference_frame_double_3d                           polar_frame_3d;
    typedef reference_frame_tag< point_tuple_3, polar_frame_3d >      polar_point_3d;
    typedef reference_frame_tag< vector_vector_3, polar_frame_3d >    polar_vector_3d;

    fraction_tolerance_comparison_policy<double> compare(1e-10);
    cartesian_point_3d cPoint( construction_traits<point_tuple_3>::construct( 0., 1., 20.0 ) );
    cartesian_vector_3d vPoint( construction_traits<vector_vector_3>::construct( 1., 0., 11.0 ) );

    polar_point_3d pPoint( construction_traits<point_tuple_3>::construct( 1., constants< double >::pi() / 2., constants< double >::pi() ) );
    cartesian_point_3d cPoint2 = pPoint;
    cPoint2 = cPoint + vPoint;
    BOOST_CHECK( compare.equals( cPoint2.get<0>(), 1.0 ) );
    BOOST_CHECK( compare.equals( cPoint2.get<1>(), 1.0 ) );
}

#endif //GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
