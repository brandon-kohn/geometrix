//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef GEOMETRIX_MEMOIZE_HPP
#define GEOMETRIX_MEMOIZE_HPP
#pragma once

#include <tuple>
#include <functional>

namespace geometrix
{
	namespace detail
	{
		template <typename T>
		struct function_traits
			: public function_traits < decltype(&T::operator()) >
		{};

		template <typename T, typename Return, typename... Args>
		struct function_traits < Return( T::* )(Args...) const >
		{
			enum { arity = sizeof...(Args) };

			typedef Return result_type;

			template <size_t i>
			struct arg
			{
				typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
			};

			typedef std::function<Return( Args... )> std_function_type;
		};
		
		template<typename Return, typename Cache, typename ...Args>
		inline std::function<Return( Args... )> memoize_helper( Cache& cache, std::function<Return( Args... )> fn )
		{
			return [&cache, fn]( Args... a )
			{
				auto key = std::make_tuple( a... );
				auto it = cache.lower_bound( key );
				if( it != cache.end() && !cache.key_comp()(it->first, key) )
					return it->second;
				Return ret = fn( a... );
				cache.insert( it, std::make_pair( key, ret ) );
				return ret;
			};
		}
	}//! namespace detail;

	template<typename Cache, typename Lambda>
	inline auto memoize(Cache& cache, Lambda fn) -> typename detail::function_traits<decltype(fn)>::std_function_type
	{
		typedef typename detail::function_traits<decltype(fn)>::std_function_type result_type;
		return detail::memoize_helper( cache, result_type( fn ) );
	}

}//! namespace geometrix;

#endif // GEOMETRIX_MEMOIZE_HPP
