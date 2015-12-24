//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_HPP
#define GEOMETRIX_MATRIX_HPP

#if !defined( GEOMETRIX_MAX_MATRIX_ROWS )
    #define GEOMETRIX_MAX_MATRIX_ROWS 16
#endif
#if !defined( GEOMETRIX_MAX_MATRIX_COLUMNS )
    #define GEOMETRIX_MAX_MATRIX_COLUMNS 16
#endif

#include <geometrix/tensor/detail/matrix_base.hpp>
#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/index_operator_matrix_access_policy.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/tensor/detail/matrix.hpp>

#include <boost/array.hpp>

namespace geometrix {

    template <typename T, std::size_t Rows, std::size_t Columns>
    struct construction_policy< matrix< T, Rows, Columns> >
    {    
        typedef matrix<T, Rows, Columns> result_type;

        template <typename Matrix>                                                                         
        static result_type construct( const Matrix& m )
        {                                                                                                           
            return detail::matrix_pod_constructor<Rows,Columns>::template construct<result_type>( m );
        }
    };

    template <typename T, std::size_t Rows, std::size_t Columns>
    struct geometric_traits< matrix<T,Rows,Columns> >  
    {
        typedef dimension<Rows>    row_dimension;
        typedef dimension<Columns> col_dimension;
        typedef void               is_matrix;
        typedef void               is_homogeneous;
    };

    template <typename T, std::size_t Rows, std::size_t Columns, std::size_t Row>
    struct geometric_traits< row<matrix<T,Rows,Columns>,Row> >                                                    
    {
        typedef boost::mpl::vector<T> storage_types;
        typedef dimension<Columns>    dimension_type;
        typedef void                  is_sequence;
        typedef void                  is_numeric_sequence;
        typedef void                  is_vector;
        typedef void                  is_homogeneous;
    };                                                                                                                                                      

    template <typename T, std::size_t Rows, std::size_t Columns, std::size_t Column>
    struct geometric_traits< column<matrix<T,Rows,Columns>,Column> >                                                    
    {
        typedef boost::mpl::vector<T> storage_types;
        typedef dimension<Rows>       dimension_type;      
        typedef void                  is_sequence;         
        typedef void                  is_numeric_sequence; 
        typedef void                  is_vector;           
        typedef void                  is_homogeneous;                                                                                                            
    }; 

    template <typename T, std::size_t Rows, std::size_t Columns>
    struct tensor_traits< matrix<T, Rows, Columns> >
    {
        typedef index_operator_matrix_access_policy< matrix<T,Rows,Columns> > access_policy;
        typedef boost::mpl::int_<2>                                           tensor_order;
        typedef void                                                          rank_2;
        typedef void                                                          is_tensor;
    };

}//namespace geometrix;

#endif//GEOMETRIX_MATRIX_HPP

