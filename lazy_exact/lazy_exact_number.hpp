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
class lazy_exact_number : public lazy_exact_filter<FilterType>
{
public:

    typedef ExactType                                              exact_type;
    typedef FilterType                                             filter_type;
    typedef typename boost::call_traits< exact_type >::param_type  exact_param;
    typedef typename boost::call_traits< filter_type >::param_type filter_param;


	lazy_exact_number( filter_param value, filter_param filterPrecision )
	    : lazy_exact_filter<filter_type>( value, filterPrecision )
        , m_value( value )
    {}

	lazy_exact_number( filter_param value )
        : lazy_exact_filter<filter_type>( value )
        , m_value( value )
    {}

	lazy_exact_number( exact_param exact, filter_param filter )
	    : lazy_exact_filter<filter_type>( filter )
        , m_value( exact )
    {}

	lazy_exact_number( exact_param exact, const boost::numeric::interval<filter_type>& filter )
	    : lazy_exact_filter<filter_type>( filter )
        , m_value( exact )
    {}

	virtual ~lazy_exact_number(){}

    ///Calculate the exact value
    inline const exact_type& get_exact() const { return m_value ;}

private:

    exact_type m_value;

};

}}///namespace boost::numeric

#endif //_LAZY_EXACT_NUMBER_HPP