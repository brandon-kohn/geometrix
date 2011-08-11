//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TENSOR_TRAITS_HPP
#define GEOMETRIX_TENSOR_TRAITS_HPP

#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/detail/tensor_access_defines.hpp>
#include <geometrix/utility/type_traits.hpp>
#include <geometrix/utility/preprocessor.hpp>

#include <boost/utility.hpp>

namespace geometrix {

//! A tag specifier to tell which kind of access, run-time or compile-time should be used when accessing the elements of an tensor.
template <typename T, typename EnableIf=void>
struct tensor_traits
{};

//! \brief Tag to check if a type is an tensor.
template <typename T, typename EnableIf = void>
struct is_tensor : boost::false_type{};

//! A concept check to ensure that access_policy have been defined for the specified type.
template <typename Tensor>
struct TensorConcept
{
    void constraints() const
    {
        BOOST_STATIC_ASSERT(( is_tensor<Tensor>::value ));
        typedef typename remove_const_ref<Tensor>::type            tensor_type;
        typedef typename tensor_traits<tensor_type>::access_policy access_policy;
        typedef typename tensor_traits<tensor_type>::tensor_order  tensor_order;
    }
};

template <typename T>
struct is_tensor
    <
        T
      , typename tensor_traits
        <
            typename remove_const_ref<T>::type
        >::is_tensor
    > 
    : boost::true_type
{};

template <typename T, typename EnableIf=void>
struct access_policy_of
{
    typedef typename tensor_traits
        <
            typename remove_const_ref<T>::type
        >::access_policy type;
};

template <typename T, typename EnableIf=void>
struct tensor_order_of
{
    typedef typename tensor_traits
        <
            typename remove_const_ref<T>::type
        >::tensor_order type;
};

}//namespace geometrix;

#define GEOMETRIX_TENSOR_TAG( Order )                                                  \
    BOOST_PP_IF                                                                        \
    (                                                                                  \
        BOOST_PP_LESS( Order, 3 )                                                      \
        , rank_##Order                                                                 \
        , BOOST_MPL_ASSERT_MSG((false), TENSOR_ORDER_TAG_MUST_BE_LESS_THAN_3,(Order)); \
    )                                                                                  \
/***/

#define GEOMETRIX_DEFINE_MPL_SEQUENCE_TRAITS( T ) \
namespace boost { namespace mpl {               \
template<typename>                              \
struct sequence_tag;                            \
                                                \
template<>                                      \
struct sequence_tag<T>                          \
{                                               \
    typedef fusion::fusion_sequence_tag type;   \
};                                              \
template<>                                      \
struct sequence_tag<const T>                    \
{                                               \
    typedef fusion::fusion_sequence_tag type;   \
};                                              \
}}                                              \
/***/

//! \def GEOMETRIX_DEFINE_TENSOR_TRAITS( T, TensorOrder, AccessPolicy )
//! Macro for defining traits for a tensor type which adapts the type to be a fusion sequence.
#define GEOMETRIX_DEFINE_TENSOR_TRAITS( T, TensorOrder, AccessPolicy )                \
namespace geometrix {                                                                 \
template <> struct tensor_traits< typename remove_const_ref<T>::type > : AccessPolicy \
{                                                                                     \
    typedef AccessPolicy                  access_policy;                              \
    typedef boost::mpl::int_<TensorOrder> tensor_order;                               \
    typedef void GEOMETRIX_TENSOR_TAG(TensorOrder);                                   \
    typedef void is_tensor;                                                           \
    typedef void make_fusion_sequence;                                                \
};                                                                                    \
}                                                                                     \
/***/

//! \def GEOMETRIX_DEFINE_FUSION_TENSOR_TRAITS( T, TensorOrder, AccessPolicy )
//! Macro for defining traits for a tensor type which is already a fusion sequence.
#define GEOMETRIX_DEFINE_FUSION_TENSOR_TRAITS( T, TensorOrder, AccessPolicy )         \
namespace geometrix {                                                                 \
template <> struct tensor_traits< typename remove_const_ref<T>::type > : AccessPolicy \
{                                                                                     \
    typedef AccessPolicy                  access_policy;                              \
    typedef boost::mpl::int_<TensorOrder> tensor_order;                               \
    typedef void GEOMETRIX_TENSOR_TAG(TensorOrder);                                   \
    typedef void is_tensor;                                                           \
};                                                                                    \
}                                                                                     \
/***/

/*
#pragma message( BOOST_PP_STRINGIZE( (GEOMETRIX_TYPE_DIVERSITY_TAG(A, (1)(2) ) ) ) )
#pragma message( BOOST_PP_STRINGIZE( (rank_0 ) ) )
#pragma message( BOOST_PP_STRINGIZE( (rank_1 ) ) )
#pragma message( BOOST_PP_STRINGIZE( (rank_2 ) ) )
#pragma message( BOOST_PP_STRINGIZE( (GEOMETRIX_TENSOR_TAG(3) ) ) )
*/
#endif //GEOMETRIX_TENSOR_TRAITS_HPP
