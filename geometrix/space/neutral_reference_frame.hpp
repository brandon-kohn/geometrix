//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NEUTRAL_REFERENCE_FRAME_HPP
#define GEOMETRIX_NEUTRAL_REFERENCE_FRAME_HPP

#include <geometrix/space/reference_frame_traits.hpp>
#include <geometrix/space/affine_space.hpp>
#include <geometrix/space/coordinate_basis.hpp>
#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/tensor_traits.hpp>

#include <boost/units/systems/si.hpp>

namespace geometrix { 

//! \brief a type for defining a neutral frame. This is used when the user has not specified the frame and all transforms are 
//! therefore turned off. 

//! \ingroup CoordinateReferenceFrames
template < unsigned int Dimension, typename UnitsSystem = boost::units::si::system >
class neutral_reference_frame
{
    typedef affine_space<Dimension>          space_type;
    typedef UnitsSystem                      units_system;

    template <unsigned int Dimension>
    struct basis
    {
        typedef typename boost::units::dimensionless_type               metric_type;
        typedef typename boost::units::unit< metric_type, UnitsSystem > unit_type;
    };

    template <unsigned int Dimension>
    struct metric_at
    {
        typedef typename basis<Dimension>::metric_type type;
    };

    template <unsigned int Dimension>
    struct unit_at
    {
        typedef typename basis<Dimension>::unit_type type;
    };

    template <typename Type, unsigned int Dimension, typename EnableIf=void>
    struct coordinate_type_of
    {
        typedef typename boost::units::quantity
        <
            typename basis<Dimension>::unit_type
          , Type
        > type;
    };

    template <typename Type, unsigned int Dimension>
    struct coordinate_type_of<Type, Dimension, typename geometric_traits<Type>::is_coordinate_sequence >
    {
        typedef typename boost::units::quantity
        <
              typename basis<Dimension>::unit_type
            , typename type_at<Type,Dimension>::type
        > type;
    };

    typedef affine_space<Dimension> space_type;
};

template <unsigned int Dimension>
struct reference_frame_traits< neutral_reference_frame<Dimension> >
{
    //! Reference frame belongs to some affine space.
    typedef affine_space<Dimension>            space_type;
    typedef neutral_reference_frame<Dimension> reference_frame;//! self reference for tagging primitives.
};

typedef neutral_reference_frame<2> neutral_reference_frame_2D;
typedef neutral_reference_frame<3> neutral_reference_frame_3D;

}//namespace geometrix;

#endif //GEOMETRIX_NEUTRAL_REFERENCE_FRAME_HPP
