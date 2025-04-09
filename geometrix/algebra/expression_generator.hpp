//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/matrix_traits.hpp>
#if defined(GEOMETRIX_USE_COMPOSITE_MATRICES)
#include <geometrix/tensor/composite_matrix.hpp>
#endif
#include <geometrix/tensor/scalar.hpp>
#include <geometrix/algebra/grammar.hpp>

#include <geometrix/algebra/binary_functions.hpp>
#include <geometrix/algebra/unary_functions.hpp>

#include <geometrix/utility/as.hpp>

#include <boost/mpl/assert.hpp>

#if !defined(BOOST_RESULT_OF_USE_TR1_WITH_DECLTYPE_FALLBACK) && !defined(BOOST_RESULT_OF_USE_TR1)
#error "Expression traits require the TR1 policy of boost::result_of."
#endif

GEOMETRIX_EXPRESSION_NAMESPACE_START

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
    : boost::proto::domain<GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::detail::expr_generator, geometrix::grammar>
{};

BOOST_PROTO_DEFINE_UNARY_OPERATOR(-, boost::proto::tag::negate, geometrix::should_expression_negate, domain, 0);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(det, geometrix::tag::determinant, domain, geometrix::is_matrix);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(trans, geometrix::tag::transpose, domain, geometrix::is_matrix);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(transpose, geometrix::tag::transpose, domain, geometrix::is_matrix);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(inv, geometrix::tag::inverse, domain, geometrix::is_matrix);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(inverse, geometrix::tag::inverse, domain, geometrix::is_matrix);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(mag, geometrix::tag::magnitude, domain, geometrix::is_vector);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(norm, geometrix::tag::normalize, domain, geometrix::is_vector);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(abs, geometrix::tag::absolute_value, domain, geometrix::is_tensor);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(rec, geometrix::tag::reciprocal, domain, geometrix::is_tensor);
GEOMETRIX_LINEAR_ALGEBRA_UNARY_FUNCTION(reciprocal, geometrix::tag::reciprocal, domain, geometrix::is_tensor);

//! * == dot product for vectors
//! * == matrix product for matrices
//! ^ == cross product for vectors
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(+, boost::proto::tag::plus, geometrix::is_binary_op, domain)
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(-, boost::proto::tag::minus, geometrix::is_binary_op, domain)
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(*, boost::proto::tag::multiplies, geometrix::is_binary_op, domain)
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(/ , boost::proto::tag::divides, geometrix::is_binary_op, domain)
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(^, geometrix::tag::cross_product, geometrix::is_binary_op, domain)
GEOMETRIX_PROTO_DEFINE_BINARY_OPERATOR(&, geometrix::tag::tensor_product, geometrix::is_binary_op, domain)

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
    struct traits : boost::result_of<geometrix::grammar(Expr)>::type {};
    GEOMETRIX_PROTO_BASIC_EXTENDS(Expr, GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, domain);
};

template <typename Expr, typename EnableIf=void>
struct is_expression_impl : std::false_type {};

template <typename Expr>
struct is_expression_impl<GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>> : std::true_type {};

template <typename Expr>
using is_expression = is_expression_impl<std::decay_t<Expr>>;

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
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_scalar
    >
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> scalar_expression;

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
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_sequence
    >
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> sequence_expression;

    template <std::size_t I>
    struct type_at
        : boost::proto::result_of::eval
            <
                sequence_expression
              , typename sequence_expression::traits::template context<I>
            >
    {
        BOOST_MPL_ASSERT_MSG
        (
            ( I < sequence_expression::traits::dimension_type::value )
            , SEQUENCE_INDEX_OUT_OF_BOUNDS
            , (sequence_expression)
        );
    };

    //! \brief compile time access
    template <std::size_t Index>
    static typename type_at<Index>::type get( const sequence_expression& v )
    {
        return boost::proto::eval( v, typename sequence_expression::traits::template context<Index>() );
    }
};

template <typename Expr>
struct expr_access_policy
    <
        Expr
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_matrix
    >
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> matrix_expression;

    template <std::size_t Row, std::size_t Column>
    struct type_at
        : boost::proto::result_of::eval
            <
                matrix_expression
              , typename matrix_expression::traits::template context<Row, Column>
            >
    {
        static_assert
        (
            Row < matrix_expression::traits::row_dimension::type::value &&
              Column < matrix_expression::traits::col_dimension::type::value
            , "matrix indices out of bounds"
        );
    };

    //! \brief compile time access
    template <std::size_t Row, std::size_t Column>
    static typename type_at<Row, Column>::type get( const matrix_expression& m )
    {
        return boost::proto::eval( m, typename matrix_expression::traits::template context<Row, Column>() );
    }
};

namespace detail
{
    template <typename T, typename Expr, typename TagLHS=void, typename TagRHS=void>
    struct assigner_impl
    {
        BOOST_MPL_ASSERT_MSG
        (
                (false)
              , CALL_TO_ASSIGN_LINEAR_ALGEBRA_EXPRESSION_RESULT_HAS_INCOMPATIBLE_TYPE
              , (T)
        );
    };

    template <typename T, typename Expr>
    struct assigner_impl
        <
            T
          , Expr
          , typename geometrix::geometric_traits<T>::is_scalar
          , typename geometrix::geometric_traits<Expr>::is_scalar
        >
    {
        template <typename LHS, typename RHS>
        static void assign( LHS& l, const RHS& e )
        {
            l = geometrix::get(e);
        }
    };

    template <typename T, typename Expr>
    struct assigner_impl
        <
            T
          , Expr
          , typename geometrix::geometric_traits<T>::is_vector
          , typename geometrix::geometric_traits<Expr>::is_vector
        >
    {
        template <typename LHS, typename RHS>
        static void assign( LHS& l, const RHS& e )
        {
            l = geometrix::construct<T>(e);
        }
    };

    template <typename T, typename Expr>
    struct assigner_impl
        <
            T
          , Expr
          , typename geometrix::geometric_traits<T>::is_point
          , typename geometrix::geometric_traits<Expr>::is_point
        >
    {
        template <typename LHS, typename RHS>
        static void assign( LHS& l, const RHS& e )
        {
            l = geometrix::construct<T>(e);
        }
    };

    template <typename T, typename Expr>
    struct assigner_impl
        <
            T
          , Expr
          , typename geometrix::geometric_traits<T>::is_matrix
          , typename geometrix::geometric_traits<Expr>::is_matrix
        >
    {
        template <typename LHS, typename RHS>
        static void assign( LHS& l, const RHS& e )
        {
            l = geometrix::construct<LHS>(e);
        }
    };

    template <typename LHS, typename RHS>
    using assigner = assigner_impl<std::decay_t<LHS>, std::decay_t<RHS>>;

}//namespace detail;

template <typename LHS, typename Expr, typename std::enable_if<is_expression<Expr>::value, int>::type = 0>
inline LHS& operator <<= ( LHS& lhs, const Expr& rhs )
{
    detail::assigner<LHS, Expr>::template assign( lhs, rhs );
    return lhs;
}

//! Assign from the same type.
template <typename LHS>
inline LHS& operator <<= ( LHS& lhs, const LHS& rhs )
{
    return lhs = geometrix::construct<LHS>( rhs );
}

template <typename T, typename Expr>
struct expr_cast_wrapper
{
    typedef T cast_type;

    expr_cast_wrapper( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& e )
        : e( e )
    {}

    GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> e;
};

template <typename T, typename Expr>
inline expr_cast_wrapper<T,Expr> expr_cast( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& e )
{
    return expr_cast_wrapper<T,Expr>( e );
}

template <typename LHS, typename T, typename Expr>
inline LHS& operator <<= ( LHS& lhs, const expr_cast_wrapper<T, Expr>& rhs )
{
    detail::assigner<LHS, T>::template assign( lhs, rhs.e );
    return lhs;
}

GEOMETRIX_EXPRESSION_NAMESPACE_END

namespace geometrix {

template <typename T, typename Expr>
struct as_converter < T, GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_scalar >
{
    T operator()( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& e ) const
    {
        return get( e );
    }
};

template <typename T, typename Expr>
struct as_converter < T, GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_vector  >
{
    T operator()( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& e ) const
    {
        return construct<T>( e );
    }
};

template <typename T, typename Expr>
struct as_converter < T, GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_point >
{
    T operator()( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& e ) const
    {
        return construct<T>( e );
    }
};

template <typename T, typename Expr>
struct as_converter < T, GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_matrix >
{
    T operator()( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& e ) const
    {
        return construct<T>( e );
    }
};

template <typename Expr>
struct tensor_traits
    <
        GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr< Expr >
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_scalar
    >
    : GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr_access_policy< Expr > access_policy;
    typedef void                                is_tensor;
    typedef boost::mpl::int_<0>                 tensor_order;
    typedef void                                make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

template <typename Expr>
struct tensor_traits
    <
        const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr< Expr >
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_scalar
    >
    : GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr_access_policy< Expr > access_policy;
    typedef void                                is_tensor;
    typedef boost::mpl::int_<0>                 tensor_order;
    typedef void                                make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

template <typename Expr>
struct tensor_traits
    <
        GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr< Expr >
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_vector
    >
    : GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr_access_policy< Expr > access_policy;
    typedef void                                is_tensor;
    typedef boost::mpl::int_<1>                 tensor_order;
    typedef void                                make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

template <typename Expr>
struct tensor_traits
    <
        GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr< Expr >
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_point
    >
    : GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr_access_policy< Expr > access_policy;
    typedef void                                is_tensor;
    typedef boost::mpl::int_<1>                 tensor_order;
    typedef void                                make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

template <typename Expr>
struct tensor_traits
    <
        GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr< Expr >
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits::is_matrix
    >
    : GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>::traits
{
    typedef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr_access_policy< Expr > access_policy;
    typedef void                                is_tensor;
    typedef boost::mpl::int_<2>                 tensor_order;
    typedef void                                make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
};

template <typename T>
struct is_scalar
    <
        GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>::traits::is_scalar
    > : boost::true_type
{};

template <typename T>
struct is_scalar
    <
        const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>::traits::is_scalar
    > : boost::true_type
{};

template <typename T>
struct numeric_traits
    <
        GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>
      , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>::traits::is_scalar
    >
{
    typedef typename boost::proto::result_of::eval
        <
            T
          , typename GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>::traits::context
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
struct geometric_traits<GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>>
    : GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>::traits
{};

template <typename T>
struct geometric_traits<const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>>
    : GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T>::traits
{};

//! Row/Column
template <typename Expr, std::size_t Column>
struct column< GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, Column>
{
    typedef boost::mpl::int_<Column> index;
    column( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& v )
        : m(v)
    {};

    template <std::size_t Index>
    struct type_at
    {
        typedef typename geometrix::type_at
            <
                GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
              , Index
              , Column
            >::type type;
    };

    template <std::size_t Row>
    typename type_at<Row>::type get() const
    {
        return geometrix::get<Row, Column>( m );
    }

    const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& m;
};

template <typename Expr, std::size_t Column>
struct geometric_traits< column<GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, Column> >
    : diversity_base< GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> >
{
    typedef typename row_dimension_of
        <
            GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
        >::type  dimension_type;
    typedef void is_sequence;
    typedef void is_numeric_sequence;
};

template <typename Expr, std::size_t Column>
struct column<const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, Column>
{
    typedef boost::mpl::int_<Column> index;
    column( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& v )
        : m(v)
    {};

    template <std::size_t Index>
    struct type_at
    {
        typedef typename geometrix::type_at
            <
                GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
              , Index
              , Column
            >::type type;
    };

    template <std::size_t Row>
    typename type_at<Row>::type get() const
    {
        return geometrix::get<Row, Column>( m );
    }

    const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& m;
};

template <typename Expr, std::size_t Column>
struct geometric_traits< column<const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, Column> >
    : diversity_base< GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> >
{
    typedef typename row_dimension_of
        <
            GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
        >::type  dimension_type;
    typedef void is_sequence;
    typedef void is_numeric_sequence;
};

template <typename Expr, std::size_t Row>
struct row< GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, Row>
{
    typedef boost::mpl::int_<Row> index;
    row( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& v )
        : m(v)
    {};

    template <std::size_t Index>
    struct type_at
    {
        typedef typename geometrix::type_at
            <
                GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
              , Row
              , Index
            >::type type;
    };

    template <std::size_t Column>
    typename type_at<Column>::type get() const
    {
        return GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::get<Row, Column>( m );
    }

    const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& m;
};

template <typename Expr, std::size_t Index>
struct geometric_traits< row<GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>,Index> >
    : diversity_base< GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> >
{
    typedef typename column_dimension_of
        <
            GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
        >::type  dimension_type;
    typedef void is_sequence;
    typedef void is_numeric_sequence;
};

template <typename Expr, std::size_t Row>
struct row< const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>, Row>
{
    typedef boost::mpl::int_<Row> index;
    row( const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& v )
        : m(v)
    {};

    template <std::size_t Index>
    struct type_at
    {
        typedef typename geometrix::type_at
            <
                GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
              , Row
              , Index
            >::type type;
    };

    template <std::size_t Column>
    typename type_at<Column>::type get() const
    {
        return GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::get<Row, Column>( m );
    }

    const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>& m;
};

template <typename Expr, std::size_t Index>
struct geometric_traits< row<const GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>,Index> >
    : diversity_base< GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr> >
{
    typedef typename column_dimension_of
        <
            GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<Expr>
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
struct sequence_tag< GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T> >
{
    typedef fusion::fusion_sequence_tag type;
};
template<typename T>
struct sequence_tag<GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE::expr<T> const>
{
    typedef fusion::fusion_sequence_tag type;
};
}}//namespace boost::mpl
