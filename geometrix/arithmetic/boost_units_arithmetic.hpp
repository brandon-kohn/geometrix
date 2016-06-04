//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BOOST_UNITS_ARITHMETIC_HPP
#define GEOMETRIX_BOOST_UNITS_ARITHMETIC_HPP

#include <geometrix/arithmetic/arithmetic.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/units/cmath.hpp>

//#pragma message(BOOST_PP_STRINGIZE((GEOMETRIX_DECLARE_MATH_BINARY_FUNCTION(pow))))
namespace geometrix {
	namespace math {
		namespace detail {
			template <typename T, typename EnableIf = void>
			struct is_boost_unit_helper : std::false_type
			{};

			template <typename Unit, typename T>
			struct is_boost_unit_helper<boost::units::quantity<Unit, T>> : std::true_type
			{};

			template <typename T>
			struct is_boost_unit : is_boost_unit_helper<typename remove_const_ref<T>::type>
			{};
		}//! detail

		template <typename T1, typename T2>
		struct pow_function_impl <T1, T2, typename std::enable_if<detail::is_boost_unit<T1>::value && detail::is_boost_unit<T2>::value>::type>
		{
			struct result
			{
				using type = decltype(pow(std::declval<T1>().value(), std::declval<T2>().value()));
			};;
			typedef typename result::type result_type; result_type operator() (const T1& a, const T2& b) const
			{
				using std::pow;
				return pow(a.value(), b.value());
			}
		};

		//! Convert fundamental types into dimensionless quantities if opposite a boost unit.
		template <typename T1, typename T2>
		struct pow_function_impl<T1, T2, typename std::enable_if<detail::is_boost_unit<T1>::value && std::is_arithmetic<T2>::value>::type>
		{
			struct result
			{
				using type = decltype(pow(std::declval<T1>().value(), std::declval<T2>()));
			};;
			typedef typename result::type result_type; result_type operator() (const T1& a, const T2& b) const
			{
				using std::pow;
				return pow(a.value(), b);
			}
		};
		template <typename T1, typename T2>
		struct pow_function_impl<T1, T2, typename std::enable_if<detail::is_boost_unit<T2>::value && std::is_arithmetic<T1>::value>::type>
		{
			struct result
			{
				using type = decltype(pow(std::declval<T1>(), std::declval<T2>().value()));
			};;
			typedef typename result::type result_type; result_type operator() (const T1& a, const T2& b) const
			{
				using std::pow;
				return pow(a, b.value());
			}
		};
	}
}//! namespace geometrix::math;

namespace boost { namespace units {

		template<typename S, typename Number, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		inline boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), decltype(std::declval<Number>() + std::declval<T>())>
			operator +(const boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), Number>& a, const T& b)
		{
			using result = decltype(std::declval<Number>() + std::declval<T>());
			using quantity_type = boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), result>;
			return quantity_type::from_value(a.value() + b);
		}

		template<typename S, typename Number, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		inline boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), decltype(std::declval<T>() + std::declval<Number>())>
			operator +(const T& a, const boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), Number>& b)
		{
			using result = decltype(std::declval<T>() + std::declval<Number>());
			using quantity_type = boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), result>;
			return quantity_type::from_value(a + b.value());
		}

		template<typename S, typename Number, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		inline boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), decltype(std::declval<Number>() - std::declval<T>())>
			operator -(const boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), Number>& a, const T& b)
		{
			using result = decltype(std::declval<Number>() - std::declval<T>());
			using quantity_type = boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), result>;
			return quantity_type::from_value(a.value() - b);
		}

		template<typename S, typename Number, typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
		inline boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), decltype(std::declval<T>() - std::declval<Number>())>
			operator -(const T& a, const boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), Number>& b)
		{
			using result = decltype(std::declval<T>() - std::declval<Number>());
			using quantity_type = boost::units::quantity<BOOST_UNITS_DIMENSIONLESS_UNIT(S), result>;
			return quantity_type::from_value(a - b.value());
		}
	}//! namespace units;
}//! namespace boost;


#endif//! GEOMETRIX_BOOST_UNITS_ARITHMETIC_HPP
