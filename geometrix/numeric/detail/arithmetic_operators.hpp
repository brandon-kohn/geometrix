//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ARITHMETIC_OPERATORS_HPP
#define GEOMETRIX_ARITHMETIC_OPERATORS_HPP

#include <geometrix/utility/preprocessor.hpp>
#include <boost/config.hpp>

//!Arithmetic
#define GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE(T, U, OP, TEMPLATE)                             \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_SIZE(TEMPLATE), template<BOOST_PP_TUPLE_ENUM(TEMPLATE)>,) \
    BOOST_CONSTEXPR inline T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }     \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_SIZE(TEMPLATE), template<BOOST_PP_TUPLE_ENUM(TEMPLATE)>, )\
    BOOST_CONSTEXPR inline T operator OP( const U& lhs, T rhs ) { return rhs OP##= lhs; }     \
/***/

#define GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE_SELF(T, OP, TEMPLATE)                           \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_SIZE(TEMPLATE), template<BOOST_PP_TUPLE_ENUM(TEMPLATE)>,) \
    BOOST_CONSTEXPR inline T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }     \
/***/

#define GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE(T, U, OP, TEMPLATE)                         \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_SIZE(TEMPLATE), template<BOOST_PP_TUPLE_ENUM(TEMPLATE)>,) \
    BOOST_CONSTEXPR inline T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }     \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_SIZE(TEMPLATE), template<BOOST_PP_TUPLE_ENUM(TEMPLATE)>,) \
    BOOST_CONSTEXPR inline T operator OP( const U& lhs, const T& rhs )                        \
    { return T( lhs ) OP##= rhs; }                                                            \
/***/

#define GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE_SELF(T, OP, TEMPLATE)                       \
    BOOST_PP_EXPR_IF(BOOST_PP_TUPLE_SIZE(TEMPLATE), template<BOOST_PP_TUPLE_ENUM(TEMPLATE)>,) \
    BOOST_CONSTEXPR inline T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }     \
/***/

#define GEOMETRIX_IMPLEMENT_BINARY_ARITHMETIC_OPERATORS(T, U, TEMPLATE) \
GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE(T, U, *, TEMPLATE)                \
GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE(T, U, +, TEMPLATE)                \
GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE(T, U, -, TEMPLATE)            \
GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE(T, U, /, TEMPLATE)            \
/***/

#define GEOMETRIX_IMPLEMENT_BINARY_ARITHMETIC_OPERATORS_SELF(T, TEMPLATE)\
GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE_SELF(T, *, TEMPLATE)               \
GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE_SELF(T, +, TEMPLATE)               \
GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE_SELF(T, -, TEMPLATE)           \
GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE_SELF(T, /, TEMPLATE)           \
/***/

#endif//! GEOMETRIX_ARITHMETIC_OPERATORS_HPP
