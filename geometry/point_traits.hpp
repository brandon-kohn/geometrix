//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_TRAITS_HPP
#define _BOOST_GEOMETRY_POINT_TRAITS_HPP
#pragma once

#include "indexed_access_traits.hpp"
#include "coordinate_sequence_traits.hpp"

namespace boost
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
//! \see BOOST_DEFINE_USER_POINT_TRAITS
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
		BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<Point>::dimension_type::value > 0 );			
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
		BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<Point>::dimension_type::value == 2 );
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
		BOOST_STATIC_ASSERT( boost::numeric::geometry::point_traits<Point>::dimension_type::value == 3 );
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

//! \def BOOST_DEFINE_USER_POINT_TRAITS( Point, NumericType, Dimension, ReferenceFrame, IndexedSequenceAccess )
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
//!     inline double&           get() { return access<Index>::get( *this ); }
//!     template <unsigned int Index>
//!     inline const double&     get() const { return access<Index>::get( *this ); }
//! };
//! 
//! BOOST_DEFINE_USER_POINT_TRAITS( point, double, 2, neutral_reference_frame, use_compile_time_access );
//! \endcode
#define BOOST_DEFINE_USER_POINT_TRAITS( Point, NumericType, Dimension, ReferenceFrame, IndexedSequenceAccess )\
BOOST_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( Point, NumericType, Dimension, ReferenceFrame )                 \
BOOST_DEFINE_USER_INDEXED_ACCESS_TRAITS( Point, IndexedSequenceAccess )                                       \
namespace boost{ namespace numeric{ namespace geometry {                                                      \
template <> struct is_point< Point > : boost::true_type{};                                                    \
template <>                                                                                                   \
struct point_traits< Point > : public coordinate_sequence_traits< Point >                                     \
{                                                                                                             \
    typedef Point point_type;                                                                                 \
};                                                                                                            \
}}}                                                                                                           \

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_TRAITS_HPP
