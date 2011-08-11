//
//! Copyright © 2008-2011
//! Brandon Kohn
//! Derived/copied from struct example in Boost.Fusion. Thanks guys ;).
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_ADAPT_MEMBER_FUNCTION_HPP
#define GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_ADAPT_MEMBER_FUNCTION_HPP

#include <boost/fusion/support/tag_of_fwd.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/extension.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/member_function_iterator.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/is_view_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/is_sequence_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/category_of_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/begin_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/end_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/size_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/at_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/value_at_impl.hpp>
#include <geometrix/utility/member_function_fusion_adaptor/member_function_proxy.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/int.hpp>
#include <boost/config/no_tr1/utility.hpp>

//! void argument (no arguments) case:
#define GEOMETRIX_MEMBER_FUNCTION_FUSION_SEQUENCE(name, bseq)            \
    GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_I(                                  \
    name, BOOST_PP_CAT(GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_X bseq, 0))      \
/***/

#define GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_X(A, B, C, D) ((A, B, C, D)) GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_Y
#define GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_Y(A, B, C, D) ((A, B, C, D)) GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_X
#define GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_X0
#define GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_Y0

#define GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_I(name, seq)                                   \
    namespace boost { namespace fusion { namespace traits                               \
    {                                                                                   \
        template <>                                                                     \
        struct tag_of<name>                                                             \
        {                                                                               \
            typedef member_function_tag type;                                           \
        };                                                                              \
    }}}                                                                                 \
    namespace boost { namespace fusion { namespace extension                            \
    {                                                                                   \
        template <>                                                                     \
        struct member_function_size<name> : mpl::int_<BOOST_PP_SEQ_SIZE(seq)> {};       \
        BOOST_PP_SEQ_FOR_EACH_I(GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_C, name, seq)          \
    }}}                                                                                 \
/***/

#define GEOMETRIX_ADAPT_MEMBER_FUNCTIONS_C(r, name, i, xy)                         \
    GEOMETRIX_DEFINE_MEMBER_FUNCTION_PROXY                                         \
    (                                                                              \
        name                                                                       \
      , i                                                                          \
      , BOOST_PP_TUPLE_ELEM(4,0,xy)                                                \
      , BOOST_PP_TUPLE_ELEM(4,1,xy)                                                \
      , BOOST_PP_TUPLE_ELEM(4,2,xy)                                                \
      , BOOST_PP_TUPLE_ELEM(4,3,xy)                                                \
    )                                                                              \
    template <>                                                                    \
    struct member_function<name, i>                                                \
    {                                                                              \
        typedef member_function_proxy<name,i> type;                                \
        static member_function_proxy<name,i> call(name& instance)                  \
        {                                                                          \
            return member_function_proxy<name,i>(instance);                        \
        }                                                                          \
    };                                                                             \
    template <>                                                                    \
    struct member_function<const name, i>                                          \
    {                                                                              \
        typedef BOOST_PP_TUPLE_ELEM(4,1,xy) type;                                  \
        static BOOST_PP_TUPLE_ELEM(4,1,xy) call(const name& instance)              \
        {                                                                          \
            return instance.BOOST_PP_TUPLE_ELEM(4,2,xy)();                         \
        }                                                                          \
    };                                                                             \
/***/

//! Single run-time index argument case:
#define GEOMETRIX_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE(name, R, memfcn, N)  \
    namespace boost { namespace fusion { namespace traits                      \
    {                                                                          \
        template <>                                                            \
        struct tag_of<name>                                                    \
        {                                                                      \
            typedef member_function_tag type;                                  \
        };                                                                     \
    }}}                                                                        \
    namespace boost { namespace fusion { namespace extension                   \
    {                                                                          \
        template <>                                                            \
        struct member_function_size<name> : mpl::int_<N> {};                   \
        BOOST_PP_REPEAT( N,                                                    \
        GEOMETRIX_ADAPT_INDEXED_MEMBER_FUNCTIONS, (name, R, memfcn) )          \
    }}}                                                                        \
/***/

#define GEOMETRIX_ADAPT_INDEXED_MEMBER_FUNCTIONS(z, n, seq)                    \
    template <>                                                                \
    struct member_function<BOOST_PP_TUPLE_ELEM(3, 0, seq), n>                  \
    {                                                                          \
        typedef BOOST_PP_TUPLE_ELEM(3, 1, seq) type;                           \
        static type call(const BOOST_PP_TUPLE_ELEM(3, 0, seq)& instance)       \
        {                                                                      \
            return instance.BOOST_PP_TUPLE_ELEM(3, 2, seq)(n);                 \
        }                                                                      \
    };                                                                         \
/***/

//! Single compile-time index argument case: i.e. tp.get<10>();
#define GEOMETRIX_COMPILE_INDEXED_MEMBER_FUNCTION_FUSION_SEQUENCE(name, R, memfcn, N) \
    namespace boost { namespace fusion { namespace traits                             \
    {                                                                                 \
        template <>                                                                   \
        struct tag_of<name>                                                           \
        {                                                                             \
            typedef member_function_tag type;                                         \
        };                                                                            \
    }}}                                                                               \
    namespace boost { namespace fusion { namespace extension                          \
    {                                                                                 \
        template <>                                                                   \
        struct member_function_size<name> : mpl::int_<N> {};                          \
        BOOST_PP_REPEAT( N,                                                           \
        GEOMETRIX_ADAPT_COMPILE_INDEXED_MEMBER_FUNCTIONS, (name, R, memfcn) )         \
    }}}                                                                               \
/***/

#define GEOMETRIX_ADAPT_COMPILE_INDEXED_MEMBER_FUNCTIONS(z, n, seq)            \
    template <>                                                                \
    struct member_function<BOOST_PP_TUPLE_ELEM(3, 0, seq), n>                  \
    {                                                                          \
        typedef BOOST_PP_TUPLE_ELEM(3, 1, seq) type;                           \
        template <typename T>                                                  \
        static type call(T& instance)                                          \
        {                                                                      \
            return instance.BOOST_PP_TUPLE_ELEM(3, 2, seq)<n>();               \
        }                                                                      \
    };                                                                         \
/***/

//! Operator[] case.
#define GEOMETRIX_INDEX_OPERATOR_FUSION_SEQUENCE(name, R, N)                   \
    namespace boost { namespace fusion { namespace traits                      \
    {                                                                          \
        template <>                                                            \
        struct tag_of<name>                                                    \
        {                                                                      \
            typedef member_function_tag type;                                  \
        };                                                                     \
    }}}                                                                        \
    namespace boost { namespace fusion { namespace extension                   \
    {                                                                          \
        template <>                                                            \
        struct member_function_size<name> : mpl::int_<N> {};                   \
        BOOST_PP_REPEAT( N,                                                    \
        GEOMETRIX_ADAPT_INDEX_OPERATOR_FUNCTIONS, (name, R) )                  \
    }}}                                                                        \
    /***/

#define GEOMETRIX_ADAPT_INDEX_OPERATOR_FUNCTIONS(z, n, seq)                    \
    template <>                                                                \
    struct member_function<BOOST_PP_TUPLE_ELEM(2, 0, seq), n>                  \
    {                                                                          \
        typedef BOOST_PP_TUPLE_ELEM(2, 1, seq) type;                           \
        template <typename T>                                                  \
        static type call(T& instance)                                          \
        {                                                                      \
            return instance[n];                                                \
        }                                                                      \
    };                                                                         \
/***/

#endif //GEOMETRIX_DETAIL_MEMBER_FUNCTION_FUSION_ADAPTOR_ADAPT_MEMBER_FUNCTION_HPP
