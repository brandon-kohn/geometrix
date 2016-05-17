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

#include <geometrix/numeric/numeric_traits.hpp>
#include <boost/math/constants/constants.hpp>

#define GEOMETRIX_DEFINE_CONSTANT(Name, Value) \
template <typename NumericType>                \
inline const NumericType Name()                \
{                                              \
    return construct<NumericType>(Value);      \
}                                              \
/***/

namespace geometrix {
	    
    namespace constants
    {
		GEOMETRIX_DEFINE_CONSTANT(pi, 3.1415926535897931);
		GEOMETRIX_DEFINE_CONSTANT(zero, 0);
		GEOMETRIX_DEFINE_CONSTANT(one, 1);
		GEOMETRIX_DEFINE_CONSTANT(two, 2);
    };
	
}//namespace geometrix;

#endif //GEOMETRIX_CONSTANTS_HPP
