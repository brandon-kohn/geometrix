///////////////////////////////////////////////////////////////////////////////
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This example constructs a mini-library for linear algebra, using
// expression templates to eliminate the need for temporaries when
// adding arrays of numbers. It duplicates the vector example from
// PETE (http://www.codesourcery.com/pooma/download.html)

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/mpl/int.hpp>
#include <boost/proto/core.hpp>
#include <boost/proto/context.hpp>
#include "../geometry/vector.hpp"
#include "../geometry/point.hpp"
#include "../geometry/affine_space.hpp"

namespace mpl = boost::mpl;
namespace proto = boost::proto;
using proto::_;

namespace geometry = boost::numeric::geometry;
using namespace boost::numeric::geometry;

struct stream_f
{
    stream_f( std::ostream& os, std::string delim )
        : m_os( os )
        , m_delim( delim )
    {}

    template <typename T>
    void operator() ( const T& t ) const
    {
        m_os << t << m_delim;
    }

    std::string   m_delim;
    std::ostream& m_os;
};

template <typename T, typename Expr>
struct assign_from_expression
{
    assign_from_expression( T& t, const Expr& e )
        : m_i( 0 )
        , m_t( t )
        , m_e( e )
    {}
        
    template <typename E>
    void operator() ( const E& e ) const
    {
        m_t[m_i] = m_e[m_i];
        ++m_i;
    }
    
    mutable size_t m_i;
    T&             m_t;
    const Expr&    m_e;
};

template <typename AffineSpace>
struct affine_arithmetic
{
    typedef typename affine_space_traits< AffineSpace >::numeric_type   numeric_type;
    typedef typename affine_space_traits< AffineSpace >::dimension_type dimension_type;
    typedef point<numeric_type, dimension_type::value>                  point_type;
    typedef vector<numeric_type, dimension_type::value>                 vector_type;

    // This grammar describes which vector expressions
    // are allowed; namely, int and array terminals
    // plus, minus, multiplies and divides of vector expressions.
    struct vector_grammar
      : proto::or_<
            proto::terminal< numeric_type >
          , proto::terminal< vector_type >
          , proto::terminal< point_type >
          , proto::plus< vector_grammar, vector_grammar >
          , proto::minus< vector_grammar, vector_grammar >
          , proto::multiplies< vector_grammar, vector_grammar >
          , proto::divides< vector_grammar, vector_grammar >
        >
    {};

    template <typename Expr>
    struct vector_expression;

    // Tell proto that in the vector_domain, all
    // expressions should be wrapped in vector_expression<> and
    // must conform to the vector_grammar    
    struct vector_domain
      : proto::domain< proto::generator< vector_expression >, vector_grammar >
    {};

    // Here is an evaluation context that indexes into a vector
    // expression, and combines the result.
    struct cell_index
      : proto::callable_context< cell_index const >
    {
        typedef numeric_type result_type;

        cell_index(std::ptrdiff_t i)
          : i_(i)
        {}

        // Index array terminals with our subscript. Everything
        // else will be handled by the default evaluation context.
        numeric_type operator()(proto::tag::terminal, const vector_type& v ) const
        {
            return indexed_access_traits< vector_type >::get( v, this->i_ );
        }

        // Index array terminals with our subscript. Everything
        // else will be handled by the default evaluation context.
        numeric_type operator()(proto::tag::terminal, const point_type& v ) const
        {
            return indexed_access_traits< point_type >::get( v, this->i_ );
        }

        std::ptrdiff_t i_;
    };

    // Here is an evaluation context that prints a vector expression.
    struct stream_cell_index
      : proto::callable_context< stream_cell_index const >
    {
        typedef std::ostream &result_type;

        stream_cell_index( std::ostream& os )
            : m_os( os )
        {}

        std::ostream &operator ()(proto::tag::terminal, numeric_type i) const
        {
            return m_os << i;
        }
        
        std::ostream &operator ()(proto::tag::terminal, const vector_type& v ) const
        {
            boost::fusion::for_each( v, stream_f( m_os, " " ) );
            return m_os;
        }
        
        std::ostream &operator ()(proto::tag::terminal, const point_type& v ) const
        {
            boost::fusion::for_each( v, stream_f( m_os, " " ) );            
            return m_os;
        }

        template<typename L, typename R>
        std::ostream &operator ()(proto::tag::plus, L const &l, R const &r) const
        {
            return m_os << '(' << l << " + " << r << ')';
        }

        template<typename L, typename R>
        std::ostream &operator ()(proto::tag::minus, L const &l, R const &r) const
        {
            return m_os << '(' << l << " - " << r << ')';
        }

        template<typename L, typename R>
        std::ostream &operator ()(proto::tag::multiplies, L const &l, R const &r) const
        {
            return m_os << l << " * " << r;
        }

        template<typename L, typename R>
        std::ostream &operator ()(proto::tag::divides, L const &l, R const &r) const
        {
            return m_os << l << " / " << r;
        }

        std::ostream& m_os;

    };

    // Here is the domain-specific expression wrapper, which overrides
    // operator [] to evaluate the expression using the cell_index.
    template<typename Expr>
    struct vector_expression
      : proto::extends<Expr, vector_expression<Expr>, vector_domain >
    {
        typedef proto::extends<Expr, vector_expression<Expr>, vector_domain > base_type;

        vector_expression( Expr const & expr = Expr() )
          : base_type( expr )
        {}

        // Use the cell_index to implement subscripting
        // of a vector expression tree.
        numeric_type operator []( std::ptrdiff_t i ) const
        {
            cell_index const ctx(i);
            return proto::eval(*this, ctx);
        }

        // Use the stream_cell_index to display a vector expression tree.
        friend std::ostream &operator <<(std::ostream &sout, vector_expression<Expr> const &expr)
        {
            stream_cell_index const ctx( sout );
            return proto::eval(expr, ctx);
        }
    };

    // Here is our vector terminal, implemented in terms of vector_expression
    // It is basically just an array of 3 integers.
    template < typename Vector >
    struct vector_as_expr
      : vector_expression< typename proto::terminal< Vector >::type >
    {
        typedef typename Vector::const_iterator const_iterator;

        explicit vector_as_expr( Vector& v )
        {
            this->assign( proto::as_expr< vector_domain >( v ) );
        }

        // Here we override operator [] to give read/write access to
        // the elements of the array. (We could use the vector_expression
        // operator [] if we made the subscript context smarter about
        // returning non-const reference when appropriate.)
        typename indexed_access_traits< Vector >::indexed_type& operator []( std::ptrdiff_t i )
        {
            return proto::value(*this)[i];
        }

        const typename indexed_access_traits< Vector >::indexed_type& operator []( std::ptrdiff_t i ) const
        {
            return proto::value(*this)[i];
        }

        // Here we define a operator = for vector terminals that
        // takes a vector expression.
        template< typename Expr >
        vector_as_expr& operator =(Expr const & expr)
        {
            // proto::as_expr<vector_domain>(expr) is the same as
            // expr unless expr is an integer, in which case it
            // is made into a vector_expression terminal first.
            return this->assign( proto::as_expr< vector_domain >(expr) );
        }

        template< typename Expr >
        vector_as_expr& printAssign( const Expr& expr )
        {
            *this = expr;
            std::cout << *this << " = " << expr << std::endl;
            return *this;
        }

    private:

        template <typename Expr>
        vector_as_expr& assign( const Expr& expr )
        {
            boost::fusion::for_each( proto::value(*this), assign_from_expression< vector_as_expr, Expr >( *this, expr ) );
            return *this;
        }

    };
};

BOOST_AUTO_TEST_CASE( TestMatrixExpressionGrammar )
{
    using namespace boost::numeric::geometry;
    using namespace boost::numeric::geometry::detail;

    typedef affine_arithmetic< boost::numeric::geometry::affine_space_double_3d > affine_arithmetic_3d;

    typedef numeric_sequence< double, 3 > double_3;
    typedef point< double, 3 >            point_3;
    typedef vector< double, 3 >           vector_3;

    vector_3 a(3.,1.,2.);
    affine_arithmetic_3d::vector_as_expr< vector_3 > ax( a );
    vector_3 b(7.,33.,-99.);
    affine_arithmetic_3d::vector_as_expr< vector_3 > bx( b );

    std::cout << ax << std::endl;
    std::cout << bx << std::endl;
    
    vector_3 c(a);
    affine_arithmetic_3d::vector_as_expr< vector_3 > cx( c );
    
    std::cout << cx << std::endl;

    ax = 0.;

    std::cout << ax << std::endl;
    std::cout << bx << std::endl;
    std::cout << cx << std::endl;

    ax = bx + cx;

    std::cout << ax << std::endl;

    //ax.printAssign(bx+cx*(bx + 3.*cx));

    3.0 * cx;
    //ax = bx+cx*(bx + 3.*cx);
    std::cout << ax << std::endl;

}
