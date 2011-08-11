//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FUSION_MATRIX_HPP
#define GEOMETRIX_FUSION_MATRIX_HPP

#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/tensor/matrix_traits.hpp>
#include <geometrix/utility/preprocessor.hpp>
#include <geometrix/tensor/fusion_matrix_access_policy.hpp>

#include <boost/fusion/adapted/struct.hpp>

namespace geometrix {

#define GEOMETRIX_FUSION_MATRIX_ROW_( z, n, data )          \
    boost::fusion::vector<                                  \
        BOOST_PP_TUPLE_REM_CTOR                             \
        (                                                   \
            BOOST_PP_SEQ_ELEM(0,data)                       \
          , BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(1,data)) \
        )>                                                  \
/***/

#define GEOMETRIX_FUSION_MATRIX( Name, Types, Rows, Columns )                                           \
struct Name                                                                                             \
{                                                                                                       \
    template <BOOST_PP_ENUM(BOOST_PP_MUL(Rows,Columns), GEOMETRIX_TEMPLATE_PARAM_, _ )>                 \
    Name( BOOST_PP_ENUM(BOOST_PP_MUL(Rows,Columns), GEOMETRIX_PARAM_, _ ) )                             \
    : m( BOOST_PP_ENUM_PARAMS(BOOST_PP_MUL(Rows,Columns), a) ){}                                        \
    typedef boost::fusion::vector                                                                       \
    <                                                                                                   \
        BOOST_PP_ENUM(Rows, GEOMETRIX_FUSION_MATRIX_ROW_, (Columns)(Types))                             \
    > matrix_type;                                                                                      \
    matrix_type m;                                                                                      \
};                                                                                                      \
GEOMETRIX_DEFINE_HETEROGENEOUS_MATRIX_TRAITS( Name, Types, Rows, Columns, fusion_matrix_access_policy<Name> )\
/***/

#define GEOMETRIX_FUSION_POD_MATRIX_ROW_STRUCT( z, n, data)     \
    BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM( 1, data ))          \
    BOOST_PP_CAT(a, BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0,data),n)); \
/***/

#define GEOMETRIX_FUSION_POD_MATRIX_ROW_( z, n, data )          \
    struct row##n                                               \
    {                                                           \
        BOOST_PP_REPEAT_##z                                     \
        (                                                       \
            BOOST_PP_SEQ_ELEM(0,data)                           \
          , GEOMETRIX_FUSION_POD_MATRIX_ROW_STRUCT              \
          , (n)                                                 \
            (BOOST_PP_TUPLE_TO_SEQ                              \
            (                                                   \
                BOOST_PP_SEQ_ELEM(0,data)                       \
              , BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(1,data)) \
            ))                                                  \
         )                                                      \
    };                                                          \
    row##n r##n;                                                \
/***/

#define GEOMETRIX_FUSION_POD_MATRIX_ADAPT_ROW_( z, n, data ) \
        (data::row##n,r##n)                                  \
/***/

#define GEOMETRIX_FUSION_POD_MATRIX_ADAPT_ROW_STRUCT_MEMBERS_(z,n,data)           \
   BOOST_PP_LPAREN() BOOST_PP_SEQ_ELEM(n,BOOST_PP_SEQ_ELEM(0, data))              \
       BOOST_PP_COMMA() BOOST_PP_CAT(a,BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(1,data),n));\
   BOOST_PP_RPAREN()                                                              \
/***/

#define GEOMETRIX_FUSION_POD_MATRIX_ADAPT_ROW_STRUCT_( z, n, data ) \
BOOST_FUSION_ADAPT_STRUCT                                           \
(                                                                   \
    BOOST_PP_SEQ_ELEM(0,data)::row##n                               \
  , BOOST_PP_REPEAT_##z                                             \
    (                                                               \
        BOOST_PP_SEQ_ELEM(1,data)                                   \
      , GEOMETRIX_FUSION_POD_MATRIX_ADAPT_ROW_STRUCT_MEMBERS_       \
      , (                                                           \
            BOOST_PP_TUPLE_TO_SEQ                                   \
            (                                                       \
                BOOST_PP_SEQ_ELEM(1,data)                           \
              , BOOST_PP_SEQ_ELEM(n, BOOST_PP_SEQ_ELEM(2,data))     \
            )                                                       \
        )(n)                                                        \
    )                                                               \
)                                                                   \
/***/

#define GEOMETRIX_FUSION_POD_MATRIX( Name, Types, Rows, Columns)                  \
struct Name                                                                       \
{                                                                                 \
      BOOST_PP_REPEAT(Rows, GEOMETRIX_FUSION_POD_MATRIX_ROW_, (Columns)(Types))   \
};                                                                                \
BOOST_FUSION_ADAPT_STRUCT                                                         \
(                                                                                 \
    Name                                                                          \
  , BOOST_PP_REPEAT(Rows, GEOMETRIX_FUSION_POD_MATRIX_ADAPT_ROW_, Name)           \
)                                                                                 \
BOOST_PP_REPEAT(Rows, GEOMETRIX_FUSION_POD_MATRIX_ADAPT_ROW_STRUCT_, (Name)(Columns)(Types) ) \
GEOMETRIX_DEFINE_HETEROGENEOUS_MATRIX_TRAITS( Name, Types, Rows, Columns, fusion_matrix_access_policy<Name> )\
/***/

}//namespace geometrix;

#endif//GEOMETRIX_FUSION_MATRIX_HPP
