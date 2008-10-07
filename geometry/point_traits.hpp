//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_TRAITS_HPP
#define _BOOST_GEOMETRY_POINT_TRAITS_HPP
#pragma once

#include "coordinate_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! A type tag specialized on dimension.
template <unsigned int Dimension>
struct dimension_tag 
{
	static const unsigned int value = Dimension;
};

//! Default point traits struct. 
//! NOTE: must be specialized for user types.
template <typename Point>
struct point_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POINT_TRAITS_NOT_DEFINED
		, (Point) );	

	typedef void*                           coordinate_type;
	typedef dimension_tag<0>                dimenstion_type;
};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_POINT_TRAITS( Point )                                               \
template <>                                                                              \
struct point_traits< Point >                                                             \
{                                                                                        \
	BOOST_STATIC_ASSERT( Point::dimension_type::value > 0 );                             \
    typedef Point                  point_type;                                           \
    typedef coordinate_traits<Point::coordinate_type>::coordinate_type coordinate_type;  \
	typedef Point::dimension_type  dimension_type;                                       \
};

//! Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_USER_POINT_TRAITS( Point, Coordinate, Dimension )     \
template <>                                                                \
struct point_traits< Point >                                               \
{                                                                          \
	BOOST_STATIC_ASSERT( Dimension > 0 );                                  \
    typedef Point                     point_type;                          \
    typedef coordinate_traits<Coordinate>::coordinate_type coordinate_type;\
	typedef dimension_tag<Dimension>  dimension_type;                      \
};

//! Access traits for points in cartesian coordinates
//! NOTE: must be specialized for user types.
template <typename Point>
struct cartesian_access_traits
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, CARTESIAN_ACCESS_TRAITS_NOT_DEFINED
		, (Point) );

    //! Example interface for access traits.
    typedef Point point_type;
    typedef typename point_traits< point_type >::coordinate_type coordinate_type;
    static inline coordinate_type get_x( const point_type& p ) { return p.get_x(); }  
    static inline coordinate_type get_y( const point_type& p ) { return p.get_y(); }  
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, typename point_traits<T>::coordinate_type >::type get_z( const T& p ) { return p.get_z(); }

    //! factory traits.
    template <typename T>
    static point_type construct( const coordinate_type& x, const coordinate_type& y ) { return point_type( x, y ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, point_type >::type construct( const coordinate_type& x, const coordinate_type& y, const coordinate_type& z ) { return point_type( x, y, z ); }

};

//! Access traits for points in polar coordinates
//! NOTE: must be specialized for user types.
template <typename Point>
struct polar_access_traits
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POLAR_ACCESS_TRAITS_NOT_DEFINED
		, (Point) );	

    //! Example interface for access traits.
    typedef Point point_type;
    typedef typename point_traits< point_type >::coordinate_type coordinate_type;
    static inline coordinate_type get_radius( const point_type& p ) { return p.get_radius(); }  
    static inline coordinate_type get_theta( const point_type& p ) { return p.get_theta(); }  
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, typename point_traits<T>::coordinate_type >::type get_phi( const T& p ) { return p.get_phi(); }

    //! factory traits
    template <typename T>
    static point_type construct( const coordinate_type& r, const coordinate_type& t ) { return point_type( r, t ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_tag<3> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return point_type( r, t, phi ); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_TRAITS_HPP
