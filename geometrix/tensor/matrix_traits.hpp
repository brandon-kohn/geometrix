//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_TRAITS_HPP
#define GEOMETRIX_MATRIX_TRAITS_HPP

#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/detail/matrix_traits.hpp>
#include <geometrix/tensor/matrix_row.hpp>
#include <geometrix/tensor/matrix_column.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/fusion_vector_access_policy.hpp>

namespace geometrix {

//! \brief type check to check if a type is a matrix type.

//! \ingroup Type Traits
template <typename Matrix, typename Enable = void>
struct is_matrix 
    : boost::false_type
{};

template <typename Matrix>
struct is_matrix<Matrix, typename geometric_traits<Matrix>::is_matrix>
    : boost::true_type
{};

//! \brief A tag to designate a matrix.
struct matrix_tag {};

//! Index type for matrices
template <std::size_t Row, std::size_t Column>
struct matrix_index
{
    typedef boost::mpl::int_<Row>    row;
    typedef boost::mpl::int_<Column> col;
};

//! Concept to describe an NxM matrix.
//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
template <typename Matrix>
struct MatrixConcept
{
    typedef typename geometric_traits< Matrix >::matrix_type matrix_type;

    //! Check that there is a greater than zero tensor_order
    //!    - row dimension_type  (static size)
    GEOMETRIX_STATIC_ASSERT( geometric_traits<Matrix>::row_dimension::value > 0 );
    //!    - column dimension_type  (static size)
    GEOMETRIX_STATIC_ASSERT( geometric_traits<Matrix>::col_dimension::value > 0 );    
};

template <typename Matrix, typename EnableIf=void>
struct row_dimension_of
{
    typedef typename geometric_traits<Matrix>::row_dimension type;
    BOOST_STATIC_CONSTANT( std::size_t, value = type::value );
};

template <typename Matrix, typename EnableIf=void>
struct column_dimension_of
{
    typedef typename geometric_traits<Matrix>::col_dimension type;
    BOOST_STATIC_CONSTANT( std::size_t, value = type::value );
};

template <typename Matrix1, typename Matrix2>
struct dimensions_equal
       <
           Matrix1
         , Matrix2
       > 
    :   boost::mpl::and_
        <
            boost::mpl::equal< row_dimension_of<Matrix1>, row_dimension_of<Matrix2> >
          , boost::mpl::equal< column_dimension_of<Matrix1>, column_dimension_of<Matrix2 > >
        >
{};

template <typename Matrix>
struct is_square_matrix
    : boost::mpl::equal
      < 
          typename row_dimension_of<Matrix>::type
        , typename column_dimension_of<Matrix>::type 
      >
{};

namespace detail
{
    template <std::size_t Rows, std::size_t Columns, typename EnableIf=void>
    struct matrix_assigner
    {};

    template <std::size_t Rows, std::size_t Columns, typename EnableIf=void>
    struct matrix_pod_constructor
    {
        template <typename M, typename R>
        static M construct(const R& m);        
    };
}

}//namespace geometrix;

//! \def GEOMETRIX_DEFINE_MATRIX_TRAITS( Matrix, ElementTypes, Rows, Columns, ArithmeticType, AccessPolicy )
//! A macro for defining geometric_traits for a user defined matrix type.\n
//! Example usage:
//! \code
//! template <std::size_t Rows, std::size_t Columns>
//! struct matrix
//! {
//!     double m[Rows][Columns];
//! 
//!     template <std::size_t RowIndex, std::size_t ColumnIndex>
//!     struct access
//!     {
//!         return m[RowIndex][ColumnIndex];
//!     };
//!     
//!     template <std::size_t RowIndex, std::size_t ColumnIndex>
//!     double&       get() { return access<RowIndex, ColumnIndex>::get( *this ); }
//!     template <std::size_t RowIndex, std::size_t ColumnIndex>
//!     const double& get() const { return access<RowIndex, ColumnIndex>::get( *this ); }
//! };
//! 
//! GEOMETRIX_DEFINE_HOMOGENEOUS_MATRIX_TRAITS( Matrix, double, Rows, Columns, double, index_operator_matrix_access_policy );
//! \endcode
#define GEOMETRIX_DEFINE_HOMOGENEOUS_MATRIX_TRAITS( Matrix, NumericType, Rows, Columns, ArithmeticType, AccessPolicy ) \
GEOMETRIX_DEFINE_MATRIX_ROW_GEOMETRIC_TRAITS_                                                                          \
(                                                                                                                      \
    Matrix                                                                                                             \
  , Rows, Columns                                                                                                      \
  , GEOMETRIX_GENERATE_MATRIX_TYPE_ELEMENTS(NumericType, Rows, Columns), is_homogeneous                                \
)                                                                                                                      \
GEOMETRIX_DEFINE_MATRIX_COLUMN_GEOMETRIC_TRAITS_                                                                       \
(                                                                                                                      \
    Matrix                                                                                                             \
  , Rows, Columns                                                                                                      \
  , GEOMETRIX_GENERATE_MATRIX_TYPE_ELEMENTS(NumericType, Rows, Columns), is_homogeneous                                \
)                                                                                                                      \
GEOMETRIX_DEFINE_TENSOR_TRAITS( Matrix, 2, AccessPolicy )                                                              \
namespace geometrix {                                                                                                  \
template <>                                                                                                            \
struct geometric_traits< Matrix >                                                                                      \
{                                                                                                                      \
    typedef dimension<Rows>    row_dimension;                                                                          \
    typedef dimension<Columns> col_dimension;                                                                          \
    typedef void               is_matrix;                                                                              \
    typedef void               is_homogeneous;                                                                         \
};                                                                                                                     \
}                                                                                                                      \
/***/

#define GEOMETRIX_DEFINE_HETEROGENEOUS_MATRIX_TRAITS( Matrix, NumericTypes, Rows, Columns, AccessPolicy ) \
GEOMETRIX_DEFINE_MATRIX_ROW_GEOMETRIC_TRAITS_( Matrix, Rows, Columns, NumericTypes, is_heterogeneous )    \
GEOMETRIX_DEFINE_MATRIX_COLUMN_GEOMETRIC_TRAITS_( Matrix, Rows, Columns, NumericTypes, is_heterogeneous ) \
GEOMETRIX_DEFINE_TENSOR_TRAITS( Matrix, 2, AccessPolicy )                                                 \
namespace geometrix {                                                                                     \
template <>                                                                                               \
struct geometric_traits< Matrix >                                                                         \
{                                                                                                         \
    typedef dimension<Rows>    row_dimension;                                                             \
    typedef dimension<Columns> col_dimension;                                                             \
    typedef void               is_matrix;                                                                 \
    typedef void               is_heterogeneous;                                                          \
};                                                                                                        \
}                                                                                                         \
/***/

//! \brief Define a construction policy for a matrix type which uses a default constructor followed by assignment of each element.
#define GEOMETRIX_DEFINE_DEFAULT_MATRIX_CONSTRUCTION_POLICY( Matrix, Types, Rows, Columns )       \
template <>                                                                                       \
struct construction_policy< Matrix >                                                              \
{                                                                                                 \
    template <BOOST_PP_ENUM(BOOST_PP_MUL(Rows,Columns), GEOMETRIX_TEMPLATE_PARAM_, _ )>           \
    static Matrix construct( BOOST_PP_ENUM(BOOST_PP_MUL(Rows,Columns), GEOMETRIX_PARAM_, _ ) )    \
    {                                                                                             \
        Matrix m( BOOST_PP_ENUM_PARAMS(BOOST_PP_MUL(Rows,Columns), a) );                          \
        return m;                                                                                 \
    }                                                                                             \
                                                                                                  \
    template <typename M>                                                                         \
    static Matrix construct( const M& m )                                                         \
    {                                                                                             \
        Matrix r;                                                                                 \
        return detail::matrix_assigner<Rows,Columns>::assign( r, m );                             \
        return r;                                                                                 \
    }                                                                                             \
};                                                                                                \
/***/

//! \brief Define a construction policy for a POD matrix type which uses static init.
#define GEOMETRIX_DEFINE_POD_MATRIX_CONSTRUCTION_POLICY( Matrix, Types, Rows, Columns )               \
template <>                                                                                           \
struct construction_policy< Matrix >                                                                  \
{                                                                                                     \
    template <BOOST_PP_ENUM(BOOST_PP_MUL(Rows,Columns), GEOMETRIX_TEMPLATE_PARAM_, _ )>               \
    static Matrix construct( BOOST_PP_ENUM(BOOST_PP_MUL(Rows,Columns), GEOMETRIX_PARAM_, _ ) )        \
    {                                                                                                 \
        Matrix m = { BOOST_PP_ENUM_PARAMS(BOOST_PP_MUL(Rows,Columns), a) };                           \
        return m;                                                                                     \
    }                                                                                                 \
                                                                                                      \
    template <typename M>                                                                             \
    static Matrix construct( const M& m )                                                             \
    {                                                                                                 \
        return detail::matrix_pod_constructor<Rows,Columns>::construct<Matrix>( m );                  \
    }                                                                                                 \
};                                                                                                    \
/***/

#endif //GEOMETRIX_MATRIX_TRAITS_HPP
