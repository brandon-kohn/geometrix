//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DOT_PRODUCT_HPP
#define GEOMETRIX_DOT_PRODUCT_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>
#include <geometrix/algebra/detail/dot_product.hpp>

#include <boost/fusion/include/transform_view.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/functional/adapter/fused_function_object.hpp>

namespace geometrix {

    //! Calculate the dot product between two NumericSequences.
    template <typename NumericSequence1, typename NumericSequence2>
    inline typename result_of::dot_product
    <
        NumericSequence1
      , NumericSequence2
    >::type dot_product( const NumericSequence1& v1, const NumericSequence2& v2 )
    {
        BOOST_CONCEPT_ASSERT(( TensorConcept< NumericSequence1 > ));
        BOOST_CONCEPT_ASSERT(( TensorConcept< NumericSequence2 > ));
        GEOMETRIX_STATIC_ASSERT( dimension_of<NumericSequence1>::value == dimension_of<NumericSequence2>::value );
        return detail::dot_product<NumericSequence1, NumericSequence2>()( v1, v2 );
    }

	//! Calculate the scalar_projection between two NumericSequences.
    template <typename Vector1, typename Vector2>
    inline typename result_of::dot_product
    <
        Vector1
      , Vector2
    >::type scalar_projection( const Vector1& v1, const Vector2& v2 )
    {
        BOOST_CONCEPT_ASSERT(( VectorConcept< Vector1 > ));
        BOOST_CONCEPT_ASSERT(( VectorConcept< Vector2 > ));
        GEOMETRIX_STATIC_ASSERT( dimension_of<Vector1>::value == dimension_of<Vector2>::value );
		static_assert(is_dimensionless<Vector2>::value, "scalar_projection requires Vector2 to be unit and dimensionless.");
        return detail::dot_product<Vector1, Vector2>()( v1, v2 );
    }

}//namespace geometrix;

#endif //GEOMETRIX_DOT_PRODUCT_HPP
