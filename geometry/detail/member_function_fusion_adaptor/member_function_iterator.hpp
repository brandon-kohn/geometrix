//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_MEMBER_FUNCTION_ITERATOR_HPP
#define _GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_MEMBER_FUNCTION_ITERATOR_HPP
#pragma once

#include <boost/fusion/iterator/iterator_facade.hpp>
#include "extension.hpp"
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/config/no_tr1/utility.hpp>

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    template <typename T, int N_>
    struct member_function_iterator
        : iterator_facade<member_function_iterator<T, N_>, random_access_traversal_tag>
    {
        BOOST_MPL_ASSERT_RELATION(N_, >=, 0);
        BOOST_MPL_ASSERT_RELATION(N_, <=, extension::member_function_size<T>::value);

        typedef mpl::int_<N_> index;
        typedef T sequence_type;

        member_function_iterator( sequence_type& instance)
            : m_instance(instance)
        {}

        sequence_type& m_instance;

        template <typename Iterator>
        struct value_of
            : extension::member_function<T, N_>
        {
        };

        template <typename Iterator>
        struct deref
        {
//             typedef typename
//                 mpl::if_< 
//                     is_const<sequence_type>,
//                     typename add_reference< typename add_const< typename extension::member_function<T, N_>::type >::type >::type,
//                     typename add_reference< typename extension::member_function<T, N_>::type >::type
//                 >::type
//                 type;

            typedef typename extension::member_function<T, N_>::type type;

            static type call(Iterator const& iter)
            {
                return extension::member_function<T, N_>::call(iter.m_instance);
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index index;
            typedef typename Iterator::sequence_type sequence_type;
            typedef member_function_iterator<sequence_type, index::value + N::value> type;

            static type call(Iterator const& iter)
            {
                return type(iter.m_instance);
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

            static type call(I1 const&, I2 const&)
            {
                return type();
            }
        };
    };
}}

#endif //_GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_MEMBER_FUNCTION_ITERATOR_HPP
