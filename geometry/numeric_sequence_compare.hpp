//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_NUMERIC_SEQUENCE_COMPARE_HPP
#define GENERATIVE_GEOMETRY_NUMERIC_SEQUENCE_COMPARE_HPP
#pragma once

#include "numeric_sequence_traits.hpp"
#include <boost/mpl/or.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
namespace detail
{
    template <typename NumericSequence, typename Enable = void>
    struct numeric_sequence_compare
    {};

    template <typename NumericSequence>
    struct numeric_sequence_compare<NumericSequence, typename boost::enable_if< typename should_use_compile_time_access1< NumericSequence >::type >::type >
    {
        template <typename NumericSequence, unsigned int D>
        struct dimension_processor
        {
            template <typename Predicate>
            static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const Predicate& nCompare )
            {   
                typedef indexed_access_traits< NumericSequence > access;                    
                if( !nCompare( access::get<D>( lhs ), access::get<D>( rhs ) ) )
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
                typedef indexed_access_traits< NumericSequence > access;                    
                if( !nCompare( access::get<0>( lhs ), access::get<0>( rhs ) ) )
                    return false;
                else
                    return true;
            }
        };
        
        template <typename Predicate>
        static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const Predicate& nCompare )
        {
            return dimension_processor<NumericSequence, numeric_sequence_traits<NumericSequence>::dimension_type::value - 1>::compare( lhs, rhs, nCompare );
        }
    };

    template <typename NumericSequence>
    struct numeric_sequence_compare<NumericSequence, typename boost::enable_if< typename should_use_run_time_access1< NumericSequence >::type >::type >
    {
        template <typename Predicate>
        static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const Predicate& nCompare )
        {
            typedef indexed_access_traits< NumericSequence > access;
            for( size_t i=0;i < access::dimension_type::value; ++i )
            {
                if( !nCompare( access::get( lhs, i ), access::get( rhs, i ) ) )
                    return false;
            }

            return true;//all in range.
        }
    };

}//detail

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence>
bool numeric_sequence_equals( const NumericSequence& A,
                                     const NumericSequence& B,
                                     const NumberComparisonPolicy& compare,
                                     typename boost::enable_if<
                                        boost::is_same<
                                            typename numeric_sequence_traits<NumericSequence>::dimension_type,
                                            dimension_traits<2>
                                        >
                                     >::type* = 0 )
{        
    typedef indexed_access_traits< NumericSequence > access_traits;                
    return compare.equals( access_traits::get<0>( A ), access_traits::get<0>( B ) ) &&
           compare.equals( access_traits::get<1>( A ), access_traits::get<1>( B ) );
}

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence>
bool numeric_sequence_equals( const NumericSequence& A,
                                     const NumericSequence& B,
                                     const NumberComparisonPolicy& compare,
                                     typename boost::enable_if<
                                        boost::is_same<
                                            typename numeric_sequence_traits<NumericSequence>::dimension_type,
                                            dimension_traits<3>
                                        >
                                     >::type* = 0 )
{        
    typedef indexed_access_traits< NumericSequence > access_traits;                
    return compare.equals( access_traits::get<0>( A ), access_traits::get<0>( B ) ) && 
        compare.equals( access_traits::get<1>( A ), access_traits::get<1>( B ) ) && 
        compare.equals( access_traits::get<2>( A ), access_traits::get<2>( B ) );
}

//! Function to determine if two numeric_sequences are equal to within tolerance.
template <typename NumberComparisonPolicy, typename NumericSequence>
bool numeric_sequence_equals( const NumericSequence& A,
                                     const NumericSequence& B,
                                     const NumberComparisonPolicy& compare,
                                     typename boost::disable_if<
                                        boost::mpl::or_<
                                            boost::is_same<
                                                typename numeric_sequence_traits<NumericSequence>::dimension_type,
                                                dimension_traits<2>
                                            >,
                                            boost::is_same<
                                                typename numeric_sequence_traits<NumericSequence>::dimension_type,
                                                dimension_traits<3>
                                            >
                                        >
                                     >::type* = 0 )
{        
    typedef indexed_access_traits< NumericSequence > access_traits;            
    equals<NumberComparisonPolicy> nCompare( compare );
    return numeric_sequence_compare< NumericSequence >::compare( A, B, nCompare );
}

}}}//namespace generative::numeric::geometry

#endif //GENERATIVE_GEOMETRY_NUMERIC_SEQUENCE_COMPARE_HPP
