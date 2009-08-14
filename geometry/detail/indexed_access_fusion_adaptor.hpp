//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_DETAIL_INDEXED_ACCESS_FUSION_ADAPTOR_HPP
#define GENERATIVE_GEOMETRY_DETAIL_INDEXED_ACCESS_FUSION_ADAPTOR_HPP
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

namespace generative 
{ 
namespace numeric
{
namespace geometry
{
    //! \brief An adaptor to mark indexed_sequences. This is required so fusion's specialization mechanisms have a handle on the dispatch type.
    template <typename Sequence>
    struct indexed_access_fusion_adaptor : public indexed_access_traits< Sequence >
    {
        typedef Sequence                                                   indexed_sequence_type;
        typedef typename indexed_access_traits< Sequence >::dimension_type dimension_type;

        indexed_access_fusion_adaptor( Sequence& sequence )
            : m_sequence( sequence ) 
        {}

        Sequence& m_sequence;
    };

    template <typename Sequence>
    struct const_indexed_access_fusion_adaptor : public indexed_access_traits< Sequence >
    {
        typedef const Sequence                                             indexed_sequence_type;
        typedef typename indexed_access_traits< Sequence >::dimension_type dimension_type;

        const_indexed_access_fusion_adaptor( const Sequence& sequence )
            : m_sequence( sequence ) 
        {}

        const Sequence& m_sequence;
    };
}}}//namespace generative::numeric::geometry

namespace boost 
{
namespace fusion 
{
    //! Fusion sequence definitions.
    struct indexed_access_sequence_tag; //sequence tag for sequence of dimension N.

    namespace traits
    {
        template<typename Sequence>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of< generative::numeric::geometry::indexed_access_fusion_adaptor<Sequence>, void >
#else
        struct tag_of< generative::numeric::geometry::indexed_access_fusion_adaptor<Sequence> >
#endif
        {
            typedef indexed_access_sequence_tag type;
        };

        template<typename Sequence, typename ReferenceFrame>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of< generative::numeric::geometry::reference_frame_tag<Sequence, ReferenceFrame>, void >
#else
        struct tag_of< generative::numeric::geometry::reference_frame_tag<Sequence, ReferenceFrame> >
#endif
        {
            typedef indexed_access_sequence_tag type;
        };

        template<typename Sequence>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of< generative::numeric::geometry::const_indexed_access_fusion_adaptor<Sequence>, void >
#else
        struct tag_of< generative::numeric::geometry::const_indexed_access_fusion_adaptor<Sequence> >
#endif
        {
            typedef indexed_access_sequence_tag type;
        };
    }

    struct random_access_traversal_tag;

    template <typename IndexedSequence, int Pos>
    struct indexed_access_iterator
        : iterator_facade<indexed_access_iterator<IndexedSequence, Pos>, random_access_traversal_tag>
    {
        BOOST_MPL_ASSERT_RELATION( Pos, >=, 0 );
        BOOST_MPL_ASSERT_RELATION( Pos, <=, IndexedSequence::dimension_type::value );

        typedef mpl::int_<Pos>  index;
        typedef IndexedSequence indexed_sequence_type;

        indexed_access_iterator(IndexedSequence& v)
            : m_sequence(v) {}

        IndexedSequence& m_sequence;

        template <typename Iterator>
        struct value_of
        {
            typedef typename Iterator::indexed_sequence_type indexed_sequence_type;
            typedef typename indexed_sequence_type::value_type type;
        };

        template <typename Iterator>
        struct deref
        {
            typedef typename Iterator::indexed_sequence_type indexed_sequence_type;
            typedef typename 
                mpl::if_<
                  is_const<typename Iterator::indexed_sequence_type::indexed_sequence_type>
                  , typename indexed_sequence_type::const_reference
                  , typename indexed_sequence_type::reference
                >::type 
            type;
            typedef typename indexed_sequence_type::value_type return_type;

            static return_type call(Iterator const & it)
            {
                return indexed_sequence_type::get<Iterator::index::value>( it.m_sequence.m_sequence );
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index index;
            typedef typename Iterator::indexed_sequence_type indexed_sequence_type;
            typedef indexed_access_iterator<indexed_sequence_type, index::value + N::value> type;

            static type
            call(Iterator const& i)
            {
                return type(i.m_sequence);
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

        indexed_access_iterator<IndexedSequence, Pos>& operator=(indexed_access_iterator<IndexedSequence, Pos> const&);

    };

    namespace extension
    {
        template<typename T>
        struct value_at_impl;

        template <>
        struct value_at_impl<indexed_access_sequence_tag>
        {
            template <typename Sequence, typename N>
            struct apply 
            {
                typedef typename Sequence::indexed_type type;
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct size_impl;

        template<>
        struct size_impl<indexed_access_sequence_tag>
        {
            template<typename Sequence>
            struct apply : mpl::int_<Sequence::dimension_type::value> {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct is_view_impl;

        template<>
        struct is_view_impl<indexed_access_sequence_tag>
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
        struct is_sequence_impl<indexed_access_sequence_tag>
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
        struct end_impl<indexed_access_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef indexed_access_iterator<Sequence, Sequence::dimension_type::value> type;
    
                static type call(Sequence& v)
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
        struct category_of_impl<indexed_access_sequence_tag>
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
        struct begin_impl<indexed_access_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef indexed_access_iterator<Sequence, 0> type;
    
                static type call(Sequence& v)
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
        struct at_impl<indexed_access_sequence_tag>
        {
            template<typename Sequence, typename N>
            struct apply
            {
                typedef typename mpl::if_<
                    is_const<Sequence>, 
                    typename Sequence::const_reference, 
                    typename Sequence::reference>::type type;

                typedef typename Sequence::value_type return_type;

                static return_type call(Sequence& seq)
                {
                    return Sequence::get<N::value>( seq.m_sequence );
                }
            };
        };
    }

}}//boost::fusion

#endif //GENERATIVE_GEOMETRY_DETAIL_INDEXED_ACCESS_FUSION_ADAPTOR_HPP
