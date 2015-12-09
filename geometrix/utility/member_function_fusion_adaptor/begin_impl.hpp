//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_BEGIN_IMPL_HPP
#define GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_BEGIN_IMPL_HPP


#include <geometrix/utility/member_function_fusion_adaptor/member_function_iterator.hpp>

namespace boost
{
namespace fusion
{
    struct member_function_tag;

    namespace extension
    {
        template<typename T>
        struct begin_impl;

        template <>
        struct begin_impl<member_function_tag>
        {
            template <typename Sequence>
            struct apply
            {
                typedef member_function_iterator<Sequence, 0> type;

                static type call(Sequence& v)
                {
                    return type(v);
                }
            };
        };
    }
}}

#endif //GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_BEGIN_IMPL_HPP
