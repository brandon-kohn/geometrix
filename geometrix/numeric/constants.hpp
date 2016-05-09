//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CONSTANTS_HPP
#define GEOMETRIX_CONSTANTS_HPP

#include <geometrix/numeric/rational_utilities.hpp>
#include <geometrix/numeric/numeric_traits.hpp>
#include <boost/math/constants/constants.hpp>

namespace geometrix {

    template <typename CoordinateType>
    struct constants
    {
		static CoordinateType pi() { return construct<CoordinateType>(3.1415926535897931); }
    };
	
}//namespace geometrix;

#endif //GEOMETRIX_CONSTANTS_HPP
