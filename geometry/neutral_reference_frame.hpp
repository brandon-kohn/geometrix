//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_NEUTRAL_REFERENCE_FRAME_HPP
#define GENERATIVE_GEOMETRY_NEUTRAL_REFERENCE_FRAME_HPP
#pragma once

#include <geometry\reference_frame_traits.hpp>
#include <geometry\affine_space.hpp>
#include <geometry\coordinate_basis.hpp>

#include <boost\units\systems\si.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{ 

//! \brief a type for defining a neutral frame. This is used when the user has not specified the frame and all transforms are 
//! therefore turned off. 

//! \ingroup CoordinateReferenceFrames
template <typename NumericType, unsigned int Dimension, typename UnitsSystem = boost::units::si::system >
class neutral_reference_frame
{
    template <typename AffineSpace, typename Units>
    struct neutral_coordinate_basis
    {   
        typedef AffineSpace space_type;
        typedef Units       units_system_type;

        template <unsigned int Dimension>
        struct coordinate_properties
        {
            typedef typename affine_space_traits<space_type>::numeric_type     numeric_type;
            typedef typename boost::units::dimensionless_type                  metric_type;                
            typedef typename boost::units::unit< metric_type, UnitsSystem >    unit_type;
            typedef typename boost::units::quantity< unit_type, numeric_type > quantity_type;
        };
    };
public:

    typedef affine_space<NumericType,Dimension> space_type;
};

template <typename NumericType, unsigned int Dimension>
struct reference_frame_traits< neutral_reference_frame<NumericType,Dimension> >
{
    //! Reference frame belongs to some affine space.
    typedef affine_space<NumericType,Dimension>            space_type;
    typedef neutral_reference_frame<NumericType,Dimension> reference_frame_type;//! self reference for tagging primitives.
};

typedef neutral_reference_frame<float,2>     neutral_reference_frame_float_2d;
typedef neutral_reference_frame<float,3>     neutral_reference_frame_float_3d;
typedef neutral_reference_frame<double,2>    neutral_reference_frame_double_2d;
typedef neutral_reference_frame<double,3>    neutral_reference_frame_double_3d;
typedef neutral_reference_frame<int,2>       neutral_reference_frame_int_2d;
typedef neutral_reference_frame<int,3>       neutral_reference_frame_int_3d;
typedef neutral_reference_frame<long long,2> neutral_reference_frame_int64_2d;
typedef neutral_reference_frame<long long,3> neutral_reference_frame_int64_3d;

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_NEUTRAL_REFERENCE_FRAME_HPP
