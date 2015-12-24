//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_TYPE_TRAITS_HPP
#define GEOMETRIX_UTILITY_TYPE_TRAITS_HPP

#include <geometrix/utility/assert.hpp>
#include <boost/type_traits.hpp>

namespace geometrix {
		
	template <typename ...T>
	void ignore_unused_warning_of(){}

	template <typename ...T>
	void ignore_unused_warning_of(T const&...){}

	//! \brief remove const and reference qualifiers from a type.
	template <typename T, typename EnableIf=void>
	struct remove_const_ref
	{
		typedef typename boost::remove_const< typename boost::remove_reference< T >::type >::type type;
	};

	//! \brief add const and reference qualifiers to a type.
	template <typename T, typename EnableIf=void>
	struct add_const_ref
	{
		typedef typename boost::add_const< typename boost::add_reference< T >::type >::type type;
	};

}//namespace geometrix;

#endif//GEOMETRIX_UTILITY_TYPE_TRAITS_HPP
