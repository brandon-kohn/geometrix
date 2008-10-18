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
#include <boost/smart_ptr.hpp>

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

	typedef ExactType                                              exact_type;
	typedef FilterType                                             filter_type;
    typedef typename boost::call_traits< exact_type >::param_type  exact_param;
    typedef typename boost::call_traits< filter_type >::param_type filter_param;

	lazy_exact()
        : m_representation( detail::make_lazy_exact_representation( lazy_exact_number<filter_type,exact_type>( filter_type( 0 ) ) ) )
	{}

    explicit lazy_exact( exact_param value, filter_param filterPrecision )
        : m_representation( detail::make_lazy_exact_representation( lazy_exact_number<filter_type,exact_type>( value, filterPrecision ) ) )
	{}
    
    explicit lazy_exact( filter_param value )
        : m_representation( detail::make_lazy_exact_representation( lazy_exact_number<filter_type,exact_type>( value  ) ) )
	{}

    template <typename T>
    lazy_exact( T v )
        : m_representation( detail::make_lazy_exact_representation( lazy_exact_number< filter_type, exact_type >( filter_type( v ) ) ) )
    {}

    explicit lazy_exact( const typename detail::exact_provider< filter_type, exact_type >::pointer& representation )
	    : m_representation( representation )
	{}

    virtual ~lazy_exact(){ } //delete m_representation; }

	///Assignment from filter type.
	lazy_exact& operator=( filter_param n );

	///Compare this number with another for less than
	bool operator < (const lazy_exact<filter_type, exact_type> & other) const;

	///Compare equality
	bool operator == ( const lazy_exact& other ) const;

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

	//! Access the interval approximation
	boost::numeric::interval<filter_type> approximate_value() const { return m_representation->approximate_value(); }

	///Access the exact value
	exact_type exact_value() const;

	//! Access the representation 
    const typename detail::exact_provider< filter_type, exact_type >::pointer representation() const { return m_representation; }

private:

    mutable typename detail::exact_provider< filter_type, exact_type >::pointer m_representation;    

};

///Assignment from filter_type
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType, ExactType>::operator=( filter_param n )
{
	m_representation = detail::make_lazy_exact_representation( lazy_exact_number< filter_type, exact_type >( n ) );
	return *this;
}

template <typename FilterType, typename ExactType>
ExactType lazy_exact<FilterType, ExactType>::exact_value() const 
{
    if( !m_representation->is_exact() )
    {                
        m_representation = detail::make_lazy_exact_representation( lazy_exact_number< filter_type, exact_type >( m_representation->get_exact(), approximate_value() ) );
    }	
    
    return m_representation->get_exact();
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
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator += ( const lazy_exact<FilterType,ExactType> &rhs )
{    
    m_representation  = detail::make_lazy_exact_representation( lazy_exact_binary_operation< filter_type, exact_type, lazy_addition< exact_type > >( approximate_value()+rhs.approximate_value(), m_representation, rhs.m_representation->clone() ) );    	
	return *this;
}

///Multiplication Operator
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator *= ( const lazy_exact<FilterType,ExactType> &rhs )
{    
    m_representation = detail::make_lazy_exact_representation( lazy_exact_binary_operation< filter_type, exact_type, lazy_multiplication< exact_type > >( approximate_value()*rhs.approximate_value(), m_representation, rhs.m_representation->clone() ) );	
	return *this;
}

///Division Operator
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator /= ( const lazy_exact<FilterType,ExactType> &rhs )
{
    ///first check if the interval of rhs contains zero.. if so the reciprocal interval must be fixed before evaluation.
    if( rhs.approximate_value() == filter_type(0) )
    {
        lazy_exact_number< filter_type, exact_type > eRHS( rhs.exact_value(), rhs.approximate_value() );
        m_representation = detail::make_lazy_exact_representation( lazy_exact_binary_operation< filter_type, exact_type, lazy_division< exact_type > >( approximate_value()/rhs.approximate_value(), m_representation, detail::make_lazy_exact_representation( eRHS ) ) );
    }
    else
    {
        m_representation = detail::make_lazy_exact_representation( lazy_exact_binary_operation< filter_type, exact_type, lazy_division< exact_type > >( approximate_value()/rhs.approximate_value(), m_representation, rhs.m_representation->clone() ) );
    }   
    
	return *this;
}

///Subtraction Operator
template <typename FilterType, typename ExactType>
lazy_exact<FilterType,ExactType>& lazy_exact<FilterType,ExactType>::operator -= ( const lazy_exact<FilterType,ExactType> &rhs )
{	
    m_representation = detail::make_lazy_exact_representation( lazy_exact_binary_operation< filter_type, exact_type, lazy_subtraction< exact_type > >( approximate_value()-rhs.approximate_value(), m_representation, rhs.m_representation->clone() ) );
    return *this;
}

///Check the sign of the number
template <typename FilterType, typename ExactType> inline
int	lazy_exact<FilterType,ExactType>::sign() const
{
	///if the interval contains zero.. we must perform the exact evaluation.
	filter_type zero(0);
	if( approximate_value() == zero )
	{
		if( exact_value() < exact_type(0) )
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
    return lazy_exact<FilterType,ExactType>( detail::make_lazy_exact_representation( lazy_exact_unary_operation< FilterType, ExactType, lazy_sqrt< ExactType > >( sqrt( value.approximate_value() ), value.representation()->clone() ) ) );
}

///Absolute value operator
template <typename FilterType, typename ExactType> inline
lazy_exact<FilterType,ExactType> abs( const lazy_exact<FilterType,ExactType> &value )
{
    return lazy_exact<FilterType,ExactType>( detail::make_lazy_exact_representation( lazy_exact_unary_operation< FilterType, ExactType, lazy_abs< ExactType > >( abs( value.approximate_value() ), value.representation()->clone() ) ) );
}

}} ///namespace boost::numeric

#endif //_LAZY_EXACT_TYPE_HPP
