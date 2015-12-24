//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_TRAITS_HPP
#define GEOMETRIX_VECTOR_TRAITS_HPP

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/space/coordinate_sequence.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/space/reference_frame_adaptor.hpp>
#include <boost/lambda/lambda.hpp>

namespace geometrix {

//! \brief type check to check if a type is a vector type.

//! \ingroup Type Traits
template <typename Vector, typename Enable = void>
struct is_vector : boost::false_type{};

//! \brief A tag to designate a vector.
struct vector_tag {};

template <typename Vector>
struct is_vector<Vector, typename geometric_traits<Vector>::is_vector> : boost::true_type{};

//! Concept to describe a vector magnitude and direction in \f$n\f$-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Vector>
struct VectorConcept
{
    BOOST_CONCEPT_ASSERT((CoordinateSequenceConcept< Vector >));    
};

//! Concept to describe a vector magnitude and direction in 2-dimensional space.

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename Vector>
struct Vector2DConcept
{
    BOOST_CONCEPT_ASSERT((VectorConcept< Vector >));
    BOOST_CONCEPT_ASSERT((DimensionConcept<Vector,2>));
};

//! Concept to describe a vector magnitude and direction in 3-dimensional space.

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename Vector>
struct Vector3DConcept
{
    BOOST_CONCEPT_ASSERT((VectorConcept<Vector>));
    BOOST_CONCEPT_ASSERT((DimensionConcept<Vector,3>));
};

//! \def GEOMETRIX_DEFINE_VECTOR_TRAITS( Vector, NumericTypes, Dimension, ReferenceFrame, AccessPolicy)
//! A macro for defining geometric_traits for a user defined vector type.\n
//! Example usage:
//! \code
//! struct vector
//! {
//!     double x;
//!     double y;
//! 
//!     template <std::size_t Index>
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
//!     template <std::size_t Index>
//!     double&       get() { return access<Index>::get( *this ); }
//!     template <std::size_t Index>
//!     const double& get() const { return access<Index>::get( *this ); }
//! };
//! 
//! GEOMETRIX_DEFINE_USER_VECTOR_TRAITS( vector, double, 2, double, neutral_reference_frame);
//! \endcode
#define GEOMETRIX_DEFINE_VECTOR_TRAITS( Vector, NumericTypes, Dimension, ArithmeticType, ReferenceFrame, AccessPolicy) \
GEOMETRIX_DEFINE_TENSOR_TRAITS( Vector, 1, AccessPolicy )                                                              \
GEOMETRIX_DEFINE_MPL_SEQUENCE_TRAITS(Vector)                                                                           \
namespace geometrix {                                                                                                  \
template <>                                                                                                            \
struct geometric_traits< Vector >                                                                                      \
{                                                                                                                      \
    typedef Vector vector_type;                                                                                        \
    typedef void   is_vector;                                                                                          \
    typedef ReferenceFrame                        reference_frame;                                                     \
    typedef void                                  is_coordinate_sequence;                                              \
    typedef ArithmeticType                        arithmetic_type;                                                     \
    typedef void                                  is_numeric_sequence;                                                 \
    typedef Vector                                sequence_type;                                                       \
    typedef GEOMETRIX_AS_MPL_VECTOR(NumericTypes) storage_types;                                                       \
    typedef dimension<Dimension>                  dimension_type;                                                      \
    typedef void                                  is_sequence;                                                         \
    typedef void GEOMETRIX_TYPE_DIVERSITY_TAG(NumericTypes);                                                           \
};                                                                                                                     \
}                                                                                                                      \
/***/

//! Traits for objects which are already fusion sequences.
#define GEOMETRIX_DEFINE_FUSION_VECTOR_TRAITS( Vector, NumericTypes, Dimension, ReferenceFrame, AccessPolicy)          \
GEOMETRIX_DEFINE_FUSION_TENSOR_TRAITS( Vector, 1, AccessPolicy )                                                       \
namespace geometrix {                                                                                                  \
template <>                                                                                                            \
struct geometric_traits< Vector >                                                                                      \
{                                                                                                                      \
    typedef void                                  is_vector;                                                           \
    typedef ReferenceFrame                        reference_frame;                                                     \
    typedef void                                  is_coordinate_sequence;                                              \
    typedef void                                  is_numeric_sequence;                                                 \
    typedef Vector                                sequence_type;                                                       \
    typedef GEOMETRIX_AS_MPL_VECTOR(NumericTypes) storage_types;                                                       \
    typedef dimension<Dimension>                  dimension_type;                                                      \
    typedef void                                  is_sequence;                                                         \
    typedef void GEOMETRIX_TYPE_DIVERSITY_TAG(NumericTypes);                                                           \
};                                                                                                                     \
}                                                                                                                      \
/***/

//! \brief Reference frame must define a tagging type for vector sequences.
template <typename Vector, typename ReferenceFrame>
struct reference_frame_adaptor
    <
        Vector
      , ReferenceFrame
      , typename geometric_traits<Vector>::is_vector
    > : remove_const_ref<Vector>::type
{
    typedef typename remove_const_ref<Vector>::type                  sequence_type;    
    typedef ReferenceFrame                                           reference_frame;
    typedef typename geometric_traits<sequence_type>::dimension_type dimension_type;

    //! construct from the raw sequence.
    explicit reference_frame_adaptor( const sequence_type& sequence )
        : sequence_type( sequence )
    {}

    //! construct from a copy
    explicit reference_frame_adaptor( const reference_frame_adaptor<sequence_type, reference_frame>& sequence )
        : sequence_type( sequence )
    {}

    //! automatic transform
    template <typename OtherCoordinateSequence, typename OtherReferenceFrame>
    reference_frame_adaptor( const reference_frame_adaptor<OtherCoordinateSequence, OtherReferenceFrame>& sequence )
        : sequence_type( reference_frame_transformation<OtherReferenceFrame, reference_frame>::template transform<sequence_type>( sequence ) )
    {}

    reference_frame_adaptor& operator =( const sequence_type& sequence )
    {
        *this = reference_frame_adaptor( sequence );
        return *this;
    }

    template <typename OtherCoorindateSequence>
    reference_frame_adaptor& operator =( const OtherCoorindateSequence& sequence )
    {
        *this = reference_frame_adaptor( construct< sequence_type >( sequence ) );
        return *this;
    }

    //! automatic transform
    template <typename OtherCoordinateSequence, typename OtherReferenceFrame>
    reference_frame_adaptor& operator =( const reference_frame_adaptor< OtherCoordinateSequence, OtherReferenceFrame >& sequence )
    {
        *this = reference_frame_transformation< OtherReferenceFrame, reference_frame >::template transform<sequence_type>( sequence );
        return *this;
    }
};

//! Treat a vector as a point.
template <typename Vector>
struct point_adaptor : Vector
{ 
    BOOST_STATIC_ASSERT(( is_vector<Vector>::value ));
    point_adaptor( const Vector& p ) 
        : Vector( p )
    {}
};

template <typename Vector>
struct is_vector< point_adaptor<Vector> > : boost::false_type{};

template <typename Sequence>
struct geometric_traits< point_adaptor< Sequence > > 
    : diversity_base< Sequence >
{
    typedef point_adaptor<Sequence>                              point_type;
    typedef void                                                 is_point;
    typedef typename geometric_traits<Sequence>::reference_frame reference_frame;               
    typedef void                                                 is_coordinate_sequence;        
    typedef typename geometric_traits<Sequence>::arithmetic_type arithmetic_type;               
    typedef void                                                 is_numeric_sequence;           
    typedef Sequence                                             sequence_type;                 
    typedef typename dimension_of<Sequence>::type                dimension_type;                
    typedef void                                                 is_sequence;    
};

template <typename Vector>
point_adaptor<Vector> as_point( const Vector& p )
{
    return point_adaptor<Vector>( p );
}

template <typename Vector>
struct tensor_traits< point_adaptor<Vector> > 
    : tensor_traits< typename remove_const_ref<Vector>::type >
{};

}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_TRAITS_HPP
