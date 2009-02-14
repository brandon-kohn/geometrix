//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_NEUTRAL_REFERENCE_FRAME_HPP
#define _GENERATIVE_GEOMETRY_NEUTRAL_REFERENCE_FRAME_HPP
#pragma once

#include "reference_frame_traits.hpp"
#include "affine_space.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{ 

//! \brief a type for defining a neutral frame. This is used when the user has not specified the frame and all transforms are 
//! therefore turned off. 

//! \ingroup CoordinateReferenceFrames
template <typename NumericType, unsigned int Dimension>
struct neutral_reference_frame
{
    typedef affine_space<NumericType,Dimension> affine_space_type;
};

template <typename NumericType, unsigned int Dimension>
struct reference_frame_traits< neutral_reference_frame<NumericType,Dimension> >
{
    //! Reference frame belongs to some affine space.
    typedef affine_space<NumericType,Dimension>            affine_space_type;
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

#endif //_GENERATIVE_GEOMETRY_NEUTRAL_REFERENCE_FRAME_HPP
