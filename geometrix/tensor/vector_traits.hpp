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
    void constraints() const
    {
        boost::function_requires< CoordinateSequenceConcept< Vector > >();

        //! Check that there is a greater than zero tensor_order
        BOOST_STATIC_ASSERT( geometrix::geometric_traits<Vector>::dimension_type::value > 0 );
    }
};

//! Concept to describe a vector magnitude and direction in 2-dimensional space.

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename Vector>
struct Vector2DConcept
{
    void constraints() const
    {
        boost::function_requires< VectorConcept< Vector > >();        
        BOOST_STATIC_ASSERT( geometrix::geometric_traits<Vector>::dimension_type::value == 2 );
    }
};

//! Concept to describe a vector magnitude and direction in 3-dimensional space.

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename V>
struct Vector3DConcept
{
    void constraints() const
    {
        boost::function_requires< VectorConcept< V > >();        
        BOOST_STATIC_ASSERT( geometrix::geometric_traits<V>::dimension_type::value == 3 );
    }
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
//!     double&       get() { return access<Index>::get( *this ); }
//!     template <unsigned int Index>
//!     const double& get() const { return access<Index>::get( *this ); }
//! };
//! 
//! GEOMETRIX_DEFINE_USER_VECTOR_TRAITS( vector, double, 2, double, neutral_reference_frame, require_compile_time_access_policy );
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
    > : public Vector
{
    typedef typename remove_const_ref< Vector >::type                  sequence_type;    
    typedef ReferenceFrame                                             reference_frame;
    typedef typename geometric_traits< sequence_type >::dimension_type dimension_type;

    //! construct from the raw sequence.
    explicit reference_frame_adaptor( const sequence_type& sequence )
        : sequence_type( sequence )
    {}

    //! construct from the raw sequence.
    template <typename OtherCoordinateSequence>
    explicit reference_frame_adaptor( const OtherCoordinateSequence& sequence )
        : sequence_type( construct< sequence_type >( sequence ) )
    {}

    //! construct from a copy
    explicit reference_frame_adaptor( const reference_frame_adaptor<sequence_type, reference_frame>& sequence )
        : sequence_type( sequence )
    {}

    //! automatic transform
    template <typename OtherCoordinateSequence, typename OtherReferenceFrame>
    reference_frame_adaptor( const reference_frame_adaptor< OtherCoordinateSequence, OtherReferenceFrame >& sequence )
        : sequence_type( reference_frame_transformation< OtherReferenceFrame, reference_frame >::transform<sequence_type>( sequence ) )
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
        *this = reference_frame_transformation< OtherReferenceFrame, reference_frame >::transform<sequence_type>( sequence );
        return *this;
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_TRAITS_HPP
