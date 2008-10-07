//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_COORDINATE_TRAITS_HPP
#define _BOOST_GEOMETRY_COORDINATE_TRAITS_HPP
#pragma once

#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{


//! Default point traits struct. 
//! NOTE: must be specialized for user types.
template <typename Coordinate>
struct coordinate_traits
{
    typedef typename Coordinate coordinate_type;

	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, COORDINATE_TRAITS_NOT_DEFINED
		, (Coordinate) );

};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_COORDINATE_TRAITS( Coordinate )                    \
template <>                                                             \
struct coordinate_traits< Coordinate >                                  \
{                                                                       \
    typedef Coordinate                                  coordinate_type;\
    typedef boost::is_float< coordinate_type >::type    is_float_t;     \
    typedef boost::is_integral< coordinate_type >::type is_integral_t;  \
    BOOST_MPL_ASSERT_MSG(                                               \
        ( !( is_float_t::value && is_integral_t::value ) )              \
		, COORDINATE_TRAITS_CANNOT_DEFINE_FLOAT_AND_INTEGRAL            \
		, (Coordinate) );                                               \
};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_INTEGRAL_COORDINATE_TRAITS( Coordinate )          \
template <>                                                            \
struct coordinate_traits< Coordinate >                                 \
{                                                                      \
    typedef Coordinate                                 coordinate_type;\
    typedef boost::false_type                          is_float_t ;    \
    typedef boost::true_type                           is_integral_t;   \
};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_FLOATING_POINT_COORDINATE_TRAITS( Coordinate )     \
template <>                                                             \
struct coordinate_traits< Coordinate >                                  \
{                                                                       \
    typedef Coordinate                                  coordinate_type;\
    typedef boost::true_type                            is_float_t;     \
    typedef boost::false_type                           is_integral_t;   \
};

BOOST_DEFINE_COORDINATE_TRAITS( double );
BOOST_DEFINE_COORDINATE_TRAITS( long double );
BOOST_DEFINE_COORDINATE_TRAITS( float );
BOOST_DEFINE_COORDINATE_TRAITS( int );
BOOST_DEFINE_COORDINATE_TRAITS( char );
BOOST_DEFINE_COORDINATE_TRAITS( short );
BOOST_DEFINE_COORDINATE_TRAITS( long );
BOOST_DEFINE_COORDINATE_TRAITS( long long );
BOOST_DEFINE_COORDINATE_TRAITS( unsigned int );
BOOST_DEFINE_COORDINATE_TRAITS( unsigned char );
BOOST_DEFINE_COORDINATE_TRAITS( unsigned short );
BOOST_DEFINE_COORDINATE_TRAITS( unsigned long );
BOOST_DEFINE_COORDINATE_TRAITS( unsigned long long );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_COORDINATE_TRAITS_HPP
