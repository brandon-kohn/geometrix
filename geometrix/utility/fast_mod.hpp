//
//! Copyright © 2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <cstdint>

namespace geometrix {

	inline std::uint32_t fast_modulus(std::uint32_t q, std::uint32_t d) {
		return ( static_cast<std::uint64_t>(q) * static_cast<std::uint64_t>(d) ) >> 32;
	}

}//! namespace geometrix;


