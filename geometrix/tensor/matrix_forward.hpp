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

    template <typename T, unsigned int Rows, unsigned int Columns>
    struct matrix;

    template <typename T, unsigned int Row, typename EnableIf=void>
    struct row;

    template <typename T, unsigned int Column, typename EnableIf=void>
    struct column;

}//namespace geometrix;

#endif //GEOMETRIX_DETAIL_MATRIX_FORWARD_HPP
