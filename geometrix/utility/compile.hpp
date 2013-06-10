//
//! Copyright © 2008-2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_COMPILE_HPP
#define GEOMETRIX_COMPILE_HPP
#pragma once

#include <boost\preprocessor\stringize.hpp>

//!Usage: #pragma GEOMETRIX_WARNING("This is a user warning.")
#define GEOMETRIX_WARNING(str)                                              \
	message(__FILE__ "(" BOOST_PP_STRINGIZE(__LINE__) ") : warning : " str) \
/***/

#endif //GEOMETRIX_COMPILE_HPP
