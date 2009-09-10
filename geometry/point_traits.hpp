//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_POINT_TRAITS_HPP
#define GENERATIVE_GEOMETRY_POINT_TRAITS_HPP
#pragma once

#include "reference_frame_tag.hpp"
#include "vector_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a point type.

//! \ingroup Type Traits
template <typename Point>
struct is_point : boost::false_type{};

//! \brief A traits type to define a point

//! \ingroup PrimitiveTraits
//! \ingroup Primitives
//! point_traits are a type trait which contains information about 
//! a type which models a PointConcept.
//! The point_traits type is a specialization of coordinate_sequence_traits.
//! It is meant to be specialized and to contain:
//!    - dimension_type  (static size)
//!    - value_type      (type of elements contained)
//!    - numeric_type    (same as value_type but named more descriptively)
//!    - coordinate_type (numeric type with additional metric notion attached)
//!    - reference       (a definition of a reference type to element)
//!    - const_reference (a definition of a const reference type to element)
//!
//! \see GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS
//! \note point_traits must not overlap with vector_traits if defined via macros.\n
//! point_traits must be specialized for user types.
template <typename Point>
struct point_traits : public coordinate_sequence_traits<Point>
{
	BOOST_MPL_ASSERT_MSG( 
         ( false )
		,POINT_TRAITS_NOT_DEFINED
		,(Point) );

    typedef Point point_type;

};

//! \brief Concept to describe a point location in \f$n\f$-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Point>
struct PointConcept
{
	void constraints()
	{
		boost::function_requires< CoordinateSequenceConcept< Point > >();

        //! Check that there is a greater than zero dimensionality
		BOOST_STATIC_ASSERT( generative::numeric::geometry::point_traits<Point>::dimension_type::value > 0 );			
	}
};

//! \brief Concept to describe a point location in 2-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Point>
struct Point2DConcept
{
	void constraints()
	{
		boost::function_requires< PointConcept< Point > >();
		boost::function_requires< DimensionConcept< Point, 2 > >();
	}
};

//! \brief Concept to describe a point location in 3-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Point>
struct Point3DConcept
{
	void constraints()
	{			
		boost::function_requires< PointConcept< Point > >();
		boost::function_requires< DimensionConcept< Point, 3 > >();
	}
};	

//! \def BOOST_DEFINE_POINT_TRAITS( Point, ReferenceFrame )
//! Macro for point type with deducible traits.
#define BOOST_DEFINE_POINT_TRAITS( Point, ReferenceFrame )              \
BOOST_DEFINE_COORDINATE_SEQUENCE_TRAITS( Point, ReferenceFrame )        \
BOOST_DEFINE_INDEXED_ACCESS_TRAITS( Point )                             \
template <> struct is_point< Point > : boost::true_type{};              \
template <>                                                             \
struct point_traits< Point > : public coordinate_sequence_traits<Point> \
{                                                                       \
    typedef Point point_type;                                           \
};

//! \def GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS( Point, NumericType, Dimension, ReferenceFrame, IndexedSequenceAccess )
//! A macro for defining point_traits for a user defined point type.\n
//! Example usage:
//! \code
//! struct point
//! {
//!     double x;
//!     double y;
//! 
//!     template <unsigned int Index>
//!     struct access{ };
//! 
//!     template <>
//!     struct access<0>
//!     {
//!         static const double& get( const point& p ){ return p.x; } 
//!         static double&       get( point& p ){ return p.x; } 
//!     };
//!
//!     template <>
//!     struct access<1>
//!     {
//!         static const double& get( const point& p ){ return p.y; } 
//!         static double&       get( point& p ){ return p.y; } 
//!     };
//!     
//!     template <unsigned int Index>
//!     double&           get() { return access<Index>::get( *this ); }
//!     template <unsigned int Index>
//!     const double&     get() const { return access<Index>::get( *this ); }
//! };
//! 
//! GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS( point, double, 2, neutral_reference_frame, require_compile_time_access_policy );
//! \endcode
#define GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS( Point, NumericType, Dimension, ReferenceFrame, IndexedSequenceAccess )\
GENERATIVE_GEOMETRY_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( Point, NumericType, Dimension, ReferenceFrame )                 \
GENERATIVE_GEOMETRY_DEFINE_USER_INDEXED_ACCESS_TRAITS( Point, IndexedSequenceAccess )                                       \
namespace generative{ namespace numeric{ namespace geometry {                                                               \
template <> struct is_point< Point > : boost::true_type{};                                                                  \
template <>                                                                                                                 \
struct point_traits< Point > : public coordinate_sequence_traits< Point >                                                   \
{                                                                                                                           \
    typedef Point point_type;                                                                                               \
};                                                                                                                          \
}}}                                                                                                                         \
/***/

//! \brief Reference frame must define a tagging type for point sequences.
template <typename CoordinateSequence, typename ReferenceFrame>
struct reference_frame_tag< CoordinateSequence,
                            ReferenceFrame,
                            typename boost::enable_if< is_point<CoordinateSequence> >::type >
: public CoordinateSequence
{
    typedef typename resolve_coordinate_sequence< CoordinateSequence >::sequence_type sequence_type;    
    typedef ReferenceFrame                                                            reference_frame_type;
    typedef typename reference_frame_traits< reference_frame_type >::coordinate_type  coordinate_type;
    typedef typename reference_frame_traits< reference_frame_type >::dimension_type   dimension_type;

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

template <typename Sequence, typename ReferenceFrame>
struct point_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public point_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_point< reference_frame_tag< Sequence, ReferenceFrame > > : is_point< Sequence >{};

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_POINT_TRAITS_HPP
