//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_CARTESIAN_POINT_ACCESS_TRAITS_HPP
#define _BOOST_GEOMETRY_CARTESIAN_POINT_ACCESS_TRAITS_HPP
#pragma once

#include "point_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

template < typename Point >
struct indexed_access_traits
{
    typedef Point                                                point_type;
    typedef typename point_traits< point_type >::coordinate_type coordinate_type;
    typedef typename point_traits< point_type >::dimension_type  dimension_type;
    
    //! dimension_access needs to be specialized on point type up to point's dimension_type...
    //! there is no generic way to do this for a generic point type as it requires knowledge of the underlying type.
    //! Assume the general case needs to support random access via unsigned integer indices.
    template <typename Index>
    static inline coordinate_type get( const point_type& p ) 
    {
        boost::function_requires< PointIndexedAccessConcept< point_type > >();
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Index, Point)
        );

        return p[ dimension_traits<Index>::value ];
    }

    static inline coordinate_type get( const point_type& p, size_t index ) 
    {
        boost::function_requires< PointIndexedAccessConcept< point_type > >();
        assert( index >= 0 && index < dimension_type::value );		   
        return p[ index ];
    }
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
    static inline coordinate_type get<0>( const point_type& p ) { return p.get<0>(); }  
    static inline coordinate_type get<1>( const point_type& p ) { return p.get<1>(); }  
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<3> >, typename point_traits<T>::coordinate_type >::type get<2>( const T& p ) { return p.get<2>(); }

    //! factory traits.
    template <typename T>
    static point_type construct( const coordinate_type& x, const coordinate_type& y ) { return point_type( x, y ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<3> >, point_type >::type construct( const coordinate_type& x, const coordinate_type& y, const coordinate_type& z ) { return point_type( x, y, z ); }

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
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<3> >, typename point_traits<T>::coordinate_type >::type get_phi( const T& p ) { return p.get_phi(); }

    //! factory traits
    template <typename T>
    static point_type construct( const coordinate_type& r, const coordinate_type& t ) { return point_type( r, t ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<3> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return point_type( r, t, phi ); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_CARTESIAN_POINT_ACCESS_TRAITS_HPP
