//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTIONS_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTIONS_HPP

#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/cross_product.hpp>
#include <geometrix/arithmetic/matrix_arithmetic.hpp>
#include <geometrix/algebra/type_traits.hpp>

#include <boost/proto/core.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/context.hpp>
#include <boost/proto/transform.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/result_of.hpp>

namespace geometrix { namespace algebra {

    #define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(Name, Tag, Domain, Trait) \
    template<typename A>                                                     \
    typename boost::proto::detail::enable_unary<                             \
            Domain                                                           \
          , Domain::proto_grammar                                            \
          , Trait<A>                                                         \
          , Tag                                                              \
          , A                                                                \
        >::type const                                                        \
      Name(A& a)                                                             \
    {                                                                        \
        return boost::proto::make_expr                                       \
               <                                                             \
                   Tag                                                       \
                 , Domain                                                    \
                 , A                                                         \
               >(boost::ref(a));                                             \
    }                                                                        \
    template<typename A>                                                     \
    typename boost::proto::detail::enable_unary<                             \
            Domain                                                           \
          , Domain::proto_grammar                                            \
          , Trait<A>                                                         \
          , Tag                                                              \
          , const A                                                          \
        >::type const                                                        \
      Name(const A& a)                                                       \
    {                                                                        \
        return boost::proto::make_expr                                       \
               <                                                             \
                   Tag                                                       \
                 , Domain                                                    \
                 , const A                                                   \
               >(boost::ref(a));                                             \
    }                                                                        \
    /***/

    namespace tag
    {
        struct determinant{};
        struct transpose{};
        struct inverse{};
        struct normalize{};
        struct magnitude{};
    }
    
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

    //! Determinant of a Matrix.
    template <typename T>
    struct un_fun
        <
            algebra::tag::determinant
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        > 
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef algebra::tag::determinant tag;
        
            typedef typename result_of::determinant<T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return geometrix::determinant( a );
            }
        };
    };

    //! Transpose a matrix.
    template <typename T>
    struct un_fun
        <
            algebra::tag::transpose
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        >
        : diversity_base<T>
    {
        typedef void                                  is_matrix;
        typedef void                                  rank_2;
        typedef typename row_dimension_of<T>::type    col_dimension;
        typedef typename column_dimension_of<T>::type row_dimension;

        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef algebra::tag::transpose tag;

            typedef typename type_at<T, Column, Row>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return geometrix::get<Column, Row>( a );
            }
        };
    };

    //! Inverse of a Matrix.
    template <typename T>
    struct un_fun
        <
            algebra::tag::inverse
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        > 
        : diversity_base<T>
    {
        typedef void                                  is_matrix;
        typedef void                                  rank_2;
        typedef typename row_dimension_of<T>::type    row_dimension;
        typedef typename column_dimension_of<T>::type col_dimension;

        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef algebra::tag::inverse tag;
        
            typedef typename result_of::determinant<T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return geometrix::inverse_at_index<Row, Column>( a );
            }
        };
    };

    //! Normalize a vector.
    template <typename T>
    struct un_fun
        <
            algebra::tag::normalize
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
        : diversity_base<T>
    {
        typedef void                           is_vector;
        typedef void                           rank_1;
        typedef typename dimension_of<T>::type dimension_type;
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;

        template <unsigned int Index>
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef algebra::tag::normalize tag;
            
            typedef typename result_of::normal_at_index<Index, T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return normal_at_index<Index>( a ); 
            }
        };    
    };

    //! Magnitude of a vector.
    template <typename T>
    struct un_fun
        <
            algebra::tag::magnitude
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef algebra::tag::magnitude tag;
            typedef typename result_of::magnitude<T>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return geometrix::magnitude(a);
            }
        };
    };

    //! negate a vector.
    template <typename T>
    struct un_fun
        < 
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
        : diversity_base<T>
    {
        typedef void                           is_vector;
        typedef void                           rank_1;
        typedef typename dimension_of<T>::type dimension_type;
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;
    
        template <unsigned int Index>
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::negate tag;
            typedef typename type_at<T, Index>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return -geometrix::get<Index>(a);
            }
        };
    };

    //! negate a point.
    template <typename T>
    struct un_fun
        < 
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_point 
        > 
        : diversity_base<T>
    {
        typedef void                           is_point;
        typedef void                           rank_1;
        typedef typename dimension_of<T>::type dimension_type;
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;
    
        template <unsigned int Index>
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::negate tag;
            typedef typename type_at<T, Index>::type result_type;

            result_type operator()(tag, const T& a) const
            {            
                return -geometrix::get<Index>(a);
            }
        };
    };

    //! negate a scalar.
    template <typename T>
    struct un_fun
        < 
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_scalar 
        > 
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::negate tag;
        
            typedef typename type_at<T>::type result_type;
            result_type operator()(tag, const T& a) const
            {            
                return -geometrix::get(a);
            }
        };
    };

    //! negate a matrix.
    template <typename T>
    struct un_fun
        <
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        >  
        : diversity_base<T>
    {
        typedef void                                  is_matrix;
        typedef void                                  rank_2;
        typedef typename row_dimension_of<T>::type    row_dimension;
        typedef typename column_dimension_of<T>::type col_dimension;

        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef boost::proto::tag::negate tag;
        
            typedef typename type_at<T, Row, Column>::type result_type;
            result_type operator()(tag, const T& a) const
            {            
                return -geometrix::get<Row, Column>( a );
            }
        };
    };

}//namespace algebra;
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTIONS_HPP
