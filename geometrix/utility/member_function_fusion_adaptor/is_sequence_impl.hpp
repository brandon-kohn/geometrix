//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_IS_SEQUENCE_IMPL_HPP
#define GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_IS_SEQUENCE_IMPL_HPP


#include <boost/mpl/bool.hpp>

namespace boost 
{
namespace fusion
{
    struct member_function_tag;

    namespace extension
    {
        template<typename Tag>
        struct is_sequence_impl;

        template<>
        struct is_sequence_impl<member_function_tag>
        {
            template<typename Sequence>
            struct apply : mpl::true_ {};
        };
    }
}}

#endif //GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_IS_SEQUENCE_IMPL_HPP
