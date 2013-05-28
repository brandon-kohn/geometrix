//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(GEOMETRIX_PREPROCESSOR_HPP)
#define GEOMETRIX_PREPROCESSOR_HPP

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/selection/max.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#define GEOMETRIX_TEXT(z, n, text) text
#define GEOMETRIX_CAT(s, state, elem) BOOST_PP_CAT(state,elem)

#define GEOMETRIX_SEQ_SENTINEL(r, state) \
   BOOST_PP_NOT_EQUAL( \
      BOOST_PP_SEQ_ELEM(0, state), \
      BOOST_PP_INC(BOOST_PP_SEQ_ELEM(1, state)) \
   ) \
/***/

#define GEOMETRIX_SEQ_LESS_SENTINEL(r, state) \
   BOOST_PP_LESS( \
      BOOST_PP_SEQ_ELEM(0, state), \
      BOOST_PP_SEQ_ELEM(1, state)  \
   ) \
/***/

#define GEOMETRIX_SEQ_INC(r, state) \
      (BOOST_PP_INC(BOOST_PP_SEQ_ELEM(0, state))) \
      (BOOST_PP_SEQ_ELEM(1, state)) \
      (BOOST_PP_SEQ_ELEM(2, state)) \
/***/

#define GEOMETRIX_TUPLE_SENTINEL(r, state) \
   BOOST_PP_NOT_EQUAL( \
      BOOST_PP_TUPLE_ELEM(2, 0, state), \
      BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2, 1, state)) \
   ) \
/***/

#define GEOMETRIX_TUPLE_LESS_SENTINEL(r, state) \
   BOOST_PP_LESS( \
      BOOST_PP_TUPLE_ELEM(2, 0, state), \
      BOOST_PP_TUPLE_ELEM(2, 1, state)  \
   ) \
/***/

#define GEOMETRIX_TUPLE_GREATER_SENTINEL(r, state) \
   BOOST_PP_GREATER( \
      BOOST_PP_TUPLE_ELEM(2, 0, state), \
      BOOST_PP_TUPLE_ELEM(2, 1, state)  \
   ) \
/***/

#define GEOMETRIX_TUPLE_GREATER_EQUAL_SENTINEL(r, state) \
   BOOST_PP_GREATER_EQUAL( \
      BOOST_PP_TUPLE_ELEM(2, 0, state), \
      BOOST_PP_TUPLE_ELEM(2, 1, state)  \
   ) \
/***/

#define GEOMETRIX_TUPLE_INC(r, state) \
   ( \
      BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(2, 0, state)), \
      BOOST_PP_TUPLE_ELEM(2, 1, state) \
   ) \
/***/

#define GEOMETRIX_TUPLE_DEC(r, state) \
   ( \
      BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 0, state)), \
      BOOST_PP_TUPLE_ELEM(2, 1, state) \
   ) \
/***/

namespace geometrix { namespace detail {
    template <template <typename, unsigned int> class A>
    struct TemplateMacroHelper
    {
        template <typename T1>
        struct Arg1
        {
            typedef T1 t1;
            template <unsigned int I>
            struct Arg2
            {
                typedef A<t1, I> type;
            };
        };
    };
}}//namespace geometrix::detail;

#define GEOMETRIX_TEMPLATE_PARAM_(z, n, data ) \
    typename T##n                              \

#define GEOMETRIX_PARAM_(z, n, data ) \
    const T##n& a##n                  \

#define GEOMETRIX_AS_MPL_VECTOR(T)       \
boost::mpl::vector<BOOST_PP_SEQ_ENUM(T)> \
/***/

//GEOMETRIX_PREPROCESSOR_HPP
#endif
