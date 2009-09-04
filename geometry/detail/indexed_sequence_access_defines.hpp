//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_DETAIL_INDEXED_SEQUENCE_ACCESS_DEFINES_HPP
#define GENERATIVE_GEOMETRY_DETAIL_INDEXED_SEQUENCE_ACCESS_DEFINES_HPP
#pragma once

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! Enum to define access strategies. 
    //! Note: The order is important here... changing this may break the access logic machinery.
    enum indexed_sequence_access_type
    {
        run_time_access            = 0,//! Always use run-time (compile-time will be used when mixed with a compile-time type because it seems very contrived to make compile-time access work in run-time.).
        prefer_run_time_access     = 1,//! Both are available, but when all sequences in an operation have run-time access.. use that.
        compile_time_access        = 2,//! Always use compile-time in all operations this sequence is involved with.
        prefer_compile_time_access = 3 //! Both are available, but when all sequences in an operation have compile-time access.. use that.
    };
}}}//namespace generative::numeric::geometry

#if !defined( GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE )
    #define GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE generative::numeric::geometry::prefer_compile_time_access
#else
    #if GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE < 0
        #undef GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE generative::numeric::geometry::both_prefer_compile_time_access
    # endif
    #if GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE > 3
        #undef GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE generative::numeric::geometry::both_prefer_compile_time_access
    # endif

    #if GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE == 1
        #undef GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE generative::numeric::geometry::both_prefer_compile_time_access
    # endif

    #if GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE == 3
        #undef GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE
        #define GENERATIVE_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE generative::numeric::geometry::both_prefer_compile_time_access
    # endif
#endif

#endif //GENERATIVE_GEOMETRY_DETAIL_INDEXED_SEQUENCE_ACCESS_DEFINES_HPP
