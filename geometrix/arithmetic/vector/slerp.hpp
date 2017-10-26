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
        BOOST_CONCEPT_ASSERT((DimensionlessVectorConcept<Vector1>));
        BOOST_CONCEPT_ASSERT((DimensionlessVectorConcept<Vector2>));

        using std::acos;
        using std::sin;

        auto d = dot_product(u, v);
        if (d < constants::one<decltype(d)>())
        {
            auto theta = acos(d);
            auto rSinTheta = 1.0 / sin(theta);
            return u * rSinTheta * sin((constants::one<Scalar>() - t) * theta) + v * rSinTheta * sin(t*theta);
        }
        else
            return normalize(u + t * (v - u));
    }
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_SLERP_HPP
