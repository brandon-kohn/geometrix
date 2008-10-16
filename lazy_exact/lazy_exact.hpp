//  Boost lazy_exact.hpp header file  ------------------------------------------//

//  (C) Copyright Brandon Kohn 2005.

//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or
//   copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef _LAZY_EXACT_HPP
#define _LAZY_EXACT_HPP

//Includes
#include <boost/numeric/lazy_exact/lazy_exact_number.hpp>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/compare/tribool.hpp>
#include <boost/operators.hpp>
#include <boost/shared_ptr.hpp>

//////////////////////////////////////////////
//Forward Declarations

namespace boost
{
namespace numeric
{

////////////////////////////////////////////////////////////////////
//
// CLASS lazy_exact
//
// The lazy_exact class provides a means for doing exact real arithmetic
// using the lazy filtering paradigm. In this paradigm, real numbers
// are approximated by an interval and a history of all mathematical
// operations which were performed to construct the resulting number
// are stored in a directed acyclic graph (DAG). By storing this history
// the lazy_exact is able to reconstruct an exact representation of the number
// while making it possible to delay this costly operation until such a time
// that they become unavoidable (e.g. cases when two numbers are being compared
// whose intervals overlap). 
// 
// 
//
template <typename FilterType, typename ExactType>
class lazy_exact :
	partially_ordered < lazy_exact<FilterType,ExactType>,				 ///lazy_exact < lazy_exact
	equality_comparable < lazy_exact<FilterType,ExactType>,				 ///lazy_exact == lazy_exact
	less_than_comparable2 < lazy_exact<FilterType,ExactType>, FilterType,///lazy_exact < FilterType|FilterType < lazy_exact
	equality_comparable2 < lazy_exact<FilterType,ExactType>, FilterType, ///lazy_exact == FilterType | FilterType == lazy_exact
	addable < lazy_exact<FilterType,ExactType>,							 ///lazy_exact + lazy_exact
	subtractable < lazy_exact<FilterType,ExactType>,					 ///lazy_exact - lazy_exact
	multipliable < lazy_exact<FilterType,ExactType>,					 ///lazy_exact * lazy_exact
	dividable < lazy_exact<FilterType,ExactType>,						 ///lazy_exact / lazy_exact
	addable2 < lazy_exact<FilterType,ExactType>, FilterType,			 ///lazy_exact + FilterType | FilterType + lazy_exact
	subtractable2 < lazy_exact<FilterType,ExactType>, FilterType,		 ///lazy_exact - FilterType | FilterType - lazy_exact
	subtractable2_left < lazy_exact<FilterType,ExactType>, FilterType,	 ///lazy_exact - FilterType | FilterType - lazy_exact
	multipliable2 < lazy_exact<FilterType,ExactType>, FilterType,		 ///lazy_exact * FilterType | FilterType * lazy_exact
	dividable2 < lazy_exact<FilterType,ExactType>, FilterType,			 ///lazy_exact / FilterType | FilterType / lazy_exact
	dividable2_left < lazy_exact<FilterType,ExactType>, FilterType		 ///lazy_exact / FilterType | FilterType / lazy_exact
	> > > > > > > > > > > > > >
{

public:

	typedef ExactType exact_type;
	typedef FilterType filter_type;
	typedef typename boost::call_traits<FilterType>::param_type param_type;

	lazy_exact()
	    : m_representation( new lazy_exact_number<FilterType,ExactType>( 0 ) )
	{}

	lazy_exact( param_type value, param_type filterPrecision )
	    : m_representation( new lazy_exact_number<FilterType,ExactType>( value,filterPrecision ) )
	{}

	lazy_exact( param_type value )
	    : m_representation( new lazy_exact_number<FilterType,ExactType>( value ) )
	{}

	lazy_exact( boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > representation )
	    : m_representation( representation )
	{}

	lazy_exact( const ExactType& value, const FilterType& filter )
	    : m_representation( new lazy_exact_number<FilterType,ExactType>( value,filter ) )
	{}

	virtual ~lazy_exact(){}

	///Assignment from FilterType
	lazy_exact& operator=(param_type n);

	///Compare this number with another for less than
	bool operator < (const lazy_exact<FilterType, ExactType> & other) const;

	///Compare equality
	bool operator == (const lazy_exact& other) const;

	///Addition Operator
	lazy_exact& operator += ( const lazy_exact& rhs);

	///Multiplication Operator
	lazy_exact& operator *= ( const lazy_exact& rhs);

	///Division Operator
	lazy_exact& operator /= ( const lazy_exact& rhs);

	///Subtraction Operator
	lazy_exact& operator -= ( const lazy_exact& rhs);

	///Check the sign of the number
	int	sign() const;

	///Access the interval approximation
	const boost::numeric::interval<FilterType>& approximate_value() const { return m_representation->approximate_value(); }

	///Access the exact value
	ExactType exact_value() const;

	///Access the cell base type
	const boost::shared_ptr<lazy_exact_base<FilterType,ExactType> >& representation() const { return m_representation; }

private:

	mutable boost::shared_ptr< lazy_exact_base<FilterType,ExactType> > m_representation;

};

///Assignment from FilterType
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType, ExactType>::operator=(param_type n)
{
	m_representation = boost::shared_ptr< lazy_exact_base<FilterType,ExactType> >(new lazy_exact_number<FilterType,ExactType>(n));
	return *this;
}

template <typename FilterType, typename ExactType>
ExactType lazy_exact<FilterType, ExactType>::exact_value() const 
{
	return m_representation->exact_value();
}

///Compare this number with another for less than
template <typename FilterType, typename ExactType>
bool lazy_exact<FilterType, ExactType>::operator <( const lazy_exact<FilterType, ExactType> & other) const
{
    using namespace boost::numeric::interval_lib::compare::tribool;
    boost::tribool result = approximate_value() < other.approximate_value();
    if( result )
        return true;
    else if( !result )
        return false;
    else
        return ( exact_value() < other.exact_value() );
}

///Compare equality
template <typename FilterType, typename ExactType>
bool lazy_exact<FilterType, ExactType>::operator ==( const lazy_exact<FilterType, ExactType> & other) const
{
    using namespace boost::numeric::interval_lib::compare::tribool;
    boost::tribool result = approximate_value() == other.approximate_value();
    if( result )
        return true;
    else if( !result )
        return false;
    else
        return ( exact_value() == other.exact_value() );
}

///Addition Operator
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator += (const lazy_exact<FilterType,ExactType> &rhs)
{
	m_representation = boost::shared_ptr< lazy_exact_base<FilterType,ExactType>	>(m_representation + rhs.m_representation);
	return *this;
}

///Multiplication Operator
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator *= (const lazy_exact<FilterType,ExactType> &rhs)
{
	m_representation = (m_representation * rhs.m_representation);
	return *this;
}

///Division Operator
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator /= (const lazy_exact<FilterType,ExactType> &rhs)
{
	m_representation = (m_representation / rhs.m_representation);
	return *this;
}

///Subtraction Operator
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator -= (const lazy_exact<FilterType,ExactType> &rhs)
{	
	m_representation = (m_representation - rhs.m_representation);
	return *this;
}

///Check the sign of the number
template <typename FilterType, typename ExactType> inline
int	lazy_exact<FilterType,ExactType>::sign() const
{
	///if the interval contains zero.. we must perform the exact evaluation.
	FilterType zero(0);
	if( approximate_value() == zero )
	{
		if( exact_value() < 0 )
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if( approximate_value() < zero )
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
}

///Square Root operator
template <typename FilterType, typename ExactType> inline
lazy_exact<FilterType,ExactType> sqrt( const lazy_exact<FilterType,ExactType> &value )
{
	return lazy_exact<FilterType,ExactType>(sqrt(value.representation()));	
}

///Absolute value operator
template <typename FilterType, typename ExactType> inline
lazy_exact<FilterType,ExactType> abs( const lazy_exact<FilterType,ExactType> &value )
{
	return lazy_exact<FilterType,ExactType>(abs(value.representation()));
}

}} ///namespace boost::numeric

#endif //_LAZY_EXACT_TYPE_HPP
