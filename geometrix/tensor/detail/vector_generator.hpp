//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_VECTOR_GENERATOR_HPP
#define GEOMETRIX_DETAIL_VECTOR_GENERATOR_HPP

#include <geometrix/tensor/detail/numeric_sequence_limits.hpp>
#include <geometrix/tensor/detail/numeric_sequence_base.hpp>
#include <geometrix/tensor/detail/vector_base.hpp>
#include <geometrix/tensor/vector_forward.hpp>

#define BOOST_PP_FILENAME_1 <geometrix/tensor/detail/vector_n.hpp>
#define BOOST_PP_ITERATION_LIMITS (1, GEOMETRIX_MAX_NUMERIC_SEQUENCE_SIZE)
#include BOOST_PP_ITERATE()

//GEOMETRIX_DETAIL_VECTOR_GENERATOR_HPP
#endif 
