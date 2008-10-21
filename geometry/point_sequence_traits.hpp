//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_SEQUENCE_TRAITS_HPP
#define _BOOST_GEOMETRY_POINT_SEQUENCE_TRAITS_HPP
#pragma once

#include "point_traits.hpp"
#include <vector>

namespace boost
{
namespace numeric
{
namespace geometry
{

//! Default point traits struct. 
template <typename PointSequence>
struct point_sequence_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, POINT_SEQUENCE_TRAITS_NOT_DEFINED
		, (PointSequence) );	
};

//! Macro assumes point sequence can construct from input iterators pointing to points.
//! Assumes container has stl iterator interface + empty() and size().
#define BOOST_DEFINE_POINT_SEQUENCE_TRAITS( Point, Container )         \
template <>                                                            \
struct point_sequence_traits< Container >                              \
{                                                                      \
    	BOOST_MPL_ASSERT_MSG(                                          \
        ( !boost::is_same< std::vector< Point >, Container >::value )  \
		, USE_BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS                \
		, (Container) );   	                                           \
                                                                       \
 	typedef Point                                       point_type;              \
    typedef Container                                   container_type;          \
    typedef point_traits< point_type >::coordinate_type coordinate_type;         \
    typedef point_traits< point_type >::dimension_type  dimension_type;          \
    typedef container_type::iterator                    iterator;                \
    typedef container_type::const_iterator              const_iterator;          \
    typedef container_type::reverse_iterator            reverse_iterator;        \
    typedef container_type::const_reverse_iterator      const_reverse_iterator;  \
 	static inline iterator                              begin( container_type& p ) { return p.begin(); }         \
    static inline const_iterator                        begin( const container_type& p ) { return p.begin(); }   \
 	static inline iterator                              end( container_type& p ) { return p.end(); }             \
    static inline const_iterator                        end( const container_type& p ) { return p.end(); }       \
 	static inline reverse_iterator                      rbegin( container_type& p ) { return p.rbegin(); }       \
    static inline const_reverse_iterator                rbegin( const container_type& p ) { return p.rbegin(); } \
 	static inline reverse_iterator                      rend( container_type& p ) { return p.rend(); }           \
    static inline const_reverse_iterator                rend( const container_type& p ) { return p.rend(); }     \
 	static inline size_t                                size( const container_type& p ) { return p.size(); }     \
    static inline bool                                  empty( const container_type& p ) { return p.empty(); }   \
    static inline const point_type&                     get_point( const container_type& pointSequence, size_t index ) { const_iterator iter( pointSequence.begin() ); std::advance( iter, index ); return *iter; } \
    static inline point_type&                           get_point( container_type& pointSequence, size_t index ) { iterator iter( pointSequence.begin() ); std::advance( iter, index ); return *iter; }       \
    static inline const point_type&                     front( const container_type& pointSequence ) { return *pointSequence.begin(); }  \
    static inline point_type&                           front( container_type& pointSequence ) { return *pointSequence.begin(); }        \
    static inline const point_type&                     back( const container_type& pointSequence ) { const_iterator iter( pointSequence.begin() ); std::advance( iter, pointSequence.size() - 1 ); return *iter;} \
    static inline point_type&                           back( container_type& pointSequence ) { iterator iter( pointSequence.begin() ); std::advance( iter, pointSequence.size() - 1 ); return *iter; } \
    template <typename InputIterator>                   \
    static inline boost::shared_ptr< container_type >   construct( InputIterator first, InputIterator last ){ boost::shared_ptr< container_type > p( new container_type( first, last ) ); return p; } \
};

//! specialize vector.
#define BOOST_DEFINE_VECTOR_POINT_SEQUENCE_TRAITS( Point )                     \
template <>                                                                    \
struct point_sequence_traits< std::vector< Point > >                           \
{                                                                              \
 	typedef Point                                       point_type;            \
    typedef std::vector< point_type >                   container_type;        \
    typedef point_traits< point_type >::coordinate_type coordinate_type;       \
    typedef point_traits< point_type >::dimension_type  dimension_type;        \
    typedef container_type::iterator                    iterator;              \
    typedef container_type::const_iterator              const_iterator;        \
    typedef container_type::reverse_iterator            reverse_iterator;      \
    typedef container_type::const_reverse_iterator      const_reverse_iterator;\
 	static inline iterator                              begin( container_type& p ) { return p.begin(); }         \
    static inline const_iterator                        begin( const container_type& p ) { return p.begin(); }   \
 	static inline iterator                              end( container_type& p ) { return p.end(); }             \
    static inline const_iterator                        end( const container_type& p ) { return p.end(); }       \
 	static inline reverse_iterator                      rbegin( container_type& p ) { return p.rbegin(); }       \
    static inline const_reverse_iterator                rbegin( const container_type& p ) { return p.rbegin(); } \
 	static inline reverse_iterator                      rend( container_type& p ) { return p.rend(); }           \
    static inline const_reverse_iterator                rend( const container_type& p ) { return p.rend(); }     \
    static inline size_t                                size( const container_type& p ) { return p.size(); }     \
    static inline bool                                  empty( const container_type& p ) { return p.empty(); }   \
    static inline const point_type&                     get_point( const container_type& pointSequence, size_t index ) { return pointSequence[index]; } \
    static inline point_type&                           get_point( container_type& pointSequence, size_t index ) { return pointSequence[index]; }       \
    static inline const point_type&                     front( const container_type& pointSequence ) { return pointSequence.front(); }  \
    static inline point_type&                           front( container_type& pointSequence ) { return pointSequence.front(); }        \
    static inline const point_type&                     back( const container_type& pointSequence ) { return pointSequence.back(); } \
    static inline point_type&                           back( container_type& pointSequence ) { return pointSequence.back(); } \
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_SEQUENCE_TRAITS_HPP
