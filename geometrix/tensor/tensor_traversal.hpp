//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TENSOR_TRAVERSAL_HPP
#define GEOMETRIX_TENSOR_TRAVERSAL_HPP

#include <geometrix/tensor/tensor_traits.hpp>

#include <boost/fusion/functional/adapter/fused_procedure.hpp>
#include <boost/utility.hpp>

namespace geometrix {

//! \brief An tensor traversal type which dispatches the traversal based on the traits of the sequence.

//! tensor_traversal provides a clean interface for performing iterations across types which model the IndexedSequenceConcept. 
//! The traversals support both compile-time and run-time access and are automatically deduced from the traits of the sequence types.
//! Example use:
//! \code
//! using namespace geometrix;
//! using namespace generative::lambda;
//! typedef point< double, 3 > point_3;
//! typedef vector< double, 3 > vector_3;
//! 
//! point_3 a( 1., 2., 0. );
//! vector_3 b( 1., 2., 0. );
//! 
//! //! Add vector b to point a.
//! tensor_traversal::for_each( a, b, _1 += _2 );
//! \endcode
struct tensor_traversal
{
    //! Perform a compile-time traversal of a sequence using
    //! boost::fusion::for_each and affecting one call to Function f for each 
    //! element.
    template <typename Tensor, typename Function>
    static void for_each( Tensor& sequence,
                          Function& f,
                          typename boost::enable_if< is_compile_time< Tensor > >::type* = 0 )
    {
        boost::fusion::for_each( sequence, f );
    }

    //! Perform a run-time traversal of a sequence using a traditional for loop
    //! and affecting one call to Function f for each element.
    template <typename Tensor, typename Function>
    static void for_each( Tensor& sequence,
                          Function& f,
                          typename boost::enable_if< is_run_time< Tensor > >::type* = 0 )
    {
        for( std::size_t i=0; i < geometric_traits< typename remove_const_ref<Tensor>::type >::dimension_type::value; ++i )
        {
            f( get( sequence, i ) );
        }
    }

    //! If either sequence uses compile-time access then both are put into a 
    //! boost::fusion::zip_view and the operation is applied as a
    //! boost::fusion::fused_procedure. Then a compile-time traversal of the
    //! zipped sequence is done using boost::fusion::for_each and affecting 
    //! one call to Function f for each element.
    template <typename Tensor1, typename Tensor2, typename Function>
    static void for_each( Tensor1& sequence1,
                          Tensor2& sequence2,
                          Function& f,
                          typename boost::enable_if
                          <
                              boost::mpl::and_
                              <
                                  is_compile_time<Tensor1>
                                , is_compile_time<Tensor2>
                              >
                          >::type* dummy2 = 0 )
    {
        typedef boost::fusion::vector<Tensor1&, Tensor2&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( sequence1, sequence2 ) ),
                                 boost::fusion::fused_procedure<Function>( f ) );
    }

    //! A run-time traversal is made over the cardinality of the first sequence using
    //! a traditional for loop and affecting one call to Function f (A binary operation)
    //! on the pair of corresponding elements from each sequence.
    template <typename Tensor1, typename Tensor2, typename Function>
    static void for_each( Tensor1& sequence1, 
                          Tensor2& sequence2,
                          Function& f,
                          typename boost::enable_if
                          <
                              boost::mpl::and_
                              <
                                  is_run_time<Tensor1>
                                , is_run_time<Tensor2>
                              >
                          >::type* dummy2 = 0 )
    {
        for( std::size_t i=0; i < geometric_traits< typename remove_const_ref<Tensor>::type >::dimension_type::value; ++i )
        {            
            f( get( sequence1, i ),
               get( sequence2, i ) );
        }
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_TENSOR_TRAVERSAL_HPP
