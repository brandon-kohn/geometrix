//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CARTESIAN_REFERENCE_FRAME_HPP
#define GEOMETRIX_CARTESIAN_REFERENCE_FRAME_HPP

#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/space/reference_frame_adaptor.hpp>
#include <geometrix/space/coordinate_basis.hpp>
#include <geometrix/geometric_traits.hpp>

#include <boost/units/systems/si.hpp>

namespace geometrix {  
    //! Enum for common dimension names in Cartesian coordinates.

    //! \ingroup CoordinateReferenceFrames
    enum cartesian_dimension
    {
        e_x = 0,
        e_y = 1,
        e_z = 2
    };

    //! \brief This class models a Cartesian reference frame in some specified affine space.

    //! \ingroup CoordinateReferenceFrames
    template <unsigned int Dimension, typename UnitsSystem = boost::units::si::system>
    struct cartesian_reference_frame
    {
        typedef affine_space<Dimension>          space_type;
        typedef UnitsSystem                      units_system;

        template <unsigned int Dimension>
        struct basis
        {
            typedef typename boost::units::length_dimension                 metric_type;
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
    };

    //! Define the base traits of a frame of reference. 
    template <unsigned int Dimension>
    struct reference_frame_traits< cartesian_reference_frame<Dimension> >
    {
        //! Reference frame belongs to some affine space.
        typedef typename cartesian_reference_frame<Dimension> reference_frame;
        typedef typename reference_frame::space_type          space_type;
    };

    //! \brief typedefs for common types.
    typedef cartesian_reference_frame< 2 > cartesian_reference_frame_2d;
    typedef cartesian_reference_frame< 3 > cartesian_reference_frame_3d;
    
}//namespace geometrix;

#endif //GEOMETRIX_CARTESIAN_REFERENCE_FRAME_HPP
