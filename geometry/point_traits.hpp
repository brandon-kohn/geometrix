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

#include "numeric_traits.hpp"
#include "dimension_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
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
	typedef dimension_traits<0>             dimenstion_type;
};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_POINT_TRAITS( Point )                                       \
template <>                                                                      \
struct point_traits< Point >                                                     \
{                                                                                \
	BOOST_STATIC_ASSERT( Point::dimension_type::value > 0 );                     \
    typedef Point                  point_type;                                   \
    typedef numeric_traits<Point::coordinate_type>::numeric_type coordinate_type;\
	typedef Point::dimension_type  dimension_type;                               \
};

//! Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_USER_POINT_TRAITS( Point, Coordinate, Dimension )\
template <>                                                           \
struct point_traits< Point >                                          \
{                                                                     \
	BOOST_STATIC_ASSERT( Dimension > 0 );                             \
    typedef Point                                     point_type;     \
    typedef numeric_traits<Coordinate>::numeric_type  coordinate_type;\
	typedef dimension_traits<Dimension>               dimension_type; \
};

template < typename Point >
struct point_indexed_access_traits
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
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Index, Point)
        );

        return p.get<dimension_traits<Index>::value>();
    }

    //! dimension_access needs to be specialized on point type up to point's dimension_type...
    //! there is no generic way to do this for a generic point type as it requires knowledge of the underlying type.
    //! Assume the general case needs to support random access via unsigned integer indices.
    static inline coordinate_type get( const point_type& p, size_t index ) 
    {
        boost::function_requires< PointIndexedAccessConcept< point_type > >();
        std::assert( index >= 0 && index < dimension_type::value );
        return p[ index ];
    }
};

//! Access traits for points in cartesian coordinates
//! NOTE: must be specialized for user types.
template <typename Point>
struct cartesian_access_traits : public point_indexed_access_traits< Point >
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, CARTESIAN_ACCESS_TRAITS_NOT_DEFINED
		, (Point) );
 
    //! factory traits.
    template <typename T>
    static point_type construct( const coordinate_type& x, const coordinate_type& y ) { return point_type( x, y ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<3> >, point_type >::type construct( const coordinate_type& x, const coordinate_type& y, const coordinate_type& z ) { return point_type( x, y, z ); }

};

//! Access traits for points in polar coordinates
//! NOTE: must be specialized for user types.
template <typename Point>
struct polar_access_traits : public point_indexed_access_traits< Point >
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POLAR_ACCESS_TRAITS_NOT_DEFINED
		, (Point) );	

    //! factory traits
    template <typename T>
    static point_type construct( const coordinate_type& r, const coordinate_type& theta ) { return point_type( r, t ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<3> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return point_type( r, t, phi ); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_TRAITS_HPP
