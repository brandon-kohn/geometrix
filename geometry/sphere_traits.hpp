//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_SPHERE_TRAITS_HPP
#define _BOOST_GEOMETRY_SPHERE_TRAITS_HPP
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

//! \brief Tag to check if a type is a sphere type. (A spherical surface in N dimensions).
template <typename Sphere>
struct is_sphere : boost::false_type{};

//! Default sphere traits struct. 
//! NOTE: must be specialized for user types.
//* NOTE: sphere_traits must not overlap with vector_traits if defined via macros.
template <typename Sphere>
struct sphere_traits : public coordinate_sequence_traits<Sphere>
{
	BOOST_MPL_ASSERT_MSG( 
         ( false )
		,SPHERE_TRAITS_NOT_DEFINED
		,(Sphere) );

    typedef Sphere sphere_type;

};

//! \brief Macro for sphere type with embedded traits
#define BOOST_DEFINE_SPHERE_TRAITS( Sphere, ReferenceFrame )              \
        BOOST_DEFINE_COORDINATE_SEQUENCE_TRAITS( Sphere, ReferenceFrame ) \
        BOOST_DEFINE_INDEXED_ACCESS_TRAITS( Sphere )                      \
template<> struct is_sphere< Sphere > : boost::true_type{};               \
template <>                                                               \
struct sphere_traits< Sphere > : public coordinate_sequence_traits<Sphere>\
{                                                                         \
    typedef Sphere sphere_type;                                           \
};

//! \brief Macro for sphere type which does not have embedded traits - User Defined Spheres
#define BOOST_DEFINE_USER_SPHERE_TRAITS( Sphere, NumericType, Dimension, ReferenceFrame, IndexedSequenceAccess )\
        BOOST_DEFINE_USER_COORDINATE_SEQUENCE_TRAITS( Sphere, NumericType, Dimension, ReferenceFrame )          \
        BOOST_DEFINE_USER_INDEXED_ACCESS_TRAITS( Sphere, IndexedSequenceAccess )                                \
template<> struct is_sphere< Sphere > : boost::true_type{};                                                     \
template <>                                                                                                     \
struct sphere_traits< Sphere > : public coordinate_sequence_traits< Sphere >                                    \
{                                                                                                               \
    typedef Sphere sphere_type;                                                                                 \
};

//! \brief sphere access traits
//* NOTE: must be specialized for user types.
template <typename Sphere>
struct sphere_access_traits : public indexed_access_traits< Sphere >
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, SPHERE_ACCESS_TRAITS_NOT_DEFINED
		, (Sphere) );
 
    typedef Sphere                                                 sphere_type;    
    typedef typename sphere_traits< sphere_type >::coordinate_type coordinate_type;
    static inline const coordinate_type& get_radius( const sphere_type& s ){ return s.get_radius(); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_SPHERE_TRAITS_HPP
