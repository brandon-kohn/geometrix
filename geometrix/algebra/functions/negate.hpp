//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NEGATE_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NEGATE_HPP

#include <geometrix/algebra/functions/unary_function.hpp>

namespace geometrix { namespace algebra {

    namespace tag
    {
        struct negate{};
    }
    
    //! negate a vector.
    template <typename T>
    struct unary_fn
        < 
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_vector 
        > 
        : uniformity_base<T>
    {
        typedef void                           is_vector;
        typedef void                           rank_1;
        typedef typename dimension_of<T>::type dimension_type;
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
    
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
    struct unary_fn
        < 
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_point 
        > 
        : uniformity_base<T>
    {
        typedef void                           is_point;
        typedef void                           rank_1;
        typedef typename dimension_of<T>::type dimension_type;
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
    
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
    struct unary_fn
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
    struct unary_fn
        <
            boost::proto::tag::negate
          , T
          , typename geometric_traits<typename remove_const_ref<T>::type>::is_matrix 
        >  
        : uniformity_base<T>
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

#endif//GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION_NEGATE_HPP
