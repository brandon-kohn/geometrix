//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FUSION_TENSOR_SEQUENCE_ADAPTOR_HPP
#define GEOMETRIX_FUSION_TENSOR_SEQUENCE_ADAPTOR_HPP

#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>
#include <geometrix/tensor/sequence.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/detail/access.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/fusion/include/mpl.hpp>

namespace boost { namespace fusion {
    
    //! Fusion sequence definitions.
    struct tensor_sequence_tag; //sequence tag for tensor sequence of dimension N.
    struct fusion_sequence_tag;

    namespace traits
    {
        template<typename T>
#if defined(BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS)
        struct tag_of
            <
                T
              , typename geometrix::tensor_traits
                <
                    typename geometrix::remove_const_ref<T>::type
                >::make_fusion_sequence 
            >
#else
        struct tag_of
            <
                T
              , typename geometrix::tensor_traits
                <
                    typename geometrix::remove_const_ref<T>::type
                >::make_fusion_sequence 
            >
#endif
        {
            typedef tensor_sequence_tag type;
        }; 
    }

    struct random_access_traversal_tag;

    template <typename Sequence, int Pos>
    struct tensor_sequence_iterator
        : iterator_facade<tensor_sequence_iterator<Sequence, Pos>, random_access_traversal_tag>
    {
        BOOST_MPL_ASSERT_RELATION(Pos, >=, 0);
        BOOST_MPL_ASSERT_RELATION(Pos, <=, geometrix::geometric_traits<typename geometrix::remove_const_ref<Sequence>::type>::dimension_type::value);

        typedef mpl::int_<Pos> index;
        typedef Sequence tensor_sequence_type;
        
        tensor_sequence_iterator(Sequence& v)
            : m_numeric(v) {}

        Sequence& m_numeric;

        template <typename Iterator>
        struct value_of
        {
            typedef typename geometrix::type_at<Sequence, Iterator::index::value>::type type;
        };

        template <typename Iterator>
        struct deref
        {
            typedef typename value_of<Iterator>::type       type;

            static type call(Iterator const & it)
            {
                return geometrix::get<Iterator::index::value>(it.m_numeric);
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index                index;
            typedef typename Iterator::tensor_sequence_type tensor_sequence_type;
            typedef tensor_sequence_iterator
                <
                    tensor_sequence_type
                  , index::value + N::value
                > type;

            static type call(Iterator const& i)
            {
                return type(i.m_numeric);
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
                mpl::minus
                <
                    typename I2::index, typename I1::index
                >::type 
            type;

            static type call(I1 const&, I2 const&)
            {
                return type();
            }
        };

    private:

        tensor_sequence_iterator<Sequence, Pos>& operator=(tensor_sequence_iterator<Sequence, Pos> const&);

    };

    namespace extension
    {
        template<typename T>
        struct value_at_impl;

        template <>
        struct value_at_impl<tensor_sequence_tag>
        {
            template <typename Sequence, typename N>
            struct apply 
            {
                typedef typename geometrix::type_at<Sequence, N::value>::type type;
            };
        };
    }

    namespace extension
    {
        template<typename T>
        struct size_impl;

        template<>
        struct size_impl<tensor_sequence_tag>
        {
            template<typename Sequence>
            struct apply : mpl::int_<geometrix::geometric_traits<typename geometrix::remove_const_ref<Sequence>::type>::dimension_type::value> {};
        };
    }

    namespace extension
    {
        template<typename Tag>
        struct is_view_impl;

        template<>
        struct is_view_impl<tensor_sequence_tag>
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
        struct is_sequence_impl<tensor_sequence_tag>
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
        struct end_impl<tensor_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef tensor_sequence_iterator
                    <
                        Sequence
                      , geometrix::geometric_traits<typename geometrix::remove_const_ref<Sequence>::type>::dimension_type::value
                    > type;
    
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
        struct category_of_impl<tensor_sequence_tag>
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
        struct begin_impl<tensor_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef tensor_sequence_iterator<Sequence, 0> type;
    
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
        struct at_impl<tensor_sequence_tag>
        {
            template<typename Sequence, typename N>
            struct apply
            {
                typedef typename geometrix::type_at<Sequence, N::value>::type type;

                static type call(Sequence& seq)
                {
                    return geometrix::get<N::value>(seq);
                }
            };
        };
    }
}}//boost::fusion

#endif //GEOMETRIX_FUSION_TENSOR_SEQUENCE_ADAPTOR_HPP
