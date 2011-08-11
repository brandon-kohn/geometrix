//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_NUMERIC_SEQUENCE_LIMITS_HPP
#define GEOMETRIX_DETAIL_NUMERIC_SEQUENCE_LIMITS_HPP


#if !defined( GEOMETRIX_MAX_NUMERIC_SEQUENCE_SIZE )
    #define GEOMETRIX_MAX_NUMERIC_SEQUENCE_SIZE 20
#else
    #if GEOMETRIX_MAX_NUMERIC_SEQUENCE_SIZE < 3
        #undef GEOMETRIX_MAX_NUMERIC_SEQUENCE_SIZE
        #define GEOMETRIX_MAX_NUMERIC_SEQUENCE_SIZE 20
    # endif
#endif

//GEOMETRIX_DETAIL_NUMERIC_SEQUENCE_LIMITS_HPP
#endif 
