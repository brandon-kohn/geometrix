//  Boost lazy_exact_base.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_BASE_HPP
#define _LAZY_EXACT_BASE_HPP

//Includes
#include <boost/numeric/interval.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/call_traits.hpp>

//////////////////////////////////////////////
//Forward Declarations

namespace boost
{
namespace numeric
{

//////////////////////////////////////////////
//
// CLASS lazy_exact_base
//
// Base class and interface to the lazy_exact number type.
//
template <typename FilterType, typename ExactType>
class lazy_exact_base
{
public:
		
	typedef typename boost::call_traits<FilterType>::param_type param_type;

	lazy_exact_base( param_type value, param_type filterPrecision )
        : m_interval(value-(filterPrecision*value),value+(filterPrecision*value))
    {}

	lazy_exact_base( const boost::numeric::interval<FilterType>& interval )
        : m_interval(interval)
    {}
	
	///TODO: Need conversion from exact to filter type to set interval.
	lazy_exact_base( const FilterType& filter )
        : m_interval(filter,filter)
    {}

	virtual ~lazy_exact_base(){}

	///Access the interval approximation
	inline const boost::numeric::interval<FilterType>& approximate_value() const { return m_interval; }
	
	///Calculate the exact value
	virtual void calculate_exact() const = 0;

	///Access the exact value
	ExactType exact_value() const;

protected:

    ///Set the value of the exact
    inline void	set_exact(const ExactType& exact) const { m_exactValue.reset( new ExactType( exact ) ); }

private:

	mutable boost::numeric::interval<FilterType> m_interval;
    mutable boost::shared_ptr<ExactType>         m_exactValue;

};

///Access the exact value
template<typename FilterType, typename ExactType>
ExactType lazy_exact_base<FilterType,ExactType>::exact_value() const
{
	if( !m_exactValue )
	{
		calculate_exact();
	}

    return *m_exactValue;
}

}}///namespace boost::numeric

#endif //_LAZY_EXACT_BASE_TYPE_HPP