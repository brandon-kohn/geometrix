//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_INDEXED_SEQUENCE_TRAVERSAL_HPP
#define _BOOST_GEOMETRY_INDEXED_SEQUENCE_TRAVERSAL_HPP
#pragma once

#include "indexed_access_traits.hpp"
#include "indexed_access_fusion.hpp"
#include <boost/fusion/functional/adapter/fused_procedure.hpp>

//! \brief An indexed_sequence traversal type which dispatches the traversal based on the traits of the sequence.
struct indexed_sequence_traversal
{
    //! \brief Perform a compile-time traversal of the sequence.
    template <typename IndexedSequence, typename Visitor>
    static inline void for_each( IndexedSequence& sequence, Visitor& visitor, typename boost::enable_if< typename indexed_access_traits< IndexedSequence >::has_compile_time_access_type >::type* dummy = 0 )
    {
        boost::fusion::for_each( indexed_access_adaptor< IndexedSequence >( sequence ), visitor );
    }

    //! \brief Perform a run-time traversal of the sequence.
    template <typename IndexedSequence, typename Visitor>
    static inline void for_each( IndexedSequence& sequence, Visitor& visitor, typename boost::disable_if< typename indexed_access_traits< IndexedSequence >::has_compile_time_access_type >::type* dummy = 0 )
    {
        for( size_t i=0; i < indexed_access_traits< IndexedSequence >::dimension_type::value; ++i )
        {
            visitor( indexed_access_traits< IndexedSequence >::get( sequence, i ) );
        }
    }

    //! \brief Perform a compile-time traversal of two sequences.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Visitor>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Visitor& visitor,
        typename boost::enable_if_c< indexed_access_traits< IndexedSequence1 >::has_compile_time_access_type::value ||
                                     indexed_access_traits< IndexedSequence2 >::has_compile_time_access_type::value >::type* dummy2 = 0 )
    {
        typedef indexed_access_adaptor< IndexedSequence1 > indexed_sequence_1;
        typedef indexed_access_adaptor< IndexedSequence2 > indexed_sequence_2;
        typedef boost::fusion::vector<indexed_sequence_1&, indexed_sequence_2&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( indexed_sequence_1( sequence1 ), indexed_sequence_2( sequence2 ) ) ), boost::fusion::fused_procedure<Visitor>( visitor ) );
    }

    //! \brief Perform a run-time traversal of two sequences.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Visitor>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Visitor& visitor,
                                 typename boost::disable_if< typename indexed_access_traits< IndexedSequence1 >::has_compile_time_access_type >::type* dummy1 = 0,
                                 typename boost::disable_if< typename indexed_access_traits< IndexedSequence2 >::has_compile_time_access_type >::type* dummy2 = 0 )
    {
        for( size_t i=0; i < indexed_access_traits< IndexedSequence1 >::dimension_type::value; ++i )
        {            
            visitor( indexed_access_traits< IndexedSequence1 >::get( sequence1, i ), indexed_access_traits< IndexedSequence2 >::get( sequence2, i ) );
        }
    }

    //! \brief Perform a traversal of two sequences which have mixed access traits.
    /*
    template <typename IndexedSequence1, typename IndexedSequence2, typename Visitor>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Visitor& visitor,
                                 typename boost::enable_if< typename indexed_access_traits< IndexedSequence1 >::has_compile_time_access_type >::type* dummy1 = 0,
                                 typename boost::disable_if< typename indexed_access_traits< IndexedSequence2 >::has_compile_time_access_type >::type* dummy2 = 0 )
    {
        typedef indexed_access_adaptor< IndexedSequence1 > indexed_sequence_1;
        typedef indexed_access_adaptor< IndexedSequence2 > indexed_sequence_2;
        typedef boost::fusion::vector<indexed_sequence_1&, indexed_sequence_2&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( indexed_sequence_1( sequence1 ), indexed_sequence_2( sequence2 ) ) ), visitor );
    }

    //! \brief Perform a traversal of two sequences which have mixed access traits.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Visitor>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Visitor& visitor,
                                 typename boost::disable_if< typename indexed_access_traits< IndexedSequence1 >::has_compile_time_access_type >::type* dummy1 = 0,
                                 typename boost::enable_if< typename indexed_access_traits< IndexedSequence2 >::has_compile_time_access_type >::type* dummy2 = 0 )
    {
        typedef indexed_access_adaptor< IndexedSequence1 > indexed_sequence_1;
        typedef indexed_access_adaptor< IndexedSequence2 > indexed_sequence_2;
        typedef boost::fusion::vector<indexed_sequence_1&, indexed_sequence_2&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( indexed_sequence_1( sequence1 ), indexed_sequence_2( sequence2 ) ) ), visitor );
    }
    */
};

#endif //_BOOST_GEOMETRY_INDEXED_SEQUENCE_TRAVERSAL_HPP

