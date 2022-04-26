//
//! Copyright © 2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TAGGED_QUANTITY_CMATH_HPP
#define GEOMETRIX_TAGGED_QUANTITY_CMATH_HPP

#include <geometrix/arithmetic/cmath.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>

#define GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION( F )     \
template <typename Tag, typename Number>                  \
struct BOOST_PP_CAT(F,_function_impl)                     \
<tagged_quantity<Tag, Number>>                            \
{                                                         \
    using result_type =                                   \
     decltype(F(std::declval<Number>()));                 \
    result_type operator()                                \
    (const tagged_quantity<Tag, Number>& a) const         \
    {                                                     \
        return F(a.value());                              \
    }                                                     \
};                                                        \
template <typename Tag, typename Number>                  \
struct BOOST_PP_CAT(F,_function_impl)                     \
<const tagged_quantity<Tag, Number>>                      \
{                                                         \
    using result_type =                                   \
     decltype(F(std::declval<Number>()));                 \
    result_type operator()                                \
    (const tagged_quantity<Tag, Number>& a) const         \
    {                                                     \
        return F(a.value());                              \
    }                                                     \
};                                                        \
template <typename T>                                     \
struct F ## _op;                                          \
template <typename Tag, typename X>                       \
inline                                                    \
tagged_quantity                                           \
<                                                         \
    F ## _op<Tag>                                         \
  , decltype(F(std::declval<X>()))                        \
>                                                         \
F(const tagged_quantity<Tag, X>& a)                       \
{                                                         \
    using type = tagged_quantity                          \
    <                                                     \
        F ## _op<Tag>                                     \
      , decltype(F(std::declval<X>()))                    \
    >;                                                    \
    return type(F(a.value()));                            \
}                                                         \
/***/

#define GEOMETRIX_TAGGED_QUANTITY_BINARY_FUNCTION( F )    \
template <typename Tag1, typename T1,                     \
          typename Tag2, typename T2>                     \
struct BOOST_PP_CAT(F,_function_impl)                     \
<tagged_quantity<Tag1, T1>, tagged_quantity<Tag2, T2>>    \
{                                                         \
    using result_type =                                   \
     decltype(F(std::declval<T1>(),std::declval<T2>()));  \
    result_type operator()                                \
    (const tagged_quantity<Tag1, T1>& a                   \
    ,const tagged_quantity<Tag2, T2>& b) const            \
    {                                                     \
        return F(a.value(),b.value());                    \
    }                                                     \
};                                                        \
template <typename Tag1, typename T1,                     \
          typename T2>                                    \
struct BOOST_PP_CAT(F,_function_impl)                     \
<tagged_quantity<Tag1, T1>, T2>                           \
{                                                         \
    using result_type =                                   \
     decltype(F(std::declval<T1>(),std::declval<T2>()));  \
    result_type operator()                                \
    (const tagged_quantity<Tag1, T1>& a                   \
    ,const T2& b) const                                   \
    {                                                     \
        return F(a.value(),b);                            \
    }                                                     \
};                                                        \
template <typename T1,                                    \
          typename Tag2, typename T2>                     \
struct BOOST_PP_CAT(F,_function_impl)                     \
<T1, tagged_quantity<Tag2, T2>>                           \
{                                                         \
    using result_type =                                   \
     decltype(F(std::declval<T1>(),std::declval<T2>()));  \
    result_type operator()                                \
    (const T1& a                                          \
    ,const tagged_quantity<Tag2, T2>& b) const            \
    {                                                     \
        return F(a,b.value());                            \
    }                                                     \
};                                                        \
template <typename T1, typename T2> struct F ## _op;      \
template                                                  \
<                                                         \
    typename Tag1                                         \
  , typename X                                            \
  , typename Tag2                                         \
  , typename Y                                            \
>                                                         \
inline                                                    \
tagged_quantity                                           \
<                                                         \
    F ## _op<Tag1, Tag2>                                  \
  , decltype(F(std::declval<X>(),std::declval<Y>()))\
>                                                         \
F( const tagged_quantity<Tag1, X>& lhs                    \
 , const tagged_quantity<Tag2, Y>& rhs)                   \
{                                                         \
    using type = tagged_quantity                          \
    <                                                     \
        F ## _op<Tag1, Tag2>                              \
      , decltype(F(std::declval<X>()                      \
        , std::declval<Y>()))                             \
    >;                                                    \
    return type( F(lhs.value(),rhs.value()) );            \
}                                                         \
template                                                  \
<                                                         \
    typename Tag                                          \
  , typename X                                            \
  , typename Y                                            \
>                                                         \
inline                                                    \
tagged_quantity                                           \
<                                                         \
    F ## _op<Tag, Y>                                      \
  , decltype(F(std::declval<X>(),std::declval<Y>()))      \
>                                                         \
F( const tagged_quantity<Tag, X>& lhs                     \
 , const Y& rhs)                                          \
{                                                         \
    using type = tagged_quantity                          \
    <                                                     \
        F ## _op<Tag, Y>                                  \
      , decltype(F(std::declval<X>()                      \
        , std::declval<Y>()))                             \
    >;                                                    \
    return type(F(lhs.value(),rhs));                      \
}                                                         \
template                                                  \
<                                                         \
    typename X                                            \
  , typename Tag                                          \
  , typename Y                                            \
>                                                         \
inline                                                    \
tagged_quantity                                           \
<                                                         \
    F ## _op<X, Tag>                                      \
  , decltype(F(std::declval<X>(),std::declval<Y>()))      \
>                                                         \
F( const X& lhs                                           \
 , const tagged_quantity<Tag, Y>& rhs)                    \
{                                                         \
    using type = tagged_quantity                          \
    <                                                     \
        F ## _op<X, Tag>                                  \
      , decltype(F(std::declval<X>()                      \
        , std::declval<Y>()))                             \
    >;                                                    \
    return type(F(lhs,rhs.value()));                      \
}                                                         \
/***/ 

GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(abs);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(sqrt);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(sin);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(asin);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(cos);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(acos);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(tan);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(atan);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(floor);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(ceil);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(exp);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(log);
GEOMETRIX_TAGGED_QUANTITY_UNARY_FUNCTION(log10);

GEOMETRIX_TAGGED_QUANTITY_BINARY_FUNCTION(pow);
GEOMETRIX_TAGGED_QUANTITY_BINARY_FUNCTION(atan2);

#endif //!GEOMETRIX_TAGGED_QUANTITY_CMATH_HPP
