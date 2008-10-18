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
class lazy_exact_binary_operation : public lazy_exact_filter<FilterType>
{
public:

    typedef FilterType filter_type;
    typedef ExactType  exact_type;
    
    lazy_exact_binary_operation( const boost::numeric::interval<FilterType>& interval,
                                 const typename detail::exact_provider< filter_type, exact_type >::pointer& lhsValue,
                                 const typename detail::exact_provider< filter_type, exact_type >::pointer& rhsValue )
        : lazy_exact_filter<FilterType>( interval )
        , m_lhsValue( lhsValue )
        , m_rhsValue( rhsValue )
    {}

    ///Calculate the exact value
	inline ExactType get_exact() const
    {
        return OperationFunctor::perform_operation( m_lhsValue->get_exact(), m_rhsValue->get_exact() );
    }

private:

    mutable typename detail::exact_provider< filter_type, exact_type >::pointer m_lhsValue;
    mutable typename detail::exact_provider< filter_type, exact_type >::pointer m_rhsValue;

};

}}///namespace boost::numeric

#endif //_LAZY_EXACT_BINARY_OPERATION_HPP
