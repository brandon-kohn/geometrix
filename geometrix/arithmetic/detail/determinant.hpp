//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_DETERMINANT_HPP
#define GEOMETRIX_DETAIL_DETERMINANT_HPP

#include <geometrix/utility/preprocessor.hpp>

#if !defined( GEOMETRIX_MAX_DETERMINANT )
#define GEOMETRIX_MAX_DETERMINANT 10 
#endif

namespace geometrix { namespace result_of {   
        
        template <typename Matrix, typename Dimension = typename row_dimension_of<Matrix>::type>
        struct determinant{};
 
        template <typename Matrix>
        struct determinant<Matrix, dimension<1> >
            : type_at<Matrix, 0, 0>
        {};

        template <typename Matrix>
        struct determinant<Matrix, dimension<2> >
            : minus
                < 
                    typename multiplies
                    <
                        typename type_at<Matrix, 0, 0>::type
                      , typename type_at<Matrix, 1, 1>::type
                    >::type
                  , typename multiplies
                    <
                        typename type_at<Matrix, 1, 0>::type
                      , typename type_at<Matrix, 0, 1>::type
                    >::type
                >
        {}; 

#define GEOMETRIX_MATRIX_COFACTOR_RESULT( i, j ) \
        typename multiplies                      \
        <                                        \
            int                                  \
          , typename determinant                 \
            <                                    \
                matrix_minor<Matrix, i, j>       \
              , typename row_dimension_of        \
                <                                \
                    matrix_minor<Matrix, i, j>   \
                >::type                          \
            >::type                              \
        >::type                                  \
/***/

//|B| == get<0,0>(m) * cofactor<0,0>(m) + get<0,1>(m) * cofactor<0,1> + ... + get<0,N-1>(m) * cofactor<0,N-1>(m)
#define GEOMETRIX_DET_ROW_SEQUENCE(r, state)                        \
   BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_ELEM(2,0,state))                \
    typename type_at<Matrix,0,BOOST_PP_TUPLE_ELEM(2,0,state)>::type \
/***/

#define GEOMETRIX_DET_COFACTOR_SEQUENCE(r, state)                      \
   BOOST_PP_COMMA_IF(BOOST_PP_TUPLE_ELEM(2,0,state))                   \
    GEOMETRIX_MATRIX_COFACTOR_RESULT(0,BOOST_PP_TUPLE_ELEM(2,0,state)) \
/***/

#define GEOMETRIX_ROW_SEQUENCE(N)       \
    BOOST_PP_FOR                        \
    (                                   \
        (0,BOOST_PP_DEC(N))             \
      , GEOMETRIX_TUPLE_SENTINEL        \
      , GEOMETRIX_TUPLE_INC             \
      , GEOMETRIX_DET_ROW_SEQUENCE      \
    )                                   \
/***/

#define GEOMETRIX_COFACTOR_SEQUENCE(N)  \
    BOOST_PP_FOR                        \
    (                                   \
        (0,BOOST_PP_DEC(N))             \
      , GEOMETRIX_TUPLE_SENTINEL        \
      , GEOMETRIX_TUPLE_INC             \
      , GEOMETRIX_DET_COFACTOR_SEQUENCE \
    )                                   \
/***/

#define GEOMETRIX_RESULT_OF_DET(n)                                                     \
    template <typename Matrix>                                                         \
    struct determinant<Matrix, dimension<n> >                                          \
    {                                                                                  \
        BOOST_STATIC_ASSERT(( is_square_matrix<Matrix>::value ));                      \
        typedef boost::mpl::transform_view                                             \
            <                                                                          \
                boost::mpl::zip_view                                                   \
                <                                                                      \
                    boost::mpl::vector                                                 \
                    <                                                                  \
                        boost::mpl::vector<GEOMETRIX_ROW_SEQUENCE(n)>                  \
                      , boost::mpl::vector<GEOMETRIX_COFACTOR_SEQUENCE(n)>             \
                    >                                                                  \
                >                                                                      \
                , boost::mpl::unpack_args< multiplies<boost::mpl::_1,boost::mpl::_2> > \
            > sequence;                                                                \
            typedef typename boost::mpl::begin<sequence>::type first;                  \
            typedef typename boost::mpl::end<sequence>::type   last;                   \
            typedef typename boost::mpl::next<first>::type     start;                  \
            typedef typename boost::mpl::iter_fold                                     \
                <                                                                      \
                    boost::mpl::iterator_range<start, last>                            \
                  , typename boost::mpl::deref<first>::type                            \
                  , plus< boost::mpl::_1, boost::mpl::deref<boost::mpl::_2> >          \
                >::type type;                                                          \
    };                                                                                 \
/***/

#define BOOST_PP_LOCAL_MACRO(n) \
    GEOMETRIX_RESULT_OF_DET(n)  \
/***/

#define BOOST_PP_LOCAL_LIMITS (3, GEOMETRIX_MAX_DETERMINANT)
#include BOOST_PP_LOCAL_ITERATE()

//#pragma message( BOOST_PP_STRINGIZE( ( GEOMETRIX_RESULT_OF_DET(3) ) ) )
#undef GEOMETRIX_RESULT_OF_DET 
#undef GEOMETRIX_COFACTOR_SEQUENCE 
#undef GEOMETRIX_ROW_SEQUENCE
#undef GEOMETRIX_DET_COFACTOR_SEQUENCE
#undef GEOMETRIX_DET_ROW_SEQUENCE

}//namespace result_of;

namespace detail
{
    template <typename Matrix>
    typename type_at<Matrix,0,0>::type determinant( const Matrix& m, dimension<1> )
    {
        return get<0,0>( m );
    }
    
    template <typename Matrix>
    typename result_of::determinant<Matrix>::type determinant( const Matrix& m, dimension<2> ) 
    {
        return get<0,0>( m ) * get<1,1>( m ) - get<1,0>( m ) * get<0,1>( m );
    }
}//namespace detail;
}//namespace geometrix;

#define GEOMETRIX_MATRIX_COFACTOR( i, j ) \
        math::power_c<-1,i+j>::value * determinant( matrix_minor<Matrix, i, j>( m ), typename row_dimension_of< matrix_minor<Matrix, i, j> >::type() )

//|B| == get<0,0>(m) * cofactor<0,0>(m) + get<0,1>(m) * cofactor<0,1> + ... + get<0,N-1>(m) * cofactor<0,N-1>(m)
#define GEOMETRIX_DET_STEP(r, state)                                  \
    BOOST_PP_IF(BOOST_PP_TUPLE_ELEM(2,0,state), +, BOOST_PP_EMPTY() ) \
    get<0,BOOST_PP_TUPLE_ELEM(2,0,state)>(m) *                        \
    GEOMETRIX_MATRIX_COFACTOR(0,BOOST_PP_TUPLE_ELEM(2,0,state))       \
/***/

#define GEOMETRIX_DETERMINANT(N) \
    BOOST_PP_FOR                 \
    (                            \
        (0,BOOST_PP_DEC(N))      \
      , GEOMETRIX_TUPLE_SENTINEL \
      , GEOMETRIX_TUPLE_INC      \
      , GEOMETRIX_DET_STEP       \
    )                            \
/***/

#define BOOST_PP_LOCAL_MACRO(n)                   \
namespace geometrix { namespace detail {          \
    template <typename Matrix>                    \
    typename result_of::determinant<Matrix>::type \
    determinant( const Matrix& m, dimension<n> )  \
    {                                             \
        return GEOMETRIX_DETERMINANT(n);          \
    }                                             \
}}                                                \
/***/

//#pragma message( BOOST_PP_STRINGIZE( ( GEOMETRIX_DETERMINANT(3) ) ) )

#define BOOST_PP_LOCAL_LIMITS (3, GEOMETRIX_MAX_DETERMINANT)
#include BOOST_PP_LOCAL_ITERATE()
#undef GEOMETRIX_DETERMINANT
#undef GEOMETRIX_DET_STEP
#undef GEOMETRIX_MATRIX_COFACTOR

#endif//GEOMETRIX_DETAIL_DETERMINANT_HPP