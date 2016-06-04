//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_REFERENCE_FRAME_TRAITS_HPP
#define GEOMETRIX_REFERENCE_FRAME_TRAITS_HPP

#include <geometrix/numeric/boost_units_quantity.hpp>

namespace geometrix { 

//! \brief Defines the base traits of a frame of reference. 
template <typename ReferenceFrame, typename EnableIf=void>
struct reference_frame_traits
{
    BOOST_MPL_ASSERT_MSG( 
      ( false )
    , REFERENCE_FRAME_TRAITS_NOT_DEFINED
    , (ReferenceFrame) );

    //! Reference frame belongs to some affine space.
    typedef typename ReferenceFrame::space_type space_type;    
    typedef typename ReferenceFrame::basis_type basis_type;
    typedef ReferenceFrame                      reference_frame;//! self reference for tagging primitives.

};

template <typename ReferenceFrame>
struct ReferenceFrameConcept
{
    void constraints() const
    {
        //! defines an affine space (rather has the same traits as one).
        typedef typename reference_frame_traits< ReferenceFrame >::space_type space_type;
        BOOST_CONCEPT_ASSERT((AffineSpaceConcept< space_type >));        
    }
};

template <typename ReferenceFrame, typename Sequence, std::size_t Index>
struct unit_type_of    
{
    typedef typename ReferenceFrame::template basis<Index>::unit_type type;
};

template <typename ReferenceFrame, typename Sequence, std::size_t Index>
struct coordinate_type_of
{   
    typedef boost::units::quantity
        <
            typename unit_type_of<ReferenceFrame, Sequence, Index>::type
          , typename type_at<Sequence, Index>::type
        > type;
};

}//namespace geometrix;

#endif //GEOMETRIX_REFERENCE_FRAME_TRAITS_HPP
