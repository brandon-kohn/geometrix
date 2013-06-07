//
//! Copyright � 2008-2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTION_HPP

#include <boost/proto/core.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/context.hpp>
#include <boost/proto/transform.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

namespace geometrix { namespace algebra {

template <typename Tag, typename LHS, typename RHS, typename IsLeft=void, typename IsRight=void>
struct is_binary_op : boost::false_type 
{};

#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( Tag, LeftTrait, RightTrait ) \
template <typename LHS, typename RHS>                                    \
struct is_binary_op                                                      \
<                                                                        \
    Tag                                                                  \
  , LHS                                                                  \
  , RHS                                                                  \
  , typename geometric_traits<LHS>::LeftTrait                            \
  , typename geometric_traits<RHS>::RightTrait                           \
> : boost::true_type                                                     \
{};                                                                      \
/***/

#define GEOMETRIX_PROTO_APPLY_BINARY_(Trait, Tag, Left, Right) \
    boost::mpl::and_<                                          \
        Trait                                                  \
        <                                                      \
            Tag                                                \
          , typename remove_const_ref<Left>::type              \
          , typename remove_const_ref<Right>::type             \
        >                                                      \
      , boost::mpl::not_<                                      \
            boost::mpl::or_<                                   \
                boost::proto::is_extension<Left>               \
              , boost::proto::is_extension<Right>              \
            >                                                  \
        >                                                      \
    >                                                          \
/***/

#define GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(OP, TAG, Trait, Domain)                              \
    template<typename Left, typename Right>                                                         \
    typename boost::proto::detail::enable_binary<                                                   \
        Domain                                                                                      \
      , Domain::proto_grammar                                                                       \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, TAG, Left, Right)                                      \
      , TAG                                                                                         \
      , Left&                                                                                       \
      , Right&                                                                                      \
    >::type const                                                                                   \
    operator OP(Left &left, Right &right)                                                           \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain, Left&, Right&>()(left, right);         \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    typename boost::proto::detail::enable_binary<                                                   \
        Domain                                                                                      \
      , Domain::proto_grammar                                                                       \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, TAG, Left, Right)                                      \
      , TAG                                                                                         \
      , Left&                                                                                       \
      , Right const&                                                                                \
    >::type const                                                                                   \
    operator OP(Left &left, Right const &right)                                                     \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain, Left&, Right const&>()(left, right);   \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    typename boost::proto::detail::enable_binary<                                                   \
        Domain                                                                                      \
      , Domain::proto_grammar                                                                       \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, TAG, Left, Right)                                      \
      , TAG                                                                                         \
      , Left const&                                                                                 \
      , Right&                                                                                      \
    >::type const                                                                                   \
    operator OP(Left const &left, Right &right)                                                     \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain, Left const&, Right&>()(left, right);   \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    typename boost::proto::detail::enable_binary<                                                   \
        Domain                                                                                      \
      , Domain::proto_grammar                                                                       \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, TAG, Left, Right)                                      \
      , TAG                                                                                         \
      , Left const&                                                                                 \
      , Right const&                                                                                \
    >::type const                                                                                   \
    operator OP(Left const &left, Right const &right)                                               \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain,Left const&,Right const&>()(left,right);\
    }                                                                                               \
    /***/

    template <typename Left, typename Right, typename IsHomogeneousL=void, typename IsHomogeneousR=void>
    struct binary_diversity_base
    {
        typedef void is_heterogeneous;
    };

    template <typename Left, typename Right>
    struct binary_diversity_base
        <
            Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_homogeneous
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_homogeneous
        >
    {
        typedef void is_homogeneous;
    };
        
    template
        <
            typename Tag
          , typename Left
          , typename Right
          , typename LeftLinearAlgebraType = void
          , typename RightLinearAlgebraType = void
        >
    struct bin_fun
    {
        BOOST_MPL_ASSERT_MSG
        (
            (false)
          , BINARY_OPERATION_IS_NOT_DEFINED_FOR_THIS_EXPRESSION
          , (bin_fun<Tag, Left, Right>)
        );
    };

}//namespace algebra;
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTION_HPP
