//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_AT_IMPL_HPP
#define _BOOST_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_AT_IMPL_HPP
#pragma once

#include <boost/fusion/support/detail/access.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>

namespace boost 
{
namespace fusion
{
    struct member_function_tag;

    namespace extension
    {
        template<typename T>
        struct at_impl;

        template <typename T, int N>
        struct member_function;

        template <typename T>
        struct member_function_size;

        template <>
        struct at_impl<member_function_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                static int const n_value = N::value;
                BOOST_MPL_ASSERT_RELATION(
                    n_value, <=, extension::member_function_size<Sequence>::value);

                typedef typename
                    extension::member_function<Sequence, N::value>
                    element;

//                 typedef typename
//                     mpl::eval_if<
//                     is_const<Sequence>
//                     , detail::cref_result<element>
//                     , detail::ref_result<element>
//                     >::type
//                     type;

                typedef typename element::type type;


                static type call(Sequence& seq)
                {
                    return extension::member_function<Sequence, N::value>::call(seq);
                }
            };
        };
    }
}}

#endif //_BOOST_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_AT_IMPL_HPP
