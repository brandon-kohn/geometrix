//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#define GEOMETRIX_MATRIX_ROW( i, matrix ) \
    BOOST_PP_SEQ_ELEM( i, matrix )        \
/***/

#define GEOMETRIX_MATRIX_COLUMN_APPEND( s, data, row )                                                                                      \
    ( BOOST_PP_SEQ_ELEM(0,data) )                                                                                                           \
    ( BOOST_PP_SEQ_ELEM(1,data) )                                                                                                           \
    ( BOOST_PP_SEQ_PUSH_BACK(BOOST_PP_SEQ_ELEM(2, data), BOOST_PP_TUPLE_ELEM(BOOST_PP_SEQ_ELEM(0,data), BOOST_PP_SEQ_ELEM(1,data), row) ) ) \
/***/

#define GEOMETRIX_MATRIX_COLUMN( j, Columns, matrix )                                                                    \
    BOOST_PP_SEQ_ELEM(2, BOOST_PP_SEQ_FOLD_LEFT(GEOMETRIX_MATRIX_COLUMN_APPEND, (Columns)(j)(BOOST_PP_SEQ_NIL), matrix) )\
/***/

#define GEOMETRIX_GENERATE_MATRIX_ROW_TRAITS(Matrix, Rows)                   \
    namespace geometrix {                                                    \
    BOOST_PP_REPEAT( Rows, GEOMETRIX_ROW_TRAITS_CONSTRUCT_FUNCTION, Matrix ) \
    }                                                                        \
/***/

#define GEOMETRIX_DECLARE_ROW( i, Matrix )                                     \
    geometrix::detail::TemplateMacroHelper<row>::Arg1< Matrix >::Arg2<i>::type \
/***/

//! Rows
#define GEOMETRIX_MATRIX_ROW_NUMERIC_SEQUENCE_TRAITS_CONSTRUCT_FUNCTION( z, i, data )                                                                   \
namespace geometrix {                                                                                                                                   \
namespace detail{ typedef row<BOOST_PP_SEQ_ELEM(0,data),i> BOOST_PP_CAT( BOOST_PP_SEQ_ELEM(0,data), BOOST_PP_CAT( _row, i ) ); }                        \
template <>                                                                                                                                             \
struct geometric_traits<detail::BOOST_PP_CAT( BOOST_PP_SEQ_ELEM(0,data), BOOST_PP_CAT( _row, i ) )>                                                     \
{                                                                                                                                                       \
    typedef detail::BOOST_PP_CAT( BOOST_PP_SEQ_ELEM(0,data), BOOST_PP_CAT( _row, i ) )                                             sequence_type;       \
    typedef GEOMETRIX_AS_MPL_VECTOR(BOOST_PP_TUPLE_TO_SEQ(BOOST_PP_SEQ_ELEM(1,data), GEOMETRIX_MATRIX_ROW(i, BOOST_PP_SEQ_ELEM(2,data)))) storage_types;\
    typedef dimension<BOOST_PP_SEQ_ELEM(1, data)>                                                                                  dimension_type;      \
    typedef void                                                                                                                   is_sequence;         \
    typedef void                                                                                                                   is_numeric_sequence; \
    typedef void                                                                                                                   is_vector;           \
    typedef void BOOST_PP_SEQ_ELEM(3, data);                                                                                                            \
};                                                                                                                                                      \
}                                                                                                                                                       \
/***/

#define GEOMETRIX_DEFINE_MATRIX_ROW_GEOMETRIC_TRAITS_( Matrix, Rows, Columns, NumericTypes, ArithmeticType )                                 \
    BOOST_PP_REPEAT(Rows, GEOMETRIX_MATRIX_ROW_NUMERIC_SEQUENCE_TRAITS_CONSTRUCT_FUNCTION, (Matrix)(Columns)(NumericTypes)(ArithmeticType) ) \
/***/

//! Columns
#define GEOMETRIX_GENERATE_MATRIX_COLUMN_TRAITS(Matrix, Columns)                   \
namespace geometrix {                                                              \
    BOOST_PP_REPEAT( Columns, GEOMETRIX_COLUMN_TRAITS_CONSTRUCT_FUNCTION, Matrix ) \
}                                                                                  \
/***/

#define GEOMETRIX_DECLARE_COLUMN( i, Matrix )                                   \
geometrix::detail::TemplateMacroHelper< column >::Arg1< Matrix >::Arg2<i>::type \
/***/

#define GEOMETRIX_MATRIX_COLUMN_NUMERIC_SEQUENCE_TRAITS_CONSTRUCT_FUNCTION( z, i, data )                                              \
namespace geometrix {                                                                                                                 \
namespace detail{ typedef column<BOOST_PP_SEQ_ELEM(0,data),i> BOOST_PP_CAT( BOOST_PP_SEQ_ELEM(0,data), BOOST_PP_CAT( _column, i ) ); }\
template <>                                                                                                                           \
struct geometric_traits<detail::BOOST_PP_CAT( BOOST_PP_SEQ_ELEM(0,data), BOOST_PP_CAT( _column, i ) )>                                \
{                                                                                                                                     \
    typedef detail::BOOST_PP_CAT( BOOST_PP_SEQ_ELEM(0,data), BOOST_PP_CAT( _column, i ) )                       sequence_type;        \
    typedef GEOMETRIX_AS_MPL_VECTOR(GEOMETRIX_MATRIX_COLUMN(i, BOOST_PP_SEQ_ELEM(2, data), BOOST_PP_SEQ_ELEM(1,data))) storage_types; \
    typedef dimension<BOOST_PP_SEQ_ELEM(4, data)>                                                               dimension_type;       \
    typedef void                                                                                                is_sequence;          \
    typedef void                                                                                                is_numeric_sequence;  \
    typedef void                                                                                                is_vector;            \
    typedef void BOOST_PP_SEQ_ELEM(3, data);                                                                                          \
};                                                                                                                                    \
}                                                                                                                                     \
/***/

#define GEOMETRIX_DEFINE_MATRIX_COLUMN_GEOMETRIC_TRAITS_( Matrix, Rows, Columns, NumericTypes, ArithmeticType )                                           \
    BOOST_PP_REPEAT( Columns, GEOMETRIX_MATRIX_COLUMN_NUMERIC_SEQUENCE_TRAITS_CONSTRUCT_FUNCTION, (Matrix)(NumericTypes)(Columns)(ArithmeticType)(Rows) ) \
/***/

//! Convert a single matrix element (double) into an NxM sequence of tuples 2x2=> (( double, double )) ((double, double))
#define GEOMETRIX_GENERATE_MATRIX_TYPES_ROW_( z, n, data )                                          \
    BOOST_PP_LPAREN()                                                                               \
        BOOST_PP_LPAREN()                                                                           \
            BOOST_PP_ENUM_##z(BOOST_PP_SEQ_ELEM(0,data), GEOMETRIX_TEXT, BOOST_PP_SEQ_ELEM(1,data)) \
        BOOST_PP_RPAREN()                                                                           \
    BOOST_PP_RPAREN()                                                                               \
/***/

#define GEOMETRIX_GENERATE_MATRIX_TYPE_ELEMENTS( Type, Row, Column )             \
    BOOST_PP_REPEAT( Row, GEOMETRIX_GENERATE_MATRIX_TYPES_ROW_, (Column)(Type) ) \
/***/
