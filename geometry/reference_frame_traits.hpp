//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
#define GENERATIVE_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
#pragma once

#include <geometry\affine_space_traits.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{ 

//! \brief Defines the base traits of a frame of reference. 
template <typename ReferenceFrame>
struct reference_frame_traits
{
    BOOST_MPL_ASSERT_MSG( 
	  ( false )
	, REFERENCE_FRAME_TRAITS_NOT_DEFINED
	, (ReferenceFrame) );

    //! Reference frame belongs to some affine space.
    typedef typename ReferenceFrame::space_type space_type;    
    typedef typename ReferenceFrame::basis_type basis_type;
    typedef typename ReferenceFrame             reference_frame_type;//! self reference for tagging primitives.

};

template <typename ReferenceFrame>
struct ReferenceFrameConcept
{
    void constraints()
    {
        //! defines an affine space (rather has the same traits as one).
        typedef typename reference_frame_traits< ReferenceFrame >::space_type space_type;
        boost::function_requires< AffineSpaceConcept< space_type >();
        
        //! defines an origin. - which is accessed via template. 
        //! TODO: How to constrain this?
        //template <typename Point>
        //Point reference_frame_traits::get_origin();
    }
};

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
