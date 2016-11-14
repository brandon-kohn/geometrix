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
#include <boost/limits.hpp>

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
		GEOMETRIX_DEFINE_CONSTANT(two_pi, 2.0 * 3.1415926535897931);
		GEOMETRIX_DEFINE_CONSTANT(half_pi, 0.5 * 3.1415926535897931);
		GEOMETRIX_DEFINE_CONSTANT(zero, 0);
		GEOMETRIX_DEFINE_CONSTANT(one, 1);
		GEOMETRIX_DEFINE_CONSTANT(two, 2);
		GEOMETRIX_DEFINE_CONSTANT(one_half, 0.5);
		GEOMETRIX_DEFINE_CONSTANT(e, 2.7182818284590452);
		GEOMETRIX_DEFINE_CONSTANT(sqrt_2, 1.4142135623730950);

		template <typename T>
		inline const T infinity()
		{
			return construct<T>(std::numeric_limits<double>::infinity());
		}

		template <typename T>
		inline const T negative_infinity()
		{
			return construct<T>(-std::numeric_limits<double>::infinity());
		}
    };
	
}//namespace geometrix;

#endif //GEOMETRIX_CONSTANTS_HPP
