//  Boost lazy_exact_binary_operation.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_BINARY_OPERATION_HPP
#define _LAZY_EXACT_BINARY_OPERATION_HPP

//Includes
#include <boost/numeric/lazy_exact/lazy_exact_base.hpp>

//////////////////////////////////////////////
//Forward Declarations

namespace boost
{
namespace numeric
{

//////////////////////////////////////////////
//
// CLASS lazy_exact_binary_operation
//
// Base class and interface for binary arithmetic operations.
//
template <typename FilterType, typename ExactType, typename OperationFunctor>
class lazy_exact_binary_operation : public lazy_exact_base<FilterType, ExactType>
{
public:

	lazy_exact_binary_operation( const boost::numeric::interval<FilterType>& interval,
                                 const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> >& lhsValue,
                                 const boost::shared_ptr<lazy_exact_base<FilterType, ExactType> >& rhsValue )
        : lazy_exact_base<FilterType,ExactType>(interval),
		  m_lhsValue(lhsValue),
          m_rhsValue(rhsValue)
    {}

	virtual ~lazy_exact_binary_operation(){}

    ///Calculate the exact value
	inline void calculate_exact() const
    {
        lazy_exact_base<FilterType, ExactType>::set_exact( OperationFunctor::perform_operation( m_lhsValue->exact_value(), m_rhsValue->exact_value() ) );
    }

private:

	boost::shared_ptr< lazy_exact_base<FilterType, ExactType> >	m_lhsValue;
	boost::shared_ptr< lazy_exact_base<FilterType, ExactType> >	m_rhsValue;

};

}}///namespace boost::numeric

#endif //_LAZY_EXACT_BINARY_OPERATION_HPP
