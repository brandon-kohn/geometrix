//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_LIMITS_HPP
#define _GENERATIVE_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_LIMITS_HPP
#pragma once

#if !defined( BOOST_GEOMETRY_MAX_NUMERIC_SEQUENCE_SIZE )
    #define BOOST_GEOMETRY_MAX_NUMERIC_SEQUENCE_SIZE 20
#else
    #if BOOST_GEOMETRY_MAX_NUMERIC_SEQUENCE_SIZE < 3
        #undef BOOST_GEOMETRY_MAX_NUMERIC_SEQUENCE_SIZE
        #define BOOST_GEOMETRY_MAX_NUMERIC_SEQUENCE_SIZE 20
    # endif
#endif

#endif //_GENERATIVE_GEOMETRY_DETAIL_NUMERIC_SEQUENCE_LIMITS_HPP
