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
};

#endif //_BOOST_GEOMETRY_INDEXED_SEQUENCE_TRAVERSAL_HPP

