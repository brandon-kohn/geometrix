//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_LOCATION_CLASSIFICATION_HPP
#define GEOMETRIX_POINT_LOCATION_CLASSIFICATION_HPP
#pragma once

namespace geometrix {

    enum point_location_classification
    {
        e_inside = -1,
        e_boundary = 0,
        e_outside = 1
    };

}//! namespace geometrix;

#endif//! GEOMETRIX_POINT_LOCATION_CLASSIFICATION_HPP
