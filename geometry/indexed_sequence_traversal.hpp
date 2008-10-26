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
#include "detail/indexed_access_fusion_adaptor.hpp"
#include <boost/fusion/functional/adapter/fused_procedure.hpp>

//! \brief An indexed_sequence traversal type which dispatches the traversal based on the traits of the sequence.
struct indexed_sequence_traversal
{
    //! \brief Perform a compile-time traversal of the sequence.
    template <typename IndexedSequence, typename Function>
    static inline void for_each( IndexedSequence& sequence, Function& f, typename boost::enable_if_c< indexed_access_traits< IndexedSequence >::access_type::value == use_compile_time_access::value >::type* dummy = 0 )
    {
        boost::fusion::for_each( indexed_access_fusion_adaptor< IndexedSequence >( sequence ), f );
    }

    //! \brief Perform a run-time traversal of the sequence.
    template <typename IndexedSequence, typename Function>
    static inline void for_each( IndexedSequence& sequence, Function& f, typename boost::disable_if_c< indexed_access_traits< IndexedSequence >::access_type::value == use_compile_time_access::value >::type* dummy = 0 )
    {
        for( size_t i=0; i < indexed_access_traits< IndexedSequence >::dimension_type::value; ++i )
        {
            f( indexed_access_traits< IndexedSequence >::get( sequence, i ) );
        }
    }

    //! \brief Perform a compile-time traversal of two sequences.
    //* If either sequence uses compile-time access then both are put into a boost::fusion::zip_view and the operation is applied as a boost::fusion::fused_procedure.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Function>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Function& f,
        typename boost::enable_if_c< indexed_access_traits< IndexedSequence1 >::access_type::value == use_compile_time_access::value ||
                                     indexed_access_traits< IndexedSequence2 >::access_type::value == use_compile_time_access::value >::type* dummy2 = 0 )
    {
        typedef indexed_access_fusion_adaptor< IndexedSequence1 > indexed_sequence_1;
        typedef indexed_access_fusion_adaptor< IndexedSequence2 > indexed_sequence_2;
        typedef boost::fusion::vector<indexed_sequence_1&, indexed_sequence_2&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( indexed_sequence_1( sequence1 ), indexed_sequence_2( sequence2 ) ) ), boost::fusion::fused_procedure<Function>( f ) );
    }

    //! \brief Perform a run-time traversal of two sequences.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Function>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Function& f,
                                 typename boost::disable_if_c< indexed_access_traits< IndexedSequence1 >::access_type::value == use_compile_time_access::value >::type* dummy1 = 0,
                                 typename boost::disable_if_c< indexed_access_traits< IndexedSequence2 >::access_type::value == use_compile_time_access::value >::type* dummy2 = 0 )
    {
        for( size_t i=0; i < indexed_access_traits< IndexedSequence1 >::dimension_type::value; ++i )
        {            
            f( indexed_access_traits< IndexedSequence1 >::get( sequence1, i ), indexed_access_traits< IndexedSequence2 >::get( sequence2, i ) );
        }
    }
};

#endif //_BOOST_GEOMETRY_INDEXED_SEQUENCE_TRAVERSAL_HPP

