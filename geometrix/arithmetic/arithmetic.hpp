//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATH_FUNCTIONS_HPP
#define GEOMETRIX_MATH_FUNCTIONS_HPP

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/function_traits.hpp>
#include <geometrix/utility/assert.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/units/cmath.hpp>

#include <cmath>
#include <utility>

namespace geometrix {

	namespace result_of
	{
		template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
		struct multiplies;

		template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
		struct plus;

		template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
		struct minus;

		template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
		struct divides;
	}

	template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
	struct multiplies;

	template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
	struct plus;

	template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
	struct minus;

	template <typename LHS, typename RHS, typename LeftType = void, typename RightType = void>
	struct divides;

	namespace math
	{
		//! Compile time calculation of N^P (integral).
		template< int N, unsigned int P >
		struct power_c
		{
			static const boost::int64_t value = N * power_c<N, P - 1>::value;
		};

		template < int N >
		struct power_c<N, 0>
		{
			static const boost::int64_t value = 1;
		};

		template <typename T>
		inline T integral_pow(T radix, T exponent)
		{
			if (exponent < T(0))
				return T(0);

			T p(1);
			while (true)
			{
				if (exponent == T(0))
					break;

				p *= radix;
				--exponent;
			}

			return p;
		}

	}//namespace math;
}//namespace geometrix;

#define GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION( F )            \
namespace geometrix { namespace math {                        \
    template <typename Arg, typename EnableIf=void>           \
    struct BOOST_PP_CAT(F,_function_impl)                     \
    {                                                         \
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL                       \
        (                                                     \
            result                                            \
          , ::std::F(Arg())                                   \
        );                                                    \
        typedef typename result::type result_type;            \
        result_type operator()( Arg a ) const                 \
        {                                                     \
            return ::std::F(a);                               \
        }                                                     \
    };                                                        \
    template <typename Unit, typename Y>                      \
    struct BOOST_PP_CAT(F,_function_impl)                     \
    <boost::units::quantity<Unit,Y>>                          \
    {                                                         \
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL                       \
        (                                                     \
            result                                            \
          , boost::units::F(boost::units::quantity<Unit,Y>()) \
        );                                                    \
        typedef typename result::type result_type;            \
        result_type operator()                                \
        ( const boost::units::quantity<Unit,Y>& a ) const     \
        {                                                     \
            return boost::units::F(a);                        \
        }                                                     \
    };                                                        \
    struct BOOST_PP_CAT(F,_function)                          \
    {                                                         \
		template<typename Sig>                                \
		struct result {};                                     \
		template <typename This, typename Arg>                \
		struct result<This(Arg)>                              \
		{                                                     \
            using arg_type = typename                         \
                             geometrix::type_at<Arg>::type;   \
			using type =                                      \
             typename BOOST_PP_CAT(F,_function_impl)<arg_type>\
                ::result_type;                                \
		};                                                    \
		template <typename Arg>                               \
		typename result<BOOST_PP_CAT(F,_function)(Arg)>::type \
        operator()(const Arg& a) const                        \
		{                                                     \
            using arg_type = typename                         \
                             geometrix::type_at<Arg>::type;   \
			return BOOST_PP_CAT(F,_function_impl)<arg_type>() \
                   (geometrix::get(a));                       \
		}                                                     \
	};                                                        \
    template <typename T>                                     \
    inline                                                    \
    decltype(BOOST_PP_CAT(F,_function)()(std::declval<T>()))  \
    F(const T& v) { return BOOST_PP_CAT(F,_function)()(v); }  \
    }                                                         \
    template <>                                               \
    struct function_traits                                    \
    <                                                         \
        geometrix::math::BOOST_PP_CAT(F,_function)            \
    >                                                         \
    {                                                         \
        typedef void is_unary_function;                       \
    };                                                        \
}                                                             \
/***/ 

#define GEOMETRIX_DECLARE_MATH_BINARY_FUNCTION( F )           \
namespace geometrix { namespace math {                        \
    template <typename Arg1, typename Arg2,                   \
              typename EnableIf = void>                       \
    struct BOOST_PP_CAT(F,_function_impl)                     \
    {                                                         \
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL                       \
        (                                                     \
            result                                            \
          , ::std::F(Arg1(),Arg2())                           \
        );                                                    \
        typedef typename result::type result_type;            \
        result_type operator()( Arg1 a, Arg2 b ) const        \
        {                                                     \
            return ::std::F(a,b);                             \
        }                                                     \
    };                                                        \
    template <typename Unit1, typename Y1, typename Unit2,    \
        typename Y2>                                          \
    struct BOOST_PP_CAT(F,_function_impl)                     \
    <boost::units::quantity<Unit1, Y1>,                       \
     boost::units::quantity<Unit2, Y2>>                       \
    {                                                         \
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL                       \
        (                                                     \
            result                                            \
          , boost::units::F(                                  \
                boost::units::quantity<Unit1,Y1>()            \
              , boost::units::quantity<Unit2,Y2>())           \
        );                                                    \
        typedef typename result::type result_type;            \
        result_type operator()                                \
        ( const boost::units::quantity<Unit1,Y1>& a           \
         , const boost::units::quantity<Unit2,Y2>& b ) const  \
        {                                                     \
            return boost::units::F(a, b);                     \
        }                                                     \
    };                                                        \
    struct BOOST_PP_CAT(F,_function)                          \
    {                                                         \
		template<typename Sig>                                \
		struct result {};                                     \
		template <typename This, typename T1, typename T2>    \
		struct result<This(T1,T2)>                            \
		{                                                     \
            using t1_type = typename                          \
                             geometrix::type_at<T1>::type;    \
            using t2_type = typename                          \
                             geometrix::type_at<T2>::type;    \
			using type =                                      \
              typename BOOST_PP_CAT(F,_function_impl)         \
                <t1_type, t2_type>::result_type;              \
		};                                                    \
		template <typename T1, typename T2>                   \
		typename result<BOOST_PP_CAT(F,_function)(T1,T2)>::type\
        operator()(const T1& a, const T2& b) const            \
		{                                                     \
            using t1_type = typename                          \
                             geometrix::type_at<T1>::type;    \
            using t2_type = typename                          \
                             geometrix::type_at<T2>::type;    \
			return BOOST_PP_CAT(F,_function_impl)             \
                   <t1_type, t2_type>()                       \
                   (geometrix::get(a), geometrix::get(b));    \
		}                                                     \
	};                                                        \
    template <typename T1, typename T2>                       \
    inline                                                    \
    decltype                                                  \
    (                                                         \
        BOOST_PP_CAT(F,_function)()                           \
        (                                                     \
            std::declval<T1>()                                \
          , std::declval<T2>()                                \
        )                                                     \
    )                                                         \
    F(T1 v1, T2 v2)                                           \
    { return BOOST_PP_CAT(F,_function)()(v1, v2); }           \
    }                                                         \
    template <>                                               \
    struct function_traits                                    \
    <                                                         \
        geometrix::math::BOOST_PP_CAT(F,_function)            \
    >                                                         \
    {                                                         \
        typedef void is_binary_function;                      \
    };                                                        \
}                                                             \
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
//GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(exp);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(log);
GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(log10);

GEOMETRIX_DECLARE_MATH_BINARY_FUNCTION(pow);
GEOMETRIX_DECLARE_MATH_BINARY_FUNCTION(atan2);

#pragma message(BOOST_PP_STRINGIZE((GEOMETRIX_DECLARE_MATH_UNARY_FUNCTION(exp))))
namespace geometrix { namespace math { 
	template <typename Arg, typename EnableIf = void> 
	struct exp_function_impl 
	{
		struct result 
		{ 
			static const int _typeof_register_value = sizeof(boost::type_of::typeof_register_type<result>(::std::exp(Arg()))); 
			typedef typename boost::type_of::msvc_extract_type<result>::id2type id2type; 
			typedef typename id2type::type type; 
		};; 
		typedef typename result::type result_type; 
		result_type operator()(Arg a) const { return ::std::exp(a); } 
	}; 
	template <typename Unit, typename Y>
	struct exp_function_impl <boost::units::quantity<Unit, Y>> 
	{
		struct result 
		{ 
			static const int _typeof_register_value = sizeof(boost::type_of::typeof_register_type<result>(boost::units::exp(boost::units::quantity<Unit, Y>()))); 
			typedef typename boost::type_of::msvc_extract_type<result>::id2type id2type; 
			typedef typename id2type::type type; 
		};; 
		typedef typename result::type result_type; 
		result_type operator() (boost::units::quantity<Unit, Y> a) const { return boost::units::exp(a); } 
	};
	struct exp_function 
	{
		template<typename Sig> 
		struct result {}; 
		template <typename This, typename Arg>
		struct result<This(Arg)>
		{
			using arg_type = typename geometrix::type_at<Arg>::type; 
			using type = typename exp_function_impl<arg_type> ::result_type; 
		};
		template <typename Arg> 
		typename result<exp_function(Arg)>::type operator()(Arg a) const 
		{
			using arg_type = typename geometrix::type_at<Arg>::type; 
			return exp_function_impl<arg_type>() (geometrix::get(a)); 
		}
	};
	template <typename T> inline decltype(exp_function()(std::declval<T>())) exp(T v) 
	{
		return exp_function()(v); 
	}
}
template <> struct function_traits < geometrix::math::exp_function > { typedef void is_unary_function; }; 
}

#endif //GEOMETRIX_MATH_FUNCTIONS_HPP
