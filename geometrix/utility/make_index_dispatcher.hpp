//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MAKE_INDEX_DISPATCHER_HPP
#define GEOMETRIX_MAKE_INDEX_DISPATCHER_HPP
#pragma once

#include <utility>

namespace geometrix {

    template <std::size_t ... I>
    inline auto make_index_dispatcher(std::index_sequence<I...>)
    {
        return [](auto&& fn) { ( f( std::integral_constant<std::size_t, I>{} ), ... ); };
    }

    template <std::size_t N>
    inline auto make_index_dispatcher()
    {
        return make_index_dispatcher( std::make_index_sequence<N>{} );
    }

}//namespace geometrix;

#endif//! GEOMETRIX_MAKE_INDEX_DISPATCHER_HPP
