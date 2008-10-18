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

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Default vector traits struct. 
//! NOTE: must be specialized for user types.
template <typename Vector>
struct vector_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, VECTOR_TRAITS_NOT_DEFINED
		, (Vector) );	

	typedef void*                           coordinate_type;
	typedef dimension_traits<0>             dimenstion_type;
};

//! Macro for point type with embedded traits
#define BOOST_DEFINE_VECTOR_TRAITS( Vector )                                      \
template <>                                                                       \
struct vector_traits< Vector >                                                    \
{                                                                                 \
	BOOST_STATIC_ASSERT( Vector::dimension_type::value > 0 );                     \
    typedef Vector                  vector_type;                                  \
    typedef numeric_traits<Vector::coordinate_type>::numeric_type coordinate_type;\
	typedef Vector::dimension_type  dimension_type;                               \
};

//! Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_USER_VECTOR_TRAITS( Vector, NumericType, Dimension )\
template <>                                                             \
struct vector_traits< Vector >                                          \
{                                                                       \
	BOOST_STATIC_ASSERT( Dimension > 0 );                               \
    typedef Vector                                    vector_type;      \
    typedef numeric_traits<NumericType>::numeric_type  coordinate_type;  \
	typedef dimension_traits<Dimension>               dimension_type;   \
};

template < typename Vector >
struct vector_indexed_access_traits
{
    typedef Vector                                                 vector_type;
    typedef typename vector_traits< vector_type >::coordinate_type coordinate_type;
    typedef typename vector_traits< vector_type >::dimension_type  dimension_type;
    
    //! dimension_access needs to be specialized on point type up to point's dimension_type...
    //! there is no generic way to do this for a generic point type as it requires knowledge of the underlying type.
    //! Assume the general case needs to support random access via unsigned integer indices.
    template <typename Index>
    static inline coordinate_type get( const vector_type& v ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 || dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Index, Vector)
        );

        return v.get<dimension_traits<Index>::value>();
    }

    //! dimension_access needs to be specialized on point type up to point's dimension_type...
    //! there is no generic way to do this for a generic point type as it requires knowledge of the underlying type.
    //! Assume the general case needs to support random access via unsigned integer indices.
    static inline coordinate_type get( const vector_type& v, size_t index ) 
    {
        BOOST_ASSERT( index >= 0 && index < dimension_type::value );
        return v[ index ];
    }
};

//! Access traits for points in cartesian coordinates
//! NOTE: must be specialized for user types.
template <typename Vector>
struct cartesian_vector_access_traits : public vector_indexed_access_traits< Vector >
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, CARTESIAN_ACCESS_TRAITS_NOT_DEFINED
		, (Vector) );
 
    //! factory traits.
    template <typename T>
    static vector_type construct( const coordinate_type& x, const coordinate_type& y ) { return vector_type( x, y ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename vector_traits<T>::dimension_type, dimension_traits<3> >, vector_type >::type construct( const coordinate_type& x, const coordinate_type& y, const coordinate_type& z ) { return vector_type( x, y, z ); }

};

//! Access traits for points in polar coordinates
//! NOTE: must be specialized for user types.
template <typename Vector>
struct polar_vector_access_traits : public vector_indexed_access_traits< Vector >
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POLAR_ACCESS_TRAITS_NOT_DEFINED
		, (Vector) );	

    //! factory traits
    template <typename T>
    static vector_type construct( const coordinate_type& r, const coordinate_type& theta ) { return vector_type( r, t ); }
    template <class T>                                                                
    static typename boost::enable_if< boost::is_same< typename vector_traits<T>::dimension_type, dimension_traits<3> >, vector_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return vector_type( r, t, phi ); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_VECTOR_TRAITS_HPP
