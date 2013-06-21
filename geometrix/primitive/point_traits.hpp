//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_TRAITS_HPP
#define GEOMETRIX_POINT_TRAITS_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/space/reference_frame_adaptor.hpp>
#include <boost/concept_check.hpp>

namespace geometrix {

//! \brief type check to check if a type is a point type.

//! \ingroup Type Traits
template <typename Point, typename Enable = void>
struct is_point : boost::false_type{};

template <typename Point>
struct is_point<Point, typename geometric_traits<Point>::is_point> : boost::true_type{};

//! \brief A tag to designate a point.
struct point_tag {};

//! \brief Concept to describe a point location in \f$n\f$-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Point>
struct PointConcept
{
    BOOST_CONCEPT_ASSERT((CoordinateSequenceConcept< Point >));
};

//! \brief Concept to describe a point location in 2-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Point>
struct Point2DConcept
{
    BOOST_CONCEPT_ASSERT((PointConcept<Point>));
    BOOST_CONCEPT_ASSERT((DimensionConcept<Point, 2>));    
};

//! \brief Concept to describe a point location in 3-dimensional space.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Point>
struct Point3DConcept
{
    BOOST_CONCEPT_ASSERT((PointConcept<Point>));
    BOOST_CONCEPT_ASSERT((DimensionConcept< Point, 3 >));    
};	

//! \def GEOMETRIX_DEFINE_POINT_TRAITS( Point, NumericTypes, Dimension, ReferenceFrame, AccessPolicy )
//! A macro for defining geometric_traits for a user defined point type.\n
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
//! GEOMETRIX_DEFINE_POINT_TRAITS( point, (double), 2, double, neutral_reference_frame, my_access_policy );
//! \endcode
#define GEOMETRIX_DEFINE_POINT_TRAITS( Point, NumericTypes, Dimension, ArithmeticType, ReferenceFrame, AccessPolicy ) \
GEOMETRIX_DEFINE_TENSOR_TRAITS( Point, 1, AccessPolicy )                                                              \
GEOMETRIX_DEFINE_MPL_SEQUENCE_TRAITS(Point)                                                                           \
namespace geometrix {                                                                                                 \
template <>                                                                                                           \
struct geometric_traits< Point >                                                                                      \
{                                                                                                                     \
    BOOST_STATIC_ASSERT( Dimension > 0 );                                                                             \
    typedef Point                                 point_type;                                                         \
    typedef void                                  is_point;                                                           \
    typedef ReferenceFrame                        reference_frame;                                                    \
    typedef void                                  is_coordinate_sequence;                                             \
    typedef ArithmeticType                        arithmetic_type;                                                    \
    typedef void                                  is_numeric_sequence;                                                \
    typedef Point                                 sequence_type;                                                      \
    typedef GEOMETRIX_AS_MPL_VECTOR(NumericTypes) storage_types;                                                      \
    typedef dimension<Dimension>                  dimension_type;                                                     \
    typedef void                                  is_sequence;                                                        \
    typedef void GEOMETRIX_TYPE_DIVERSITY_TAG(NumericTypes);                                                          \
};                                                                                                                    \
}                                                                                                                     \
/***/

#define GEOMETRIX_DEFINE_FUSION_POINT_TRAITS( Point, NumericTypes, Dimension, ArithmeticType, ReferenceFrame, AccessPolicy ) \
GEOMETRIX_DEFINE_TENSOR_TRAITS( Point, 1, AccessPolicy )                                                              \
namespace geometrix {                                                                                                 \
template <>                                                                                                           \
struct geometric_traits< Point >                                                                                      \
{                                                                                                                     \
    BOOST_STATIC_ASSERT( Dimension > 0 );                                                                             \
    typedef Point                                 point_type;                                                         \
    typedef void                                  is_point;                                                           \
    typedef ReferenceFrame                        reference_frame;                                                    \
    typedef void                                  is_coordinate_sequence;                                             \
    typedef ArithmeticType                        arithmetic_type;                                                    \
    typedef void                                  is_numeric_sequence;                                                \
    typedef Point                                 sequence_type;                                                      \
    typedef GEOMETRIX_AS_MPL_VECTOR(NumericTypes) storage_types;                                                      \
    typedef dimension<Dimension>                  dimension_type;                                                     \
    typedef void                                  is_sequence;                                                        \
    typedef void GEOMETRIX_TYPE_DIVERSITY_TAG(NumericTypes);                                                          \
};                                                                                                                    \
}                                                                                                                     \
/***/

//! \brief Reference frame must define a tagging type for point sequences.
template <typename Point, typename ReferenceFrame>
struct reference_frame_adaptor
    <
        Point
      , ReferenceFrame
      , typename geometric_traits<Point>::is_point 
    >
: remove_const_ref<Point>::type
{
    typedef typename remove_const_ref< Point >::type                   sequence_type;    
    typedef ReferenceFrame                                             reference_frame;
    typedef typename geometric_traits< sequence_type >::dimension_type dimension_type;

    //! construct from the raw sequence.
    explicit reference_frame_adaptor( const sequence_type& sequence )
        : sequence_type( sequence )
    {}

    //! construct from the raw sequence.
    template <typename OtherCoordinateSequence>
    explicit reference_frame_adaptor( const OtherCoordinateSequence& sequence )
        : sequence_type( construction_policy< sequence_type >::construct( sequence ) )
    {}

    //! construct from a copy
    explicit reference_frame_adaptor( const reference_frame_adaptor<sequence_type, reference_frame>& sequence )
        : sequence_type( sequence )
    {}

    //! automatic transform
    template <typename OtherCoordinateSequence, typename OtherReferenceFrame>
    reference_frame_adaptor( const reference_frame_adaptor< OtherCoordinateSequence, OtherReferenceFrame >& sequence )
        : sequence_type( reference_frame_transformation< OtherReferenceFrame, reference_frame >::template transform<sequence_type>( sequence ) )
    {}

    reference_frame_adaptor& operator =( const sequence_type& sequence )
    {
        *this = reference_frame_adaptor( sequence );
        return *this;
    }

    template <typename OtherCoorindateSequence>
    reference_frame_adaptor& operator =( const OtherCoorindateSequence& sequence )
    {
        *this = reference_frame_adaptor( construction_policy< sequence_type >::construct( sequence ) );
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

//! Treat a point as a vector.
template <typename Point>
struct vector_adaptor : Point 
{ 
    BOOST_STATIC_ASSERT(( is_point<Point>::value ));
    vector_adaptor( const Point& p ) 
        : Point( p )
    {}
};

template <typename Point>
struct is_point< vector_adaptor<Point> > : boost::false_type{};

template <typename Sequence>
struct geometric_traits< vector_adaptor< Sequence > > 
    : uniformity_base< Sequence >
{
    typedef vector_adaptor<Sequence>                             vector_type;
    typedef void                                                 is_vector;
    typedef typename geometric_traits<Sequence>::reference_frame reference_frame;               
    typedef void                                                 is_coordinate_sequence;        
    typedef typename geometric_traits<Sequence>::arithmetic_type arithmetic_type;               
    typedef void                                                 is_numeric_sequence;           
    typedef Sequence                                             sequence_type;                 
    typedef typename dimension_of<Sequence>::type                dimension_type;                
    typedef void                                                 is_sequence;    
};

template <typename Point>
vector_adaptor<Point> as_vector( const Point& p )
{
    return vector_adaptor<Point>( p );
}

template <typename Point>
struct tensor_traits< vector_adaptor< Point > > : tensor_traits< typename remove_const_ref<Point>::type >{};

}//namespace geometrix;

namespace boost { namespace mpl {               
template<typename>                              
struct sequence_tag;                            
                                                
template<typename T>                                      
struct sequence_tag< geometrix::vector_adaptor<T> >
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
template<typename T>                                      
struct sequence_tag< const geometrix::vector_adaptor<T> >
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
}}//namespace boost::mpl;

#endif //GEOMETRIX_POINT_TRAITS_HPP
