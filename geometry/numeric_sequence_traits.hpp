//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_NUMERIC_SEQUENCE_TRAITS_HPP
#define _BOOST_GEOMETRY_NUMERIC_SEQUENCE_TRAITS_HPP
#pragma once

#include "numeric_traits.hpp"
#include "sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a numeric_sequence
template <typename Sequence>
struct is_numeric_sequence : boost::false_type{};

//! \brief A traits type to define a sequence of a numeric type with a static dimensionality.

//! numeric_sequence_traits are a type trait base type which holds information about a static sized sequence of numbers.
//! The numeric_sequence_traits type is a specialization of the sequence_type. 
//! numeric_sequence_traits is meant to be specialized and should contain:
//!    - dimension_type  (static size)
//!    - value_type      (type of elements contained)
//!    - numeric_type    (same as value_type but named more descriptively)
//!    - reference       (a definition of a reference type to element)
//!    - const_reference (a definition of a const reference type to element)
//!
//! NOTE: must be specialized for user types.
//! \see BOOST_DEFINE_USER_NUMERIC_SEQUENCE_TRAITS
template <typename NumericSequence>
struct numeric_sequence_traits
{
	BOOST_MPL_ASSERT_MSG( 
		 ( false )
		,NUMERIC_SEQUENCE_TRAITS_NOT_DEFINED
		,(NumericSequence) );	

    //! Type definitions required.
    typedef NumericSequence     numeric_sequence_type;
	typedef void*               numeric_type;

};

//! \brief Concept check for a type which models a NumericSequence.

//! A NumericSequence is a model of a Sequence type with the additional 
//! constraint that it must have a numeric type as the value_type.
template <typename NumericSequence>
struct NumericSequenceConcept
{
    void constraints()
    {
        boost::function_requires< SequenceConcept< NumericSequence > >();
        typedef typename numeric_sequence_traits<NumericSequence>::numeric_type numeric_type;
    }
};

//! \brief Macro for numeric sequence type with embedded traits
//! NOTE: This macro is called by BOOST_DEFINE_POINT_TRAITS and BOOST_DEFINE_VECTOR_TRAITS. Users should use those to avoid overlapping defines.
#define BOOST_DEFINE_NUMERIC_SEQUENCE_TRAITS( NumericSequence )                                  \
BOOST_DEFINE_SEQUENCE_TRAITS( NumericSequence )                                                  \
template <> struct is_numeric_sequence< NumericSequence > : boost::true_type{};                  \
template <>                                                                                      \
struct numeric_sequence_traits< NumericSequence > : public sequence_traits<NumericSequence>      \
{                                                                                                \
	BOOST_STATIC_ASSERT( NumericSequence::dimension_type::value > 0 );                           \
    typedef NumericSequence                                                numeric_sequence_type;\
    typedef numeric_traits<NumericSequence::coordinate_type>::numeric_type numeric_type;         \
};

//! A macro for defining sequence_traits for a user defined Sequence type.
//! NOTE: This macro is called by BOOST_DEFINE_USER_POINT_TRAITS and BOOST_DEFINE_USER_VECTOR_TRAITS. 
//! Users should use these to avoid overlapping defines.
//! Example usage:
//! \code
//! struct sequence
//! {
//!     double x;
//!     double y;
//! };
//! 
//! BOOST_DEFINE_USER_NUMERIC_SEQUENCE_TRAITS( sequence, double, 2 );
//! \endcode
#define BOOST_DEFINE_USER_NUMERIC_SEQUENCE_TRAITS( NumericSequence, NumericType, Dimension )\
BOOST_DEFINE_USER_SEQUENCE_TRAITS( NumericSequence, NumericType, Dimension )                \
namespace boost{ namespace numeric{ namespace geometry {                                    \
template <> struct is_numeric_sequence< NumericSequence > : boost::true_type{};             \
template <>                                                                                 \
struct numeric_sequence_traits< NumericSequence > : public sequence_traits<NumericSequence> \
{                                                                                           \
	BOOST_STATIC_ASSERT( Dimension > 0 );                                                   \
    typedef NumericSequence                           numeric_sequence_type;                \
    typedef numeric_traits<NumericType>::numeric_type numeric_type;                         \
};                                                                                          \
}}}                                                                                         \

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_NUMERIC_SEQUENCE_TRAITS_HPP
