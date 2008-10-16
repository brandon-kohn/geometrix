/////////////////////////////////////////////////////////////////////
//
//	file:	    lazy_exact_addition_operation.h
//	created:	2005/02/19
//	author:		Brandon Kohn
//	
//	purpose:	
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _LAZY_EXACT_ADDITION_OPERATION_H
#define _LAZY_EXACT_ADDITION_OPERATION_H

//Includes
#include <lazy_exact_binary_operation.h>

//////////////////////////////////////////////
//Forward Declarations

//////////////////////////////////////////////
//
// CLASS lazy_exact_addition_operation
//
// Description goes here.
//
template <typename FilterType, typename NumberType>
class lazy_exact_addition_operation : public lazy_exact_binary_operation<FilterType,NumberType>
{
public:

	lazy_exact_addition_operation(){}

	virtual ~lazy_exact_addition_operation(){}

    //Public Member Functions

	///Perform the associated operation
    virtual boost::shared_ptr< lazy_exact_base > perform_operation() = 0;

	///Access the exact value
	virtual const NumberType&	exact_value();

private:

    //Private Member Functions

    //Private Member Variables

};

#endif //_LAZY_EXACT_ADDITION_OPERATION_H