//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TENSOR_IDENTITY_MATRIX_HPP
#define GEOMETRIX_TENSOR_IDENTITY_MATRIX_HPP

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <type_traits>

namespace geometrix {
        
    template <typename T, std::size_t Rows, std::size_t Columns>
    struct identity_matrix
    {
        using element_type = T;
    private:
        struct index_operator_helper
        {
            index_operator_helper(std::size_t i) : i(i){}
            
            element_type operator[](std::size_t j) const
            {
                return (i != j) ? constants::zero<element_type>() : constants::one<element_type>();
            }
            
            std::size_t i;
        };

    public:
        
        index_operator_helper operator[]( std::size_t i ) const { return index_operator_helper(i); }
        
        template <int Row, int Column>        
        element_type get(typename std::enable_if<Row != Column>::type* = nullptr) const { return constants::zero<element_type>(); }
        
        template <int Row, int Column>
        element_type get(typename std::enable_if<Row == Column>::type* = nullptr) const { return constants::one<element_type>(); }
        
        element_type get( int Row, int Column ) const 
        {
            return (Row != Column) ? constants::zero<element_type>() : constants::one<element_type>();
        }
    };

    #define GEOMETRIX_GENERATE_IDENTITY_MATRIX_ELEMENT_INDEX_PAIR_( z, col, data )                                                   \
        BOOST_PP_COMMA_IF(col)                                                                                                       \
        BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_ELEM(0,data), col), geometrix::constants::one<T>(), geometrix::constants::zero<T>()) \
    /***/

    #define GEOMETRIX_GENERATE_IDENTITY_MATRIX_ELEMENTS_INDICES_( z, n, data )                                         \
        BOOST_PP_COMMA_IF(n)                                                                                           \
        {BOOST_PP_REPEAT_##z( BOOST_PP_SEQ_ELEM(0,data), GEOMETRIX_GENERATE_IDENTITY_MATRIX_ELEMENT_INDEX_PAIR_, (n) )}\
    /***/

    #define GEOMETRIX_ENUM_IDENTITY_MATRIX_SEQUENCE( M, N )                             \
        BOOST_PP_REPEAT( M, GEOMETRIX_GENERATE_IDENTITY_MATRIX_ELEMENTS_INDICES_, (N) ) \
    /***/

}//! namespace geometrix;

#endif GEOMETRIX_TENSOR_IDENTITY_MATRIX_HPP
