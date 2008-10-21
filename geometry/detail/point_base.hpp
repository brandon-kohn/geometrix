//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_POINT_BASE_HPP
#define _BOOST_GEOMETRY_DETAIL_POINT_BASE_HPP
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
#include <boost/config.hpp>

namespace boost 
{ 
namespace numeric
{
namespace geometry
{
    template <typename NumericType, unsigned int D>
    class point;
}}}

namespace boost 
{
namespace fusion 
{
    //! Fusion sequence definitions.
    struct point_sequence_tag; //sequence tag for point of dimension N.

    namespace traits
    {
        template<typename T, unsigned int D>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of< boost::numeric::geometry::point<T,D>, void >
#else
        struct tag_of< boost::numeric::geometry::point<T,D> >
#endif
        {
            typedef point_sequence_tag type;
        };
    }

    struct random_access_traversal_tag;

    template <typename Vector, int Pos>
    struct geometry_point_iterator
        : iterator_facade<geometry_point_iterator<Vector, Pos>, random_access_traversal_tag>
    {
        BOOST_MPL_ASSERT_RELATION(Pos, >=, 0);
        BOOST_MPL_ASSERT_RELATION(Pos, <=, Vector::dimension_type::value);

        typedef mpl::int_<Pos> index;
        typedef Vector point_type;

        geometry_point_iterator(Vector& v)
            : m_point(v) {}

        Vector& m_point;

        template <typename Iterator>
        struct value_of
        {
            typedef typename Iterator::point_type point_type;
            typedef typename point_type::value_type type;
        };

        template <typename Iterator>
        struct deref
        {
            typedef typename Iterator::point_type point_type;
            typedef typename 
                mpl::if_<
                    is_const<point_type>
                  , typename point_type::const_reference
                  , typename point_type::reference
                >::type 
            type;

            static type
            call(Iterator const & it)
            {
                return it.m_point[Iterator::index::value];
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index index;
            typedef typename Iterator::point_type point_type;
            typedef geometry_point_iterator<point_type, index::value + N::value> type;

            static type
            call(Iterator const& i)
            {
                return type(i.m_point);
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

        geometry_point_iterator<Vector, Pos>& operator=(geometry_point_iterator<Vector, Pos> const&);

    };

    namespace extension
    {
        template<typename T>
        struct value_at_impl;

        template <>
        struct value_at_impl<point_sequence_tag>
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
        struct size_impl<point_sequence_tag>
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
        struct is_view_impl<point_sequence_tag>
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
        struct is_sequence_impl<point_sequence_tag>
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
        struct end_impl<point_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef geometry_point_iterator<Sequence, Sequence::static_size> type;
    
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
        struct category_of_impl<point_sequence_tag>
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
        struct begin_impl<point_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef geometry_point_iterator<Sequence, 0> type;
    
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
        struct at_impl<point_sequence_tag>
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

}}

namespace boost
{
namespace numeric
{
namespace geometry
{

template <typename NumericType, unsigned int Dimension>
class point 
{
    	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POINT_DIMENSION_NOT_DEFINED
		, (NumericType) );	
};

template <typename NumericType>
class point<NumericType,0>
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POINT_CANNOT_HAVE_ZERO_DIMENSION
		, (NumericType) );	
};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_DETAIL_POINT_BASE_HPP


