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

#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/tensor_traits.hpp>

#include <boost/mpl/or.hpp>

namespace geometrix {
namespace detail
{
    template <typename NumericSequence, typename Enable = void>
    struct numeric_sequence_compare
    {};

    template <typename NumericSequence>
    struct numeric_sequence_compare<NumericSequence, typename tensor_traits<typename remove_const_ref<NumericSequence>::type>::access_policy::compile_time_access>
    {
        template <typename NumericSequence, unsigned int D>
        struct dimension_processor
        {
            template <typename Predicate>
            static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const Predicate& nCompare )
            {   
                if( !nCompare( get<D>( lhs ), get<D>( rhs ) ) )
                    return false;
                return dimension_processor<NumericSequence, D-1>::compare( lhs, rhs, nCompare );
            }
        };

        template <typename NumericSequence>
        struct dimension_processor<NumericSequence, 0>
        {
            template <typename Predicate>
            static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const Predicate& nCompare )
            {   
                if( !nCompare( get<0>( lhs ), get<0>( rhs ) ) )
                    return false;
                else
                    return true;
            }
        };
        
        template <typename Predicate>
        static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const Predicate& nCompare )
        {
            return dimension_processor<NumericSequence, geometric_traits<typename remove_const_ref<NumericSequence>::type>::dimension_type::value - 1>::compare( lhs, rhs, nCompare );
        }
    };

    template <typename NumericSequence>
    struct numeric_sequence_compare<NumericSequence, typename tensor_traits<NumericSequence>::access_policy::run_time_access>
    {
        template <typename Predicate>
        static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const Predicate& nCompare )
        {
            for( std::size_t i=0;i < geometric_traits<typename remove_const_ref<NumericSequence>::type>::dimension_type::value; ++i )
            {
                if( !nCompare( get( lhs, i ), get( rhs, i ) ) )
                    return false;
            }

            return true;//all in range.
        }
    };

}//detail

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence>
bool numeric_sequence_equals_2D( const NumericSequence& A,
                                 const NumericSequence& B,
                                 const NumberComparisonPolicy& compare )
{        
    return compare.equals( get<0>( A ), get<0>( B ) ) &&
           compare.equals( get<1>( A ), get<1>( B ) );
}

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence>
bool numeric_sequence_equals_3D( const NumericSequence& A,
                                 const NumericSequence& B,
                                 const NumberComparisonPolicy& compare )
{        
    return compare.equals( get<0>( A ), get<0>( B ) ) && 
           compare.equals( get<1>( A ), get<1>( B ) ) && 
           compare.equals( get<2>( A ), get<2>( B ) );
}

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence>
bool numeric_sequence_equals( const NumericSequence& A,
                              const NumericSequence& B,
                              const NumberComparisonPolicy& compare )
{        
    equals<NumberComparisonPolicy> nCompare( compare );
    return detail::numeric_sequence_compare< NumericSequence >::compare( A, B, nCompare );
}

}//namespace geometrix;

#endif //GEOMETRIX_NUMERIC_SEQUENCE_COMPARE_HPP
