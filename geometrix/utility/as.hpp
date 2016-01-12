//
//! Copyright © 2008-2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_AS_HPP
#define GEOMETRIX_AS_HPP

#include <boost/utility.hpp>
#include <boost/type_traits.hpp>
#include <utility>

namespace geometrix {

	template <typename To, typename From, typename EnableIf=void>
	struct as_converter;
// 	{		
// 		typedef To result_type;
// 		template <typename Arg>
// 		To operator()( Arg&& from ) const{ return To( from ); }
// 	};

    template <typename T, typename Expr>
	inline T as(Expr&& expr)
	{
		return as_converter<T, typename remove_const_ref<Expr>::type>()( std::forward<Expr>(expr) );
	};

}//namespace geometrix;

#endif//! GEOMETRIX_AS_HPP
