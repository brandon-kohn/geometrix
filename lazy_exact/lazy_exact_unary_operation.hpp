//  Boost lazy_exact_unary_operation.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_UNARY_OPERATION_HPP
#define _LAZY_EXACT_UNARY_OPERATION_HPP

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
// CLASS lazy_exact_unary_operation
//
// Base class and interface for unary operations.
//
template <typename FilterType, typename ExactType, typename OperationFunctor>
class lazy_exact_unary_operation : public lazy_exact_filter<FilterType>
{
public:

    typedef FilterType filter_type;
    typedef ExactType  exact_type;
    
    lazy_exact_unary_operation( const boost::numeric::interval<FilterType>& interval,
                                const typename detail::exact_provider< filter_type, exact_type >::pointer& value )
        : lazy_exact_filter<FilterType>(interval)
        , m_value( value )
    {}

    ///Calculate the exact value
    inline ExactType get_exact() const
    {
        return OperationFunctor::perform_operation( m_value->get_exact() );
    }
    
private:

    mutable typename detail::exact_provider< filter_type, exact_type >::pointer m_value;

};

}}///namespace boost::numeric

#endif //_LAZY_EXACT_UNARY_OPERATION_HPP