//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_IN_SOLID_CLASSIFICATION_HPP
#define GEOMETRIX_POINT_IN_SOLID_CLASSIFICATION_HPP
#pragma once

namespace geometrix {

    enum class point_in_solid_classification
    {
        in_solid = -1,
        on_boundary = 0,
        in_empty_space = 1
    };

}//! namespace geometrix;

#endif//! GEOMETRIX_POINT_IN_SOLID_CLASSIFICATION_HPP
