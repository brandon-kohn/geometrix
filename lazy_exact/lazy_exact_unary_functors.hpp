//  Boost lazy_exact_unary_functors.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_UNARY_FUNCTORS_HPP
#define _LAZY_EXACT_UNARY_FUNCTORS_HPP

//Includes
#include <boost/numeric/lazy_exact/lazy_exact_base.hpp>

//////////////////////////////////////////////
//Forward Declarations

namespace boost
{
namespace numeric
{

///SQRT
template<typename ExactType>
class lazy_sqrt
{
public:

	///Function to perform the sqrt operation
	static ExactType perform_operation(const ExactType& value);

};

template<typename ExactType>
ExactType lazy_sqrt<ExactType>::perform_operation(const ExactType& value)
{
	return sqrt2(value);
}

///SQRT
template<typename ExactType>
class lazy_abs
{
public:

	///Function to perform the sqrt operation
	static ExactType perform_operation(const ExactType& value);

};

template<typename ExactType>
ExactType lazy_abs<ExactType>::perform_operation(const ExactType& value)
{
	return abs(value);
}

}}///namespace boost::numeric

#endif //_LAZY_EXACT_UNARY_FUNCTORS_HPP
