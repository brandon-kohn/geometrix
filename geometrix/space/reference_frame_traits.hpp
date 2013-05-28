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

#include <geometrix/space/affine_space_traits.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>

#include <boost/units/quantity.hpp>

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

template <typename ReferenceFrame, typename Sequence, unsigned int Index>
struct unit_type_of    
{
    typedef typename ReferenceFrame::template basis<Index>::unit_type type;
};

template <typename ReferenceFrame, typename Sequence, unsigned int Index>
struct coordinate_type_of
{   
    typedef boost::units::quantity
        <
            typename unit_type_of<ReferenceFrame, Sequence, Index>::type
          , typename type_at<Sequence, Index>::type
        > type;
};

template <typename Unit, typename Numeric>                                          
struct construction_policy< boost::units::quantity< Unit, Numeric > >                                           
{ 
    template <typename N>
    static boost::units::quantity< Unit, Numeric > construct( const N& n ){ return boost::units::quantity< Unit, Numeric >( n * Unit() ); }
    template <typename N, typename Unit2>
    static boost::units::quantity< Unit, Numeric > construct( const boost::units::quantity<Unit2, N>& n ){ return boost::units::quantity< Unit, Numeric >( n ); }
};

//! Define numeric traits for the coordinate type.
template <typename Unit, typename Numeric>
struct is_numeric< boost::units::quantity< Unit, Numeric > > : boost::true_type{};

template <typename Unit, typename Numeric>
struct numeric_traits< boost::units::quantity< Unit, Numeric > >
{
    typedef boost::units::quantity< Unit, Numeric >      numeric_type;
    typedef typename boost::is_float< Numeric >::type    is_float;
    typedef typename boost::is_integral< Numeric >::type is_integral;
    typedef void                                         is_numeric;
    static const numeric_type                            zero(){ return construct<numeric_type>(0); }
    static const numeric_type                            one(){ return construct<numeric_type>(1); }    
    static const unsigned int digits = std::numeric_limits<Numeric>::digits10;

    BOOST_MPL_ASSERT_MSG
    (
        ( !( is_float::value && is_integral::value ) )
        , NUMERIC_TRAITS_CANNOT_DEFINE_FLOAT_AND_INTEGRAL
        , (boost::units::quantity< Unit, Numeric >)
    ); 
};

}//namespace geometrix;

#endif //GEOMETRIX_REFERENCE_FRAME_TRAITS_HPP
