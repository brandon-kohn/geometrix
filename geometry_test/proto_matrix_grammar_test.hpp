///////////////////////////////////////////////////////////////////////////////
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This example constructs a mini-library for linear algebra, using
// expression templates to eliminate the need for temporaries when
// adding arrays of numbers. It duplicates the vector example from
// PETE (http://www.codesourcery.com/pooma/download.html)

#include "vector_kernal.hpp"

#include "../geometry/vector.hpp"
#include "../geometry/point.hpp"
#include "../geometry/affine_space.hpp"
#include "../geometry/indexed_access_traits.hpp"

#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/proto/core.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/context.hpp>
#include <boost/utility/enable_if.hpp>

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;
using namespace generative::numeric::geometry;

// Here is indexed_sequence_expression, which extends a proto expr type by
// giving it an operator [] which uses the indexed_sequence_subscript_context
// to evaluate an expression with a given index.

template <typename Expr>
struct indexed_sequence_expression;

// Here is an evaluation context that indexes into a std::vector
// expression and combines the result.
struct indexed_sequence_subscript_context
{
    indexed_sequence_subscript_context(std::size_t i)
        : i_(i)
    {}

    // Unless this is a vector terminal, use the
    // default evaluation context
    template<typename Expr, typename EnableIf = void>
    struct eval
        : proto::default_eval<Expr, indexed_sequence_subscript_context const>
    {};

    // Index vector terminals with our subscript.
    template<typename Expr>
    struct eval<
        Expr
        , typename boost::enable_if
        <
            typename mpl::and_< proto::matches<Expr, proto::terminal< _ > >, 
                                is_indexed_sequence< typename proto::result_of::value<Expr>::type > >::type 
        >::type
    >
    {
        //typedef typename proto::result_of::value<Expr>::type::value_type result_type;
        typedef typename indexed_access_traits< typename proto::result_of::value<Expr>::type >::value_type result_type;

        result_type operator ()(Expr &expr, indexed_sequence_subscript_context const &ctx) const
        {
            return proto::value(expr)[ctx.i_];
        }
    };

    std::size_t i_;
};

// Here is an evaluation context that verifies that all the
// vectors in an expression have the same size.
struct indexed_sequence_size_context
{
    indexed_sequence_size_context(std::size_t size)
        : size_(size)
    {}

    // Unless this is a vector terminal, use the
    // null evaluation context
    template<typename Expr, typename EnableIf = void>
    struct eval
        : proto::null_eval<Expr, indexed_sequence_size_context const>
    {};

    // Index array terminals with our subscript. Everything
    // else will be handled by the default evaluation context.
    template<typename Expr>
    struct eval<
        Expr
        , typename boost::enable_if<
            is_indexed_sequence< typename proto::result_of::value<Expr>::type >
        >::type
    >
    {
        typedef void result_type;

        result_type operator ()(Expr &expr, indexed_sequence_size_context const &ctx) const
        {
            if(ctx.size_ != proto::value(expr).size())
            {
                throw std::runtime_error("LHS and RHS are not compatible");
            }
        }
    };

    std::size_t size_;
};

// A grammar which matches all the assignment operators,
// so we can easily disable them.
struct AssignOps
    : proto::switch_<struct AssignOpsCases>
{};

// Here are the cases used by the switch_ above.
struct AssignOpsCases
{
    template<typename Tag, int D = 0> struct case_  : proto::not_<_> {};

    template<int D> struct case_< proto::tag::plus_assign, D >         : _ {};
    template<int D> struct case_< proto::tag::minus_assign, D >        : _ {};
    template<int D> struct case_< proto::tag::multiplies_assign, D >   : _ {};
    template<int D> struct case_< proto::tag::divides_assign, D >      : _ {};
    template<int D> struct case_< proto::tag::modulus_assign, D >      : _ {};
    template<int D> struct case_< proto::tag::shift_left_assign, D >   : _ {};
    template<int D> struct case_< proto::tag::shift_right_assign, D >  : _ {};
    template<int D> struct case_< proto::tag::bitwise_and_assign, D >  : _ {};
    template<int D> struct case_< proto::tag::bitwise_or_assign, D >   : _ {};
    template<int D> struct case_< proto::tag::bitwise_xor_assign, D >  : _ {};
};

// A vector grammar is a terminal or some op that is not an
// assignment op. (Assignment will be handled specially.)
struct indexed_sequence_arithmetic_grammer
    : proto::or_<
        proto::terminal<_>
        , proto::and_<proto::nary_expr<_, proto::vararg<indexed_sequence_arithmetic_grammer> >, proto::not_<AssignOps> >
    >
{};

// Expressions in the vector domain will be wrapped in indexed_sequence_expression<>
// and must conform to the indexed_sequence_arithmetic_grammer
struct indexed_sequence_domain
    : proto::domain<proto::generator<indexed_sequence_expression>, indexed_sequence_arithmetic_grammer>
{};

template <typename Expr, typename EnableIf = void>
struct terminal_sequence_traits
{
    typedef void         value_type;
    typedef void*        sequence_type;
    typedef dimension<1> dimension_type;
};

template <typename T>
struct always_true : mpl::true_ {};

template <typename T>
struct always_false : mpl::false_ {};

template <typename Expr>
struct terminal_sequence_traits
       <
           Expr,
           typename boost::enable_if
           <
               is_indexed_sequence
               <
                   typename proto::result_of::value< Expr >::type 
               >
           >::type 
       >
{
    typedef typename proto::result_of::value< Expr >::type                  sequence_type;    
    typedef typename indexed_access_traits< sequence_type >::value_type     value_type;
    typedef typename indexed_access_traits< sequence_type >::dimension_type dimension_type;
};

template <typename Expr>
struct indexed_sequence_expression
    : proto::extends<Expr, indexed_sequence_expression<Expr>, indexed_sequence_domain>
{
    typedef typename terminal_sequence_traits< Expr >::sequence_type  sequence_type;
    typedef typename terminal_sequence_traits< Expr >::value_type     value_type;
    typedef typename terminal_sequence_traits< Expr >::dimension_type dimension_type;

    explicit indexed_sequence_expression(Expr const &expr)
        : proto::extends<Expr, indexed_sequence_expression<Expr>, indexed_sequence_domain>(expr)
    {}

    // Use the indexed_sequence_subscript_context to implement subscripting
    // of a IndexedSequence expression tree.
    typename proto::result_of::eval<Expr const, indexed_sequence_subscript_context const>::type
        operator []( std::size_t i ) const
    {
        indexed_sequence_subscript_context const ctx(i);
        return proto::eval(*this, ctx);
    }
};

namespace generative { namespace numeric { namespace geometry {                 
template <typename T>
struct is_sequence< indexed_sequence_expression<T> > : boost::true_type{};             

template <typename T>
struct sequence_traits< indexed_sequence_expression<T> >
{
    typedef typename indexed_sequence_expression<T>::sequence_type  sequence_type;
    typedef typename indexed_sequence_expression<T>::value_type     value_type;
    typedef typename indexed_sequence_expression<T>::dimension_type dimension_type;
    typedef value_type&                                     reference;                         
    typedef const value_type&                               const_reference;                   
};                                                                         
}}}                                                                        

template< typename T >
struct generative::numeric::geometry::is_indexed_sequence
<
    indexed_sequence_expression< T >, 
    typename boost::enable_if
    <
        is_indexed_sequence
        <
            typename proto::result_of::value< T >::type 
        >
    >::type 
> : boost::true_type
{};

template < typename T >
struct generative::numeric::geometry::indexed_access_policy
<
    indexed_sequence_expression< T >
>
: boost::integral_constant<indexed_sequence_access_type, require_run_time_access_policy::value> {};

template <typename T, typename EnableIf = void>
struct is_sequence_expression : mpl::false_{};

template <typename Expr>
struct is_sequence_expression< indexed_sequence_expression< Expr > > : mpl::true_ {};

template <typename T, typename EnableIf = void>
struct is_sequence_terminal : mpl::false_ 
{};

template <typename T>
struct is_sequence_terminal
<
    T,
    typename boost::enable_if
    < 
        typename mpl::and_
        <
            typename mpl::not_< is_sequence_expression< T > >::type,
            typename mpl::not_< is_indexed_sequence< T > >::type
        >::type            
    >::type
> : mpl::true_ 
{};

namespace IndexedSequenceOps
{
    // This defines all the overloads to make expressions involving
    // std::vector to build expression templates.
    BOOST_PROTO_DEFINE_OPERATORS( is_indexed_sequence, indexed_sequence_domain )

    typedef indexed_sequence_subscript_context const CIndexedSequenceSubscriptCtx;
}

BOOST_AUTO_TEST_CASE( TestProtoExpressions )
{
    using namespace IndexedSequenceOps;

    typedef point_vector_3 Point;
    
    Point a,b,c,d;
    Point e(1,1,1);
    Point f(2,2,2);

    for( int i = 0; i < 3; ++i)
    {
        a[i] = i;
        b[i] = (2*i);
        c[i] = (3*i);
        d[i] = (i);
    }

    BOOST_ASSERT( is_indexed_sequence< Point >::value );
    BOOST_ASSERT( ( boost::is_same< indexed_access_traits< Point >::value_type, double >::value ) );
    e = construction_traits<Point>::construct( e + f );
    BOOST_CHECK_CLOSE( indexed_access_traits<Point>::get<0>( e ), 3., 1e-10 );
    BOOST_CHECK_CLOSE( indexed_access_traits<Point>::get<1>( e ), 3., 1e-10 );
    BOOST_CHECK_CLOSE( indexed_access_traits<Point>::get<2>( e ), 3., 1e-10 );

    e = construction_traits< Point >::construct( e * 4 );
    BOOST_CHECK_CLOSE( indexed_access_traits<Point>::get<0>( e ), 12., 1e-10 );
    BOOST_CHECK_CLOSE( indexed_access_traits<Point>::get<1>( e ), 12., 1e-10 );
    BOOST_CHECK_CLOSE( indexed_access_traits<Point>::get<2>( e ), 12., 1e-10 );

    typedef BOOST_TYPEOF( e - 4 / (c + 1) ) SpecificExpression;

    BOOST_AUTO( expr, e - 4 / (c + 1) );

    BOOST_ASSERT( is_indexed_sequence< SpecificExpression >::value );
//     std::cout << typeid( indexed_access_traits< SpecificExpression >::sequence_type ).name() << std::endl;
//     std::cout << typeid( indexed_access_traits< SpecificExpression >::value_type ).name() << std::endl;
//     std::cout << typeid( indexed_access_traits< SpecificExpression >::dimension_type ).name() << std::endl;

    double x0 = proto::as_expr<indexed_sequence_domain>(expr)[0];
    double x = indexed_access_traits< SpecificExpression >::get<0>( proto::as_expr<indexed_sequence_domain>(expr) );

    BOOST_ASSERT( x0 == x );

    std::cout << "x: " << x << std::endl;
    //e = construction_traits<Point>::construct( proto::as_expr<indexed_sequence_domain>( e - 4 / (c + 1) ) );
    e = construction_traits<Point>::construct( e - 4 / (c + 1) );

    for( int i = 0; i < 3; ++i)
    {
        std::cout
            << " a(" << i << ") = " << a[i]
            << " b(" << i << ") = " << b[i]
            << " c(" << i << ") = " << c[i]
            << " d(" << i << ") = " << d[i]
            << " e(" << i << ") = " << e[i]
            << std::endl;
    }
}

/*
///////////////////////////////////////////////////////////////////////////////
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This is an example of using BOOST_PROTO_DEFINE_OPERATORS to Protofy
// expressions using std::vector<>, a non-proto type. It is a port of the
// Vector example from PETE (http://www.codesourcery.com/pooma/download.html).

#include <vector>
#include <iostream>
#include <stdexcept>
#include <boost/mpl/bool.hpp>
#include <boost/proto/core.hpp>
#include <boost/proto/debug.hpp>
#include <boost/proto/context.hpp>
#include <boost/utility/enable_if.hpp>
namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

template<typename Expr>
struct VectorExpr;

// Here is an evaluation context that indexes into a std::vector
// expression and combines the result.
struct VectorSubscriptCtx
{
    VectorSubscriptCtx(std::size_t i)
        : i_(i)
    {}

    // Unless this is a vector terminal, use the
    // default evaluation context
    template<typename Expr, typename EnableIf = void>
    struct eval
        : proto::default_eval<Expr, VectorSubscriptCtx const>
    {};

    // Index vector terminals with our subscript.
    template<typename Expr>
    struct eval<
        Expr
        , typename boost::enable_if<
        proto::matches<Expr, proto::terminal<std::vector<_, _> > >
        >::type
    >
    {
        typedef typename proto::result_of::value<Expr>::type::value_type result_type;

        result_type operator ()(Expr &expr, VectorSubscriptCtx const &ctx) const
        {
            return proto::value(expr)[ctx.i_];
        }
    };

    std::size_t i_;
};

// Here is an evaluation context that verifies that all the
// vectors in an expression have the same size.
struct VectorSizeCtx
{
    VectorSizeCtx(std::size_t size)
        : size_(size)
    {}

    // Unless this is a vector terminal, use the
    // null evaluation context
    template<typename Expr, typename EnableIf = void>
    struct eval
        : proto::null_eval<Expr, VectorSizeCtx const>
    {};

    // Index array terminals with our subscript. Everything
    // else will be handled by the default evaluation context.
    template<typename Expr>
    struct eval<
        Expr
        , typename boost::enable_if<
        proto::matches<Expr, proto::terminal<std::vector<_, _> > >
        >::type
    >
    {
        typedef void result_type;

        result_type operator ()(Expr &expr, VectorSizeCtx const &ctx) const
        {
            if(ctx.size_ != proto::value(expr).size())
            {
                throw std::runtime_error("LHS and RHS are not compatible");
            }
        }
    };

    std::size_t size_;
};

// A grammar which matches all the assignment operators,
// so we can easily disable them.
struct AssignOps
    : proto::switch_<struct AssignOpsCases>
{};

// Here are the cases used by the switch_ above.
struct AssignOpsCases
{
    template<typename Tag, int D = 0> struct case_  : proto::not_<_> {};

    template<int D> struct case_< proto::tag::plus_assign, D >         : _ {};
    template<int D> struct case_< proto::tag::minus_assign, D >        : _ {};
    template<int D> struct case_< proto::tag::multiplies_assign, D >   : _ {};
    template<int D> struct case_< proto::tag::divides_assign, D >      : _ {};
    template<int D> struct case_< proto::tag::modulus_assign, D >      : _ {};
    template<int D> struct case_< proto::tag::shift_left_assign, D >   : _ {};
    template<int D> struct case_< proto::tag::shift_right_assign, D >  : _ {};
    template<int D> struct case_< proto::tag::bitwise_and_assign, D >  : _ {};
    template<int D> struct case_< proto::tag::bitwise_or_assign, D >   : _ {};
    template<int D> struct case_< proto::tag::bitwise_xor_assign, D >  : _ {};
};

// A vector grammar is a terminal or some op that is not an
// assignment op. (Assignment will be handled specially.)
struct VectorGrammar
    : proto::or_<
    proto::terminal<_>
    , proto::and_<proto::nary_expr<_, proto::vararg<VectorGrammar> >, proto::not_<AssignOps> >
    >
{};

// Expressions in the vector domain will be wrapped in VectorExpr<>
// and must conform to the VectorGrammar
struct VectorDomain
    : proto::domain<proto::generator<VectorExpr>, VectorGrammar>
{};

// Here is VectorExpr, which extends a proto expr type by
// giving it an operator [] which uses the VectorSubscriptCtx
// to evaluate an expression with a given index.
template<typename Expr>
struct VectorExpr
    : proto::extends<Expr, VectorExpr<Expr>, VectorDomain>
{
    explicit VectorExpr(Expr const &expr)
        : proto::extends<Expr, VectorExpr<Expr>, VectorDomain>(expr)
    {}

    // Use the VectorSubscriptCtx to implement subscripting
    // of a Vector expression tree.
    typename proto::result_of::eval<Expr const, VectorSubscriptCtx const>::type
        operator []( std::size_t i ) const
    {
        VectorSubscriptCtx const ctx(i);
        return proto::eval(*this, ctx);
    }
};

// Define a trait type for detecting vector terminals, to
// be used by the BOOST_PROTO_DEFINE_OPERATORS macro below.
template<typename T>
struct IsVector
    : mpl::false_
{};

template<typename T, typename A>
struct IsVector<std::vector<T, A> >
    : mpl::true_
{};

namespace VectorOps
{
    // This defines all the overloads to make expressions involving
    // std::vector to build expression templates.
    BOOST_PROTO_DEFINE_OPERATORS(IsVector, VectorDomain)

        typedef VectorSubscriptCtx const CVectorSubscriptCtx;

    // Assign to a vector from some expression.
    template<typename T, typename A, typename Expr>
    std::vector<T, A> &assign(std::vector<T, A> &arr, Expr const &expr)
    {
        VectorSizeCtx const size(arr.size());
        //proto::eval(proto::as_expr<VectorDomain>(expr), size); // will throw if the sizes don't match
        for(std::size_t i = 0; i < arr.size(); ++i)
        {
            arr[i] = proto::as_expr<VectorDomain>(expr)[i];
        }
        return arr;
    }

    // Add-assign to a vector from some expression.
    template<typename T, typename A, typename Expr>
    std::vector<T, A> &operator +=(std::vector<T, A> &arr, Expr const &expr)
    {
        VectorSizeCtx const size(arr.size());
        //proto::eval(proto::as_expr<VectorDomain>(expr), size); // will throw if the sizes don't match
        for(std::size_t i = 0; i < arr.size(); ++i)
        {
            //arr[i] += proto::as_expr<VectorDomain>(expr)[i];
            arr[i] += expr[i];
        }
        return arr;
    }
}

BOOST_AUTO_TEST_CASE( VectorProto )
{
    using namespace VectorOps;

    int i;
    const int n = 3;
    std::vector<int> a,b,c,d;
    std::vector<double> e(n);

    for (i = 0; i < n; ++i)
    {
        a.push_back(i);
        b.push_back(2*i);
        c.push_back(3*i);
        d.push_back(i);
    }

    e += e + a;

    VectorOps::assign(b, 2);
    VectorOps::assign(d, a + b * c);
    a += if_else(d < 30, b, c);

    VectorOps::assign(e, c);
    e += e - 4 / (c + 1);

    for (i = 0; i < n; ++i)
    {
        std::cout
            << " a(" << i << ") = " << a[i]
        << " b(" << i << ") = " << b[i]
        << " c(" << i << ") = " << c[i]
        << " d(" << i << ") = " << d[i]
        << " e(" << i << ") = " << e[i]
        << std::endl;
    }
}
*/