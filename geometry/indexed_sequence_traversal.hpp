//
//! Copyright © 2008
//! Brandon Kohn
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
#include <boost/utility.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief An indexed_sequence traversal type which dispatches the traversal based on the traits of the sequence.

//! indexed_sequence_traversal provides a clean interface for performing iterations across types which model the IndexedSequenceConcept. 
//! The traversals support both compile-time and run-time access and are automatically deduced from the traits of the sequence types.
//! Example use:
//! \code
//! using namespace boost::numeric::geometry;
//! using namespace boost::lambda;
//! typedef point< double, 3 > point_3;
//! typedef vector< double, 3 > vector_3;
//! 
//! point_3 a( 1., 2., 0. );
//! vector_3 b( 1., 2., 0. );
//! 
//! //! Add vector b to point a.
//! indexed_sequence_traversal::for_each( a, b, _1 += _2 );
//! \endcode
struct indexed_sequence_traversal
{
    //! Perform a compile-time traversal of a sequence using boost::fusion::for_each and affecting one call to Function f for each element.
    template <typename IndexedSequence, typename Function>
    static inline void for_each( IndexedSequence& sequence, Function& f, typename boost::enable_if< typename should_use_compile_time_access1< IndexedSequence >::type >::type* dummy = 0 )
    {
        boost::fusion::for_each( indexed_access_fusion_adaptor< IndexedSequence >( sequence ), f );
    }

    //! Perform a run-time traversal of a sequence using a traditional for loop and affecting one call to Function f for each element.
    template <typename IndexedSequence, typename Function>
    static inline void for_each( IndexedSequence& sequence, Function& f, typename boost::enable_if< typename should_use_run_time_access1< IndexedSequence >::type >::type* dummy = 0 )
    {
        for( size_t i=0; i < indexed_access_traits< IndexedSequence >::dimension_type::value; ++i )
        {
            f( indexed_access_traits< IndexedSequence >::get( sequence, i ) );
        }
    }

    //! If either sequence uses compile-time access then both are put into a boost::fusion::zip_view and the operation is applied as a boost::fusion::fused_procedure.
    //! Then a compile-time traversal of the zipped sequence is done using boost::fusion::for_each and affecting one call to Function f for each element.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Function>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Function& f,
                                 typename boost::enable_if< typename should_use_compile_time_access2< IndexedSequence1, IndexedSequence2 >::type >::type* dummy2 = 0 )
    {
        typedef indexed_access_fusion_adaptor< IndexedSequence1 > indexed_sequence_1;
        typedef indexed_access_fusion_adaptor< IndexedSequence2 > indexed_sequence_2;
        typedef boost::fusion::vector<indexed_sequence_1&, indexed_sequence_2&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( indexed_sequence_1( sequence1 ), indexed_sequence_2( sequence2 ) ) ), boost::fusion::fused_procedure<Function>( f ) );
    }

    //! A run-time traversal is made over the cardinality of the first sequence using a traditional for loop and affecting one call to Function f (A binary operation) on the pair of corresponding elements from each sequence.
    template <typename IndexedSequence1, typename IndexedSequence2, typename Function>
    static inline void for_each( IndexedSequence1& sequence1, IndexedSequence2& sequence2, Function& f,
                                 typename boost::enable_if< typename should_use_run_time_access2< IndexedSequence1, IndexedSequence2 >::type >::type* dummy2 = 0 )
    {
        for( size_t i=0; i < indexed_access_traits< IndexedSequence1 >::dimension_type::value; ++i )
        {            
            f( indexed_access_traits< IndexedSequence1 >::get( sequence1, i ), indexed_access_traits< IndexedSequence2 >::get( sequence2, i ) );
        }
    }
};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_INDEXED_SEQUENCE_TRAVERSAL_HPP
