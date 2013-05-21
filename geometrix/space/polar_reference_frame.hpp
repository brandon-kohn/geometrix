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

    //! \brief This class models a polar reference frame in some specified affine space.

    //! \ingroup CoordinateReferenceFrames
    template <unsigned int Dimension, typename UnitsSystem = boost::units::si::system>
    class polar_reference_frame
    {
    public:

        typedef affine_space<Dimension> space_type;
        typedef UnitsSystem             units_system;

        template <unsigned int Dimension>
        struct basis
        {
            typedef typename boost::units::plane_angle_dimension             metric_type;
            typedef typename boost::units::unit< metric_type, units_system > unit_type;
         };
        
        template <>
        struct basis<0>
        {
            typedef typename boost::units::length_dimension                  metric_type;
            typedef typename boost::units::unit< metric_type, units_system > unit_type;
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

        //! Reference frame must define an origin.
        //! FIXME: Need to figure out point default construction as well as how to define origins.- For now assume default is 0.
//         template <typename Point>
//         static reference_frame_adaptor< Point, polar_reference_frame > get_origin()
//         {
//             return reference_frame_adaptor< Point, polar_reference_frame >( construction_policy<Point>( get_origin_sequence() ) );
//         }

    private:

//         static const numeric_sequence< typename affine_space_traits<space_type>::numeric_type, affine_space_traits<space_type>::dimension_type::value >& get_origin_sequence()
//         {
//             static numeric_sequence< typename affine_space_traits<space_type>::numeric_type, affine_space_traits<space_type>::dimension_type::value > theOrigin( make_initialized_array( typename affine_space_traits<space_type>::numeric_type(0) ) );
//             return theOrigin;
//         }

    };

    //! \brief Define the base traits of a frame of reference. 
    template <unsigned int Dimension>
    struct reference_frame_traits< polar_reference_frame<Dimension> >
    {
        //! Reference frame belongs to some affine space.
        typedef typename polar_reference_frame<Dimension> reference_frame;
        typedef typename reference_frame::space_type      space_type;

        //! Reference frame must define an origin.
//         template <typename Point>
//         static reference_frame_adaptor< Point, reference_frame > get_origin( const reference_frame& frame )
//         {
//             return frame.get_origin<Point>(); 
//         }
    };

    template <typename Sequence>
    struct geometric_traits< 
        reference_frame_adaptor< Sequence,
                             polar_reference_frame<                                       
                                       geometric_traits< Sequence >::dimension_type::value > > >
        : public geometric_traits< Sequence >
    {};
        
    //! \brief typedefs for common types.
    typedef polar_reference_frame< 2 >     polar_reference_frame_2d;
    typedef polar_reference_frame< 3 >     polar_reference_frame_3d;

}//namespace geometrix;

#endif //GEOMETRIX_POLAR_REFERENCE_FRAME_HPP
