//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_SEGMENT_TRAITS_HPP
#define _BOOST_GEOMETRY_SEGMENT_TRAITS_HPP
#pragma once

#include "point_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! segment traits struct. 
template <typename Segment>
struct segment_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, SEGMENT_TRAITS_NOT_DEFINED
		, (Segment) );	
};

//! Macro for defining a segment type traits.
#define BOOST_DEFINE_SEGMENT_TRAITS( Point, Segment )                    \
template <>                                                              \
struct segment_traits< Segment >                                         \
{                                                                        \
 	typedef Point                                       point_type;      \
    typedef Segment                                     segment_type;    \
    typedef point_traits< point_type >::dimension_type  dimension_type;  \
    typedef point_traits< point_type >::coordinate_type coordinate_type; \
};

//! segment access traits struct
//! NOTE: must be specialized for user types.
template <typename Segment>
struct segment_access_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, SEGMENT_ACCESS_TRAITS_NOT_DEFINED
		, (Segment) );	
 
    typedef Segment                                                  segment_type;                                           
    typedef typename segment_traits< segment_type >::point_type      point_type;   
    typedef typename segment_traits< segment_type >::coordinate_type coordinate_type;
                                                                                                        
    static inline const point_type& get_start( const segment_type& s ) { return s.get_start(); }        
	static inline const point_type& get_end( const segment_type& s )   { return s.get_end(); }          
    static inline void              set_start( segment_type& s, const point_type& start ) { s.set_start( start ); } 
	static inline void              set_end( segment_type& s, const point_type& end ) { s.set_end( end ); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_SEGMENT_TRAITS_HPP
