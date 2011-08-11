//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_VALUE_AT_IMPL_HPP
#define GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_VALUE_AT_IMPL_HPP


#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>

namespace boost 
{
namespace fusion
{
    struct member_function_tag;

    namespace extension
    {
        template<typename T>
        struct value_at_impl;

        template <typename T, int N>
        struct member_function;

        template <typename T>
        struct member_function_size;

        template <>
        struct value_at_impl<member_function_tag>
        {
            template <typename Sequence, typename N>
            struct apply
            {
                static int const n_value = N::value;

                BOOST_MPL_ASSERT_RELATION(
                    n_value, <=,
                    extension::member_function_size<Sequence>::value );

                typedef typename
                    extension::member_function<Sequence, N::value>::type
                    type;
            };
        };
    }
}}

#endif //GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_VALUE_AT_IMPL_HPP
