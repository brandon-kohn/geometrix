//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_SLERP_HPP
#define GEOMETRIX_VECTOR_MATH_SLERP_HPP

#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/orientation_enum.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/numeric/constants.hpp>

namespace geometrix {

	namespace result_of
	{
		template <typename Vector1, typename Vector2, typename Scalar, typename EnableIf = void>
		struct slerp
		{
			using dimensionless_t = dimensionless_type_of<Vector1>::type;
			using type = vector<dimensionless_t, dimension_of<Vector1>::value>;
		};
	}//namespace result_of;

	template <typename Vector1, typename Vector2, typename Scalar>
	inline typename result_of::slerp<Vector1, Vector2, Scalar>::type slerp(const Vector1& u, const Vector2& v, const Scalar& t)
	{
		using std::acos;
		using std::cos;
		using std::sin;

		auto d = scalar_projection(u, v);
		auto w = normalize(v - d * u);
		auto theta = t * acos(u, v);
		return u * cos(theta) + w * sin(theta);
	}
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_SLERP_HPP
