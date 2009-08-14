//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_REFERENCE_FRAME_TAG_HPP
#define GENERATIVE_GEOMETRY_REFERENCE_FRAME_TAG_HPP
#pragma once

#include "indexed_access_traits.hpp"
#include "coordinate_sequence_traits.hpp"
#include "reference_frame_traits.hpp"
#include "reference_frame_transformation.hpp"
#include "construction_traits.hpp"
#include "indexed_sequence_traversal.hpp"
#include <boost/operators.hpp>
#include <boost/utility.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    
//! type trait to check if a type is tagged with a frame of reference.
template <typename T>
struct is_reference_frame_tag : boost::false_type {};

//! Base reference frame tag declaration.
template <typename CoordinateSequence, typename ReferenceFrame, typename Enable = void>
struct reference_frame_tag{};

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

    static inline frame_tag construct( const boost::array<
        coordinate_type,
        dimension_type::value 
    >& sequence )
    {
        return frame_tag( sequence );
    }
};

//! \brief Mark reference frame tagged types as having a reference frame.
template <typename T, typename F>
struct is_reference_frame_tag< reference_frame_tag<T,F> > : boost::true_type {};

//! setup traits for base types
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

}}}//namespace generative::numeric::geometry

#endif //GENERATIVE_GEOMETRY_REFERENCE_FRAME_TAG_HPP
