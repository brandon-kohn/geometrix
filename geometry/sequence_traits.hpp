//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_SEQUENCE_TRAITS_HPP
#define _GENERATIVE_GEOMETRY_SEQUENCE_TRAITS_HPP
#pragma once

#include "dimension_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{
//! \brief Tag to check if a type is a sequence
template <typename Sequence>
struct is_sequence : boost::false_type{};

//! \brief A traits type to define a sequence of types with a specified static dimensionality.

//! sequence_traits are a type trait base type which holds information about a static sized sequence.
//! The sequence_traits type is meant to be specialized and to contain:
//!    - dimension_type  (static size)
//!    - value_type      (type of elements contained)
//!    - reference       (a definition of a reference type to element)
//!    - const_reference (a definition of a const reference type to element)
//!
//! NOTE: must be specialized for user types.
//! \see BOOST_DEFINE_USER_SEQUENCE_TRAITS
template <typename Sequence>
struct sequence_traits
{
    //! FIXME: This is causing issues when the indexed_operation types are going through SFINAE.. even though I used lazy_enable_if_c...
	//BOOST_MPL_ASSERT_MSG( 
	//	 ( false )
	//	,SEQUENCE_TRAITS_NOT_DEFINED
	//	,(Sequence) );	

    //! Type definitions required.
	typedef dimension_traits<0> dimension_type;
    
    //! Typedefs common for containers
    typedef void*               value_type;
    typedef void*               reference;
    typedef void*               const_reference;
    
    //! TODO: Do we want iterators?
    //typedef void*               iterator;
    //typedef const void*         const_iterator;   

};

//! \brief A SequenceConcept concept check for a type which models a Sequence.

//! A Sequence is defined as an ordered collection of values with a static dimensionality.
//! The SequenceConcept checks that valid sequence_traits have been defined for a Sequence type.
template <typename Sequence>
struct SequenceConcept
{
    void constraints()
    {
        typedef typename sequence_traits<Sequence>::value_type      value_type;            
        typedef typename sequence_traits<Sequence>::dimension_type  dimension_type;
        typedef typename sequence_traits<Sequence>::reference       reference;
        typedef typename sequence_traits<Sequence>::const_reference const_reference;
    }
};

//! \brief Macro for sequence type with deducible traits
//! NOTE: This macro is called by deducible and BOOST_DEFINE_VECTOR_TRAITS. Users should use these to avoid overlapping defines.
#define BOOST_DEFINE_SEQUENCE_TRAITS( Sequence )               \
template <> struct is_sequence<Sequence> : boost::true_type{}; \
template <>                                                    \
struct sequence_traits< Sequence >                             \
{                                                              \
	BOOST_STATIC_ASSERT( Sequence::dimension_type::value > 0 );\
    typedef Sequence                 sequence_type;            \
    typedef Sequence::value_type     value_type;               \
	typedef Sequence::dimension_type dimension_type;           \
    typedef value_type&              reference;                \
    typedef const value_type&        const_reference;          \
};

//! A macro for defining sequence_traits for a user defined Sequence type.
//! NOTE: This macro is called by BOOST_DEFINE_USER_POINT_TRAITS and BOOST_DEFINE_USER_VECTOR_TRAITS. 
//! Example usage:
//! \code
//! struct sequence
//! {
//!     double x;
//!     double y;
//! };
//! 
//! BOOST_DEFINE_USER_SEQUENCE_TRAITS( sequence, double, 2 );
//! \endcode
#define BOOST_DEFINE_USER_SEQUENCE_TRAITS( Sequence, ValueType, Dimension )\
namespace generative { namespace numeric { namespace geometry {                 \
template <> struct is_sequence<Sequence> : boost::true_type{};             \
template <>                                                                \
struct sequence_traits< Sequence >                                         \
{                                                                          \
	BOOST_STATIC_ASSERT( Dimension > 0 );                                  \
    typedef Sequence                    sequence_type;                     \
    typedef ValueType                   value_type;                        \
	typedef dimension_traits<Dimension> dimension_type;                    \
    typedef value_type&                 reference;                         \
    typedef const value_type&           const_reference;                   \
};                                                                         \
}}}                                                                        \

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_SEQUENCE_TRAITS_HPP
