//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_PERP_HPP
#define GEOMETRIX_VECTOR_MATH_PERP_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {        
        template <typename Vector>
        struct perp
            : boost::mpl::identity<Vector>
        {};

    }//namespace result_of;

    //! Get the perpendicular of a 2d vector.
    template <typename Vector>
    inline Vector perp( const Vector& u )
    {
        BOOST_CONCEPT_ASSERT(( Vector2DConcept<Vector> ));
        return construct<Vector>( - get<1>(u), get<0>(u) );
    }

}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_PERP_HPP
