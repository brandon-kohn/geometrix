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

#include <geometrix/algebra/expression.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/vector.hpp>

#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {
        template <typename Vector, typename EnableIf=void>
        struct normalize
            : boost::mpl::identity<Vector>
        {};

// 		template <typename Expr>
// 		struct normalize< geometrix::expr<Expr>, typename geometrix::expr<Expr>::traits::is_vector >
// 			: boost::mpl::identity
// 			  <
// 			      vector<typename geometric_traits<geometrix::expr<Expr>>::arithmetic_type, dimension_of<geometrix::expr<Expr>>::type::value>
// 			  >
// 		{};
    }//namespace result_of;
    
    //! \brief Normalize a vector (returns a new unit vector with the same orientation as the original).
    template <typename Vector>
    inline typename result_of::normalize<Vector>::type normalize( const Vector& v )
    {
        BOOST_CONCEPT_ASSERT(( VectorConcept<Vector> ));
		typedef typename geometric_traits<Vector>::arithmetic_type scalar;
		GEOMETRIX_ASSERT(magnitude(v) != 0);
		scalar factor = scalar( 1 ) / magnitude( v );
		return v * factor;
    }
        
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_NORMALIZE_HPP
