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

namespace geometrix {

    //! Default gemetric traits struct. 
    //! NOTE: must be specialized for user types.
    template <typename T, typename EnableIf=void>
    struct geometric_traits
    {};

    //! \brief Tag to check if a type is a gemetric.
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
    struct uniformity_base
    {
        typedef void is_heterogeneous;
    };

    template <typename Arg>
    struct uniformity_base
        <
            Arg
          , typename geometric_traits<typename remove_const_ref<Arg>::type>::is_homogeneous
        >
    {
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
