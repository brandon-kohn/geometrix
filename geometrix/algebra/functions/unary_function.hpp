//
//! Copyright © 2008-2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_HPP

#include <boost/proto/core.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/context.hpp>
#include <boost/proto/make_expr.hpp>
#include <boost/proto/transform.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

namespace geometrix { namespace algebra {

    #define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(Name, Tag, Domain, Trait)\
    template<typename A>                                                     \
    typename boost::proto::detail::enable_unary<                             \
            Domain                                                           \
          , Domain::proto_grammar                                            \
          , Trait<A>                                                         \
          , Tag                                                              \
          , const A&                                                         \
        >::type const                                                        \
      Name(const A& a)                                                       \
    {                                                                        \
        return boost::proto::make_expr                                       \
               <                                                             \
                   Tag                                                       \
                 , Domain                                                    \
               >(boost::ref(a));                                             \
    }                                                                        \
    /***/

    template <typename Tag, typename T, typename LinearAlgebraType=void>
    struct un_fun
    {
        BOOST_MPL_ASSERT_MSG
        (
            (false)
          , UNARY_FUNCTION_IS_NOT_DEFINED_FOR_THIS_EXPRESSION
          , (un_fun<Tag,T>)
        );
    };

}//namespace algebra;
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_HPP
