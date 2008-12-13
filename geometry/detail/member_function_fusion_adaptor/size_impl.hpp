//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_SIZE_IMPL_HPP
#define _BOOST_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_SIZE_IMPL_HPP
#pragma once

#include <boost/mpl/int.hpp>

namespace boost
{
namespace fusion
{
    namespace extension
    {
        template <typename T>
        struct member_function_size;
    }

    struct member_function_tag;

    namespace extension
    {
        template<typename T>
        struct size_impl;

        template <>
        struct size_impl<member_function_tag>
        {
            template <typename Sequence>
            struct apply : extension::member_function_size<Sequence> {};
        };
    }
}}

#endif //_BOOST_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_SIZE_IMPL_HPP
