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
template < std::size_t Dimension, typename UnitsSystem = boost::units::si::system >
struct neutral_reference_frame
{
    typedef affine_space<Dimension>          space_type;
    typedef UnitsSystem                      units_system;

    template <std::size_t Index>
    struct basis
    {
        typedef typename boost::units::dimensionless_type               metric_type;
        typedef typename boost::units::unit< metric_type, UnitsSystem > unit_type;
    };
        
    template <std::size_t Index>
    struct unit_type_at
    {
        typedef typename basis<Index>::unit_type type;
    };

    template <typename Type, std::size_t Index, typename EnableIf=void>
    struct coordinate_type_at
    {
        typedef typename boost::units::quantity
        <
            typename basis<Dimension>::unit_type
          , Type
        > type;
    };

    template <typename Type, std::size_t Index>
    struct coordinate_type_at<Type, Index, typename geometric_traits<Type>::is_coordinate_sequence >
    {
        typedef typename boost::units::quantity
        <
              typename basis<Index>::unit_type
            , typename type_at<Type,Index>::type
        > type;
    };
};

template <std::size_t Dimension>
struct reference_frame_traits< neutral_reference_frame<Dimension> >
{
    //! Reference frame belongs to some affine space.
    typedef affine_space<Dimension>            space_type;
    typedef neutral_reference_frame<Dimension> reference_frame;//! self reference for tagging primitives.
};

typedef neutral_reference_frame<2> neutral_reference_frame_2d;
typedef neutral_reference_frame<3> neutral_reference_frame_3d;

}//namespace geometrix;

#endif //GEOMETRIX_NEUTRAL_REFERENCE_FRAME_HPP
