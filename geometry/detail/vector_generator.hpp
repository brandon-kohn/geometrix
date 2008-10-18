//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_VECTOR_GENERATOR_HPP
#define _BOOST_GEOMETRY_DETAIL_VECTOR_GENERATOR_HPP
#pragma once

#include "numeric_sequence_limits.hpp"
#include "numeric_sequence_headers.hpp"
#include "vector_base.hpp"

#define BOOST_PP_FILENAME_1 "detail/vector_n.hpp"
#define BOOST_PP_ITERATION_LIMITS (1, BOOST_GEOMETRY_MAX_NUMERIC_SEQUENCE_SIZE)
#include BOOST_PP_ITERATE()

#endif //_BOOST_GEOMETRY_DETAIL_VECTOR_GENERATOR_HPP


