//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_END_IMPL_HPP
#define GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_END_IMPL_HPP
#pragma once

#include "member_function_iterator.hpp"

namespace boost
{
namespace fusion
{
    struct member_function_tag;

    namespace extension
    {
        template <typename Tag>
        struct end_impl;

        template <typename T>
        struct member_function_size;

        template <>
        struct end_impl<member_function_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef
                    member_function_iterator<
                    Sequence
                    , member_function_size<Sequence>::value
                    >
                    type;

                static type call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif //GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_END_IMPL_HPP
