//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_MATRIX_FORWARD_HPP
#define GEOMETRIX_DETAIL_MATRIX_FORWARD_HPP
#pragma once

namespace geometrix {

    template <typename T, std::size_t Rows, std::size_t Columns>
    struct matrix;

    template <typename T, std::size_t Row, typename EnableIf=void>
    struct row;

    template <typename T, std::size_t Column, typename EnableIf=void>
    struct column;

}//namespace geometrix;

#endif //GEOMETRIX_DETAIL_MATRIX_FORWARD_HPP
