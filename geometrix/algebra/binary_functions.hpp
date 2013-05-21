//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_HPP

#include <geometrix/tensor/matrix_traits.hpp>
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
      , Left                                                                                        \
      , Right                                                                                       \
    >::type const                                                                                   \
    operator OP(Left &left, Right &right)                                                           \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain, Left, Right>()(left, right);           \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    typename boost::proto::detail::enable_binary<                                                   \
        Domain                                                                                      \
      , Domain::proto_grammar                                                                       \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, TAG, Left, Right)                                      \
      , TAG                                                                                         \
      , Left                                                                                        \
      , Right const                                                                                 \
    >::type const                                                                                   \
    operator OP(Left &left, Right const &right)                                                     \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain, Left, Right const>()(left, right);     \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    typename boost::proto::detail::enable_binary<                                                   \
        Domain                                                                                      \
      , Domain::proto_grammar                                                                       \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, TAG, Left, Right)                                      \
      , TAG                                                                                         \
      , Left const                                                                                  \
      , Right                                                                                       \
    >::type const                                                                                   \
    operator OP(Left const &left, Right &right)                                                     \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain, Left const, Right>()(left, right);     \
    }                                                                                               \
                                                                                                    \
    template<typename Left, typename Right>                                                         \
    typename boost::proto::detail::enable_binary<                                                   \
        Domain                                                                                      \
      , Domain::proto_grammar                                                                       \
      , GEOMETRIX_PROTO_APPLY_BINARY_(Trait, TAG, Left, Right)                                      \
      , TAG                                                                                         \
      , Left const                                                                                  \
      , Right const                                                                                 \
    >::type const                                                                                   \
    operator OP(Left const &left, Right const &right)                                               \
    {                                                                                               \
        return boost::proto::detail::make_expr_<TAG, Domain, Left const, Right const>()(left,right);\
    }                                                                                               \
    /**/

    namespace tag
    {
        typedef boost::proto::tag::bitwise_xor cross_product;
        typedef boost::proto::tag::multiplies  dot_product;
        typedef boost::proto::tag::multiplies  matrix_product;
        typedef boost::proto::tag::bitwise_and tensor_product;
    }//namespace tag;

    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::plus, is_sequence, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::plus, is_matrix, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::plus, is_scalar, is_scalar);
    
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::minus, is_sequence, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::minus, is_point, is_point );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::minus, is_matrix, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::minus, is_scalar, is_scalar );

    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_scalar, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_scalar, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_scalar, is_scalar );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_vector, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::multiplies, is_matrix, is_vector );
    
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::divides, is_vector, is_scalar );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::divides, is_matrix, is_scalar );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( boost::proto::tag::divides, is_scalar, is_scalar );
    
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::dot_product, is_vector, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::matrix_product, is_matrix, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::cross_product, is_vector, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_vector, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_vector, is_matrix );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_matrix, is_vector );
    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::tensor_product, is_matrix, is_matrix );

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

    //! Product of Scalar and Scalar
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::multiplies
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get( l ) * get( r );
            }
        };
    };
        
    //! Division of Scalar and Scalar
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::divides
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::divides tag_t;
            typedef typename result_of::divides
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get( l ) / get( r );
            }
        };
    };
    
    //! Sum of Scalar and Scalar
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::plus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;

        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get( l ) + get( r );
            }
        };
    };
        
    //! Subtraction of Scalar and Scalar
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::minus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;
        
        struct context : boost::proto::callable_context< const context >
        {            
            typedef boost::proto::tag::minus tag_t;            
            typedef typename result_of::minus
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get( l ) - get( r );
            }
        };
    };

    //! Product of Scalar and Vector
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : diversity_base<Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::multiplies tag_t;            
            typedef typename result_of::multiplies
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right, Index>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get( l ) * get<Index>( r );
            }
        };
    };
    
    //! Division of Vector by Scalar
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::divides
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
        : diversity_base<Left>
    {
        typedef void                                    is_vector;
        typedef void                                    rank_1;
        typedef typename dimension_of<Left>::type       dimension_type;
        typedef typename reference_frame_of<Left>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                    is_sequence;
        typedef void                                    is_numeric_sequence;
        typedef void                                    is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::divides tag_t;
            typedef typename result_of::divides
                    <
                        typename type_at<Left, Index>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Index>( l ) / get( r );            
            }
        };
    };
    
    //! Product of Scalar and Matrix
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_scalar
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : diversity_base<Right>
    {
        typedef void                                      is_matrix;
        typedef void                                      rank_2;
        typedef typename row_dimension_of<Right>::type    row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;

        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::multiplies
                    <
                        typename type_at<Left>::type
                      , typename type_at<Right, Row, Column>::type
                    >::type result_type;
            
            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get( l ) * get<Row, Column>( r );
            }
        };
    };

    //! Division of Matrix by Scalar
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::divides
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_scalar 
        >
        : diversity_base<Left>
    {
        typedef void                                     is_matrix;
        typedef void                                     rank_2;
        typedef typename row_dimension_of<Left>::type    row_dimension;
        typedef typename column_dimension_of<Left>::type col_dimension;

        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef boost::proto::tag::divides tag_t;
            typedef typename result_of::divides
                    <
                        typename type_at<Left, Row, Column>::type
                      , typename type_at<Right>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Row, Column>( l ) / get( r );
            }
        };
    };

    //! Product of Matrices
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::matrix_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                      is_matrix;
        typedef void                                      rank_2;
        typedef typename row_dimension_of<Left>::type     row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;
        
        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef tag::matrix_product tag_t;
            typedef typename result_of::dot_product
                <
                    row<Left,Row>
                  , column<Right,Column> 
                >::type result_type;
            
            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return matrix_product_element<Row, Column>( l, r );
            }
        };
    };
        
    //! Product of Row Vector with Matrix
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                      rank_2;
        typedef void                                      is_matrix;
        typedef typename row_dimension_of<Right>::type    row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;

        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::dot_product
                <
                    Left
                  , column<Right, Column>
                >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return dot_product( l, column<Right,Column>(r) );
            }
        };
    };

    //! Product of Matrix with Column Vector
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::multiplies
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     rank_1;
        typedef void                                     is_vector;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::multiplies tag_t;
            typedef typename result_of::dot_product
                <
                    row<Left,Index>
                  , Right
                >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return dot_product( row<Left,Index>(l), r );
            }
        };
    };
    
    //! Addition of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::plus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left,Index>::type
                      , typename type_at<Right,Index>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Index>( l ) + get<Index>( r );
            }
        };
    };
    
    //! Subtraction of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::minus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::minus tag_t;
            typedef typename result_of::minus
                    <
                        typename type_at<Left,Index>::type
                      , typename type_at<Right,Index>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Index>( l ) - get<Index>( r );
            }
        };
    };
    
    //! Addition of a Vector to a Point
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::plus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_point
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_point;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef void                                     rank_1;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left,Index>::type
                      , typename type_at<Right,Index>::type
                    >::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Index>( l ) + get<Index>( r );
            }
        };
    };
    
    //! Subtraction of Vector from a Point
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::minus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_point
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_point;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::minus tag_t;
            typedef typename result_of::minus
                    <
                        typename type_at<Left,Index>::type
                      , typename type_at<Right,Index>::type
                    >::type result_type;
                
            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Index>( l ) - get<Index>( r );
            }
        };
    };

    //! Subtraction of a Point from a Point
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::minus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_point
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_point 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                           is_sequence;
        typedef void                           is_numeric_sequence;
        typedef void                           is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {            
            typedef boost::proto::tag::minus tag_t;
            typedef typename result_of::minus
                    <
                        typename type_at<Left,Index>::type
                      , typename type_at<Right,Index>::type
                    >::type result_type;
                
            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Index>( l ) - get<Index>( r );
            }
        };
    };
    
    //! Addition of Matrices
    template <typename Left, typename Right>
    struct bin_fun
        <
            boost::proto::tag::plus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                      is_matrix;
        typedef void                                      rank_2;
        typedef typename row_dimension_of<Right>::type    row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;

        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef boost::proto::tag::plus tag_t;
            typedef typename result_of::plus
                    <
                        typename type_at<Left,Row,Column>::type
                      , typename type_at<Right,Row,Column>::type
                    >::type result_type;
            
            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Row, Column>( l ) + get<Row, Column>( r );
            }
        };
    };
    
    //! Subtraction of Matrices
    template <typename Left, typename Right>
    struct bin_fun
        < 
            boost::proto::tag::minus
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                      is_matrix;
        typedef void                                      rank_2;
        typedef typename row_dimension_of<Right>::type    row_dimension;
        typedef typename column_dimension_of<Right>::type col_dimension;
                
        template <unsigned int Row, unsigned int Column, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Row, Column> >
        {            
            typedef boost::proto::tag::minus tag_t;
            typedef typename result_of::minus
                    <
                        typename type_at<Left,Row,Column>::type
                      , typename type_at<Right,Row,Column>::type
                    >::type result_type;  

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return get<Row, Column>( l ) - get<Row, Column>( r );
            }
        };
    };
    
    //! Dot Product of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::dot_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
    {
        typedef void is_scalar;
        typedef void rank_0;
        typedef void is_homogeneous;
        
        struct context : boost::proto::callable_context< const context >
        {            
            typedef tag::dot_product tag_t;
            typedef typename result_of::dot_product<Left, Right>::type result_type;

            result_type operator()(tag_t, const Left& l, const Right& r ) const
            {
                return geometrix::dot_product( l, r );
            }
        };
    };
        
    //! Cross Product of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::cross_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

        template <unsigned int Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index> >
        {
            typedef typename result_of::cross_product_at_index<Left, Right, Index>::type result_type;

            result_type operator()(tag::cross_product, const Left& l, const Right& r ) const
            {
                return cross_product_at_index<Index>( l, r );
            }
        };
    };

    //! Tensor Product of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                          is_matrix;
        typedef void                                                          rank_2;
        typedef typename result_of::tensor_product<Left,Right>::row_dimension row_dimension;
        typedef typename result_of::tensor_product<Left,Right>::col_dimension col_dimension;
        
        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };

    //! Tensor Product of Vector and Matrix
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                                        is_matrix;
        typedef void                                                                        rank_2;
        typedef typename result_of::tensor_product<Left, row_vector<Right> >::row_dimension row_dimension;
        typedef typename result_of::tensor_product<Left, row_vector<Right> >::col_dimension col_dimension;
        
        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };

    //! Tensor Product of Matrix and Vector
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                                          is_matrix;
        typedef void                                                                          rank_2;
        typedef typename result_of::tensor_product<column_vector<Left>, Right>::row_dimension row_dimension;
        typedef typename result_of::tensor_product<column_vector<Left>, Right>::col_dimension col_dimension;
        
        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };
    
    //! Tensor Product of Matrix and Vector
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::tensor_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_matrix
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_matrix 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                                           is_matrix;
        typedef void                                                           rank_2;
        typedef typename result_of::tensor_product<Left, Right>::row_dimension row_dimension;
        typedef typename result_of::tensor_product<Left, Right>::col_dimension col_dimension;
        
        template <unsigned int Row, unsigned int Column>
        struct context : boost::proto::callable_context< const context<Row,Column> >
        {            
            typedef typename result_of::tensor_product_element<Left, Right, Row, Column>::type result_type;

            result_type operator()(tag::tensor_product, const Left& l, const Right& r ) const
            {
                return geometrix::tensor_product_element<Row,Column>( l, r );
            }
        };
    };

}//namespace algebra;
}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_HPP
