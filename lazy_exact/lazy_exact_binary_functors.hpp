//  Boost lazy_exact_binary_functors.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_BINARY_FUNCTORS_HPP
#define _LAZY_EXACT_BINARY_FUNCTORS_HPP

#include <boost/numeric/lazy_exact/lazy_exact_base.hpp>

//////////////////////////////////////////////
//Forward Declarations
namespace boost
{
namespace numeric
{

template<typename ExactType>
struct lazy_addition
{
	///Function to perform the addition operation
	static ExactType perform_operation(const ExactType& lhs,const ExactType& rhs);
};

template<typename ExactType>
struct lazy_division
{
	///Function to perform the division operation
	static ExactType perform_operation(const ExactType& lhs,const ExactType& rhs);
};

template<typename ExactType>
struct lazy_subtraction
{
	///Function to perform the subtraction operation
	static ExactType perform_operation(const ExactType& lhs,const ExactType& rhs);

};

template<typename ExactType>
struct lazy_multiplication
{
	///Function to perform the multiplication operation
	static ExactType perform_operation( const ExactType& lhs,const ExactType& rhs );
};

template<typename ExactType>
struct lazy_pow
{
	///Function to perform the pow operation
	static ExactType perform_operation( const ExactType& base, const ExactType& exponent );
};

template<typename ExactType>
ExactType lazy_addition<ExactType>::perform_operation( const ExactType& lhs, const ExactType& rhs )
{
	return (lhs + rhs);
}

template<typename ExactType>
ExactType lazy_subtraction<ExactType>::perform_operation( const ExactType& lhs, const ExactType& rhs )
{
	return (lhs - rhs);
}

template<typename ExactType>
ExactType lazy_division<ExactType>::perform_operation( const ExactType& lhs, const ExactType& rhs )
{
	return (lhs / rhs);
}

template<typename ExactType>
ExactType lazy_multiplication<ExactType>::perform_operation( const ExactType& lhs, const ExactType& rhs )
{
	return (lhs * rhs);
}

}}///namespace boost::numeric

#endif //_LAZY_EXACT_BINARY_FUNCTORS_HPP