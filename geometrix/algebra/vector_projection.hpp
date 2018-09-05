//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_PROJECTION_HPP
#define GEOMETRIX_VECTOR_PROJECTION_HPP

#include <geometrix/algebra/dot_product.hpp>

namespace geometrix {

    namespace result_of{
        template<typename Vector1, typename Vector2, typename EnableIf=void>
        struct vector_projection
        {
        private:
        
            using length_type = typename arithmetic_type_of<Vector2>::type;

        public:

            using type = vector<length_type, dimension_of<Vector2>::value>;
        
        };
    }//! namespace result_of;

    //! Calculate the projection of v1 onto v2.
    template <typename Vector1, typename Vector2>
    inline typename result_of::vector_projection<Vector1 , Vector2>::type vector_projection( const Vector1& v1, const Vector2& v2 )
    {
        BOOST_CONCEPT_ASSERT((VectorConcept<Vector1>));
        BOOST_CONCEPT_ASSERT((VectorConcept<Vector2>));
        auto vp = normalize(v2);
        auto a = scalar_projection(v1, vp);
        return a * vp;
    }

}//! namespace geometrix;

#endif//! GEOMETRIX_VECTOR_PROJECTION_HPP

