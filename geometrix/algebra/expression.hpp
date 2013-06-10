//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_EXPRESSION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_EXPRESSION_HPP

#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/composite_matrix.hpp>
#include <geometrix/tensor/scalar.hpp>
#include <geometrix/algebra/grammar.hpp>

#include <geometrix/algebra/binary_functions.hpp>
#include <geometrix/algebra/unary_functions.hpp>

namespace geometrix { namespace algebra {

template <typename Expr, typename EnableIf=void>
struct expr{};

namespace detail
{
    struct expr_generator
    {
        BOOST_PROTO_CALLABLE()
        BOOST_PROTO_USE_BASIC_EXPR()

        template<typename Sig>
        struct result;

        template<typename This, typename Expr>
        struct result<This(Expr)>
        {
            typedef expr<Expr> type;
        };

        template<typename This, typename Expr>
        struct result<This(Expr &)>
        {
            typedef expr<Expr> type;
        };

        template<typename This, typename Expr>
        struct result<This(Expr const &)>
        {
            typedef expr<Expr> type;
        };

        template<typename Expr>
        expr<Expr> operator ()(Expr const &e) const
        {
            expr<Expr> that = {e};
            return that;
        }
    };
}

struct domain
    : boost::proto::domain<detail::expr_generator, grammar>
{};

BOOST_PROTO_DEFINE_UNARY_OPERATOR(-, boost::proto::tag::negate, is_tensor, domain, 0);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION( det, tag::determinant, domain, is_matrix ); 
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION( trans, tag::transpose, domain, is_matrix );     
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION( inv, tag::inverse, domain, is_matrix );     
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION( mag, tag::magnitude, domain, is_vector );   
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION( norm, tag::normalize, domain, is_vector );  

//! * == dot product for vectors
//! * == matrix product for matrices
//! ^ == cross product for vectors
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(+, boost::proto::tag::plus, is_binary_op, domain)        
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(-, boost::proto::tag::minus, is_binary_op, domain)       
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(*, boost::proto::tag::multiplies, is_binary_op, domain)  
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(/, boost::proto::tag::divides, is_binary_op, domain)    
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(^, geometrix::algebra::tag::cross_product, is_binary_op, domain) 
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(&, geometrix::algebra::tag::tensor_product, is_binary_op, domain) 

//! Proto expr that isn't a fusion sequence in the tree traversal sense.
#define GEOMETRIX_PROTO_BASIC_EXTENDS(Expr, Derived, Domain)                                    \
 GEOMETRIX_PROTO_BASIC_EXTENDS_(Expr, Derived, Domain)                                          \
 typedef void proto_is_aggregate_;                                                              \
 typedef Domain::proto_generator proto_generator;                                               \
/***/

#define GEOMETRIX_PROTO_BASIC_EXTENDS_(Expr, Derived, Domain)                                   \
    Expr proto_expr_;                                                                           \
                                                                                                \
    typedef Expr proto_base_expr_; /**< INTERNAL ONLY */                                        \
    typedef typename proto_base_expr_::proto_base_expr proto_base_expr;                         \
    typedef Domain proto_domain;                                                                \
    typedef Derived proto_derived_expr;                                                         \
    typedef typename proto_base_expr::proto_tag proto_tag;                                      \
    typedef typename proto_base_expr::proto_args proto_args;                                    \
    typedef typename proto_base_expr::proto_arity proto_arity;                                  \
    typedef typename proto_base_expr::proto_grammar proto_grammar;                              \
    typedef typename proto_base_expr::address_of_hack_type_ proto_address_of_hack_type_;        \
    typedef void proto_is_expr_; /**< INTERNAL ONLY */                                          \
    BOOST_STATIC_CONSTANT(long, proto_arity_c = proto_base_expr::proto_arity_c);                \
    BOOST_PP_REPEAT(BOOST_PROTO_MAX_ARITY, BOOST_PROTO_EXTENDS_CHILD, ~)                        \
                                                                                                \
    static proto_derived_expr const make(Expr const &e)                                         \
    {                                                                                           \
        proto_derived_expr that = {e};                                                          \
        return that;                                                                            \
    }                                                                                           \
                                                                                                \
    proto_base_expr &proto_base()                                                               \
    {                                                                                           \
        return this->proto_expr_.proto_base();                                                  \
    }                                                                                           \
                                                                                                \
    proto_base_expr const &proto_base() const                                                   \
    {                                                                                           \
        return this->proto_expr_.proto_base();                                                  \
    }                                                                                           \
                                                                                                \
    operator proto_address_of_hack_type_() const                                                \
    {                                                                                           \
        return boost::addressof(this->proto_base().child0);                                     \
    }                                                                                           \
/***/

template <typename Expr>
struct expr
    <
        Expr
      , typename Expr::proto_is_expr_
    >
{
    struct traits : boost::result_of<grammar(Expr)>::type {};
    GEOMETRIX_PROTO_BASIC_EXTENDS(Expr, expr<Expr>, domain);    
};

//! \brief Helper type to create expr type defs.
template <typename A, typename B, char Op>
struct expression_type_helper{};

#define GEOMETRIX_EXPRESSION_HELPER( op, opcode ) \
template <typename A, typename B>                 \
struct expression_type_helper<A, B,opcode>        \
{                                                 \
    typedef BOOST_TYPEOF_TPL( A() op B() ) type;  \
                                                  \
};                                                \
/***/

GEOMETRIX_EXPRESSION_HELPER( -, '-' );
GEOMETRIX_EXPRESSION_HELPER( +, '+' );
GEOMETRIX_EXPRESSION_HELPER( *, '*' );
GEOMETRIX_EXPRESSION_HELPER( ^, '^' );
GEOMETRIX_EXPRESSION_HELPER( /, '/' );

#undef GEOMETRIX_EXPRESSION_HELPER

#define GEOMETRIX_TYPEOF_BINARY_EXPR( op, A, B ) \
geometrix::algebra::expression_type_helper<A,B,op>::type
/***/

#define GEOMETRIX_TYPEOF_BINARY_EXPR_TPL( op, A, B ) \
typename geometrix::algebra::expression_type_helper<A,B,op>::type
/***/

template<typename Expr, typename EnableIf=void>
struct expr_access_policy
{
    BOOST_MPL_ASSERT_MSG
    (
        (false)
      , LINEAR_ALGEBRA_EXPRESSION_ACCESS_POLICY_NOT_DEFINED
      , (Expr)
    );
};

template <typename Expr>
struct expr_access_policy
    <
        Expr
      , typename expr<Expr>::traits::is_scalar
    >
{
    typedef expr<Expr> scalar_expression;
    
    struct type_at
        : boost::proto::result_of::eval
          <
              scalar_expression
            , typename scalar_expression::traits::context
          >
    {};
        
    static typename type_at::type get( const scalar_expression& c ) 
    {
        return boost::proto::eval( c, typename scalar_expression::traits::context() );
    }
};

template <typename Expr>
struct expr_access_policy
    < 
        Expr
      , typename expr<Expr>::traits::is_sequence
    >
{
    typedef expr<Expr> sequence_expression;
    
    template <unsigned int Index>
    struct type_at
        : boost::proto::result_of::eval
            <
                sequence_expression
              , typename sequence_expression::traits::template context<Index>
            >
    {   
        BOOST_MPL_ASSERT_MSG
        (
            ( Index < sequence_expression::traits::dimension_type::value )
            , SEQUENCE_INDEX_OUT_OF_BOUNDS
            , (sequence_expression)
        );
    };
    
    //! \brief compile time access
    template <unsigned int Index>
    static typename type_at<Index>::type get( const sequence_expression& v ) 
    {
        return boost::proto::eval( v, typename sequence_expression::traits::template context<Index>() );
    }
};

template <typename Expr>
struct expr_access_policy
    <
        Expr
      , typename expr<Expr>::traits::is_matrix
    >
{
    typedef expr<Expr> matrix_expression;
    
    template <unsigned int Row, unsigned int Column>
    struct type_at
        : boost::proto::result_of::eval
            <
                matrix_expression
              , typename matrix_expression::traits::template context<Row, Column>
            >
    {        
        BOOST_MPL_ASSERT_MSG
        (
            ( Row < matrix_expression::traits::row_dimension::type::value &&
              Column < matrix_expression::traits::col_dimension::type::value )
            , MATRIX_ACCESS_INDICES_OUT_OF_BOUNDS
            , (matrix_expression)
        );
    };
   
    //! \brief compile time access
    template <unsigned int Row, unsigned int Column>
    static typename type_at<Row, Column>::type get( const matrix_expression& m ) 
    {
        return boost::proto::eval( m, typename matrix_expression::traits::template context<Row, Column>() );
    }
};

namespace detail
{
    template <typename T, typename Expr, typename TagLHS=void, typename TagRHS=void>
    struct assigner
    {    
        BOOST_MPL_ASSERT_MSG
        (
                (false)
              , CALL_TO_ASSIGN_LINEAR_ALGEBRA_EXPRESSION_RESULT_HAS_INCOMPATIBLE_TYPE
              , (T)
        );      
    };

    template <typename T, typename Expr>
    struct assigner
        <
            T
          , Expr
          , typename geometric_traits<T>::is_scalar
          , typename geometric_traits<Expr>::is_scalar
        > 
    {
        template <typename LHS, typename RHS>
        static LHS& assign( LHS& l, const RHS& e ) 
        {
            return l = get(e);
        }
    };

    template <typename T, typename Expr>
    struct assigner
        <
            T
          , Expr
          , typename geometric_traits<T>::is_vector
          , typename geometric_traits<Expr>::is_vector
        > 
    {
        template <typename LHS, typename RHS>
        static LHS& assign( LHS& l, const RHS& e ) 
        {
            return l = construct<T>(e);
        }
    };

    template <typename T, typename Expr>
    struct assigner
        <
            T
          , Expr
          , typename geometric_traits<T>::is_point
          , typename geometric_traits<Expr>::is_point
        > 
    {
        template <typename LHS, typename RHS>
        static LHS& assign( LHS& l, const RHS& e ) 
        {
            return l = construct<T>(e);
        }
    };

    /*
    template <typename T, typename Expr>
    struct assigner
        <
            T
          , Expr
          , typename geometric_traits<T>::is_matrix
          , typename geometric_traits<Expr>::is_matrix
        > 
    {
        template <typename LHS, typename RHS>
        static LHS& assign( LHS& l, const RHS& e ) 
        {
            return geometrix::detail::matrix_assigner
                <
                    row_dimension_of<LHS>::value
                  , column_dimension_of<LHS>::value
                >::assign(l,e);
        }
    };
    */

    template <typename T, typename Expr>
    struct assigner
        <
            T
          , Expr
          , typename geometric_traits<T>::is_matrix
          , typename geometric_traits<Expr>::is_matrix
        > 
    {
        template <typename LHS, typename RHS>
        static LHS& assign( LHS& l, const RHS& e ) 
        {
            l = construct<LHS>(e);
            return l;
        }
    };

}//namespace detail;

template <typename LHS, typename Expr>
inline LHS& operator <<= ( LHS& lhs, const expr<Expr>& rhs )
{
    return detail::assigner<LHS, expr<Expr> >::template assign( lhs, rhs );
}

template <typename T, typename Expr>
struct expr_cast_wrapper
{
    typedef T cast_type;

    expr_cast_wrapper( const expr<Expr>& e )
        : e( e )
    {}

    expr<Expr> e;
};

template <typename T, typename Expr>
inline expr_cast_wrapper<T,Expr> expr_cast( const expr<Expr>& e )
{
    return expr_cast_wrapper<T,Expr>( e ); 
}

template <typename LHS, typename T, typename Expr>
inline LHS& operator <<= ( LHS& lhs, const expr_cast_wrapper<T, Expr>& rhs )
{
    return detail::assigner<LHS, T>::template assign( lhs, rhs.e );
}

}}//namespace geometrix::algebra;

namespace geometrix { 

template <typename Expr>
struct tensor_traits
    <
        algebra::expr< Expr > 
    >
    : algebra::expr<Expr>::traits
{
    typedef algebra::expr_access_policy< Expr > access_policy;
    typedef void                                is_tensor;    
    typedef void                                make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

template <typename T>
struct is_scalar
    <
        algebra::expr<T>
      , typename algebra::expr<T>::traits::is_scalar
    > : boost::true_type
{};

template <typename T> 
struct numeric_traits
    <
        algebra::expr<T>
      , typename algebra::expr<T>::traits::is_scalar
    >
{                                                                                     
    typedef typename boost::proto::result_of::eval
        <
            T
          , typename algebra::expr<T>::traits::context
        >::type                                               numeric_type;
    typedef typename boost::is_float< numeric_type >::type    is_float;
    typedef typename boost::is_integral< numeric_type >::type is_integral;
    static const numeric_type                                 zero(){ return numeric_traits<numeric_type>::zero(); }
    static const numeric_type                                 one(){ return numeric_traits<numeric_type>::one(); } 
    BOOST_STATIC_CONSTANT                                                             
    (                                                                                 
        unsigned int
      , digits = std::numeric_limits<numeric_type>::digits10
    );
};          

template <typename T>
struct geometric_traits
    < 
        algebra::expr<T>
    > 
    : algebra::expr<T>::traits
{};

//! Row/Column
template <typename Expr, unsigned int Column> 
struct column< algebra::expr<Expr>, Column>
{
    typedef boost::mpl::int_<Column> index;
    column( const algebra::expr<Expr>& v )                                                                                      
        : v(v)                                                                                       
    {};         
    
    template <unsigned int Index>
    struct type_at
    {
        typedef typename geometrix::type_at
            <
                algebra::expr<Expr>
              , Index
              , Column 
            >::type type;
    };

    template <unsigned int Row>                                                                                                       
    typename type_at<Row>::type get() const  
    {                                                                                                             
        return geometrix::get<Row, Column>( v );                                                                     
    }                                                                                                                                 
    
    const algebra::expr<Expr>& v;
};

template <typename Expr, unsigned int Column>
struct geometric_traits< column<algebra::expr<Expr>, Column> > 
    : diversity_base< algebra::expr<Expr> >
{
    typedef typename row_dimension_of
        < 
            algebra::expr<Expr> 
        >::type  dimension_type;
    typedef void is_sequence;
    typedef void is_numeric_sequence;    
};

template <typename Expr, unsigned int Row> 
struct row< algebra::expr<Expr>, Row>
{
    typedef boost::mpl::int_<Row> index;
    row( const algebra::expr<Expr>& v )                                                                                      
        : v(v)                                       
    {};
    
    template <unsigned int Index>
    struct type_at
    {
        typedef typename geometrix::type_at
            < 
                algebra::expr<Expr>
              , Row
              , Index 
            >::type type;
    };

    template <unsigned int Column>
    typename type_at<Column>::type get() const  
    {                                                                                                             
        return geometrix::get<Row, Column>( v );                                                                     
    }                                                                                                                                 
    
    const algebra::expr<Expr>& v;
};

template <typename Expr, unsigned int Index>
struct geometric_traits< row<algebra::expr<Expr>,Index> >
    : diversity_base< algebra::expr<Expr> >
{
    typedef typename column_dimension_of
        <
            algebra::expr<Expr> 
        >::type  dimension_type;
    typedef void is_sequence;
    typedef void is_numeric_sequence;
};

}//namespace geometrix;

//! Mark the expr as an MPL type sequence.
namespace boost { namespace mpl {

template<typename>                              
struct sequence_tag;                            
                                                
template<typename T>                            
struct sequence_tag< geometrix::algebra::expr<T> >
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
template<typename T>             
struct sequence_tag<geometrix::algebra::expr<T> const>                    
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
}}//namespace boost::mpl

#endif //GEOMETRIX_LINEAR_ALGEBRA_EXPRESSION_HPP
