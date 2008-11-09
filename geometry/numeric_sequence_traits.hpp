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

//! \brief A traits type to define a sequence of numeric types with a specified static dimensionality.
//* A value_type is also defined for use in containers via the indexed_access_traits type.
//* NOTE: must be specialized for user types.
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

//! \brief Macro for numeric sequence type with embedded traits
//* NOTE: This macro is called by BOOST_DEFINE_POINT_TRAITS and BOOST_DEFINE_VECTOR_TRAITS. Users should use those to avoid overlapping defines.
#define BOOST_DEFINE_NUMERIC_SEQUENCE_TRAITS( NumericSequence )                                  \
        BOOST_DEFINE_SEQUENCE_TRAITS( NumericSequence )                                          \
template <> struct is_numeric_sequence< NumericSequence > : boost::true_type{};                  \
template <>                                                                                      \
struct numeric_sequence_traits< NumericSequence > : public sequence_traits<NumericSequence>      \
{                                                                                                \
	BOOST_STATIC_ASSERT( NumericSequence::dimension_type::value > 0 );                           \
    typedef NumericSequence                                                numeric_sequence_type;\
    typedef numeric_traits<NumericSequence::coordinate_type>::numeric_type numeric_type;         \
};

//! Macro for point type which does not have embedded traits - User Defined NumericSequences
//* NOTE: This macro is called by BOOST_DEFINE_USER_POINT_TRAITS and BOOST_DEFINE_USER_VECTOR_TRAITS. Users should use those to avoid overlapping defines.
#define BOOST_DEFINE_USER_NUMERIC_SEQUENCE_TRAITS( NumericSequence, NumericType, Dimension )\
        BOOST_DEFINE_USER_SEQUENCE_TRAITS( NumericSequence, NumericType, Dimension )        \
template <> struct is_numeric_sequence< NumericSequence > : boost::true_type{};             \
template <>                                                                                 \
struct numeric_sequence_traits< NumericSequence > : public sequence_traits<NumericSequence> \
{                                                                                           \
	BOOST_STATIC_ASSERT( Dimension > 0 );                                                   \
    typedef NumericSequence                           numeric_sequence_type;                \
    typedef numeric_traits<NumericType>::numeric_type numeric_type;                         \
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_NUMERIC_SEQUENCE_TRAITS_HPP
