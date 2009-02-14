//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_DIMENSION_TRAITS_HPP
#define _GENERATIVE_GEOMETRY_DIMENSION_TRAITS_HPP
#pragma once

#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! Enum for common dimension names in cartesian coordinates.
    enum cartesian_dimension
    {
	    e_x = 0,
	    e_y = 1,
	    e_z = 2
    };

    //! Enum for common dimension names in polar/spherical coordinates.
    enum polar_dimension
    {
	    e_radius = 0,
	    e_theta = 1,
	    e_phi = 2
    };

    //! A type trait specialized on dimension.
    template <unsigned int Dimension>
    struct dimension_traits
    {
    	static const unsigned int value = Dimension;
    };

    //! A metafunction class to access the dimension of a type.
    template <typename T>
    struct dimension_of
    {
        typedef typename T::dimension_type dimension_type;
        static const unsigned int value = T::dimension_type::value;
    };

    //! A concept type to express membership in a dimension.
    //! 
    //! Membership to a dimension means the item requires a number of linearly independent vectors to be described
    //! (no more and no less.)
    //! (e.g. A 2-dimensional point has coordinates which are described by a basis of 2 linearly independent 
    //!  vectors.)
    template <typename T, unsigned int Dimension>
    struct DimensionConcept
    {
        void constraints()
        {
            BOOST_MPL_ASSERT_MSG( 
                ( dimension_of< T >::value == Dimension ),
                ITEM_DOES_NOT_MODEL_DIMENSION_CONCEPT,
                ( T ) );
        }
    };

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_DIMENSION_TRAITS_HPP
