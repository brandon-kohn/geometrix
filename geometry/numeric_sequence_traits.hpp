//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_NUMERIC_SEQUENCE_TRAITS_HPP
#define _BOOST_GEOMETRY_NUMERIC_SEQUENCE_TRAITS_HPP
#pragma once

#include "numeric_traits.hpp"
#include "dimension_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
//! Default numeric sequence traits struct. 
//! NOTE: must be specialized for user types.
template <typename NumericSequence>
struct numeric_sequence_traits
{
	BOOST_MPL_ASSERT_MSG( 
		 ( false )
		,NUMERIC_SEQUENCE_TRAITS_NOT_DEFINED
		,(NumericSequence) );	

    typedef NumericSequence     numeric_sequence_type;
	typedef void*               numeric_type;
	typedef dimension_traits<0> dimension_type;
    typedef numeric_type        value_type;

};

//! \brief Macro for numeric sequence type with embedded traits
#define BOOST_DEFINE_NUMERIC_SEQUENCE_TRAITS( NumericSequence )                                  \
template <>                                                                                      \
struct numeric_sequence_traits< NumericSequence >                                                \
{                                                                                                \
	BOOST_STATIC_ASSERT( NumericSequence::dimension_type::value > 0 );                           \
    typedef NumericSequence                                                numeric_sequence_type;\
    typedef numeric_traits<NumericSequence::coordinate_type>::numeric_type numeric_type;         \
	typedef NumericSequence::dimension_type                                dimension_type;       \
    typedef numeric_type                                                   value_type;           \
};

//! Macro for point type which does not have embedded traits - User Defined NumericSequences
#define BOOST_DEFINE_USER_NUMERIC_SEQUENCE_TRAITS( NumericSequence, NumericType, Dimension )\
template <>                                                                                 \
struct numeric_sequence_traits< NumericSequence >                                           \
{                                                                                           \
	BOOST_STATIC_ASSERT( Dimension > 0 );                                                   \
    typedef NumericSequence                           numeric_sequence_type;                \
    typedef numeric_traits<NumericType>::numeric_type numeric_type;                         \
	typedef dimension_traits<Dimension>               dimension_type;                       \
    typedef numeric_type                              value_type;                           \
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_NUMERIC_SEQUENCE_TRAITS_HPP
