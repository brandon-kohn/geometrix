//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_CATEGORY_OF_IMPL_HPP
#define _GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_CATEGORY_OF_IMPL_HPP
#pragma once

#include <boost/config/no_tr1/utility.hpp>

namespace boost 
{
namespace fusion
{
    struct member_function_tag;
    struct random_access_traversal_tag;

    namespace extension
    {
        template<typename T>
        struct category_of_impl;

        template<>
        struct category_of_impl<member_function_tag>
        {
            template<typename T>
            struct apply
            {
                typedef random_access_traversal_tag type;
            };
        };
    }
}}
#endif //_GENERATIVE_GEOMETRY_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_CATEGORY_OF_IMPL_HPP
