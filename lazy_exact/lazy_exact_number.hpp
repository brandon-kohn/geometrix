//  Boost lazy_exact_number.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_NUMBER_HPP
#define _LAZY_EXACT_NUMBER_HPP

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
// CLASS lazy_exact_number
//
// Initial representation of the number from the interval or from an exact type.
//
template <typename FilterType, typename ExactType>
class lazy_exact_number : public lazy_exact_base<FilterType, ExactType>
{
public:

	typedef typename boost::call_traits<FilterType>::param_type param_type;

	lazy_exact_number( param_type value, param_type filterPrecision )
	    : lazy_exact_base<FilterType,ExactType>( value, filterPrecision )
        , m_value( value )
    {}

	lazy_exact_number( param_type value )
        : lazy_exact_base<FilterType,ExactType>( value )
        , m_value( value )
    {}

	lazy_exact_number( const ExactType& exact, const FilterType& filter )
	    : lazy_exact_base<FilterType,ExactType>( filter )
        , m_value( exact )
    {}

	lazy_exact_number( const ExactType& exact, const boost::numeric::interval<FilterType>& filter )
	    : lazy_exact_base<FilterType,ExactType>( filter )
        , m_value( exact )
    {}

	virtual ~lazy_exact_number(){}

    ///Calculate the exact value
	void calculate_exact() const;

    ExactType m_value;

};

template <typename FilterType, typename ExactType>
void lazy_exact_number<FilterType,ExactType>::calculate_exact() const
{
	///Use the median of the interval (as good an approximation as any)
	set_exact( m_value );
}

}}///namespace boost::numeric

#endif //_LAZY_EXACT_NUMBER_HPP