//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_GEOMETRIC_TRAITS_HPP
#define GEOMETRIX_GEOMETRIC_TRAITS_HPP

#include <geometrix/utility/type_traits.hpp>
#include <geometrix/utility/static_assert.hpp>
#include <geometrix/utility/assert.hpp>
#include <geometrix/utility/ignore_unused_warnings.hpp>

namespace geometrix {

    //! Default geometric traits struct. 
    //! NOTE: must be specialized for user types.
    template <typename T, typename EnableIf=void>
    struct geometric_traits
    {};

	//! Specialization for pulling arithmetic types out of geometric_traits.
	template <typename T, typename EnableIf=void>
	struct arithmetic_type_of
	{
		using type = typename geometric_traits<T>::arithmetic_type;
	};

    //! \brief Tag to check if a type is a geometric.
    template <typename NumericType, typename Enable = void>
    struct is_geometric : boost::false_type{};    
    
    template <typename T, typename EnableIf=void>
    struct is_heterogeneous : boost::mpl::false_ {};

    template <typename T>
    struct is_heterogeneous<T, typename geometric_traits<typename remove_const_ref<T>::type>::is_heterogeneous> : boost::mpl::true_ {};

    template <typename T, typename EnableIf=void>
    struct is_homogeneous : boost::mpl::false_ {};

    template <typename T>
    struct is_homogeneous<T, typename geometric_traits<typename remove_const_ref<T>::type>::is_homogeneous> : boost::mpl::true_ {};

    template <typename Arg, typename IsHomogeneous=void>
    struct diversity_base
    {
        typedef void is_heterogeneous;
    };

    template <typename Arg>
    struct diversity_base
        <
            Arg
          , typename geometric_traits<typename remove_const_ref<Arg>::type>::is_homogeneous
        >
    {
		using arithmetic_type = typename geometric_traits<typename remove_const_ref<Arg>::type>::arithmetic_type;
		using dimensionless_type = typename geometric_traits<typename remove_const_ref<Arg>::type>::dimensionless_type;//!FIXME
        typedef void is_homogeneous;
    };

#define GEOMETRIX_TYPE_DIVERSITY_TAG( Types )         \
    BOOST_PP_IF                                       \
    (                                                 \
        BOOST_PP_SUB( BOOST_PP_SEQ_SIZE( Types ), 1 ) \
      , is_heterogeneous                              \
      , is_homogeneous                                \
    )                                                 \
/***/

}//namespace geometrix;

#endif//GEOMETRIX_GEOMETRIC_TRAITS_HPP
