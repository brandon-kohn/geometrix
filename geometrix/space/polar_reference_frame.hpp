//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POLAR_REFERENCE_FRAME_HPP
#define GEOMETRIX_POLAR_REFERENCE_FRAME_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>

#include <boost/units/systems/si.hpp>

namespace geometrix {  

    //! \brief Enum for common dimension names in polar/spherical coordinates.
    
    //! \ingroup CoordinateReferenceFrames
    enum polar_dimension
    {
        e_radius = 0,
        e_theta = 1,
        e_phi = 2
    };

    namespace polar { namespace detail {

        template <typename UnitsSystem, std::size_t Index>
        struct basis
        {
            typedef typename boost::units::plane_angle_dimension metric_type;
            typedef boost::units::unit<metric_type, UnitsSystem> unit_type;
        };

        template <typename UnitsSystem>
        struct basis<UnitsSystem, 0>
        {
            typedef typename boost::units::length_dimension      metric_type;
            typedef boost::units::unit<metric_type, UnitsSystem> unit_type;
        };

        template <typename Frame, typename Type, std::size_t Index, typename EnableIf=void>
        struct coordinate_type_at
        {
            typedef boost::units::quantity
            <
                typename Frame::template basis<Index>::unit_type
              , Type
            > type;
        };

        template <typename Frame, typename Type, std::size_t Index>
        struct coordinate_type_at<Frame, Type, Index, typename geometric_traits<Type>::is_coordinate_sequence >
        {
            typedef boost::units::quantity
            <
                typename Frame::template basis<Index>::unit_type
              , typename type_at<Type,Index>::type
            > type;
        };

    }}//! namespace polar::detail;

    //! \brief This class models a polar reference frame in some specified affine space.

    //! \ingroup CoordinateReferenceFrames
    template <std::size_t Dimension, typename UnitsSystem = boost::units::si::system>
    struct polar_reference_frame
    {    
        typedef affine_space<Dimension> space_type;
        typedef UnitsSystem             units_system;

        template <std::size_t Index>
        struct basis 
            : polar::detail::basis<units_system, Index>
        {};
       
        template <std::size_t Index>
        struct unit_type_at
        {
            typedef typename basis<Index>::unit_type type;
        };

        template <typename Type, std::size_t Index, typename EnableIf=void>
        struct coordinate_type_at 
            : polar::detail::coordinate_type_at
              <
                  polar_reference_frame<Dimension, UnitsSystem>
                , Type
                , Index
              >
        {};
    };

    //! \brief Define the base traits of a frame of reference. 
    template <std::size_t Dimension, typename UnitsSystem>
    struct reference_frame_traits< polar_reference_frame<Dimension, UnitsSystem> >
    {
        //! Reference frame belongs to some affine space.
        typedef polar_reference_frame<Dimension, UnitsSystem> reference_frame;
        typedef typename reference_frame::space_type          space_type;
    };

    template <typename Sequence>
    struct geometric_traits
        < 
            reference_frame_adaptor
            <
                Sequence
              , polar_reference_frame
                <
                    geometric_traits< Sequence >::dimension_type::value 
                >
            >
        >
        : public geometric_traits< Sequence >
    {};
        
    //! \brief typedefs for common types.
    typedef polar_reference_frame< 2 > polar_reference_frame_2d;
    typedef polar_reference_frame< 3 > polar_reference_frame_3d;

}//namespace geometrix;

#endif //GEOMETRIX_POLAR_REFERENCE_FRAME_HPP
