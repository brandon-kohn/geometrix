//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_GOLDEN_RATIO_COLOR_GENERATOR_HPP
#define GEOMETRIX_GOLDEN_RATIO_COLOR_GENERATOR_HPP

#include <geometrix/utility/random_generator.hpp>
#include <cstdint>
#include <tuple>
#include <cmath>
#include <vector>

namespace geometrix
{
	struct argb { enum { Alpha, Red, Green, Blue }; };
	typedef std::tuple<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t> argb_tuple;
	inline argb_tuple hsv2rgb( float h, float s, float v )
	{
		float r{}, g{}, b{};

		int i = static_cast<int>(std::floor( h * 6. ));
		float f = h * 6 - i;
		float p = v * (1 - s);
		float q = v * (1 - f * s);
		float t = v * (1 - (1 - f) * s);

		switch( i % 6 )
		{
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		return std::make_tuple( 255, static_cast<std::uint8_t>(r * 255), static_cast<std::uint8_t>(g * 255), static_cast<std::uint8_t>(b * 255) );
	}

	inline std::vector<argb_tuple> generate_colors( std::size_t sz = 1000, double s = 0.99, double v = 0.99 )
	{
		std::vector<argb_tuple> c;
		const double golden_ratio_conjugate = 0.618033988749895;
		geometrix::random_real_generator<> rnd;
		for( std::size_t i = 0; i < sz; ++i )
		{
			double h = fmod( golden_ratio_conjugate + rnd(), 1.0 );
			c.push_back( hsv2rgb( static_cast<float>(h), static_cast<float>(s), static_cast<float>(v) ) );
		}

		return std::move( c );
	}
}//! namespace geometrix;

#endif//! GEOMETRIX_GOLDEN_RATIO_COLOR_GENERATOR_HPP
