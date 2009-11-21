//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_DETAIL_VECTOR_GENERATOR_HPP
#define GENERATIVE_GEOMETRY_DETAIL_VECTOR_GENERATOR_HPP
#pragma once

#include <geometry\detail\numeric_sequence_limits.hpp>
#include <geometry\detail\numeric_sequence_headers.hpp>
#include <geometry\detail\vector_base.hpp>
#include <geometry\detail\vector_forward.hpp>

#define BOOST_PP_FILENAME_1 <geometry\detail\vector_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (1, GENERATIVE_GEOMETRY_MAX_NUMERIC_SEQUENCE_SIZE)
#include BOOST_PP_ITERATE()

#endif //GENERATIVE_GEOMETRY_DETAIL_VECTOR_GENERATOR_HPP


