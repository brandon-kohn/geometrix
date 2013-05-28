//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_NORMALIZE_HPP
#define GEOMETRIX_VECTOR_MATH_NORMALIZE_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/algebra/expression.hpp>

#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {
        template <typename Vector>
        struct normalize
            : boost::mpl::identity<Vector>
        {};

    }//namespace result_of;
    
    //! \brief Normalize a vector (returns a new unit vector with the same orientation as the original).
    template <typename Vector>
    inline Vector normalize( const Vector& v )
    {
        BOOST_CONCEPT_ASSERT(( VectorConcept<Vector> ));
        return construct<Vector>( algebra::norm(v) );
    }
        
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_NORMALIZE_HPP
