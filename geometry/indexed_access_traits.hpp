//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
#define _BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
#pragma once

#include <boost/type_traits.hpp>
#include <boost/utility.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{

template <typename T>
struct has_compile_time_access : boost::false_type{};
template <typename T>
struct has_run_time_access : boost::false_type{};

template < typename T >
struct indexed_access_traits
{
    typedef T                                               numeric_sequence_type;
    typedef typename numeric_sequence_type::coordinate_type coordinate_type;
    typedef typename numeric_sequence_type::dimension_type  dimension_type;
    typedef typename has_compile_time_access<T>             has_compile_time_access_type;
    typedef typename has_run_time_access<T>                 has_run_time_access_type;
    
    //! dimension_access needs to be specialized on point type up to T's dimension_type...    
    //! This needs to support both runtime random access and compile time random access.
    template <unsigned int Index>
    static inline coordinate_type get( const T& p, typename boost::enable_if< has_compile_time_access_type >::type* dummy = 0 ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (T)
        );

        return p.get<dimension_traits<Index>::value>();
    }

    static inline coordinate_type get( const T& p, size_t index, typename boost::enable_if< has_run_time_access_type >::type* dummy = 0  ) 
    {        
        assert( index >= 0 && index < dimension_type::value );		   
        return p[ index ];
    }
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
