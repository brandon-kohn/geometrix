//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_SEQUENCE_TRAITS_HPP
#define _BOOST_GEOMETRY_SEQUENCE_TRAITS_HPP
#pragma once

#include "dimension_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief Tag to check if a type is a sequence
template <typename Sequence>
struct is_sequence : boost::false_type{};

//! \brief A traits type to define a sequence of types with a specified static dimensionality.
//* A value_type is also defined for use in containers via the indexed_access_traits type.
//* NOTE: must be specialized for user types.
template <typename Sequence>
struct sequence_traits
{
    //! FIXME: This is causing issues when the indexed_operation types are going through SFINAE.. even though I used lazy_enable_if_c...
	//BOOST_MPL_ASSERT_MSG( 
	//	 ( false )
	//	,SEQUENCE_TRAITS_NOT_DEFINED
	//	,(Sequence) );	

    //! Type definitions required.
    typedef Sequence            sequence_type;
	typedef dimension_traits<0> dimension_type;
    
    //! Typedefs common for containers
    typedef void*               value_type;
    typedef void*               reference;
    typedef void*               const_reference;
    
    //! TODO: Do we want iterators?
    //typedef void*               iterator;
    //typedef const void*         const_iterator;   

};

//! \brief Macro for sequence type with embedded traits
//* NOTE: This macro is called by BOOST_DEFINE_POINT_TRAITS and BOOST_DEFINE_VECTOR_TRAITS. Users should use those to avoid overlapping defines.
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

//! Macro for sequence type which does not have embedded traits - User Defined Sequences
//* NOTE: This macro is called by BOOST_DEFINE_USER_POINT_TRAITS and BOOST_DEFINE_USER_VECTOR_TRAITS. Users should use those to avoid overlapping defines.
#define BOOST_DEFINE_USER_SEQUENCE_TRAITS( Sequence, ValueType, Dimension )\
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
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_SEQUENCE_TRAITS_HPP
