//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_COORDINATE_SEQUENCE_TRAITS_HPP
#define GENERATIVE_GEOMETRY_COORDINATE_SEQUENCE_TRAITS_HPP
#pragma once

#include <geometry\numeric_sequence_traits.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a coordinate_sequence
template <typename Sequence, typename EnableIf = void>
struct is_coordinate_sequence : boost::false_type{};

//! \brief A traits type to define a sequence of a coordinate type with a static dimensionality.

//! A coordinate sequence is a numeric sequence with an additional property assigned to the coordinate type.
//! In practical terms the additional property would conform to a metric type on the type of the coordinate for each dimension.
//! Examples of metric type include length for Cartesian coordinates and the radius in polar coordinates, angle for the
//! angular distances expressed in the theta and phi coordinates of polar coordinates etc.
//! \todo It remains to rationalize how units would work at this level and impact the derived types.
//! coordinate_sequence_traits are a type trait base type which holds information about a static sized sequence of numbers.
//! The coordinate_sequence_traits type is a specialization of numeric_sequence_traits.
//! coordinate_sequence_traits is meant to be specialized and should contain:
//!    - dimension_type  (static size)
//!    - value_type      (type of elements contained)
//!    - numeric_type    (same as value_type but named more descriptively)
//!    - coordinate_type (numeric type with additional metric notion attached)
//!    - reference       (a definition of a reference type to element)
//!    - const_reference (a definition of a const reference type to element)
//!
//! NOTE: must be specialized for user types.
//! \see GENERATIVE_GEOMETRY_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS
template <typename CoordinateSequence>
struct coordinate_sequence_traits : public numeric_sequence_traits<CoordinateSequence>
{
	BOOST_MPL_ASSERT_MSG(
		 ( false )
		,COORDINATE_SEQUENCE_TRAITS_NOT_DEFINED
		,(CoordinateSequence) );	

    typedef CoordinateSequence coordinate_sequence_type;
	typedef void               coordinate_type;    
    typedef void               reference_frame_type;

};

//! \brief Concept check for a type which models a CoordinateSequence.

//! A CoordinateSequence is a model of a NumericSequence type with
//! additional information about the metrics of the numeric type.
template <typename CoordinateSequence>
struct CoordinateSequenceConcept
{
    void constraints()
    {
        boost::function_requires< NumericSequenceConcept< CoordinateSequence > >();
        typedef typename coordinate_sequence_traits<CoordinateSequence>::coordinate_type coordinate_type;
        //!TODO possibly add metric-identifying coordinate traits.
    }
};

//! \brief helper types to figure out the frame type between reference_frame_tag types and regular sequences.
template <typename T>
struct resolve_reference_frame
{
    typedef typename coordinate_sequence_traits< T >::reference_frame_type reference_frame_type;
};

//! \brief helper types to figure out the sequence type between reference_frame_tag types and regular sequences.
template <typename CoordinateSequence>
struct resolve_coordinate_sequence
{
    typedef CoordinateSequence sequence_type;
};

//! \brief Macro for coordinate sequence type with embedded traits
//! NOTE: This macro is called by GENERATIVE_GEOMETRY_DEFINE_POINT_TRAITS and GENERATIVE_GEOMETRY_DEFINE_VECTOR_TRAITS. Users should use those to avoid overlapping defines.
#define GENERATIVE_GEOMETRY_DEFINE_COORDINATE_SEQUENCE_TRAITS( CoordinateSequence, ReferenceFrame )       \
GENERATIVE_GEOMETRY_DEFINE_NUMERIC_SEQUENCE_TRAITS( CoordinateSequence )                                  \
template <> struct is_coordinate_sequence< CoordinateSequence > : boost::true_type{};                     \
template <>                                                                                               \
struct coordinate_sequence_traits<CoordinateSequence> : public numeric_sequence_traits<CoordinateSequence>\
{                                                                                                         \
	BOOST_STATIC_ASSERT( CoordinateSequence::dimension_type::value > 0 );                                 \
    typedef CoordinateSequence coordinate_sequence_type;                                                  \
    typedef numeric_type       coordinate_type;                                                           \
    typedef ReferenceFrame     reference_frame_type;                                                      \
};                                                                                                        \
/***/

//! A macro for defining coordinate_sequence_traits for a user defined Sequence type.
//! NOTE: This macro is called by GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS and GENERATIVE_GEOMETRY_DEFINE_USER_VECTOR_TRAITS. 
//! Users should use these to avoid overlapping defines.
//! Example usage:
//! \code
//! struct sequence
//! {
//!     double x;
//!     double y;
//! };
//! 
//! GENERATIVE_GEOMETRY_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( sequence, double, 2 );
//! \endcode
#define GENERATIVE_GEOMETRY_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( CoordinateSequence, NumericType, Dimension, ReferenceFrame )\
GENERATIVE_GEOMETRY_DEFINE_USER_NUMERIC_SEQUENCE_TRAITS( CoordinateSequence, NumericType, Dimension )             \
namespace generative{ namespace numeric{ namespace geometry {                                                     \
template <> struct is_coordinate_sequence< CoordinateSequence > : boost::true_type{};                             \
template <>                                                                                                       \
struct coordinate_sequence_traits<CoordinateSequence> : public numeric_sequence_traits<CoordinateSequence>        \
{                                                                                                                 \
    typedef numeric_sequence_type coordinate_sequence_type;                                                       \
    typedef numeric_type          coordinate_type;                                                                \
    typedef ReferenceFrame        reference_frame_type;                                                           \
};                                                                                                                \
}}}                                                                                                               \
/***/

}}}//namespace generative::coordinate::geometry;

#endif //GENERATIVE_GEOMETRY_COORDINATE_SEQUENCE_TRAITS_HPP
