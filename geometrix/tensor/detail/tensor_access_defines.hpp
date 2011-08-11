//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_TENSOR_ACCESS_DEFINES_HPP
#define GEOMETRIX_DETAIL_TENSOR_ACCESS_DEFINES_HPP


namespace geometrix {
    //! Enum to define access strategies. 
    //! Note: The order is important here... changing this may break the access logic machinery.
    enum access_type
    {
        e_run_time_access            = 0,//! Always use run-time (compile-time will be used when mixed with a compile-time type because it seems very contrived to make compile-time access work in run-time.).
        e_prefer_run_time_access     = 1,//! Both are available, but when all sequences in an operation have run-time access.. use that.
        e_compile_time_access        = 2,//! Always use compile-time in all operations this sequence is involved with.
        e_prefer_compile_time_access = 3 //! Both are available, but when all sequences in an operation have compile-time access.. use that.
    };
}//namespace geometrix;

#if !defined( GEOMETRIX_TENSOR_ACCESS_TYPE )
    #define GEOMETRIX_TENSOR_ACCESS_TYPE geometrix::e_compile_time_access
#else
    #if GEOMETRIX_TENSOR_ACCESS_TYPE < 0
        #undef GEOMETRIX_TENSOR_ACCESS_TYPE
        #define GEOMETRIX_TENSOR_ACCESS_TYPE geometrix::e_prefer_compile_time_access
    # endif
    #if GEOMETRIX_TENSOR_ACCESS_TYPE > 3
        #undef GEOMETRIX_TENSOR_ACCESS_TYPE
        #define GEOMETRIX_TENSOR_ACCESS_TYPE geometrix::e_prefer_compile_time_access
    # endif

    #if GEOMETRIX_TENSOR_ACCESS_TYPE == 1
        #undef GEOMETRIX_TENSOR_ACCESS_TYPE
        #define GEOMETRIX_TENSOR_ACCESS_TYPE geometrix::e_prefer_compile_time_access
    # endif

    #if GEOMETRIX_TENSOR_ACCESS_TYPE == 3
        #undef GEOMETRIX_TENSOR_ACCESS_TYPE
        #define GEOMETRIX_TENSOR_ACCESS_TYPE geometrix::e_prefer_compile_time_access
    # endif
#endif

#endif //GEOMETRIX_DETAIL_TENSOR_ACCESS_DEFINES_HPP
