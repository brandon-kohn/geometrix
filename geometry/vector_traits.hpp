//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_VECTOR_TRAITS_HPP
#define _GENERATIVE_GEOMETRY_VECTOR_TRAITS_HPP
#pragma once

#include "numeric_traits.hpp"
#include "dimension_traits.hpp"
#include "indexed_access_traits.hpp"
#include "coordinate_sequence_traits.hpp"
#include "reference_frame_tag.hpp"
#include <boost/lambda/lambda.hpp>

namespace generative
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
		BOOST_STATIC_ASSERT( generative::numeric::geometry::vector_traits<Vector>::dimension_type::value > 0 );
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
		BOOST_STATIC_ASSERT( generative::numeric::geometry::vector_traits<Vector>::dimension_type::value == 2 );
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
		BOOST_STATIC_ASSERT( generative::numeric::geometry::vector_traits<V>::dimension_type::value == 3 );
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
namespace generative{ namespace numeric{ namespace geometry {                                                       \
template <> struct is_vector< Vector > : boost::true_type{};                                                   \
template <>                                                                                                    \
struct vector_traits< Vector > : public coordinate_sequence_traits< Vector >                                   \
{                                                                                                              \
    typedef Vector vector_type;                                                                                \
};                                                                                                             \
}}}                                                                                                            \
/***/


//! \brief Reference frame must define a tagging type for vector sequences.
template <typename CoordinateSequence, typename ReferenceFrame>
struct reference_frame_tag< CoordinateSequence,
                            ReferenceFrame,
                            typename boost::enable_if< is_vector<CoordinateSequence> >::type
                          > : public CoordinateSequence
    , boost::addable< reference_frame_tag< CoordinateSequence, ReferenceFrame >       // vector + vector
    , boost::subtractable< reference_frame_tag< CoordinateSequence, ReferenceFrame >  // vector - vector
    , boost::dividable2< reference_frame_tag< CoordinateSequence, ReferenceFrame >,
    typename reference_frame_traits< ReferenceFrame >::coordinate_type     // vector / T
    , boost::multipliable2< reference_frame_tag< CoordinateSequence, ReferenceFrame >,
    typename reference_frame_traits< ReferenceFrame >::coordinate_type  // vector * T, T * vector
    > > > >
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

    //! Operator interface    
    template <typename NumericSequence>
    typename boost::enable_if< is_vector<NumericSequence>, reference_frame_tag >::type operator+= ( const NumericSequence& p )
    {        
        typedef typename resolve_coordinate_sequence< NumericSequence >::sequence_type numeric_sequence_type;
        boost::array< coordinate_type, dimension_type::value > temp = make_initialized_array( static_cast< sequence_type& >( *this ) );

        typedef boost::fusion::vector< boost::array< coordinate_type, dimension_type::value >&, const numeric_sequence_type& > sequences;
        boost::fusion::for_each( 
            boost::fusion::zip_view<sequences>( 
                sequences( temp, p ) ),
            make_fused_procedure( boost::lambda::_1 += boost::lambda::_2 ) );

        *this = construction_traits< sequence_type >::construct( temp );
        return *this;
    }

    // numeric_sequence operator+(numeric_sequence, const numeric_sequence&) automatically
    // generated by addable.
    template <typename NumericSequence>
    typename boost::enable_if< is_vector<NumericSequence>, reference_frame_tag >::type operator-= ( const NumericSequence& p )
    {
        typedef typename resolve_coordinate_sequence< NumericSequence >::sequence_type numeric_sequence_type;
        boost::array< coordinate_type, dimension_type::value > temp = make_initialized_array( static_cast< sequence_type& >( *this ) );

        typedef boost::fusion::vector< boost::array< coordinate_type, dimension_type::value >&, const numeric_sequence_type& > sequences;
        boost::fusion::for_each( 
            boost::fusion::zip_view<sequences>( 
            sequences( temp, p ) ),
            make_fused_procedure( boost::lambda::_1 -= boost::lambda::_2 ) );

        *this = construction_traits< sequence_type >::construct( temp );
        return *this;
    }
    // numeric_sequence operator-(numeric_sequence, const numeric_sequence&) automatically
    // generated by subtractable.

    reference_frame_tag operator*= ( const coordinate_type& v ) 
    {
        boost::array< coordinate_type, dimension_type::value > temp = make_initialized_array( static_cast< sequence_type& >( *this ) );
        boost::fusion::for_each( temp, 
            static_cast< const numeric_sequence_type& >( p ),
            boost::lambda::_1 *= v );
        *this = construction_traits< sequence_type >::construct( temp );
        return *this;
    }
    // numeric_sequence operator*(numeric_sequence, const T&) and
    // numeric_sequence operator*(const T&, numeric_sequence) auto-generated
    // by multipliable.

    reference_frame_tag operator/= ( const coordinate_type& v ) 
    {
        boost::array< coordinate_type, dimension_type::value > temp = make_initialized_array( static_cast< sequence_type& >( *this ) );
        boost::fusion::for_each( temp, 
            static_cast< const numeric_sequence_type& >( p ),
            boost::lambda::_1 /= v );
        *this = construction_traits< sequence_type >::construct( temp );
        return *this; 
    }

};

template <typename Sequence, typename ReferenceFrame>
struct vector_traits< reference_frame_tag< Sequence, ReferenceFrame > > : public vector_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_vector< reference_frame_tag< Sequence, ReferenceFrame > > : is_vector< Sequence >{};

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_VECTOR_TRAITS_HPP
