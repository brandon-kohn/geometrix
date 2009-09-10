//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_NUMERIC_TRAITS_HPP
#define GENERATIVE_GEOMETRY_NUMERIC_TRAITS_HPP
#pragma once

#include "construction_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a numeric type.
template <typename NumericType>
struct is_numeric : boost::false_type{};

//! Default numeric traits struct. 
//! NOTE: must be specialized for user types.
template <typename NumericType>
struct numeric_traits
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, NUMERIC_TRAITS_NOT_DEFINED
		, (NumericType) );
    
    typedef typename NumericType numeric_type;
    static const numeric_type    zero = 0;
    typedef boost::false_type    is_float;    
    typedef boost::false_type    is_integral;
};

//! Macro for native types which work with boost::is_float/is_integral for automatic detmination of type.
#define BOOST_DEFINE_NUMERIC_TRAITS( NumericType )                                    \
namespace generative{ namespace numeric{ namespace geometry{                               \
template <> struct is_numeric< NumericType > : boost::true_type{};                    \
template <>                                                                           \
struct numeric_traits< NumericType >                                                  \
{                                                                                     \
    typedef NumericType                              numeric_type;                    \
    typedef boost::is_float< numeric_type >::type    is_float;                        \
    typedef boost::is_integral< numeric_type >::type is_integral;                     \
    static const numeric_type                        zero;                            \
    static const unsigned int precision = std::numeric_limits<numeric_type>::digits10;\
                                                                                      \
    BOOST_MPL_ASSERT_MSG(                                                             \
        ( !( is_float::value && is_integral::value ) )                                \
		, NUMERIC_TRAITS_CANNOT_DEFINE_FLOAT_AND_INTEGRAL                             \
		, (NumericType) );                                                            \
};                                                                                    \
const numeric_traits<NumericType>::numeric_type numeric_traits<NumericType>::zero=0;  \
template <> struct construction_traits< NumericType >{ static const NumericType& construct( const NumericType& t ){ return t; } };\
}}}//namespace generative::numeric::geometry;                                              \

//! Macro for defining integral types.
#define BOOST_DEFINE_INTEGRAL_NUMERIC_TRAITS( NumericType, Precision, ZERO )           \
namespace generative{namespace numeric{namespace geometry{                                  \
template <> struct is_numeric< NumericType > : boost::true_type{};                     \
template <>                                                                            \
struct numeric_traits< NumericType >                                                   \
{                                                                                      \
    typedef NumericType       numeric_type;                                            \
    static const unsigned int precision = Precision;                                   \
    typedef boost::false_type is_float ;                                               \
    typedef boost::true_type  is_integral;                                             \
    static const numeric_type zero;                                                    \
};                                                                                     \
const numeric_traits<NumericType>::numeric_type numeric_traits<NumericType>::zero=ZERO;\
}}}//namespace generative::numeric::geometry;                                               \

//! Macro for defining floating point types.
#define BOOST_DEFINE_FLOATING_POINT_NUMERIC_TRAITS( NumericType, Precision, ZERO )     \
namespace generative{ namespace numeric{ namespace geometry{                                \
template <> struct is_numeric< NumericType > : boost::true_type{};                     \
template <>                                                                            \
struct numeric_traits< NumericType >                                                   \
{                                                                                      \
    typedef NumericType       numeric_type;                                            \
    static const unsigned int precision = Precision;                                   \
    typedef boost::true_type  is_float;                                                \
    typedef boost::false_type is_integral;                                             \
    static const numeric_type zero;                                                    \
};                                                                                     \
const numeric_traits<NumericType>::numeric_type numeric_traits<NumericType>::zero=ZERO;\
}}}//namespace generative::numeric::geometry;                                               \

}}}//namespace generative::numeric::geometry;

BOOST_DEFINE_NUMERIC_TRAITS( double );
BOOST_DEFINE_NUMERIC_TRAITS( long double );
BOOST_DEFINE_NUMERIC_TRAITS( float );
BOOST_DEFINE_NUMERIC_TRAITS( int );
BOOST_DEFINE_NUMERIC_TRAITS( char );
BOOST_DEFINE_NUMERIC_TRAITS( short );
BOOST_DEFINE_NUMERIC_TRAITS( long );
BOOST_DEFINE_NUMERIC_TRAITS( long long );
BOOST_DEFINE_NUMERIC_TRAITS( unsigned int );
BOOST_DEFINE_NUMERIC_TRAITS( unsigned char );
BOOST_DEFINE_NUMERIC_TRAITS( unsigned short );
BOOST_DEFINE_NUMERIC_TRAITS( unsigned long );
BOOST_DEFINE_NUMERIC_TRAITS( unsigned long long );

#endif //GENERATIVE_GEOMETRY_NUMERIC_TRAITS_HPP
