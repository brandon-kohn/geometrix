#define ROWS BOOST_PP_FRAME_ITERATION(1)
#define COLUMNS BOOST_PP_FRAME_ITERATION(2)

namespace geometrix { namespace detail {

    //! Define a sequence of Row,Column dimension tuples from which to generate the matrix traits definitions.
    #define GEOMETRIX_GENERATE_MATRIX_COL_ASSIGNMENTS_( z, n, data )  \
            set<Row BOOST_PP_COMMA() n>                               \
            BOOST_PP_LPAREN()                                         \
                l                                                     \
              BOOST_PP_COMMA() get<Row BOOST_PP_COMMA() n>            \
                BOOST_PP_LPAREN()                                     \
                    r                                                 \
                BOOST_PP_RPAREN()                                     \
            BOOST_PP_RPAREN();                                        \
    /***/
        
    #define GEOMETRIX_GENERATE_MATRIX_ELEMENTS_COLUMN_ASSIGNMENT_SEQUENCE_( N ) \
        BOOST_PP_REPEAT( N, GEOMETRIX_GENERATE_MATRIX_COL_ASSIGNMENTS_, _ )     \
    /***/

    #define GEOMETRIX_GENERATE_MATRIX_ROW_ASSIGNMENTS_( z, n, data ) \
        assign_row<n>(l,r);                                          \
    /***/

    #define GEOMETRIX_GENERATE_MATRIX_ELEMENTS_ROW_ASSIGNMENT_SEQUENCE_( M ) \
        BOOST_PP_REPEAT( M, GEOMETRIX_GENERATE_MATRIX_ROW_ASSIGNMENTS_, _ )  \
    /***/

    template <>
    struct matrix_assigner<ROWS, COLUMNS>
    {    
        template <std::size_t Row, typename L, typename R>
        static void assign_row(L&l,const R&r)
        {
            GEOMETRIX_GENERATE_MATRIX_ELEMENTS_COLUMN_ASSIGNMENT_SEQUENCE_(COLUMNS);
        }

        template <typename L, typename R>
        static L& assign(L&l,const R&r)
        {
            GEOMETRIX_GENERATE_MATRIX_ELEMENTS_ROW_ASSIGNMENT_SEQUENCE_(ROWS);
            return l;
        }
    };

    //! Define a sequence of Row,Column dimension tuples from which to generate the matrix traits definitions.
    #define GEOMETRIX_GENERATE_MATRIX_ELEMENT_INDEX_PAIR_( z, n, data ) \
        BOOST_PP_COMMA_IF(n)                                            \
                get                                                     \
                <                                                       \
                    BOOST_PP_SEQ_ELEM(0,data) BOOST_PP_COMMA() n        \
                >                                                       \
                BOOST_PP_LPAREN() m BOOST_PP_RPAREN()                   \
    /***/

    #define GEOMETRIX_GENERATE_MATRIX_ELEMENTS_INDICES_( z, n, data )                                         \
        BOOST_PP_COMMA_IF(n)                                                                                  \
        {BOOST_PP_REPEAT_##z( BOOST_PP_SEQ_ELEM(0,data), GEOMETRIX_GENERATE_MATRIX_ELEMENT_INDEX_PAIR_, (n) )}\
    /***/

    #define GEOMETRIX_ENUM_MATRIX_ACCESS_SEQUENCE_( M, N )                     \
        BOOST_PP_REPEAT( M, GEOMETRIX_GENERATE_MATRIX_ELEMENTS_INDICES_, (N) ) \
    /***/

    template <>
    struct matrix_pod_constructor<ROWS, COLUMNS>
    {    
        template <typename M, typename R>
        static M construct(const R& m)
        {
            M r = {{ GEOMETRIX_ENUM_MATRIX_ACCESS_SEQUENCE_(ROWS, COLUMNS) }};
            return r;
        }
    };
}}//namespace geometrix::detail;

#undef GEOMETRIX_GENERATE_MATRIX_ELEMENT_INDEX_PAIR_
#undef GEOMETRIX_GENERATE_MATRIX_ELEMENTS_INDICES_
#undef GEOMETRIX_GENERATE_MATRIX_ACCESS_SEQUENCE_

#undef GEOMETRIX_GENERATE_MATRIX_COL_ASSIGNMENTS_
#undef GEOMETRIX_GENERATE_MATRIX_ELEMENTS_COLUMN_ASSIGNMENT_SEQUENCE_
#undef GEOMETRIX_GENERATE_MATRIX_ROW_ASSIGNMENTS_
#undef GEOMETRIX_GENERATE_MATRIX_ELEMENTS_ROW_ASSIGNMENT_SEQUENCE_

#undef COLUMNS
#undef ROWS
