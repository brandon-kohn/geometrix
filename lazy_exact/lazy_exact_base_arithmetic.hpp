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
// template <typename LazyType> 
// inline boost::intrusive_ptr< lazy_exact_base<typename LazyType::filter_type, typename LazyType::exact_type> > operator * ( const LazyType& lhs,
//                                                                                                                            const LazyType& rhs )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
//     boost::intrusive_ptr< lazy_exact_base<filter_type, exact_type> > op( new lazy_exact_binary_operation< filter_type, exact_type, lazy_multiplication< exact_type > >( (lhs.approximate_value()*rhs.approximate_value()), lhs, rhs ) );
//     return op;
// }
// 
// ///Addition operator
// template <typename LazyType> 
// inline boost::intrusive_ptr< lazy_exact_base<typename LazyType::filter_type, typename LazyType::exact_type> > operator + ( const LazyType& lhs,
//                                                                                                                            const LazyType& rhs )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
//     boost::intrusive_ptr< lazy_exact_base<filter_type, exact_type> > op( new lazy_exact_binary_operation< filter_type, exact_type, lazy_addition< exact_type > >( (lhs.approximate_value() + rhs.approximate_value()), lhs, rhs ) );
//     return op;
// }
// 
// ///Subtraction operator
// template <typename LazyType> 
// inline boost::intrusive_ptr< lazy_exact_base<typename LazyType::filter_type, typename LazyType::exact_type> > operator - ( const LazyType& lhs,
//                                                                                                                            const LazyType& rhs )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
//     boost::intrusive_ptr< lazy_exact_base<filter_type, exact_type> > op( new lazy_exact_binary_operation< filter_type, exact_type, lazy_subtraction< exact_type > >( (lhs.approximate_value() - rhs.approximate_value()), lhs, rhs ) );
//     return op;
// }
// 
// ///Division operator
// template <typename LazyType> 
// inline boost::intrusive_ptr< lazy_exact_base<typename LazyType::filter_type, typename LazyType::exact_type> > operator / ( const LazyType& lhs,
//                                                                                                                            const LazyType& rhs )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
// 
//     ///first check if the interval of rhs contains zero.. if so the reciprocal interval must be fixed before evaluation.
//     if( rhs.approximate_value() == filter_type(0) )
//     {
//         lazy_exact_number< filter_type, exact_type > eRHS( rhs.get_exact(), rhs.approximate_value() );
//         boost::intrusive_ptr< lazy_exact_base<filter_type, exact_type> > op( new lazy_exact_binary_operation<filter_type, exact_type, lazy_division<exact_type> >( lhs.approximate_value() / rhs.approximate_value(), lhs, eRHS ) );
//         return op;
//     }
//     else
//     {
//         boost::intrusive_ptr< lazy_exact_base<filter_type, exact_type> > op( new lazy_exact_binary_operation<filter_type, exact_type, lazy_division<exact_type> >( lhs.approximate_value() / rhs.approximate_value(), lhs, rhs) );
//         return op;
//     }
// }
// 
// ///Absolute value
// template <typename LazyType> 
// inline boost::intrusive_ptr< lazy_exact_base<typename LazyType::filter_type, typename LazyType::exact_type> > abs ( const LazyType& value )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
//     boost::intrusive_ptr< lazy_exact_base<filter_type, exact_type> > op( new lazy_exact_unary_operation<filter_type, exact_type, lazy_abs<exact_type> >( abs( value.approximate_value() ), value ) );
//     return op;
// }
// 
// ///Square Root 
// template <typename LazyType> 
// inline boost::intrusive_ptr< lazy_exact_base<typename LazyType::filter_type, typename LazyType::exact_type> > sqrt ( const LazyType& value )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
//     boost::intrusive_ptr< lazy_exact_base<filter_type, exact_type> > op( new lazy_exact_unary_operation<filter_type, exact_type, lazy_sqrt<exact_type> >( sqrt( value.approximate_value() ), value ) );
//     return op;
// }

///Multiplication operator
// template <typename LazyType1, typename LazyType2> 
// inline lazy_exact_binary_operation< typename LazyType1::filter_type, typename LazyType1::exact_type, LazyType1, LazyType2, lazy_multiplication< typename LazyType1::exact_type > > operator * ( const LazyType1& lhs,
//                                                                                                                                                                                                 const LazyType2& rhs )
// {
//     typedef typename LazyType1::filter_type filter_type;
//     typedef typename LazyType1::exact_type exact_type;
//     return lazy_exact_binary_operation< filter_type, exact_type, LazyType1, LazyType2, lazy_multiplication< exact_type > >( lhs.approximate_value()*rhs.approximate_value(), lhs, rhs );
// }
// 
// ///Addition operator
// template <typename LazyType1, typename LazyType2> 
// inline lazy_exact_binary_operation< typename LazyType1::filter_type, typename LazyType1::exact_type, LazyType1, LazyType2, lazy_addition< typename LazyType1::exact_type > > operator + ( const LazyType1& lhs,
//                                                                                                                                                                                           const LazyType2& rhs )
// {
//     typedef typename LazyType1::filter_type filter_type;
//     typedef typename LazyType1::exact_type exact_type;
//     return lazy_exact_binary_operation< filter_type, exact_type, LazyType1, LazyType2, lazy_addition< exact_type > >( lhs.approximate_value()+rhs.approximate_value(), lhs, rhs );
// }
// 
// ///Subtraction operator
// template <typename LazyType1, typename LazyType2> 
// inline lazy_exact_binary_operation< typename LazyType1::filter_type, typename LazyType1::exact_type, LazyType1, LazyType2, lazy_subtraction< typename LazyType1::exact_type > > operator + ( const LazyType1& lhs,
//                                                                                                                                                                                              const LazyType2& rhs )
// {
//     typedef typename LazyType1::filter_type filter_type;
//     typedef typename LazyType1::exact_type exact_type;
//     return lazy_exact_binary_operation< filter_type, exact_type, LazyType1, LazyType2, lazy_subtraction< exact_type > >( lhs.approximate_value()-rhs.approximate_value(), lhs, rhs );
// }
// 
// ///Division operator
// template <typename LazyType1, typename LazyType2> 
// inline lazy_exact_binary_operation< typename LazyType1::filter_type, typename LazyType1::exact_type, lazy_division< typename LazyType1::exact_type > > operator / ( const LazyType1& lhs,
//                                                                                                                                                                                           const LazyType2& rhs )
// {
//     typedef typename LazyType1::filter_type filter_type;
//     typedef typename LazyType1::exact_type  exact_type;
// 
//     ///first check if the interval of rhs contains zero.. if so the reciprocal interval must be fixed before evaluation.
//     if( rhs.approximate_value() == filter_type(0) )
//     {
//         lazy_exact_number< filter_type, exact_type > eRHS( rhs.get_exact(), rhs.approximate_value() );
//         return lazy_exact_binary_operation< filter_type, exact_type, lazy_subtraction< exact_type > >( lhs.approximate_value()/rhs.approximate_value(), lhs, eRHS );        
//     }
//     else
//     {
//         return lazy_exact_binary_operation< filter_type, exact_type, LazyType1, LazyType2, lazy_division< exact_type > >( lhs.approximate_value()/rhs.approximate_value(), lhs, rhs );        
//     }
// }
// 
// ///Absolute value
// template <typename LazyType> 
// inline lazy_exact_unary_operation< typename LazyType::filter_type, typename LazyType::exact_type, lazy_abs< typename LazyType::exact_type > > abs ( const LazyType& value )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
//     return lazy_exact_unary_operation< filter_type, exact_type, LazyType, lazy_abs< exact_type > >( value.approximate_value(), value ); 
// }
// 
// ///Square Root 
// template <typename LazyType> 
// inline lazy_exact_unary_operation< typename LazyType::filter_type, typename LazyType::exact_type, lazy_sqrt< typename LazyType::exact_type > > sqrt ( const LazyType& value )
// {
//     typedef typename LazyType::filter_type filter_type;
//     typedef typename LazyType::exact_type exact_type;
//     return lazy_exact_unary_operation< filter_type, exact_type, lazy_sqrt< exact_type > >( value.approximate_value(), value ); 
// }

}}///namespace boost::numeric

#endif //_LAZY_EXACT_BASE_ARITHMETIC_HPP
