//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_VECTOR_TRAITS_HPP
#define _BOOST_GEOMETRY_VECTOR_TRAITS_HPP
#pragma once

#include "numeric_traits.hpp"
#include "dimension_traits.hpp"
#include "indexed_access_traits.hpp"
#include "coordinate_sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a vector type.

//! \ingroup Type Traits
template <typename Vector>
struct is_vector : boost::false_type{};

//! \ingroup PrimitiveTraits
//! \ingroup Primitives
//! vector_traits are a type trait base type which holds information about 
//! a type which models a PointConcept.
//! The vector_traits type is a specialization of coordinate_sequence_traits.
//! vector_traits is meant to be specialized and to contain:
//!    - dimension_type  (static size)
//!    - value_type      (type of elements contained)
//!    - numeric_type    (same as value_type but named more descriptively)
//!    - coordinate_type (numeric type with additional metric notion attached)
//!    - reference       (a definition of a reference type to element)
//!    - const_reference (a definition of a const reference type to element)
//!
//! \see BOOST_DEFINE_USER_POINT_TRAITS
//! \note vector_traits must not overlap with point_traits if defined via macros.
//! \note vector_traits must be specialized for user types.
template <typename Vector>
struct vector_traits : public coordinate_sequence_traits<Vector>
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, VECTOR_TRAITS_NOT_DEFINED
		, (Vector) );	

    typedef void vector_type;
};

//! Concept to describe a vector magnitude and direction in \f$n\f$-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Vector>
struct VectorConcept
{
	void constraints()
	{
		boost::function_requires< CoordinateSequenceConcept< Vector > >();

        //! Check that there is a greater than zero dimensionality
		BOOST_STATIC_ASSERT( boost::numeric::geometry::vector_traits<Vector>::dimension_type::value > 0 );
	}
};

//! Concept to describe a vector magnitude and direction in 2-dimensional space.

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename Vector>
struct Vector2DConcept
{
	void constraints()
	{
		boost::function_requires< VectorConcept< Vector > >();        
		BOOST_STATIC_ASSERT( boost::numeric::geometry::vector_traits<Vector>::dimension_type::value == 2 );
	}
};

//! Concept to describe a vector magnitude and direction in 3-dimensional space.

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename V>
struct Vector3DConcept
{
	void constraints()
	{
		boost::function_requires< VectorConcept< V > >();        
		BOOST_STATIC_ASSERT( boost::numeric::geometry::vector_traits<V>::dimension_type::value == 3 );
	}
};

//! \def BOOST_DEFINE_VECTOR_TRAITS( Vector, ReferenceFrame )
//! Macro for vector type with deducible traits
#define BOOST_DEFINE_VECTOR_TRAITS( Vector, ReferenceFrame )                \
BOOST_DEFINE_COORDINATE_SEQUENCE_TRAITS( Vector, ReferenceFrame )           \
BOOST_DEFINE_INDEXED_ACCESS_TRAITS( Vector )                                \
template <> struct is_vector< Vector > : boost::true_type{};                \
template <>                                                                 \
struct vector_traits< Vector > : public coordinate_sequence_traits< Vector >\
{                                                                           \
    typedef Vector vector_type;                                             \
};

//! \def BOOST_DEFINE_USER_VECTOR_TRAITS( Vector, NumericType, Dimension, ReferenceFrame, IndexedSequenceAccess)
//! A macro for defining vector_traits for a user defined vector type.\n
//! Example usage:
//! \code
//! struct vector
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
//!         static const double& get( const vector& v ){ return v.x; } 
//!         static double&       get( vector& v ){ return v.x; } 
//!     };
//!
//!     template <>
//!     struct access<1>
//!     {
//!         static const double& get( const vector& v ){ return v.y; } 
//!         static double&       get( vector& v ){ return v.y; } 
//!     };
//!     
//!     template <unsigned int Index>
//!     inline double&       get() { return access<Index>::get( *this ); }
//!     template <unsigned int Index>
//!     inline const double& get() const { return access<Index>::get( *this ); }
//! };
//! 
//! BOOST_DEFINE_USER_VECTOR_TRAITS( vector, double, 2, neutral_reference_frame, use_compile_time_access );
//! \endcode
#define BOOST_DEFINE_USER_VECTOR_TRAITS( Vector, NumericType, Dimension, ReferenceFrame, IndexedSequenceAccess)\
BOOST_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( Vector, NumericType, Dimension, ReferenceFrame )                 \
BOOST_DEFINE_USER_INDEXED_ACCESS_TRAITS( Vector, IndexedSequenceAccess )                                       \
namespace boost{ namespace numeric{ namespace geometry {                                                       \
template <> struct is_vector< Vector > : boost::true_type{};                                                   \
template <>                                                                                                    \
struct vector_traits< Vector > : public coordinate_sequence_traits< Vector >                                   \
{                                                                                                              \
    typedef Vector vector_type;                                                                                \
};                                                                                                             \
}}}                                                                                                            \

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_VECTOR_TRAITS_HPP
