//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_IGNORE_UNUSED_WARNINGS_HPP
#define GEOMETRIX_UTILITY_IGNORE_UNUSED_WARNINGS_HPP

namespace geometrix {
		
	template <typename ...T>
	void ignore_unused_warning_of(){}

	template <typename ...T>
	void ignore_unused_warning_of(T const&...){}
	
}//namespace geometrix;

#endif //! GEOMETRIX_UTILITY_IGNORE_UNUSED_WARNINGS_HPP
