//
//! Copyright � 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_LINE_TRAITS_HPP
#define _BOOST_GEOMETRY_LINE_TRAITS_HPP
#pragma once

#include "vector_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{
//! \brief Tag to check if a type is a line
template <typename Line>
struct is_line : boost::false_type{};

//! \brief line traits struct. 
template <typename Line>
struct line_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, LINE_TRAITS_NOT_DEFINED
		, (Line) );	
};

//! \brief Macro for defining a line type traits.
#define BOOST_DEFINE_LINE_TRAITS( Vector, Line )                          \
template <> is_line< Line > : boost::true_type{};                         \
template <>                                                               \
struct line_traits< Line >                                                \
{                                                                         \
 	typedef Vector                                        vector_type;    \
    typedef Line                                          line_type;      \
    typedef vector_traits< vector_type >::dimension_type  dimension_type; \
    typedef vector_traits< vector_type >::coordinate_type coordinate_type;\
};

//! \brief line access traits struct
//! NOTE: must be specialized for user types.
template <typename Line>
struct line_access_traits
{
	BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, LINE_ACCESS_TRAITS_NOT_DEFINED
		, (Line) );	
 
    typedef Line                                               line_type;                                           
    typedef typename line_traits< line_type >::vector_type     vector_type;   
    typedef typename line_traits< line_type >::coordinate_type coordinate_type;
    typedef typename line_traits< line_type >::dimension_type  dimension_type; 
                         
    static inline const vector_type&      get_u( const line_type& l ){ return l.get_u(); }        
	static inline const vector_type&      get_v( const line_type& l ){ return l.get_v(); }
    static inline const coordinate_type&  get_t( const line_type& l ){ return l.get_t(); }

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_LINE_TRAITS_HPP
