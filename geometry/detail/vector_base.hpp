//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_DETAIL_VECTOR_BASE_HPP
#define _GENERATIVE_GEOMETRY_DETAIL_VECTOR_BASE_HPP
#pragma once

#include <boost/mpl/assert.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/config/no_tr1/utility.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/config.hpp>

namespace generative 
{ 
namespace numeric
{
namespace geometry
{
    template <typename NumericType, unsigned int D>
    class vector;
}}}

namespace boost 
{
namespace fusion 
{
    //! Fusion sequence definitions.
    struct vector_sequence_tag; //sequence tag for vector of dimension N.

    namespace traits
    {
        template<typename T, unsigned int D>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of< generative::numeric::geometry::vector<T,D>, void >
#else
        struct tag_of< generative::numeric::geometry::vector<T,D> >
#endif
        {
            typedef vector_sequence_tag type;
        };
    }

    struct random_access_traversal_tag;

    template <typename Vector, int Pos>
    struct geometry_vector_iterator
        : iterator_facade<geometry_vector_iterator<Vector, Pos>, random_access_traversal_tag>
    {
        BOOST_MPL_ASSERT_RELATION(Pos, >=, 0);
        BOOST_MPL_ASSERT_RELATION(Pos, <=, Vector::static_size);

        typedef mpl::int_<Pos> index;
        typedef Vector vector_type;

        geometry_vector_iterator(Vector& v)
            : m_vector(v) {}

        Vector& m_vector;

        template <typename Iterator>
        struct value_of
        {
            typedef typename Iterator::vector_type vector_type;
            typedef typename vector_type::value_type type;
        };

        template <typename Iterator>
        struct deref
        {
            typedef typename Iterator::vector_type vector_type;
            typedef typename 
                mpl::if_<
                    is_const<vector_type>
                  , typename vector_type::const_reference
                  , typename vector_type::reference
                >::type 
            type;

            static type
            call(Iterator const & it)
            {
                return it.m_vector[Iterator::index::value];
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index index;
            typedef typename Iterator::vector_type vector_type;
            typedef geometry_vector_iterator<vector_type, index::value + N::value> type;

            static type
            call(Iterator const& i)
            {
                return type(i.m_vector);
            }
        };

        template <typename Iterator>
        struct next : advance<Iterator, mpl::int_<1> > {};

        template <typename Iterator>
        struct prior : advance<Iterator, mpl::int_<-1> > {};

        template <typename I1, typename I2>
        struct distance : mpl::minus<typename I2::index, typename I1::index>
        {
            typedef typename
                mpl::minus<
                    typename I2::index, typename I1::index
                >::type 
            type;

            static type
            call(I1 const&, I2 const&)
            {
                return type();
            }
        };

    private:

        geometry_vector_iterator<Vector, Pos>& operator=(geometry_vector_iterator<Vector, Pos> const&);

    };

    namespace extension
    {
        template<typename T>
        struct value_at_impl;

        template <>
        struct value_at_impl<vector_sequence_tag>
        {
            template <typename Sequence, typename N>
            struct apply 
            {
                typedef typename Sequence::value_type type;
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct size_impl;

        template<>
        struct size_impl<vector_sequence_tag>
        {
            template<typename Sequence>
            struct apply : mpl::int_<Sequence::static_size> {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct is_view_impl;

        template<>
        struct is_view_impl<vector_sequence_tag>
        {
            template<typename T>
            struct apply : mpl::false_
            {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct is_sequence_impl;

        template<>
        struct is_sequence_impl<vector_sequence_tag>
        {
            template<typename Sequence>
            struct apply : mpl::true_ {};
        };
    }

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <>
        struct end_impl<vector_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef geometry_vector_iterator<Sequence, Sequence::static_size> type;
    
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct category_of_impl;

        template<>
        struct category_of_impl<vector_sequence_tag>
        {
            template<typename T>
            struct apply
            {
                typedef random_access_traversal_tag type;
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct begin_impl;

        template <>
        struct begin_impl<vector_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef geometry_vector_iterator<Sequence, 0> type;
    
                static type
                call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct at_impl;

        template<>
        struct at_impl<vector_sequence_tag>
        {
            template<typename Sequence, typename N>
            struct apply
            {
                typedef typename mpl::if_<
                    is_const<Sequence>, 
                    typename Sequence::const_reference, 
                    typename Sequence::reference>::type type;

                static type
                call(Sequence& seq)
                {
                    return seq[N::value];
                }
            };
        };
    }

}}//boost::fusion

namespace generative
{
namespace numeric
{
namespace geometry
{

template <typename NumericType, unsigned int Dimension>
class vector 
{
    	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, VECTOR_NOT_DEFINED
		, (NumericType) );	
};

template <typename NumericType>
class vector<NumericType,0>
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, VECTOR_CANNOT_HAVE_ZERO_DIMENSION
		, (NumericType) );	
};
/*
template <typename NumericType, typename Function>
struct vector_summation
{
    typedef NumericType result_type;

    vector_summation( const Function& f = Function() )
        : m_function( f )
    {}

    template <typename T>
    NumericType operator()( const T& t, const NumericType& sum ) const
    {
        return sum + m_function( t );
    }

    const Function& m_function;

};

// dot product operator.
template <typename NumericType, unsigned int Dimension>
NumericType operator*( const vector<NumericType, Dimension>& v1, const vector<NumericType, Dimension>& v2 )
{
    typedef boost::fusion::vector< const vector<NumericType,Dimension>&, const vector<NumericType,Dimension>& > sequences;
    return boost::fusion::accumulate( boost::fusion::zip_view<sequences>( sequences( v1, v2 ) ),
                                      NumericType(0), 
                                      vector_summation< NumericType, 
                                      boost::fusion::fused< std::multiplies<NumericType> > >() );
}

//! \brief Return the magnitude of the vector squared.
template <typename NumericType, unsigned int Dimension>
NumericType magnitude_squared( const vector<NumericType,Dimension>& v )
{
    return v * v;
}

//! \brief Return the magnitude of the vector.
template <typename NumericType, unsigned int Dimension>
NumericType magnitude( const vector<NumericType,Dimension>& v )
{
    return math_functions<NumericType>::sqrt( v * v );
}

//! \brief Return the magnitude of the vector.
template <typename NumericType, unsigned int Dimension>
vector<NumericType,Dimension> normalize( const vector<NumericType,Dimension>& v )
{
    return ( v / math_functions<NumericType>::sqrt( v * v ) );
}
*/
}}}//namespace generative::numeric::geometry

#endif //_GENERATIVE_GEOMETRY_DETAIL_VECTOR_BASE_HPP


