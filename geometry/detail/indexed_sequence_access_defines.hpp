//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_INDEXED_SEQUENCE_ACCESS_DEFINES_HPP
#define _BOOST_GEOMETRY_DETAIL_INDEXED_SEQUENCE_ACCESS_DEFINES_HPP
#pragma once

namespace boost
{
namespace numeric
{
namespace geometry
{
    //! Enums to define access strategies. 
    //! Note: The order is important here... changing this may break the access logic machinery.
    enum indexed_sequence_access_type
    {
        run_time_access                 = 0,//! Always use run-time (compile-time will be used when mixed with a compile-time type because it seems very contrived to make compile-time access work in run-time.).
        both_prefer_run_time_access     = 1,//! Both are available, but when all sequences in an operation have run-time access.. use that.
        compile_time_access             = 2,//! Always use compile-time in all operations this sequence is involved with.
        both_prefer_compile_time_access = 3 //! Both are available, but when all sequences in an operation have compile-time access.. use that.
    };
}}}//namespace boost::numeric::geometry

#if !defined( BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE )
    #define BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE boost::numeric::geometry::both_prefer_compile_time_access
#else
    #if BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE < 0
        #undef BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE boost::numeric::geometry::both_prefer_compile_time_access
    # endif
    #if BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE > 3
        #undef BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE boost::numeric::geometry::both_prefer_compile_time_access
    # endif

    #if BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE == 1
        #undef BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE boost::numeric::geometry::both_prefer_compile_time_access
    # endif

    #if BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE == 3
        #undef BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE boost::numeric::geometry::both_prefer_compile_time_access
    # endif
#endif

#endif //_BOOST_GEOMETRY_DETAIL_INDEXED_SEQUENCE_ACCESS_DEFINES_HPP
