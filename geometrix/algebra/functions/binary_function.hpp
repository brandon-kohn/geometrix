//
//! Copyright © 2008-2013
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

namespace geometrix {

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
  , typename geometrix::geometric_traits<LHS>::LeftTrait                 \
  , typename geometrix::geometric_traits<RHS>::RightTrait                \
> : boost::true_type                                                     \
{};                                                                      \
/***/

#define GEOMETRIX_PROTO_APPLY_BINARY_(Trait, Tag, Left, Right)           \
    boost::mpl::and_<                                                    \
        Trait                                                            \
        <                                                                \
            Tag                                                          \
          , typename geometrix::remove_const_ref<Left>::type             \
          , typename geometrix::remove_const_ref<Right>::type            \
        >                                                                \
      , boost::mpl::not_<                                                \
            boost::mpl::or_<                                             \
                boost::proto::is_extension<Left>                         \
              , boost::proto::is_extension<Right>                        \
            >                                                            \
        >                                                                \
    >                                                                    \
/***/

#define GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(Op, Tag, Trait, Domain)                   \
    template<typename Left, typename Right>                                              \
    typename boost::proto::detail::enable_binary<                                        \
        Domain                                                                           \
      , Domain::proto_grammar                                                            \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, Tag, Left, Right)                           \
      , Tag                                                                              \
      , Left const&                                                                      \
      , Right const&                                                                     \
    >::type const                                                                        \
    operator Op(Left const &left, Right const &right)                                    \
    {                                                                                    \
        return boost::proto::make_expr<Tag, Domain>(boost::ref(left),boost::ref(right)); \
    }                                                                                    \
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
	private:
		using left_arithmetic_type = typename geometric_traits<typename remove_const_ref<Left>::type>::arithmetic_type;
		using right_arithmetic_type = typename geometric_traits<typename remove_const_ref<Right>::type>::arithmetic_type;
		using left_dimensionless_type = typename geometric_traits<typename remove_const_ref<Left>::type>::dimensionless_type;
		using right_dimensionless_type = typename geometric_traits<typename remove_const_ref<Right>::type>::dimensionless_type;

	public:
		using arithmetic_type = typename select_arithmetic_type_from_2<left_arithmetic_type, right_arithmetic_type>::type;
		using dimensionless_type = typename select_arithmetic_type_from_2<left_dimensionless_type, right_dimensionless_type>::type;
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

}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTION_HPP
