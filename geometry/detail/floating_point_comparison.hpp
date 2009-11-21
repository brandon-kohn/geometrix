//
//! - Based on original code from: 
//!   © Gennadiy Rozental 2001-2002 
//! Brandon Kohn 2008 - extended to use two different types.
//! 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_DETAIL_FLOATING_POINT_COMPARISON_HPP
#define GENERATIVE_GEOMETRY_DETAIL_FLOATING_POINT_COMPARISON_HPP
#pragma once

#include <boost/test/floating_point_comparison.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/units/unit.hpp>
#include <geometry\numeric_traits.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    
//! \brief Function to compute the absolute value of a numeric type.
template <typename NumericType>
NumericType absolute_value( const NumericType& v ) 
{
    return v < numeric_traits<NumericType>::zero ? -v : v;
}

//! \brief Function to perform a division with care being taken to avoid over/underflow. 
//! Note: lhs and rhs are considered unsigned here.
template<typename NumericType1, typename NumericType2>
BOOST_TYPEOF_TPL( NumericType1() / NumericType2() ) safe_division( const NumericType1& lhs, const NumericType2& rhs )
{
    typedef BOOST_TYPEOF_TPL( NumericType1() / NumericType2() ) result_type;

    NumericType2 one2 = construction_traits< NumericType2 >::construct(1);

    // Avoid overflow.
    if( rhs < one2 && 
        lhs > rhs * (std::numeric_limits<NumericType2>::max)() )
    {
        return (std::numeric_limits<result_type>::max)();
    }

    // Avoid underflow.
    if( lhs == numeric_traits< NumericType1 >::zero ||
        ( rhs > one2 &&
          lhs < rhs * (std::numeric_limits<NumericType2>::min)() ) )
    {
        return numeric_traits<result_type>::zero;
    }

    return lhs/rhs;
}

//! \brief Check if a number equals zero to within a tolerance range (i.e. 0 +/- tolerance).
template<typename NumberType, typename ToleranceType>
bool equals_zero( const NumberType& v, const ToleranceType& tolerance )
{
    return absolute_value( v ) < construction_traits< NumberType >::construct( absolute_value( tolerance ) );
}

//! \brief A predicate type to compare if two numbers are equal to within a fuzzy tolerance bound.
template <typename ToleranceType>
class equals_within_tolerance_predicate
{
public:

    explicit equals_within_tolerance_predicate( const ToleranceType& tolerance, 
                                                boost::test_tools::floating_point_comparison_type fpc_type = boost::test_tools::FPC_STRONG ) 
    : m_fractionTolerance( absolute_value( tolerance ) )
    , m_fpcType( fpc_type == boost::test_tools::FPC_STRONG )
    {}

    template <typename NumericType1, typename NumericType2>
    bool operator()( const NumericType1& lhs, const NumericType2& rhs ) const
    {
        BOOST_AUTO( diff, absolute_value( lhs - rhs ) );
        BOOST_AUTO( d1, safe_division( diff, absolute_value( rhs ) ) );
        BOOST_AUTO( d2, safe_division( diff, absolute_value( lhs ) ) );

        return m_fpcType ? (d1 <= m_fractionTolerance && d2 <= m_fractionTolerance) 
                         : (d1 <= m_fractionTolerance || d2 <= m_fractionTolerance);
    }

private:

    ToleranceType  m_fractionTolerance;
    bool           m_fpcType;
};

//! \brief Function to check if two numbers are equal to within a specified tolerance.
template<typename NumericType1, typename NumericType2, typename ToleranceType>
bool equals_within_tolerance( const NumericType1& lhs, 
                              const NumericType2& rhs,
                              const ToleranceType& tolerance, 
                              boost::test_tools::floating_point_comparison_type fpc_type = boost::test_tools::FPC_STRONG )
{
    equals_within_tolerance_predicate<ToleranceType> pred( tolerance, fpc_type );
    return pred( lhs, rhs );
}

}}}//namespace generative::numeric::geometry

#endif //GENERATIVE_GEOMETRY_DETAIL_FLOATING_POINT_COMPARISON_HPP
