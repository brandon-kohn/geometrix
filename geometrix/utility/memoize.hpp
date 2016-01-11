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
		template <typename Fn>
		struct function_deducer{};

		template <typename T>
		struct std_function_type_generator : std_function_type_generator < decltype(&T::operator()) >{};

		template <typename T, typename Return, typename... Args>
		struct std_function_type_generator < Return( T::* )(Args...) const >
		{
			typedef std::function<Return( Args... )> type;
			typedef function_deducer<Return( Args... )> deducer_type;
		};
		
		template<typename Return, typename Cache, typename Lambda, typename ...Args>
		inline std::function<Return( Args... )> memoize_helper( Cache& cache, Lambda fn, function_deducer<Return( Args... )> )
		{
			return [&cache, fn]( Args... a )
			{
				auto key = std::make_tuple( a... );
				auto it = cache.lower_bound( key );
				if( it != cache.end() && !cache.key_comp()(key, it->first) )
					return it->second;
				Return ret = fn( a... );
				cache.insert( it, std::make_pair( key, ret ) );
				return ret;
			};
		}
	}//! namespace detail;

	template<typename Cache, typename Fn>
	inline auto memoize(Cache& cache, Fn fn) -> typename detail::std_function_type_generator<decltype(fn)>::type
	{
		typedef typename detail::std_function_type_generator<decltype(fn)>::deducer_type deducer;
		return detail::memoize_helper( cache, fn, deducer() );
	}

}//! namespace geometrix;

#endif // GEOMETRIX_MEMOIZE_HPP
