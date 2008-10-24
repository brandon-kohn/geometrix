//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
#define _BOOST_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
#pragma once

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/lambda/lambda.hpp>
#include "../geometry/numeric_sequence.hpp"
#include "../geometry/point.hpp"
#include "../geometry/vector.hpp"
#include "../geometry/indexed_sequence_traversal.hpp"
#include "../geometry/indexed_sequence_operators.hpp"

struct point_t_3
{
    point_t_3( double x, double y, double z )
        : p( boost::make_tuple( x, y, z ) )
    {}

    template <unsigned int Index>
    const double& get() const
    {
        return boost::fusion::at_c<Index>( p );
    }

    template <unsigned int Index>
    double& get()
    {
        return boost::fusion::at_c<Index>( p );
    }

    boost::tuple< double, double, double > p;
};

template <>
struct construction_traits< point_t_3 >
{    
    static inline point_t_3 construct( const double& x, const double& y, const double& z )
    {
        return point_t_3( x,y,z );
    }

    template <typename NumericSequence>
    static inline point_t_3 construct( const NumericSequence& args )
    {
        return point_t_3( boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
                          boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
                          boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) );
    }
};

struct vector_t_3
{
    vector_t_3( double x, double y, double z )
        : p( boost::make_tuple( x, y, z ) )
    {
    }

    template <unsigned int Index>
    const double& get() const
    {
        return boost::fusion::at_c<Index>( p );
    }

    template <unsigned int Index>
    double& get()
    {
        return boost::fusion::at_c<Index>( p );
    }

    boost::tuple< double, double, double > p;
};

template <>
struct construction_traits< vector_t_3 >
{    
    static inline vector_t_3 construct( const double& x, const double& y, const double& z )
    {
        return vector_t_3( x,y,z );
    }

    template <typename NumericSequence>
    static inline vector_t_3 construct( const NumericSequence& args )
    {
        return vector_t_3( boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
                          boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
                          boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) ); 
    }
};

struct point_s_3
{
    point_s_3( double x, double y, double z )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=3;
    }

    const double&  operator[]( size_t i ) const { return p[i]; }
    double& operator[]( size_t i ) { return p[i]; }

    std::vector< double > p;
};

template <>
struct construction_traits< point_s_3 >
{    
    static inline point_s_3 construct( const double& x, const double& y, const double& z )
    {
        return point_s_3( x,y,z );
    }

    template <typename NumericSequence>
    static inline point_s_3 construct( const NumericSequence& args )
    {
        return point_s_3( boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
                          boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
                          boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) );
    }
};

struct vector_s_3
{
    vector_s_3( double x, double y, double z )
        : p(3)
    {
        p[0]=x; p[1]=y; p[2]=3;
    }

    const double& operator[]( size_t i ) const { return p[i]; }
    double&       operator[]( size_t i ) { return p[i]; }

    std::vector< double > p;
};

template <>
struct construction_traits< vector_s_3 >
{    
    static inline vector_s_3 construct( const double& x, const double& y, const double& z )
    {
        return vector_s_3( x,y,z );
    }

    template <typename NumericSequence>
    static inline vector_s_3 construct( const NumericSequence& args )
    {
        return vector_s_3( boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<0>( args ),
                           boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<1>( args ),
                           boost::numeric::geometry::indexed_access_traits<NumericSequence>::get<2>( args ) );
    }
};

BOOST_DEFINE_USER_POINT_TRAITS( point_s_3, double, 3, use_run_time_access<true>, use_compile_time_access<false> );
BOOST_DEFINE_USER_VECTOR_TRAITS( vector_s_3, double, 3, use_run_time_access<true>, use_compile_time_access<false> );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_s_3 );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( vector_s_3 );

BOOST_DEFINE_USER_POINT_TRAITS( point_t_3, double, 3, use_run_time_access<false>, use_compile_time_access<true> );
BOOST_DEFINE_USER_VECTOR_TRAITS( vector_t_3, double, 3, use_run_time_access<false>, use_compile_time_access<true> );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_t_3 );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( vector_t_3 );

BOOST_AUTO_TEST_CASE( TestIndexedSequence )
{
    using namespace boost::numeric::geometry;
    using namespace boost::numeric::geometry::detail;
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
        point_s_3 a( 1., 2., 0. );
        vector_s_3 b( 1., 2., 0. );

	    //! Check addition
	    {
            point_s_3 c = a;
            indexed_sequence_traversal::for_each( c, a, _1 += _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 4., 1e-10 );
	    }

	    //! Check subtraction
	    {
		    point_s_3 c = a;
            indexed_sequence_traversal::for_each( c, a, _1 -= _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 0., 1e-10 );
	    }

	    //! Check scalar multiplication
	    {
		    point_s_3 c = a;
            indexed_sequence_traversal::for_each( c, _1 *= 4.0 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 8., 1e-10 );
	    }

	    //! Check scalar division
	    {
		    point_s_3 c = a;
            indexed_sequence_traversal::for_each( c, _1 /= 4.0 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), .25, 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), .5, 1e-10 );
	    }
    }
    //! compile-time access.
    {
        point_t_3 a( 1., 2., 0. );
	    vector_t_3 b( 1., 2., 0. );
        
	    //! Check addition
	    {
            point_t_3 c = a;
            indexed_sequence_traversal::for_each( c, a, _1 += _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( c ), 4., 1e-10 );
	    }

        //! Check addition
	    {
            point_t_3 c = a;
            c = c + b;
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( c ), 4., 1e-10 );
	    }

	    //! Check subtraction
	    {
		    point_t_3 c = a;
            indexed_sequence_traversal::for_each( c, a, _1 -= _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( c ), 0., 1e-10 );
	    }

        //! Check subtraction
	    {
		    point_t_3 c = a;
            c = c - b;            
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( c ), 0., 1e-10 );
	    }

	    //! Check scalar multiplication
	    {
		    point_t_3 c = a;
            indexed_sequence_traversal::for_each( c, _1 *= 4.0 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( c ), 8., 1e-10 );
	    }

        //! Check scalar multiplication
	    {
		    vector_t_3 c = b;
            c = c * 4.0;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_t_3>::get<0>( c ), 4., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_t_3>::get<1>( c ), 8., 1e-10 );
	    }

	    //! Check scalar division
	    {
		    point_t_3 c = a;
            indexed_sequence_traversal::for_each( c, _1 /= 4.0 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( c ), .25, 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( c ), .5, 1e-10 );
	    }

        //! Check scalar division
	    {
		    vector_t_3 c = b;
            c = c / 4.0;
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_t_3>::get<0>( c ), .25, 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<vector_t_3>::get<1>( c ), .5, 1e-10 );
	    }
    }

    //! mixed access.
    {
        point_t_3 a( 1., 2., 0. );
	    vector_t_3 b( 1., 2., 0. );
        
	    //! Check addition
	    {
            point_s_3 c( 1., 2., 0. );
            indexed_sequence_traversal::for_each( c, a, _1 += _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 4., 1e-10 );
	    }

	    //! Check subtraction
	    {
		    point_s_3 c( 1., 2., 0. );
            indexed_sequence_traversal::for_each( c, a, _1 -= _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<0>( c ), 0., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_s_3>::get<1>( c ), 0., 1e-10 );
	    }

        //! Check addition
	    {
            point_s_3 c( 1., 2., 0. );
            indexed_sequence_traversal::for_each( a, c, _1 += _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( a ), 2., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( a ), 4., 1e-10 );
	    }

	    //! Check subtraction
	    {
		    point_s_3 c( 1., 2., 0. );
            indexed_sequence_traversal::for_each( a, c, _1 -= _2 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<0>( a ), 1., 1e-10 );
            BOOST_CHECK_CLOSE( cartesian_access_traits<point_t_3>::get<1>( a ), 2., 1e-10 );
	    }

    }

}

#endif //_BOOST_GEOMETRY_INDEXED_SEQUENCE_TESTS_HPP
