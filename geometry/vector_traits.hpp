//
//!  Copyright (c) 2008
//!  Brandon Kohn
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
//! \brief Default vector_traits type.
//* NOTE: must be specialized for user types.
//* NOTE: vector_traits must not overlap with point_traits if defined via macros.
template <typename Vector>
struct vector_traits : public coordinate_sequence_traits<Vector>
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, VECTOR_TRAITS_NOT_DEFINED
		, (Vector) );	

    typedef void vector_type;
};

//! \brief Macro for vector type with embedded traits
#define BOOST_DEFINE_VECTOR_TRAITS( Vector )                                \
        BOOST_DEFINE_COORDINATE_SEQUENCE_TRAITS( Vector )                   \
template <>                                                                 \
struct vector_traits< Vector > : public coordinate_sequence_traits< Vector >\
{                                                                           \
    typedef Vector vector_type;                                             \
};

//! \brief Macro for vector type which does not have embedded traits - User Defined Vectors
#define BOOST_DEFINE_USER_VECTOR_TRAITS( Vector, NumericType, Dimension, HasRunTimeAccess, HasCompileTimeAccess )\
        BOOST_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( Vector, NumericType, Dimension )                           \
        BOOST_DEFINE_USER_INDEXED_ACCESS_TRAITS( Vector, HasRunTimeAccess, HasCompileTimeAccess )                \
template <>                                                                                                      \
struct vector_traits< Vector > : public coordinate_sequence_traits< Vector >                                     \
{                                                                                                                \
    typedef Vector vector_type;                                                                                  \
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_VECTOR_TRAITS_HPP
