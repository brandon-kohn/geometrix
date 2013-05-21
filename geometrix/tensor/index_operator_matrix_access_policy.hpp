//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_INDEX_OPERATOR_MATRIX_ACCESS_POLICY_HPP
#define GEOMETRIX_INDEX_OPERATOR_MATRIX_ACCESS_POLICY_HPP

#include <geometrix/tensor/tensor_access_policy.hpp>
#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/fusion_tensor_sequence_adaptor.hpp>

namespace geometrix {

template <typename Matrix>
struct index_operator_matrix_access_policy
{
    typedef void compile_time_access;

    template <unsigned int Row, unsigned int Column>
    struct type_at
    {
        BOOST_MPL_ASSERT_MSG
        (
            ( row_dimension_of< Matrix >::value > Row && column_dimension_of< Matrix >::value > Column )
          , MATRIX_INDICES_ARE_OUT_OF_BOUNDS
          , (type_at<Row,Column>)
        );

        typedef typename remove_const_ref<Matrix>::type matrix_type;        
        typedef typename row<matrix_type,Row>::template type_at<0>::type type;        
    };

    template <unsigned int Row, unsigned int Column>
    static typename type_at<Row, Column>::type get( const Matrix& matrix ) 
    {
        return matrix[Row][Column];
    }

    template <unsigned int Row, unsigned int Column>
    static void set( Matrix& matrix, const typename type_at<Row, Column>::type& v ) 
    {
        matrix[Row][Column] = v;
    }
};

}//namespace geometrix;

#endif//GEOMETRIX_INDEX_OPERATOR_MATRIX_ACCESS_POLICY_HPP
