//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_GRAMMAR_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_GRAMMAR_HPP

#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/cross_product.hpp>
#include <geometrix/arithmetic/matrix_arithmetic.hpp>
#include <geometrix/algebra/unary_functions.hpp>
#include <geometrix/algebra/binary_functions.hpp>
#include <geometrix/algebra/terminals.hpp>

#include <boost/proto/core.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/context.hpp>
#include <boost/proto/transform.hpp>
#include <boost/utility/result_of.hpp>

namespace geometrix { namespace algebra {

    namespace transform
    {
        #define GEOMETRIX_DEFINE_UNARY_METAFUNCTION(Op, Tag)                                            \
            template<typename T>                                                                        \
            struct Op                                                                                   \
              : boost::proto::transform<Op<T>, int>                                                     \
            {                                                                                           \
                typedef boost::proto::expr<Tag, boost::proto::list1<T>, 1> type;                        \
                typedef boost::proto::basic_expr<Tag, boost::proto::list1<T>, 1> proto_grammar;         \
                                                                                                        \
                template<typename Expr, typename State, typename Data>                                  \
                struct impl : boost::proto::transform_impl<Expr, State, Data>                           \
                {                                                                                       \
                    typedef un_fun                                                                      \
                        <                                                                               \
                            Tag                                                                         \
                          , typename boost::proto::result_of::child_c<Expr,0>::type                     \
                        > result_type;                                                                  \
                                                                                                        \
                    result_type operator ()(typename impl::expr_param e                                 \
                                          , typename impl::state_param                                  \
                                          , typename impl::data_param) const                            \
                    {                                                                                   \
                        return result_type();                                                           \
                    }                                                                                   \
                                                                                                        \
                };                                                                                      \
                                                                                                        \
            };                                                                                          \
        /***/

        #define GEOMETRIX_DEFINE_BINARY_METAFUNCTION(Op, Tag)                                           \
            template<typename T, typename U>                                                            \
            struct Op                                                                                   \
              : boost::proto::transform<Op<T, U>, int>                                                  \
            {                                                                                           \
                typedef boost::proto::expr<Tag, boost::proto::list2<T, U>, 2> type;                     \
                typedef boost::proto::basic_expr<Tag, boost::proto::list2<T, U>, 2> proto_grammar;      \
                                                                                                        \
                template<typename Expr, typename State, typename Data>                                  \
                struct impl : boost::proto::transform_impl<Expr, State, Data>                           \
                {                                                                                       \
                    typedef bin_fun                                                                     \
                        <                                                                               \
                            Tag                                                                         \
                          , typename boost::proto::result_of::child_c<Expr, 0>::type                    \
                          , typename boost::proto::result_of::child_c<Expr, 1>::type                    \
                        > result_type;                                                                  \
                                                                                                        \
                    result_type operator ()(typename impl::expr_param e                                 \
                                          , typename impl::state_param                                  \
                                          , typename impl::data_param) const                            \
                    {                                                                                   \
                        return result_type();                                                           \
                    }                                                                                   \
                                                                                                        \
                };                                                                                      \
            };                                                                                          \
        /***/

        //! \brief A metafunction for generating terminal expr types,
        //! a grammar element for matching terminal expressions, and a
        //! PrimitiveTransform that returns the current expr unchanged.
        template<typename T>
        struct terminal
            : boost::proto::transform
                <
                    terminal<T>
                  , int
                >
        {
            typedef boost::proto::expr<boost::proto::tag::terminal, boost::proto::term<T>, 0>       type;
            typedef boost::proto::basic_expr<boost::proto::tag::terminal, boost::proto::term<T>, 0> proto_grammar;

            template<typename Expr, typename State, typename Data>
            struct impl : boost::proto::transform_impl<Expr, State, Data>
            {                        
                typedef term_
                < 
                    typename boost::proto::result_of::value<Expr>::type 
                > result_type;

                result_type operator ()(typename impl::expr_param e
                                      , typename impl::state_param
                                      , typename impl::data_param) const
                {
                    return result_type();
                }
            };
        };

        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( plus, boost::proto::tag::plus )
        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( minus, boost::proto::tag::minus )
        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( multiplies, boost::proto::tag::multiplies )
        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( divides, boost::proto::tag::divides )
        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( dot_product, tag::dot_product )
        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( cross_product, tag::cross_product )
        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( matrix_product, tag::matrix_product )
        GEOMETRIX_DEFINE_BINARY_METAFUNCTION( tensor_product, tag::tensor_product )
        
        GEOMETRIX_DEFINE_UNARY_METAFUNCTION( negate, boost::proto::tag::negate )
        GEOMETRIX_DEFINE_UNARY_METAFUNCTION( transpose, tag::transpose )
        GEOMETRIX_DEFINE_UNARY_METAFUNCTION( normalize, tag::normalize )
        GEOMETRIX_DEFINE_UNARY_METAFUNCTION( magnitude, tag::magnitude )
        GEOMETRIX_DEFINE_UNARY_METAFUNCTION( determinant, tag::determinant )
        GEOMETRIX_DEFINE_UNARY_METAFUNCTION( inverse, tag::inverse )

        #undef GEOMETRIX_DEFINE_BINARY_METAFUNCTION
        #undef GEOMETRIX_DEFINE_UNARY_METAFUNCTION        

    }//namespace transform;

    //! The linear algebra grammar.
    struct grammar
        : boost::proto::switch_<struct cases_>
    {};

    struct cases_
    {
        template<typename Tag, int D = 0> 
        struct case_  
            : boost::proto::not_<boost::proto::_>
        {};

        template<int D>
        struct case_< boost::proto::tag::terminal, D >
            : transform::terminal<boost::proto::_>
        {};

        template<int D>
        struct case_< boost::proto::tag::plus, D >     
            : transform::plus<grammar, grammar>
        {};        

        template<int D>
        struct case_< boost::proto::tag::minus, D >     
            : transform::minus<grammar, grammar>
        {};   

        template<int D>
        struct case_< boost::proto::tag::multiplies, D >     
            : transform::multiplies<grammar, grammar>
        {};

        template<int D>
        struct case_< boost::proto::tag::divides, D >
            : transform::divides<grammar, grammar>
        {};   
        
        template<int D>
        struct case_< tag::cross_product, D >
            : transform::cross_product<grammar, grammar>
        {};   

        template<int D>
        struct case_< tag::tensor_product, D >
            : transform::tensor_product<grammar, grammar>
        {};   

        template<int D>
        struct case_< algebra::tag::determinant, D >
            : transform::determinant<grammar>
        {};

        template<int D>
        struct case_< algebra::tag::transpose, D >
            : transform::transpose<grammar>
        {};

        template<int D>
        struct case_< algebra::tag::inverse, D >
            : transform::inverse<grammar>
        {};

        template<int D>
        struct case_< boost::proto::tag::negate, D >
            : transform::negate<grammar>
        {};

        template<int D>
        struct case_< algebra::tag::normalize, D >
            : transform::normalize<grammar>
        {};

        template<int D>
        struct case_< algebra::tag::magnitude, D >
            : transform::magnitude<grammar>
        {};
    };
    
}}// namespace geometrix::algebra;

#endif//GEOMETRIX_LINEAR_ALGEBRA_GRAMMAR_HPP
