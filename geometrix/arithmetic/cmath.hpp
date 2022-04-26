//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CMATH_HPP
#define GEOMETRIX_CMATH_HPP

#include <cmath>
#include <utility>

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/function_traits.hpp>
#include <geometrix/utility/assert.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/utility/result_of.hpp>

#define GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION( F )             \
namespace geometrix {                                          \
    using std::F;                                              \
    template <typename T, typename EnableIf=void>              \
    struct BOOST_PP_CAT(F,_function_impl)                      \
    {                                                          \
        using result_type = decltype(F(std::declval<T>()));    \
        result_type operator()( T a ) const                    \
        {                                                      \
            return F(a);                                       \
        }                                                      \
    };                                                         \
    struct BOOST_PP_CAT(F,_function)                           \
    {                                                          \
        template<typename Sig>                                 \
        struct result {};                                      \
        template <typename This, typename T>                   \
        struct result<This(T)>                                 \
        {                                                      \
            using arg_type = typename                          \
                             geometrix::type_at<T>::type;      \
            using type =                                       \
             typename BOOST_PP_CAT(F,_function_impl)<arg_type> \
                ::result_type;                                 \
        };                                                     \
        template <typename T>                                  \
        typename result<BOOST_PP_CAT(F,_function)(T)>::type    \
        operator()(const T& a) const                           \
        {                                                      \
            using arg_type = typename                          \
                             geometrix::type_at<T>::type;      \
            return BOOST_PP_CAT(F,_function_impl)<arg_type>()  \
                   (geometrix::get(a));                        \
        }                                                      \
    };                                                         \
    template <typename T>                                      \
    inline                                                     \
    decltype(BOOST_PP_CAT(F,_function)()(std::declval<T>()))   \
    F(const T& v) { return BOOST_PP_CAT(F,_function)()(v); }   \
    template <>                                                \
    struct function_traits                                     \
    <                                                          \
        geometrix::BOOST_PP_CAT(F,_function)                   \
    >                                                          \
    {                                                          \
        typedef void is_unary_function;                        \
    };                                                         \
}                                                              \
/***/

#define GEOMETRIX_DECLARE_MATH_BINARY_FUNCTION( F )            \
namespace geometrix {                                          \
    using std::F;                                              \
    template <typename T1, typename T2,                        \
              typename EnableIf = void>                        \
    struct BOOST_PP_CAT(F,_function_impl)                      \
    {                                                          \
        using result_type =                                    \
         decltype(F(std::declval<T1>(),std::declval<T2>()));   \
        result_type operator()(const T1& a,const T2& b) const  \
        {                                                      \
            return F(a,b);                                     \
        }                                                      \
    };                                                         \
    template <typename T1, typename T2>                        \
    struct BOOST_PP_CAT(F,_function_impl)                      \
        <                                                      \
            T1                                                 \
          , T2                                                 \
          , typename std::enable_if                            \
            <                                                  \
                std::is_arithmetic                             \
                <                                              \
                    typename remove_const_ref<T2>::type        \
                >::value                                       \
             && std::is_arithmetic                             \
                <                                              \
                    typename remove_const_ref<T1>::type        \
                >::value                                       \
            >::type                                            \
        >                                                      \
    {                                                          \
        struct result                                          \
        {                                                      \
            using arithmetic_type = typename                   \
                select_arithmetic_type_from_2<T1,T2>::type;    \
            using type =                                       \
                decltype(F                                     \
                (std::declval<arithmetic_type>()               \
                    , std::declval<arithmetic_type>()));       \
        };                                                     \
        typedef typename result::type result_type;             \
        result_type                                            \
            operator() (const T1& a, const T2& b) const        \
        {                                                      \
            using type = typename                              \
                select_arithmetic_type_from_2<T1,T2>::type;    \
            return F (type(a), type(b));                       \
        }                                                      \
    };                                                         \
    struct BOOST_PP_CAT(F,_function)                           \
    {                                                          \
        template<typename Sig>                                 \
        struct result {};                                      \
        template <typename This, typename T1, typename T2>     \
        struct result<This(T1,T2)>                             \
        {                                                      \
            using t1_type = typename                           \
                             geometrix::type_at<T1>::type;     \
            using t2_type = typename                           \
                             geometrix::type_at<T2>::type;     \
            using type =                                       \
              typename BOOST_PP_CAT(F,_function_impl)          \
                <t1_type, t2_type>::result_type;               \
        };                                                     \
        template <typename T1, typename T2>                    \
        typename result<BOOST_PP_CAT(F,_function)(T1,T2)>::type\
        operator()(const T1& a, const T2& b) const             \
        {                                                      \
            using t1_type = typename                           \
                             geometrix::type_at<T1>::type;     \
            using t2_type = typename                           \
                             geometrix::type_at<T2>::type;     \
            return BOOST_PP_CAT(F,_function_impl)              \
                   <t1_type, t2_type>()                        \
                   (geometrix::get(a), geometrix::get(b));     \
        }                                                      \
    };                                                         \
    template <typename T1, typename T2,                        \
      typename std::enable_if                                  \
      <                                                        \
        !std::is_arithmetic                                    \
         <                                                     \
             typename remove_const_ref<T2>::type               \
         >::value                                              \
     && !std::is_arithmetic                                    \
         <                                                     \
             typename remove_const_ref<T1>::type               \
         >::value                                              \
      >::type >                                                \
    inline                                                     \
    decltype                                                   \
    (                                                          \
        BOOST_PP_CAT(F,_function)()                            \
        (                                                      \
            std::declval<T1>()                                 \
          , std::declval<T2>()                                 \
        )                                                      \
    )                                                          \
    F(const T1& v1, const T2& v2)                              \
    { return BOOST_PP_CAT(F,_function)()(v1, v2); }            \
    template <>                                                \
    struct function_traits                                     \
    <                                                          \
        geometrix::BOOST_PP_CAT(F,_function)                   \
    >                                                          \
    {                                                          \
        typedef void is_binary_function;                       \
    };                                                         \
}                                                              \
/***/

GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(abs);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(sqrt);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(sin);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(asin);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(cos);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(acos);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(tan);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(atan);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(floor);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(ceil);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(exp);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(log);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(log10);

GEOMETRIX_DECLARE_MATH_BINARY_FUNCTION(pow);
GEOMETRIX_DECLARE_MATH_BINARY_FUNCTION(atan2);

#endif //GEOMETRIX_CMATH_HPP
