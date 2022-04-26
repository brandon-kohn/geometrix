//
//! Copyright © 2008-2022
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_SLERP_HPP
#define GEOMETRIX_VECTOR_MATH_SLERP_HPP

#include <geometrix/tensor/vector.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/numeric/constants.hpp>
#include <geometrix/arithmetic/math_kernel.hpp>

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

    template <typename Math, typename Vector1, typename Vector2, typename Scalar>
    inline typename result_of::slerp<Vector1, Vector2, Scalar>::type slerp_math(const Vector1& u, const Vector2& v, const Scalar& t)
    {
        BOOST_CONCEPT_ASSERT((DimensionlessVectorConcept<Vector1>));
        BOOST_CONCEPT_ASSERT((DimensionlessVectorConcept<Vector2>));

        auto d = dot_product(u, v);
        if (d < constants::one<decltype(d)>())
        {
            auto theta = Math::acos(d);
            auto rSinTheta = 1.0 / Math::sin(theta);
            return u * rSinTheta * Math::sin((constants::one<Scalar>() - t) * theta) + v * rSinTheta * Math::sin(t*theta);
        }
        else
            return normalize(u + t * (v - u));
    }
    
    template <typename Vector1, typename Vector2, typename Scalar>
    inline typename result_of::slerp<Vector1, Vector2, Scalar>::type slerp(const Vector1& u, const Vector2& v, const Scalar& t)
    {
        return slerp_math<std_math_kerenl>(u, v, t);
    }
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_SLERP_HPP
