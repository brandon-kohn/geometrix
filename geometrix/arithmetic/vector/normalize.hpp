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
#include <geometrix/numeric/constants.hpp>
#include <geometrix/utility/ignore_unused_warnings.hpp>

#include <boost/fusion/include/mpl.hpp>

#include <utility>

namespace geometrix {

    namespace result_of 
    {
        template <typename Vector, typename EnableIf=void>
        struct normalize
        {
			using type = vector<typename geometric_traits<Vector>::dimensionless_type, dimension_of<Vector>::value>;
		};
    }//namespace result_of;
    
    //! \brief Normalize a vector (returns a new unit vector with the same orientation as the original).
    template <typename Vector>
    inline typename result_of::normalize<Vector>::type normalize( const Vector& v )
    {
        BOOST_CONCEPT_ASSERT(( VectorConcept<Vector> ));
		using scalar = decltype(magnitude(std::declval<Vector>()));
		using dimensionless_type = typename geometric_traits<Vector>::dimensionless_type;

		ignore_unused_warning_of<scalar>();
		GEOMETRIX_ASSERT(magnitude(v) != constants::zero<scalar>());
		auto factor = constants::one<dimensionless_type>() / magnitude( v );
		return v * factor;
    }
        
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_NORMALIZE_HPP
