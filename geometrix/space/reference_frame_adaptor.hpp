//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_REFERENCE_FRAME_TAG_HPP
#define GEOMETRIX_REFERENCE_FRAME_TAG_HPP

#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/space/coordinate_sequence.hpp>
#include <geometrix/space/reference_frame_traits.hpp>
#include <geometrix/space/reference_frame_transformation.hpp>
#include <geometrix/utility/construction_policy.hpp>

#include <boost/operators.hpp>
#include <boost/utility.hpp>
#include <boost/fusion/support/is_sequence.hpp>

namespace geometrix {
    
    //! type trait to check if a type is tagged with a frame of reference.
    template <typename T>
    struct is_reference_frame_adaptor 
        : boost::false_type 
    {};

    //! Base reference frame tag declaration.
    template <typename CoordinateSequence, typename ReferenceFrame, typename Enable = void>
    struct reference_frame_adaptor;

    template <typename CoordinateSequence, typename ReferenceFrame>
    struct reference_frame_of < reference_frame_adaptor< CoordinateSequence, ReferenceFrame > >
    {
        typedef ReferenceFrame type;
    };

    template <typename CoordinateSequence, typename ReferenceFrame>
    struct construction_policy< reference_frame_adaptor< CoordinateSequence, ReferenceFrame > >
    {
        typedef reference_frame_adaptor< CoordinateSequence, ReferenceFrame >    frame_tag;
        typedef typename geometric_traits< CoordinateSequence >::arithmetic_type arithmetic_type;
        typedef typename frame_tag::dimension_type                               dimension_type;

        static frame_tag construct( const CoordinateSequence& sequence )
        {
            return frame_tag( sequence );
        }

        //! FIXME: This isn't correct.
        static frame_tag construct( const boost::array< arithmetic_type, dimension_type::value >& sequence )
        {
            return frame_tag( sequence );
        }
    };

    //! \brief Mark reference frame tagged types as having a reference frame.
    template <typename T, typename F>
    struct is_reference_frame_adaptor< reference_frame_adaptor<T,F> >
        : boost::true_type 
    {};

    //! setup traits for base types
    template <typename Sequence, typename ReferenceFrame>
    struct geometric_traits< reference_frame_adaptor< Sequence, ReferenceFrame > > 
        : geometric_traits< Sequence >
    {
        typedef ReferenceFrame reference_frame;
    };

    template <typename Sequence, typename ReferenceFrame>
    struct tensor_traits< reference_frame_adaptor< Sequence, ReferenceFrame > > 
        : tensor_traits< typename remove_const_ref<Sequence>::type >
    {};

}//namespace geometrix;

namespace boost { namespace mpl {

    template<typename>                              
    struct sequence_tag;                            
                                                
    template <typename Sequence, typename ReferenceFrame>                       
    struct sequence_tag< geometrix::reference_frame_adaptor< Sequence, ReferenceFrame > >
    {                                               
        typedef fusion::fusion_sequence_tag type;   
    };                                              
    template <typename Sequence, typename ReferenceFrame>
    struct sequence_tag<const geometrix::reference_frame_adaptor< Sequence, ReferenceFrame > >
    {                                               
        typedef fusion::fusion_sequence_tag type;   
    };

}}//namespace boost::mpl;

#endif //GEOMETRIX_REFERENCE_FRAME_TAG_HPP
