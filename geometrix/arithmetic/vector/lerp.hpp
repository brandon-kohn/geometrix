//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_LERP_HPP
#define GEOMETRIX_VECTOR_MATH_LERP_HPP

#include <geometrix/tensor/vector.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename Vector1, typename Vector2, typename Scalar, typename EnableIf = void>
        struct lerp
        {
            using length_t = arithmetic_type_of<Vector1>::type;
            using type = vector<length_t, dimension_of<Vector1>::value>;
        };
    }//namespace result_of;

    template <typename Vector1, typename Vector2, typename Scalar>
    inline typename result_of::lerp<Vector1, Vector2, Scalar>::type lerp(const Vector1& u, const Vector2& v, const Scalar& t)
    {
        return normalize(u + t * (v - u));
    }
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_LERP_HPP
