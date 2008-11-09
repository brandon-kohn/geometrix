//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_REFERENCE_FRAME_TAG_HPP
#define _BOOST_GEOMETRY_REFERENCE_FRAME_TAG_HPP
#pragma once

#include "reference_frame_traits.hpp"
#include "reference_frame_transformation.hpp"
#include "construction_traits.hpp"
#include "coordinate_sequence_traits.hpp"
#include "vector_traits.hpp"
#include "point_traits.hpp"
#include "segment_traits.hpp"
#include "sphere_traits.hpp"
#include "point_sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
    
//! type trait to check if a type is tagged with a frame of reference.
template <typename T>
struct is_reference_frame_tag : boost::false_type {};

//! Reference frame must define a tagging type templated for all sequence primitives.
template <typename CoordinateSequence, typename ReferenceFrame>
struct reference_frame_tag : public CoordinateSequence
{
    typedef CoordinateSequence                                                       sequence_type;    
    typedef ReferenceFrame                                                           reference_frame_type;
    typedef typename reference_frame_traits< reference_frame_type >::coordinate_type coordinate_type;
    typedef typename reference_frame_traits< reference_frame_type >::dimension_type  dimension_type;
    
    //! construct from the raw sequence.
    explicit reference_frame_tag( const sequence_type& sequence )
        : sequence_type( sequence )
    {}

    //! construct from the raw sequence.
    template <typename OtherCoordinateSequence>
    explicit reference_frame_tag( const OtherCoordinateSequence& sequence )
        : sequence_type( construction_traits< sequence_type >::construct( sequence ) )
    {}

    explicit reference_frame_tag( const boost::array<coordinate_type, dimension_type::value>& sequence )
        : sequence_type( construction_traits< sequence_type >::construct( sequence ) )
    {}

    //! construct from a copy
    explicit reference_frame_tag( const reference_frame_tag<sequence_type, reference_frame_type>& sequence )
        : sequence_type( sequence )
    {}

    //! automatic transform
    template <typename OtherCoordinateSequence, typename OtherReferenceFrame>
    reference_frame_tag( const reference_frame_tag< OtherCoordinateSequence, OtherReferenceFrame >& sequence )
        : sequence_type( reference_frame_transformation< OtherReferenceFrame, reference_frame_type >::transform<sequence_type>( sequence ) )
    {}

    reference_frame_tag& operator =( const sequence_type& sequence )
    {
        *this = reference_frame_tag( sequence );
        return *this;
    }

    template <typename OtherCoorindateSequence>
    reference_frame_tag& operator =( const OtherCoorindateSequence& sequence )
    {
        *this = reference_frame_tag( construction_traits< sequence_type >::construct( sequence ) );
        return *this;
    }

    //! automatic transform
    template <typename OtherCoordinateSequence, typename OtherReferenceFrame>
    reference_frame_tag& operator =( const reference_frame_tag< OtherCoordinateSequence, OtherReferenceFrame >& sequence )
    {
        *this = reference_frame_transformation< OtherReferenceFrame, reference_frame_type >::transform<sequence_type>( sequence );
        return *this;
    }
};

template <typename CoordinateSequence, typename ReferenceFrame>
struct resolve_reference_frame < reference_frame_tag< CoordinateSequence, ReferenceFrame > >
{
    typedef ReferenceFrame reference_frame_type;
};

template <typename CoordinateSequence, typename ReferenceFrame>
struct resolve_coordinate_sequence< reference_frame_tag< CoordinateSequence, ReferenceFrame > >
{
    typedef CoordinateSequence sequence_type;
};

template <typename CoordinateSequence, typename ReferenceFrame>
struct construction_traits< reference_frame_tag< CoordinateSequence, ReferenceFrame > >
{
    typedef reference_frame_tag< CoordinateSequence, ReferenceFrame > frame_tag;
    typedef typename frame_tag::coordinate_type                       coordinate_type;
    typedef typename frame_tag::dimension_type                        dimension_type;

    static inline frame_tag construct( const CoordinateSequence& sequence )
    {
        return frame_tag( sequence );
    }

    static inline frame_tag construct( const boost::array<coordinate_type,
                                       dimension_type::value>& sequence )
    {
        return frame_tag( sequence );
    }
};

//! \brief Mark reference frame tagged types as having a reference frame.
template <typename T, typename F>
struct is_reference_frame_tag< reference_frame_tag<T,F> > : boost::true_type {};

//! setup traits for base types
template <typename Sequence, typename ReferenceFrame>
struct point_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public point_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_point< reference_frame_tag< Sequence, ReferenceFrame > > : is_point< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct vector_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public vector_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_vector< reference_frame_tag< Sequence, ReferenceFrame > > : is_vector< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct sphere_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public sphere_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_sphere< reference_frame_tag< Sequence, ReferenceFrame > > : is_sphere< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct coordinate_sequence_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public coordinate_sequence_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_coordinate_sequence< reference_frame_tag< Sequence, ReferenceFrame > > : is_coordinate_sequence< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct numeric_sequence_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public numeric_sequence_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_numeric_sequence< reference_frame_tag< Sequence, ReferenceFrame > > : is_numeric_sequence< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct sequence_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public sequence_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_sequence< reference_frame_tag< Sequence, ReferenceFrame > > : is_sequence< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct use_indexed_access_type< reference_frame_tag< Sequence, ReferenceFrame > > : use_indexed_access_type< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_indexed_sequence< reference_frame_tag< Sequence, ReferenceFrame > > : is_indexed_sequence< Sequence >{};

}}}//namespace boost::numeric::geometry

#endif //_BOOST_GEOMETRY_REFERENCE_FRAME_TAG_HPP
