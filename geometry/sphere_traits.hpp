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

#include "vector_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
//! \brief Tag to check if a type is a sphere
template <typename Sphere>
struct is_sphere : boost::false_type{};

//! \brief sphere traits struct. 
template <typename Sphere>
struct sphere_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, SPHERE_TRAITS_NOT_DEFINED
		, (Sphere) );	
};

//! \brief Macro for defining a sphere type traits.
#define BOOST_DEFINE_SPHERE_TRAITS( Vector, Sphere )                      \
template <> is_sphere< Sphere > : boost::true_type{};                     \
template <>                                                               \
struct sphere_traits< Sphere >                                            \
{                                                                         \
 	typedef Vector                                        vector_type;    \
    typedef Sphere                                        sphere_type;    \
    typedef vector_traits< vector_type >::dimension_type  dimension_type; \
    typedef vector_traits< vector_type >::coordinate_type coordinate_type;\
};

//! \brief sphere access traits struct
//* NOTE: must be specialized for user types.
template <typename Sphere>
struct sphere_access_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, SPHERE_ACCESS_TRAITS_NOT_DEFINED
		, (Sphere) );	
 
    typedef Sphere                                                 sphere_type;                                           
    typedef typename sphere_traits< sphere_type >::vector_type     vector_type;   
    typedef typename sphere_traits< sphere_type >::coordinate_type coordinate_type;
    typedef typename sphere_traits< sphere_type >::dimension_type  dimension_type; 
                         
    static inline const vector_type&      get_center( const sphere_type& s ){ return s.get_center(); }        	
    static inline const coordinate_type&  get_radius( const sphere_type& s ){ return s.get_t(); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_SPHERE_TRAITS_HPP
