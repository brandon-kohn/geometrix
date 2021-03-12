//
//! Copyright © 2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ALGORITHM_ORIENTATION_ENUM_HPP
#define GEOMETRIX_ALGORITHM_ORIENTATION_ENUM_HPP

namespace geometrix {
    
    //! \enum orientation_type
    //! \brief Specifies a type for the result of the get_orientation test.
    enum orientation_type
    {
        oriented_right     = -1,
        oriented_collinear = 0,
        oriented_left      = 1
    };

    inline orientation_type opposite_orientation(orientation_type o)
    {
        return static_cast<orientation_type>(-o);
    }

}//! namespace geometrix;

#endif//GEOMETRIX_ALGORITHM_ORIENTATION_ENUM_HPP
