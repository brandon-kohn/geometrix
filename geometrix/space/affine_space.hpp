//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_AFFINE_SPACE_HPP
#define GEOMETRIX_AFFINE_SPACE_HPP

#include <geometrix/space/affine_space_traits.hpp>

namespace geometrix {  

//! \brief The notion of an affine space is used to define dimension and numeric type.

//! Affine spaces are a concept which defines the dimension of a space and encapsulates\n
//! a point and vector space.\n
//! Affine spaces are created by specifying a dimension and a numeric type.\n
//! The dimension and numeric type taken together define a numeric sequence type.\n
//! This type is then used to describe the elements of both point and vector types.\n
//! The point and vector types are related by the following concepts:\n
//! An affine space of dimension \f$n\f$ defines a point space \f$P^n\f$ and vector space \f$\hat{V}^n\f$.
//! For a point, \f$p\in P^n\f$ and vector \f$\hat{v}\in \hat{V}^n\f$ and scalar value \f$ \sigma \f$,\n
//!
//! Adding a vector to a point results in another point.
//!    - \f$p_1 + \hat{v} \Rightarrow p_2 \in P^n\f$
//! 
//! The difference between two points is a vector.
//!    - \f$p_1 - p_2 \Rightarrow \hat{v} \in \hat{V}^n\f$
//! 
//! Multiplying a vector by a scalar results in another vector.
//!    - \f$ \sigma \hat{v_1} \Rightarrow \hat{v_2} \in \hat{V}^n\f$

template <unsigned int Dimension>
struct affine_space
{
    typedef dimension<Dimension> dimension_type;    
};

//! \brief Macro to define the traits of an affine space.
#define GEOMETRIX_DEFINE_AFFINE_SPACE( Dimension )                    \
namespace geometrix {                                                 \
template <>                                                           \
struct affine_space_traits< affine_space< Dimension > >               \
{                                                                     \
    typedef affine_space< Dimension >::dimension_type dimension_type; \
};                                                                    \
}                                                                     \
/***/

}//namespace geometrix;

GEOMETRIX_DEFINE_AFFINE_SPACE( 2 );
GEOMETRIX_DEFINE_AFFINE_SPACE( 3 );

namespace geometrix {

typedef affine_space< 2 >  affine_space_2d;
typedef affine_space< 3 >  affine_space_3d;

}//namespace geometrix;

#endif //GEOMETRIX_AFFINE_SPACE_HPP
