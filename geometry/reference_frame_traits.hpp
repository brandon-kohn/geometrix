//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
#define _BOOST_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
#pragma once

#include "affine_space_traits.hpp"

namespace boost
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
    typedef typename ReferenceFrame::AffineSpace affine_space_type;        
    typedef typename ReferenceFrame              reference_frame_type;//! self reference for tagging primitives.

};

template <typename ReferenceFrame>
struct ReferenceFrameConcept
{
    void constraints()
    {
        //! defines an affine space (rather has the same traits as one).
        typedef typename reference_frame_traits< ReferenceFrame >::affine_space_type affine_space_type;
        boost::function_requires< AffineSpaceConcept< affine_space_type >();

        typedef typename affine_space_traits< affine_space_type >::dimension_type dimension_type;
        typedef typename affine_space_traits< affine_space_type >::numeric_type   numeric_type;
        BOOST_STATIC_ASSERT( dimension_type::value > 0 );            
        
        //! defines an origin. - which is accessed via template. 
        //! TODO: How to constrain this?
        //template <typename Point>
        //Point reference_frame_traits::get_origin();
    }
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_REFERENCE_FRAME_TRAITS_HPP
