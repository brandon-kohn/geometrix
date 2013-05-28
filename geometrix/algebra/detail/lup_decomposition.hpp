#if !defined(GEOMETRIX_DONT_USE_PREPROCESSED_FILES)

    #include <geometrix/algebra/detail/preprocessed/lup_decomposition.hpp>

#elif !defined(BOOST_PP_IS_ITERATING)

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/lup_decomposition.hpp")
    #endif

    //
    //! Copyright © 2008-2011
    //! Brandon Kohn
    //
    //  Distributed under the Boost Software License, Version 1.0. (See
    //  accompanying file LICENSE_1_0.txt or copy at
    //  http://www.boost.org/LICENSE_1_0.txt)
    //

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif

    #define BOOST_PP_ITERATION_PARAMS_1                                                         \
        (3, (2, GEOMETRIX_MAX_LUP_DECOMPOSITION_MATRIX_SIZE, <geometrix/algebra/detail/lup_decomposition.hpp>))
    #include BOOST_PP_ITERATE()

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif

#else

#define N BOOST_PP_ITERATION()
//#pragma message( BOOST_PP_STRINGIZE(N) )

#define GEOMETRIX_PIVOT_CALC(r, s)                          \
    if( math::abs(lu[BOOST_PP_TUPLE_ELEM(2, 0, s)][k]) > p )\
    {                                                       \
        p = math::abs(lu[BOOST_PP_TUPLE_ELEM(2, 0, s)][k]); \
        k_ = BOOST_PP_TUPLE_ELEM(2, 0, s);                  \
    }                                                       \
/***/
    
#define GEOMETRIX_SWAP_CALC(r, state)                  \
    std::swap(lu[k][BOOST_PP_TUPLE_ELEM(2, 0, state)]  \
    , lu[k_][BOOST_PP_TUPLE_ELEM(2, 0, state)] );      \
/***/

#define GEOMETRIX_M_I_J_CALC(r, state_j)          \
    lu[i][BOOST_PP_TUPLE_ELEM(2, 0, state_j)] -=  \
    lu[i][k]                                      \
   *lu[k][BOOST_PP_TUPLE_ELEM(2, 0, state_j)];    \

#define GEOMETRIX_M_I_K_CALC(r, state)            \
    i = BOOST_PP_SEQ_ELEM(0, state);              \
    lu[i][k]                                      \
    = lu[i][k]/lu[k][k];                          \
    BOOST_PP_FOR_##r(                             \
        (BOOST_PP_INC(BOOST_PP_SEQ_ELEM(2,state)) \
        , BOOST_PP_SUB(N,1))                      \
        , GEOMETRIX_TUPLE_SENTINEL                \
        , GEOMETRIX_TUPLE_INC                     \
        , GEOMETRIX_M_I_J_CALC )                  \
/***/

//! for( std::size_t k = 0; k < N - 1 ++k )
/*  {
        T p = 0;
        std::size_t k_ = 0;
        for( std::size_t i = k; i < N; ++i )
        {
            if( lu[i][k] > p )
            {
                p = lu[i][k];
                k_ = i;
            }
        }

        if( p == 0 )
            throw std::exception( "cannot lup decompose a singular matrix." );
        std::swap( pi[k], pi[k_] );
        for( std::size_t i = 1; i < N; ++i )
            std::swap( lu[k][i], lu[k_][i] );
        for( std::size_t i = k + 1; i < N; ++i )
        {
            lu[i][k] = lu[i][k]/lu[k][k];
            for( std::size_t j = k + 1; j < N; ++j )
                lu[i][j] -= lu[i][k]*lu[k][j];
        }
    }
*/  
#define GEOMETRIX_OUTER_K_LOOP(r, state )                            \
{                                                                    \
    T p = 0;                                                         \
    std::size_t k = BOOST_PP_TUPLE_ELEM(2,0,state);                  \
    std::size_t k_ = 0;                                              \
    std::size_t i = 0;                                               \
    BOOST_PP_FOR_##r( (BOOST_PP_TUPLE_ELEM(2,0,state)                \
                     , BOOST_PP_DEC(N))                              \
                     , GEOMETRIX_TUPLE_SENTINEL                      \
                     , GEOMETRIX_TUPLE_INC                           \
                     , GEOMETRIX_PIVOT_CALC )                        \
    if( p == 0 )                                                     \
        throw                                                        \
        std::logic_error("cannot lup decompose a singular matrix."); \
    std::swap( pi[k], pi[k_] );                                      \
    BOOST_PP_FOR_##r( (0, BOOST_PP_DEC(N))                           \
                     , GEOMETRIX_TUPLE_SENTINEL                      \
                     , GEOMETRIX_TUPLE_INC                           \
                     , GEOMETRIX_SWAP_CALC )                         \
    BOOST_PP_FOR_##r(                                                \
            (BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2,0,state)))           \
            (BOOST_PP_DEC(N))                                        \
            (BOOST_PP_TUPLE_ELEM(2,0,state))                         \
        , GEOMETRIX_SEQ_SENTINEL, GEOMETRIX_SEQ_INC, GEOMETRIX_M_I_K_CALC ) \
}                                                                    \
/***/

namespace geometrix { namespace algebra {

    template <typename T>
    void lup_decomposition( matrix<T, N, N>& lu, boost::array<std::size_t, N>& pi )
    {
        //! Initialize PI as zeros.
        #define GEOMETRIX_PI_INIT(z, n, data) pi[n] = n;
        BOOST_PP_REPEAT( N, GEOMETRIX_PI_INIT, _ )
        #undef GEOMETRIX_PI_INIT

        //for( std::size_t k = 1; k < N - 1; ++k )
        BOOST_PP_FOR((0,BOOST_PP_DEC(N)), GEOMETRIX_TUPLE_SENTINEL, GEOMETRIX_TUPLE_INC, GEOMETRIX_OUTER_K_LOOP)
        /*
        {
            T p = 0;
            std::size_t k_ = 0;
            for( std::size_t i = k; i < N; ++i )
            {
                if( lu[i][k] > p )
                {
                    p = lu[i][k];
                    k_ = i;
                }
            }

            if( p == 0 )
                throw std::exception( "cannot lup decompose a singular matrix." );
            std::swap( pi[k], pi[k_] );
            for( std::size_t i = 1; i < N; ++i )
                std::swap( lu[k][i], lu[k_][i] );
            for( std::size_t i = k + 1; i < N; ++i )
            {
                lu[i][k] = lu[i][k]/lu[k][k];
                for( std::size_t j = k + 1; j < N; ++j )
                    lu[i][j] -= lu[i][k]*lu[k][j];
            }
        }
        */        
    }

/***/

#define GEOMETRIX_SUM_Y_ELEM( r, state )                                      \
    sum += lu[BOOST_PP_TUPLE_ELEM(2,1,state)][BOOST_PP_TUPLE_ELEM(2,0,state)] \
         * y[BOOST_PP_TUPLE_ELEM(2,0,state)];                                 \
/***/

#define GEOMETRIX_SOLVE_Y_VECTOR_ELEM( r, state )              \
    {                                                          \
        sum = 0;                                               \
        BOOST_PP_FOR_##r( (0, BOOST_PP_TUPLE_ELEM(2, 0,state)) \
                        , GEOMETRIX_TUPLE_LESS_SENTINEL        \
                        , GEOMETRIX_TUPLE_INC                  \
                        , GEOMETRIX_SUM_Y_ELEM )               \
        y[BOOST_PP_TUPLE_ELEM(2,0,state)] =                    \
               b[pi[BOOST_PP_TUPLE_ELEM(2,0,state)]] - sum;    \
    }                                                          \
/***/

#define GEOMETRIX_SUM_X_ELEM( r, state )                              \
    sum += lu[BOOST_PP_SEQ_ELEM(2,state)][BOOST_PP_SEQ_ELEM(0,state)] \
        * x[BOOST_PP_SEQ_ELEM(0,state)];                              \
/***/

#define GEOMETRIX_SOLVE_X_VECTOR_ELEM( r, state )                 \
    {                                                             \
        sum = 0;                                                  \
        BOOST_PP_EXPR_IF                                          \
        (                                                         \
            BOOST_PP_NOT_EQUAL                                    \
            (                                                     \
                BOOST_PP_TUPLE_ELEM(2,0,state)                    \
              , N                                                 \
            )                                                     \
          , BOOST_PP_FOR_##r                                      \
            (                                                     \
                BOOST_PP_IF                                       \
                (                                                 \
                    BOOST_PP_NOT_EQUAL                            \
                    (                                             \
                        BOOST_PP_TUPLE_ELEM(2,0,state)            \
                      , N                                         \
                    )                                             \
                  , (BOOST_PP_TUPLE_ELEM(2,0,state))              \
                  , (0)                                           \
                )                                                 \
                (N)                                               \
                (BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 0,state)))   \
              , GEOMETRIX_SEQ_LESS_SENTINEL                       \
              , GEOMETRIX_SEQ_INC                                 \
              , GEOMETRIX_SUM_X_ELEM                              \
            )                                                     \
        )                                                         \
        x[BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 0,state))] =        \
        (                                                         \
            y[BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 0,state))]      \
          - sum                                                   \
        ) / lu[BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 0,state))]     \
              [BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 0,state))];    \
    }                                                             \
/***/

    template <typename T>
    boost::array<T, N> lup_solve( matrix<T, N, N>& lu, const boost::array<std::size_t,N>& pi, const boost::array<T,N>& b )
    {
        boost::array<T, N> x, y;
        T sum;
        //for( std::size_t i = 0; i < N; ++i )
        BOOST_PP_FOR( (0,N)
                    , GEOMETRIX_TUPLE_LESS_SENTINEL
                    , GEOMETRIX_TUPLE_INC              
                    , GEOMETRIX_SOLVE_Y_VECTOR_ELEM )

//#pragma message( BOOST_PP_STRINGIZE((BOOST_PP_FOR( (0,N), GEOMETRIX_TUPLE_LESS_SENTINEL, GEOMETRIX_TUPLE_INC, GEOMETRIX_SOLVE_Y_VECTOR_ELEM ))) )
//#pragma message( "\n" )
        /*
        {
            sum = 0;
            for( std::size_t j = 0; j < i; ++j )
                sum += lu[i][j] * y[j];
            y[i] = b[pi[i]] - sum;
        }
        */
        
        //for( std::size_t i = N-1; i >= 0; --i )
        //! Handle the logic for N-1 and the i>=0 inside the macro.
        BOOST_PP_FOR( (N, 1)
                    , GEOMETRIX_TUPLE_GREATER_EQUAL_SENTINEL
                    , GEOMETRIX_TUPLE_DEC
                    , GEOMETRIX_SOLVE_X_VECTOR_ELEM )

//#pragma message( BOOST_PP_STRINGIZE((BOOST_PP_FOR( (N, 1), GEOMETRIX_TUPLE_GREATER_EQUAL_SENTINEL, GEOMETRIX_TUPLE_DEC, GEOMETRIX_SOLVE_X_VECTOR_ELEM ))) )
//#pragma message( "\n" )
        /*        {
            sum = 0;
            for( std::size_t j = i+1; j < N; ++j )
                sum += lu[i][j] * x[j];
            x[i] = ( y[i] - sum ) / lu[i][i];

            if( i == 0 )
                break;
        }
        */

        return x;
    }
    
}}//namespace geometrix::algebra;

#undef GEOMETRIX_OUTER_K_LOOP
#undef GEOMETRIX_PIVOT_CALC    
#undef GEOMETRIX_SWAP_CALC
#undef GEOMETRIX_M_I_J_CALC
#undef GEOMETRIX_M_I_K_CALC
#undef GEOMETRIX_SUM_Y_ELEM
#undef GEOMETRIX_SOLVE_Y_VECTOR_ELEM
#undef GEOMETRIX_SUM_X_ELEM
#undef GEOMETRIX_SOLVE_X_VECTOR_ELEM
#undef N

#endif
