//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FRIEND_ORDERED_FIELD_OPERATORS_HPP
#define GEOMETRIX_FRIEND_ORDERED_FIELD_OPERATORS_HPP

#include <boost/config.hpp>

//! Comparison
#define GEOMETRIX_FRIEND_LESS_THAN_COMPARABLE_OPERATORS(T, U)                                        \
BOOST_CONSTEXPR friend bool operator<=(const T& x, const U& y) { return !static_cast<bool>(x > y); } \
BOOST_CONSTEXPR friend bool operator>=(const T& x, const U& y) { return !static_cast<bool>(x < y); } \
BOOST_CONSTEXPR friend bool operator>(const U& x, const T& y)  { return y < x; }                     \
BOOST_CONSTEXPR friend bool operator<(const U& x, const T& y)  { return y > x; }                     \
BOOST_CONSTEXPR friend bool operator<=(const U& x, const T& y) { return !static_cast<bool>(y < x); } \
BOOST_CONSTEXPR friend bool operator>=(const U& x, const T& y) { return !static_cast<bool>(y > x); } \
/***/

#define GEOMETRIX_FRIEND_LESS_THAN_COMPARABLE_SELF_OPERATORS(T)                                      \
BOOST_CONSTEXPR friend bool operator>(const T& x, const T& y)  { return y < x; }                     \
BOOST_CONSTEXPR friend bool operator<=(const T& x, const T& y) { return !static_cast<bool>(y < x); } \
BOOST_CONSTEXPR friend bool operator>=(const T& x, const T& y) { return !static_cast<bool>(x < y); } \
/***/

#define GEOMETRIX_FRIEND_EQUALITY_COMPARABLE_OPERATORS(T, U)                                         \
BOOST_CONSTEXPR friend bool operator==(const U& y, const T& x) { return x == y; }                    \
BOOST_CONSTEXPR friend bool operator!=(const U& y, const T& x) { return !static_cast<bool>(x == y); }\
BOOST_CONSTEXPR friend bool operator!=(const T& y, const U& x) { return !static_cast<bool>(y == x); }\
/***/

#define GEOMETRIX_FRIEND_EQUALITY_COMPARABLE_SELF_OPERATORS(T)           \
    BOOST_CONSTEXPR friend bool operator!=(const T& x, const T& y)       \
    { return !static_cast<bool>(x == y); }                               \
/***/

#define GEOMETRIX_FRIEND_INCREMENTABLE_OPERATOR(T)                       \
    BOOST_CONSTEXPR friend T operator++(T& x, int)                       \
    {                                                                    \
        T nrv(x);                                                        \
        ++x;                                                             \
        return nrv;                                                      \
    }                                                                    \
/***/
#define GEOMETRIX_FRIEND_DECREMENTABLE_OPERATOR(T)                       \
    BOOST_CONSTEXPR friend T operator--(T& x, int)                       \
    {                                                                    \
        T nrv(x);                                                        \
        --x;                                                             \
        return nrv;                                                      \
    }                                                                    \
/***/

//!Arithmetic
#define GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE( T, U, OP )                         \
    BOOST_CONSTEXPR friend T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }\
    BOOST_CONSTEXPR friend T operator OP( const U& lhs, T rhs ) { return rhs OP##= lhs; }\
/***/

#define GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE_SELF( T, OP )                       \
    BOOST_CONSTEXPR friend T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }\
/***/

#define GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE( T, U, OP )                     \
    BOOST_CONSTEXPR friend T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }\
    BOOST_CONSTEXPR friend T operator OP( const U& lhs, const T& rhs )                   \
    { return T( lhs ) OP##= rhs; }                                                       \
/***/

#define GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE_SELF( T, OP )                   \
    BOOST_CONSTEXPR friend T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }\
/***/

#define GEOMETRIX_FRIEND_IMPLEMENT_BINARY_ARITHMETIC_OPERATORS(T, U)            \
GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE( T, U, * )                         \
GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE( T, U, + )                         \
GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE( T, U, - )                     \
GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE(T, U, / )                      \
/***/

#define GEOMETRIX_FRIEND_IMPLEMENT_BINARY_ARITHMETIC_OPERATORS_SELF(T)          \
GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE_SELF( T, * )                       \
GEOMETRIX_FRIEND_BINARY_OPERATOR_COMMUTATIVE_SELF( T, + )                       \
GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE_SELF( T, - )                   \
GEOMETRIX_FRIEND_BINARY_OPERATOR_NON_COMMUTATIVE_SELF( T, / )                   \
/***/

#define GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(T, U)     \
GEOMETRIX_FRIEND_LESS_THAN_COMPARABLE_OPERATORS(T, U)                           \
GEOMETRIX_FRIEND_EQUALITY_COMPARABLE_OPERATORS(T, U)                            \
/***/

#define GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_OPERATORS(T, U)                \
GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS( T, U )           \
GEOMETRIX_FRIEND_IMPLEMENT_BINARY_ARITHMETIC_OPERATORS( T, U )                  \
/***/

#define GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS_SELF(T)   \
GEOMETRIX_FRIEND_LESS_THAN_COMPARABLE_SELF_OPERATORS(T)                         \
GEOMETRIX_FRIEND_EQUALITY_COMPARABLE_SELF_OPERATORS(T)                          \
/***/

#define GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_OPERATORS_SELF(T)              \
GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS_SELF(T)           \
GEOMETRIX_FRIEND_IMPLEMENT_BINARY_ARITHMETIC_OPERATORS_SELF(T)                  \
/***/

#endif//! GEOMETRIX_FRIEND_ORDERED_FIELD_OPERATORS_HPP
