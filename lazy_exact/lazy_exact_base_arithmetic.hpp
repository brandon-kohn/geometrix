//  Boost lazy_exact_base_arithmetic.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)


#pragma once
#ifndef _LAZY_EXACT_BASE_ARITHMETIC_HPP
#define _LAZY_EXACT_BASE_ARITHMETIC_HPP

//Includes
#include <boost/numeric/lazy_exact/lazy_exact.hpp>
#include <boost/numeric/lazy_exact/lazy_exact_binary_operation.hpp>
#include <boost/numeric/lazy_exact/lazy_exact_binary_functors.hpp>
#include <boost/numeric/lazy_exact/lazy_exact_unary_operation.hpp>
#include <boost/numeric/lazy_exact/lazy_exact_unary_functors.hpp>

namespace boost
{
namespace numeric
{

///Multiplication operator
template <typename FilterType, typename ExactType> inline
boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > operator * ( const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &lhs,
												                        const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &rhs )
{
	boost::numeric::interval<FilterType> tempInterval(lhs->approximate_value()*rhs->approximate_value());
	boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > op( new lazy_exact_binary_operation<FilterType,ExactType,lazy_multiplication<ExactType> >(tempInterval,lhs,rhs) );
    return op;
}

///Addition operator
template <typename FilterType, typename ExactType> inline
boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > operator + ( const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &lhs,
                                                                        const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &rhs )
{
	boost::numeric::interval<FilterType> tempInterval(lhs->approximate_value()+rhs->approximate_value());
	boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > op( new lazy_exact_binary_operation<FilterType,ExactType,lazy_addition<ExactType> >(tempInterval,lhs,rhs) );
    return op;
}

///Subtraction operator
template <typename FilterType, typename ExactType> inline
boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > operator - ( const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &lhs,
                                                                        const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &rhs )
{
	boost::numeric::interval<FilterType> tempInterval(lhs->approximate_value()-rhs->approximate_value());
	boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > op( new lazy_exact_binary_operation<FilterType,ExactType,lazy_subtraction<ExactType> >(tempInterval,lhs,rhs) );
    return op;
}

///Division operator
template <typename FilterType, typename ExactType> inline
boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > operator / ( const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &lhs,
                                                                        const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &rhs )
{
	///first check if the interval of rhs contains zero.. if so the reciprocal interval must be fixed before evaluation.
	if( rhs->approximate_value() == FilterType(0) )
	{
		rhs->exact_value();
	}

	boost::numeric::interval<FilterType> tempInterval(lhs->approximate_value()/rhs->approximate_value());
	boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > op( new lazy_exact_binary_operation<FilterType,ExactType,lazy_division<ExactType> >(tempInterval,lhs,rhs) );
    return op;
}

///Absolute value
template <typename FilterType, typename ExactType> inline
boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > abs ( const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &value )
{
	boost::numeric::interval<FilterType> tempInterval(abs(value->approximate_value()));
	boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > op( new lazy_exact_unary_operation<FilterType,ExactType,lazy_abs<ExactType> >(tempInterval,value) );
    return op;
}

///Square Root 
template <typename FilterType, typename ExactType> inline
boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > sqrt ( const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> > &value )
{
	boost::numeric::interval<FilterType> tempInterval(sqrt(value->approximate_value()));
	boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > op( new lazy_exact_unary_operation<FilterType,ExactType,lazy_sqrt<ExactType> >(tempInterval,value) );
    return op;
}

}}///namespace boost::numeric

#endif //_LAZY_EXACT_BASE_ARITHMETIC_HPP