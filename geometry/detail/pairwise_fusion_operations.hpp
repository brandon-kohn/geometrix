//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_DETAIL_PAIRWISE_FUSION_OPERATIONS_HPP
#define _BOOST_GEOMETRY_DETAIL_PAIRWISE_FUSION_OPERATIONS_HPP
#pragma once

#include <boost/range.hpp>
#include "indexed_access_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{  
    namespace detail
    {
        //! Policy to return the argument.
        struct ReflexiveReturnPolicy
        {
            template <typename T>
            T& operator()( T& t ) const { return t; }
        };

        //! A struct to help apply binary operations such as +- on two arrays of objects via the boost::fusion::replace algorithm.
        template <typename Sequence, typename Operator, typename Enable = void>
        struct pairwise_binary_operation
        {};

        template <typename Sequence, typename Operator>
        struct pairwise_binary_operation<Sequence, Operator, typename boost::disable_if< boost::numeric::geometry::is_indexed_sequence< Sequence > >::type >
        {           
            typedef Operator                                                 operation_type;            
            typedef typename Sequence                                        range_type;
            typedef typename boost::range_const_iterator< range_type >::type const_iterator;

            pairwise_binary_operation( const range_type& range, const operation_type& operation = operation_type() )
                : m_it( boost::begin( range ) )
                , m_end( boost::end( range ) )
                , m_operation( operation )
            {}

            template <typename T>
            inline void operator()( T& t ) const
            {                
                if( m_it != m_end )
                    t = m_operation( t, *m_it++ );
            }

            operation_type m_operation;
            mutable const_iterator m_it;
            const_iterator m_end;
        };

        //! A struct to help apply binary operations such as +- on two arrays of objects via the boost::fusion::replace algorithm.
        template <typename Sequence, typename Operator>
        struct pairwise_binary_operation<Sequence, Operator, typename boost::enable_if< boost::numeric::geometry::is_indexed_sequence< Sequence > >::type >
        {
            typedef Operator                                     operation_type;            
            typedef typename Sequence                            sequence;
            typedef typename size_t                              const_iterator;

            pairwise_binary_operation( const sequence& range, const operation_type& operation = operation_type() )
                : m_it( 0 )
                , m_end( indexed_access_traits< Sequence >::dimension_type::value )
                , m_operation( operation )
                , m_sequence( range )
            {}

            template <typename T>
            inline void operator()( T& t ) const
            {                
                if( m_it != m_end )
                {
                    t = m_operation( t, indexed_access_traits< Sequence >::get( m_sequence, m_it++ ) );
                }
            }

            operation_type         m_operation;
            mutable const_iterator m_it;
            const_iterator         m_end;
            const Sequence&        m_sequence;

        };

        template <typename T, typename Operator>
        struct assign_operation_result
        {
            typedef Operator                                                         operation_type;            
            
            assign_operation_result( const T& t, const operation_type& operation = operation_type() )
                : m_operation( operation )
                , m_rhs( t )
            {}

            template <typename T>
            inline void operator()( T& t ) const
            {                
                t = m_operation( t, m_rhs );
            }

            operation_type m_operation;            
            T m_rhs;

        };
    }

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_DETAIL_PAIRWISE_FUSION_OPERATIONS_HPP
