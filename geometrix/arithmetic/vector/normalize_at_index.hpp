//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_NORMALIZE_AT_INDEX_HPP
#define GEOMETRIX_VECTOR_MATH_NORMALIZE_AT_INDEX_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/arithmetic/vector/vector_arithmetic.hpp>
#include <geometrix/arithmetic/vector/magnitude.hpp>

#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {
        template <unsigned int Index, typename Vector>
        struct normal_at_index
            : result_of::divides
                <
                    typename arithmetic_promotion_policy
                    <
                        typename type_at<Vector,Index>::type
                    >::type
                  , typename result_of::multiplies<Vector,Vector>::type 
                >
        {};
    }//namespace result_of;
    
    //! \brief Access the normalized value at the ith index.
    template <unsigned int Index, typename Vector>
    inline typename result_of::normal_at_index<Index,Vector>::type normal_at_index( const Vector& v )
    {
        BOOST_CONCEPT_ASSERT(( VectorConcept<Vector> ));
        return arithmetic_promote( get<Index>(v) ) / magnitude( v );
    }
        
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_NORMALIZE_AT_INDEX_HPP
