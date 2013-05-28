//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_COORDINATE_BASIS_TRAITS_HPP
#define GEOMETRIX_COORDINATE_BASIS_TRAITS_HPP

#include <geometrix/space/dimension.hpp>
#include <boost/units/physical_dimensions.hpp>

namespace geometrix {  
    //! \brief Traits struct for specializing the properties of an affine space type.

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
    template < typename CoordinateBasis >
    struct coordinate_basis_traits
    {        
        typedef typename CoordinateBasis::space_type             space_type;   
        typedef typename CoordinateBasis::dimensional_properties dimensional_properties;

        BOOST_MPL_ASSERT_MSG(
            ( false )
            , COORDINATE_BASIS_TRAITS_NOT_DEFINED
            , (CoordinateBasis) );
    };

    //! \brief Concept definition of an affine space.

    //! A type models an AffineSpaceConcept if it defines a dimension type and a numeric type.
    template <typename CoordinateBasis>
    struct CoordinateBasisConcept
    {
        void constraints() const
        {   
            typedef typename coordinate_basis_traits< CoordinateBasis >::space_type space_type;
            BOOST_CONCEPT_ASSERT((AffineSpaceConcept<space_type>));

            typedef typename coordinate_basis_traits< CoordinateBasis >::dimensional_properties dimensional_properties;

        }
    };        
}//namespace geometrix;

#endif //GEOMETRIX_COORDINATE_BASIS_TRAITS_HPP
