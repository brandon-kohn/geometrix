//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_SEQUENCE_COMPARE_HPP
#define GEOMETRIX_NUMERIC_SEQUENCE_COMPARE_HPP
#pragma once

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/tensor_traits.hpp>

#include <boost/mpl/or.hpp>

namespace geometrix {
namespace detail
{
	template <typename NumericSequence, std::size_t D>
    struct dimension_processor
    {
        template <typename Predicate, typename NumericSequence2>
        static bool compare( const NumericSequence& lhs, const NumericSequence2& rhs, const Predicate& nCompare )
        {   
            if( !nCompare( get<D>( lhs ), get<D>( rhs ) ) )
                return false;
            return dimension_processor<NumericSequence, D-1>::compare( lhs, rhs, nCompare );
        }
    };

    template <typename NumericSequence>
    struct dimension_processor<NumericSequence, 0>
    {
        template <typename Predicate, typename NumericSequence2>
        static bool compare( const NumericSequence& lhs, const NumericSequence2& rhs, const Predicate& nCompare )
        {   
            if( !nCompare( get<0>( lhs ), get<0>( rhs ) ) )
                return false;
            else
                return true;
        }
    };

    template <typename NumericSequence1, typename NumericSequence2, typename Enable = void>
    struct numeric_sequence_compare
    {   
        template <typename Predicate>
        static bool compare( const NumericSequence1& lhs, const NumericSequence2& rhs, const Predicate& nCompare )
        {
            return dimension_processor<NumericSequence1, geometric_traits<typename remove_const_ref<NumericSequence1>::type>::dimension_type::value - 1>::compare( lhs, rhs, nCompare );
        }
    };
    
}//detail

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence1, typename NumericSequence2>
bool numeric_sequence_equals_2d( const NumericSequence1& A,
                                 const NumericSequence2& B,
                                 const NumberComparisonPolicy& compare )
{        
    return compare.equals( get<0>( A ), get<0>( B ) ) &&
           compare.equals( get<1>( A ), get<1>( B ) );
}

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence1, typename NumericSequence2>
bool numeric_sequence_equals_3d( const NumericSequence1& A,
                                 const NumericSequence2& B,
                                 const NumberComparisonPolicy& compare )
{        
    return compare.equals( get<0>( A ), get<0>( B ) ) && 
           compare.equals( get<1>( A ), get<1>( B ) ) && 
           compare.equals( get<2>( A ), get<2>( B ) );
}

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence1, typename NumericSequence2>
bool numeric_sequence_equals( const NumericSequence1& A,
                              const NumericSequence2& B,
                              const NumberComparisonPolicy& compare )
{        
    equals<NumberComparisonPolicy> nCompare( compare );
    return detail::numeric_sequence_compare< NumericSequence1, NumericSequence2 >::compare( A, B, nCompare );
}

}//namespace geometrix;

#endif //GEOMETRIX_NUMERIC_SEQUENCE_COMPARE_HPP
