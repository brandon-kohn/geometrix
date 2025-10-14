//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_CONCEPT_HPP
#define GEOMETRIX_UTILITY_CONCEPT_HPP

#include <boost/type_traits.hpp>
#include <boost/concept_check.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <type_traits>
#ifdef __cpp_concepts
#include <concepts>
#endif
namespace geometrix {

	template <typename Sequence, typename Predicate>
	struct all_true 
		: boost::mpl::if_c
		  <
		    boost::mpl::count_if
		    <
		      Sequence
		    , Predicate
			>::type::value == boost::mpl::size<Sequence>::value
		  , std::true_type
		  , std::false_type
		  >::type
	{};

	template <typename Sequence, typename Predicate>
	struct all_false
		: boost::mpl::if_c
		  <
		    boost::mpl::count_if
		    <
		      Sequence
		    , Predicate
			>::type::value == 0
		  , std::true_type
		  , std::false_type
		  >::type
	{};

}//namespace geometrix;

#endif//GEOMETRIX_UTILITY_CONCEPT_HPP
