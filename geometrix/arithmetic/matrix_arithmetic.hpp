//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_MATH_HPP
#define GEOMETRIX_MATRIX_MATH_HPP

#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/tensor/matrix.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/algebra/cross_product.hpp>
#include <geometrix/tensor/vector_matrix_adaptors.hpp>
#include <geometrix/tensor/matrix_minor_adaptor.hpp>
#include <geometrix/arithmetic/detail/determinant.hpp>

#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/exception/all.hpp>

namespace geometrix {

    namespace result_of
    {
        template <typename Matrix1, typename Matrix2, std::size_t Row, std::size_t Column>
        struct matrix_product_element
            : dot_product
                <
                    row<typename std::decay<Matrix1>::type, Row>
                  , column<typename std::decay<Matrix2>::type, Column>
                >
        {};

        namespace detail
        {

            template <typename Matrix1, typename Matrix2, typename Row, typename Column>
            struct matrix_product_element
                : dot_product
                    <
                        row<typename std::decay<Matrix1>::type, Row::value>
                      , column<typename std::decay<Matrix2>::type, Column::value>
                    >
            {};

            template <typename Matrix1, typename Matrix2, typename Row>
            struct matrix_product_row
                : boost::mpl::iter_fold
                    <
                        boost::mpl::range_c<int, 0, column_dimension_of<Matrix2>::value>
                      , boost::mpl::vector<>
                      , boost::mpl::push_back
                        <
                            boost::mpl::deref<boost::mpl::_1>
                          , matrix_product_element
                            <
                                typename std::decay<Matrix1>::type
                              , typename std::decay<Matrix2>::type
                              , Row
                              , boost::mpl::deref<boost::mpl::_2>
                            >
                        >
                    >
            {};

            template <typename Matrix1, typename Matrix2, typename Column>
            struct matrix_product_column
                : boost::mpl::iter_fold
                    <
                        boost::mpl::range_c<int, 0, row_dimension_of<Matrix1>::value>
                      , boost::mpl::vector<>
                      , boost::mpl::push_back
                        <
                            boost::mpl::deref<boost::mpl::_1>
                          , matrix_product_element
                            <
                                typename std::decay<Matrix1>::type
                              , typename std::decay<Matrix2>::type
                              , boost::mpl::deref<boost::mpl::_2>
                              , Column
                            >
                        >
                    >
            {};

            template <typename Scalar, typename Matrix, typename Row>
            struct matrix_scalar_multiply_row
                : multiplies
                    <
                        typename std::decay<Scalar>::type
                      , row<typename std::decay<Matrix>::type,Row::value>
                    >
            {};

            template <typename Matrix, typename Scalar, typename Row>
            struct matrix_scalar_divides_row
                : divides
                    <
                        row<typename std::decay<Matrix>::type,Row::value>
                      , typename std::decay<Scalar>::type
                    >
            {};
        }//namespace detail;

        template <typename Matrix1, typename Matrix2, typename LeftIsHomogeneous=void, typename RightIsHomogeneous=void>
        struct matrix_product
            : boost::mpl::iter_fold
                <
                    boost::mpl::range_c<int, 0, row_dimension_of<Matrix1>::value>
                  , boost::mpl::vector<>
                  , boost::mpl::push_back
                    <
                        boost::mpl::deref<boost::mpl::_1>
                      , detail::matrix_product_row<Matrix1, Matrix2, boost::mpl::deref<boost::mpl::_2> >
                    >
                >
        {};

        template <typename Matrix1, typename Matrix2>
        struct matrix_product
            <
                Matrix1
              , Matrix2
              , typename geometric_traits<typename std::decay<Matrix1>::type>::is_homogeneous
              , typename geometric_traits<typename std::decay<Matrix2>::type>::is_homogeneous
            >
            : select_arithmetic_type_from_2
                <
                    typename type_at<typename std::decay<Matrix1>::type,0,0>::type
                  , typename type_at<typename std::decay<Matrix2>::type,0,0>::type
                >
        {};

        template <typename Scalar, typename Matrix>
        struct multiplies< Scalar, Matrix, typename geometric_traits<Scalar>::is_scalar, typename geometric_traits<Matrix>::is_matrix >
            : boost::mpl::iter_fold
                <
                    boost::mpl::range_c<int, 0, row_dimension_of<Matrix>::value>
                  , boost::mpl::vector<>
                  , boost::mpl::push_back
                    <
                        boost::mpl::deref<boost::mpl::_1>
                      , detail::matrix_scalar_multiply_row<typename std::decay<Scalar>::type, typename std::decay<Matrix>::type, boost::mpl::deref<boost::mpl::_2> >
                    >
                >
        {};

        template <typename Scalar, typename Matrix>
        struct multiplies< Scalar, Matrix, typename numeric_traits<Scalar>::is_numeric, typename geometric_traits<Matrix>::is_matrix >
            : boost::mpl::iter_fold
                <
                    boost::mpl::range_c<int, 0, row_dimension_of<Matrix>::value>
                  , boost::mpl::vector<>
                  , boost::mpl::push_back
                    <
                        boost::mpl::deref<boost::mpl::_1>
                      , detail::matrix_scalar_multiply_row<typename std::decay<Scalar>::type, typename std::decay<Matrix>::type, boost::mpl::deref<boost::mpl::_2> >
                    >
                >
        {};

        template <typename Matrix, typename Scalar>
        struct divides< Matrix, Scalar, typename geometric_traits<typename std::decay<Matrix>::type>::is_matrix, typename numeric_traits<typename std::decay<Scalar>::type>::is_numeric >
            : boost::mpl::iter_fold
                <
                    boost::mpl::range_c<int, 0, row_dimension_of<Matrix>::value>
                  , boost::mpl::vector<>
                  , boost::mpl::push_back
                    <
                        boost::mpl::deref<boost::mpl::_1>
                      , detail::matrix_scalar_divides_row<typename std::decay<Matrix>::type, typename std::decay<Scalar>::type, boost::mpl::deref<boost::mpl::_2> >
                    >
                >
        {};

        template <typename Matrix, typename Scalar>
        struct divides< Matrix, Scalar, typename geometric_traits<typename std::decay<Matrix>::type>::is_matrix, typename geometric_traits<typename std::decay<Scalar>::type>::is_scalar >
            : boost::mpl::iter_fold
                <
                    boost::mpl::range_c<int, 0, row_dimension_of<Matrix>::value>
                  , boost::mpl::vector<>
                  , boost::mpl::push_back
                    <
                        boost::mpl::deref<boost::mpl::_1>
                      , detail::matrix_scalar_divides_row<typename std::decay<Matrix>::type, typename type_at<Scalar>::type, boost::mpl::deref<boost::mpl::_2> >
                    >
                >
        {};

        template <std::size_t Row, std::size_t Column, typename Matrix>
        struct inverse_at_index
            : result_of::divides
                <
                    typename result_of::determinant<Matrix>::type
                  , typename result_of::determinant<Matrix>::type
                >
        {};


        template <std::size_t Row, std::size_t Column, typename Matrix>
        struct adjugate_at_index
            : result_of::determinant<Matrix>
        {};

    }//namespace result_of;

    //! Calculate the matrix product at the specified index.
    template <std::size_t Row, std::size_t Column, typename Matrix1, typename Matrix2>
    inline typename result_of::matrix_product_element
        <
            Matrix1
          , Matrix2
          , Row
          , Column
        >::type matrix_product_element( const Matrix1& m1, const Matrix2& m2 )
    {
        using matrix1_type = typename std::decay<Matrix1>::type;
        using matrix2_type = typename std::decay<Matrix2>::type;
        GEOMETRIX_STATIC_ASSERT( is_matrix<matrix1_type>::value );
        GEOMETRIX_STATIC_ASSERT( is_matrix<matrix2_type>::value );
        BOOST_MPL_ASSERT_MSG
        (
            boost::mpl::bool_< ( column_dimension_of<Matrix1>::value == row_dimension_of<Matrix2>::value ) >::value
          , MATRIX_DIMENSIONS_SUPPLIED_TO_MATRIX_PRODUCT_ARE_NOT_COMPATIBLE
          , (std::pair<Matrix1, Matrix2>)
        );

        return dot_product( row<matrix1_type,Row>( m1 ), column<matrix2_type, Column>( m2 ) );
    }

    //! Calculate the determinant of a matrix.
    template <typename Matrix>
    inline typename result_of::determinant<Matrix>::type determinant( const Matrix& m )
    {
        typedef typename row_dimension_of<Matrix>::type row_dim;
        return detail::determinant(m, row_dim());
    }

    //! Calculate the adjugate of a matrix at the given indices.
    template <std::size_t Row, std::size_t Column, typename Matrix>
    inline typename result_of::adjugate_at_index<Row,Column,Matrix>::type adjugate_at_index( const Matrix& m )
    {
        matrix_minor<Matrix, Column, Row> mm( m );
        return power_c<-1,Row+Column>::value * determinant( mm );
     }

    //! Calculate the inverse of a matrix at the given indices.
    template <std::size_t Row, std::size_t Column, typename Matrix>
    inline typename result_of::inverse_at_index<Row,Column,Matrix>::type inverse_at_index( const Matrix& m )
    {
        typename result_of::determinant<Matrix>::type d = detail::determinant(m, typename row_dimension_of<Matrix>::type() );

        if( d != 0 )
        {
            return arithmetic_promotion_policy
                <
                    typename result_of::adjugate_at_index<Row, Column, Matrix>::type
                >::promote( adjugate_at_index<Row,Column>(m) )/ d;
        }
        else
            throw std::logic_error( "cannot invert a matrix whose determinant is zero." );
     }

}//namespace geometrix;

#endif //GEOMETRIX_MATRIX_MATH_HPP
