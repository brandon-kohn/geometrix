//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, mersion 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_MINOR_ADAPTOR_HPP
#define GEOMETRIX_MATRIX_MINOR_ADAPTOR_HPP

#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <boost/mpl/erase.hpp>

namespace geometrix {

    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn>
    struct matrix_minor
    {
        explicit matrix_minor( const Matrix& m )
            : m(m)
        {}

        const Matrix& m;
    };

    template <unsigned int ExcludedRow, unsigned int ExcludedColumn, typename Matrix>
    inline matrix_minor< Matrix, ExcludedRow, ExcludedColumn > as_matrix_minor( const Matrix& m )
    {
        return matrix_minor<Matrix, ExcludedRow, ExcludedColumn>(m);
    }
        
    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn>
    struct geometric_traits< matrix_minor< Matrix, ExcludedRow, ExcludedColumn > > 
        : uniformity_base<Matrix>
    {
        typedef matrix_minor< Matrix, ExcludedRow, ExcludedColumn > matrix_type;
        typedef dimension<row_dimension_of< Matrix >::value-1>      row_dimension;
        typedef dimension<column_dimension_of< Matrix >::value-1>   col_dimension;                                                                   
        typedef void                                                is_matrix;
    };

    //! Row/Column
    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn, unsigned int Column> 
    struct column< matrix_minor< Matrix, ExcludedRow, ExcludedColumn >, Column >
    {        
        template <unsigned int Index, unsigned int ExcludedIndex>
        struct offset_index : boost::mpl::int_<Index + (Index>=ExcludedIndex)>
        {};

        column( const matrix_minor< Matrix, ExcludedRow, ExcludedColumn >& m )
            : m(m.m)
        {};

        template <unsigned int Row>
        struct type_at
            : boost::mpl::eval_if
              <
                  is_homogeneous<Matrix>
                , boost::fusion::result_of::at_c< column<Matrix, offset_index<Column, ExcludedColumn>::value >, 0 >
                , boost::fusion::result_of::at_c<column<Matrix, offset_index<Column, ExcludedColumn>::value>, offset_index<Row, ExcludedRow>::value>
              >
        {};

        template <unsigned int Row>                                                                                                       
        typename type_at<Row>::type get() const  
        {                                                                                                             
            return geometrix::get< offset_index<Row, ExcludedRow>::value, offset_index<Column,ExcludedColumn>::value >( m );                                                                     
        }                                                                                                                                 
                                                                                                                                      
        const Matrix& m;
    };
        
    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn, unsigned int Column >
    struct geometric_traits< column<matrix_minor< Matrix, ExcludedRow, ExcludedColumn >,Column> > 
        : geometric_traits< column<Matrix,Column> >
    {
        typedef dimension<geometric_traits< column<Matrix,Column> >::dimension_type::value - 1> dimension_type;
    };
   
    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn, unsigned int Row> 
    struct row<matrix_minor< Matrix, ExcludedRow, ExcludedColumn >,Row>
    {
        template <unsigned int Index, unsigned int ExcludedIndex>
        struct offset_index : boost::mpl::int_<Index + (Index>=ExcludedIndex)>
        {};

        typedef boost::mpl::int_<Row> index;
        row( const matrix_minor< Matrix, ExcludedRow, ExcludedColumn >& m )
            : m(m.m)
        {};

        template <unsigned int Column>
        struct type_at
            : boost::mpl::eval_if
              <
                  is_homogeneous<Matrix>
                , boost::fusion::result_of::at_c< row<Matrix, offset_index<Row, ExcludedRow>::value>, 0 >
                , boost::fusion::result_of::at_c< row<Matrix, offset_index<Row, ExcludedRow>::value>, offset_index<Column, ExcludedColumn>::value >
              >
        {};
        
        template <unsigned int Column>
        typename type_at<Column>::type get() const
        {
            return geometrix::get<offset_index<Row, ExcludedRow>::value, offset_index<Column, ExcludedColumn>::value>( m );                                           
        }                                                        
                                                                                                                                                      
        const Matrix& m;
    };

    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn, unsigned int Index>
    struct geometric_traits< row<matrix_minor< Matrix, ExcludedRow, ExcludedColumn >,Index> > 
        : geometric_traits< row<Matrix,Index> >
    {
        typedef dimension<geometric_traits< row<Matrix,Index> >::dimension_type::value - 1> dimension_type;        
    };
            
    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn>
    struct matrix_minor_access_policy
    {        
        template <unsigned int Index, unsigned int ExcludedIndex>
        struct offset_index : boost::mpl::int_<Index + (Index>=ExcludedIndex)>
        {};

        template <unsigned int Row, unsigned int Column>
        struct type_at
            : boost::mpl::eval_if
              <
                  is_homogeneous<Matrix>
                , boost::fusion::result_of::at_c< row<Matrix, offset_index<Row, ExcludedRow>::value>, 0 >
                , boost::fusion::result_of::at_c< row<Matrix, offset_index<Row, ExcludedRow>::value>, offset_index<Column, ExcludedColumn>::value >
              >
        {};

        //! \brief compile time access if amailable for the collection.
        template <unsigned int Row, unsigned int Column>
        static typename type_at<Row, Column>::type get( const matrix_minor< Matrix, ExcludedRow, ExcludedColumn >& matrix ) 
        {
            return geometrix::get<offset_index<Row,ExcludedRow>::value, offset_index<Column, ExcludedColumn>::value>( matrix.m );
        }
    };

    template <typename Matrix, unsigned int ExcludedRow, unsigned int ExcludedColumn>
    struct tensor_traits< matrix_minor< Matrix, ExcludedRow, ExcludedColumn > >
    {
        typedef matrix_minor_access_policy< Matrix, ExcludedRow, ExcludedColumn > access_policy;
        typedef boost::mpl::int_<2>                                               tensor_order;
        typedef void                                                              rank_2;
        typedef void                                                              is_tensor;
        typedef void make_fusion_sequence;//Generate the fusion adaptor for the accesses to this.
    };

}//namespace geometrix;

//! Mark the expr as an MPL type sequence.
namespace boost { namespace mpl {
template<typename>                              
struct sequence_tag;                            
                                                
template<typename T, unsigned int R, unsigned int C>                            
struct sequence_tag< geometrix::matrix_minor<T,R,C> >
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
template<typename T, unsigned int R, unsigned int C>             
struct sequence_tag<geometrix::matrix_minor<T,R,C> const>
{                                               
    typedef fusion::fusion_sequence_tag type;   
};                                              
}}//namespace boost::mpl

#endif
