//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_CARTESIAN_REFERENCE_FRAME_HPP
#define GENERATIVE_GEOMETRY_CARTESIAN_REFERENCE_FRAME_HPP
#pragma once

#include <geometry\numeric_sequence.hpp>
#include <geometry\reference_frame_tag.hpp>
#include <geometry\coordinate_basis.hpp>
#include <boost\units\systems\si.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{  
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
    template <typename NumericType, unsigned int Dimension, typename UnitsSystem = boost::units::si::system>
    class cartesian_reference_frame
    {
        template <typename AffineSpace, typename Units>
        struct cartesian_coordinate_basis
        {   
            typedef AffineSpace space_type;
            typedef Units       units_system_type;

            template <unsigned int Dimension>
            struct coordinate_properties
            {
                typedef typename affine_space_traits<space_type>::numeric_type     numeric_type;
                typedef typename boost::units::length_dimension                    metric_type;                
                typedef typename boost::units::unit< metric_type, UnitsSystem >    unit_type;
                typedef typename boost::units::quantity< unit_type, numeric_type > quantity_type;
            };
        };

    public:

        typedef affine_space<NumericType,Dimension>                        space_type;        
        typedef cartesian_coordinate_basis< space_type, UnitsSystem >      basis_type;

        //! Reference frame must define an origin.
        //! FIXME: Need to figure out point default construction as well as how to define origins.- For now assume default is 0.
        template <typename Point>
        static reference_frame_tag< Point, cartesian_reference_frame > get_origin()
        {
            return reference_frame_tag< Point, cartesian_reference_frame >( construction_traits<Point>( get_origin_sequence() ) );
        }

    private:

        static const numeric_sequence< typename affine_space_traits<space_type>::numeric_type, affine_space_traits<space_type>::dimension_type::value >& get_origin_sequence()
        {
            static numeric_sequence< typename affine_space_traits<space_type>::numeric_type, affine_space_traits<space_type>::dimension_type::value > theOrigin( make_initialized_array( typename affine_space_traits<space_type>::numeric_type(0) ) );
            return theOrigin;
        }

    };

    //! Define the base traits of a frame of reference. 
    template <typename NumericType, unsigned int Dimension>
    struct reference_frame_traits< cartesian_reference_frame<NumericType, Dimension> >
    {
        //! Reference frame belongs to some affine space.
        typedef typename cartesian_reference_frame<NumericType, Dimension> reference_frame_type;
        typedef typename reference_frame_type::space_type                  space_type;      
        typedef typename reference_frame_type::basis_type                  basis_type;

        //! Reference frame must define an origin.
        template <typename Point>
        static reference_frame_tag< Point, reference_frame_type > get_origin( const reference_frame_type& frame )
        {
            return frame.get_origin<Point>();
        }
    };

    //! \brief typedefs for common types.
    typedef cartesian_reference_frame< float, 2 >     cartesian_reference_frame_float_2d;
    typedef cartesian_reference_frame< float, 3 >     cartesian_reference_frame_float_3d;
    typedef cartesian_reference_frame< double, 2 >    cartesian_reference_frame_double_2d;
    typedef cartesian_reference_frame< double, 3 >    cartesian_reference_frame_double_3d;
    typedef cartesian_reference_frame< int, 2 >       cartesian_reference_frame_int_2d;
    typedef cartesian_reference_frame< int, 3 >       cartesian_reference_frame_int_3d;
    typedef cartesian_reference_frame< long long, 2 > cartesian_reference_frame_int64_2d;
    typedef cartesian_reference_frame< long long, 3 > cartesian_reference_frame_int64_3d;    

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_CARTESIAN_REFERENCE_FRAME_HPP
