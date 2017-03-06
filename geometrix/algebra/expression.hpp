//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_EXPRESSION_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_EXPRESSION_HPP

#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/matrix_traits.hpp>
#if defined(GEOMETRIX_USE_COMPOSITE_MATRICES)
#include <geometrix/tensor/composite_matrix.hpp>
#endif
#include <geometrix/tensor/scalar.hpp>
#include <geometrix/algebra/grammar.hpp>

#include <geometrix/algebra/binary_functions.hpp>
#include <geometrix/algebra/unary_functions.hpp>

#include <boost/mpl/assert.hpp>

#define GEOMETRIX_EXPRESSION_NAMESPACE_START namespace geometrix {
#define GEOMETRIX_EXPRESSION_NAMESPACE_END }
#define GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE geometrix
#include <geometrix/algebra/expression_generator.hpp>
#undef GEOMETRIX_EXPRESSION_NAMESPACE_START
#undef GEOMETRIX_EXPRESSION_NAMESPACE_END
#undef GEOMETRIX_EXPRESSION_NAMESPACE_SCOPE

namespace geometrix {
    //! \brief Helper type to create expr type defs.
    template <typename A, typename B, char Op>
    struct expression_type_helper{};

    #define GEOMETRIX_EXPRESSION_HELPER( op, opcode ) \
    template <typename A, typename B>                 \
    struct expression_type_helper<A, B,opcode>        \
    {                                                 \
        typedef BOOST_TYPEOF_TPL( std::declval<A>() op std::declval<B>() ) type;  \
                                                      \
    };                                                \
    /***/

    GEOMETRIX_EXPRESSION_HELPER(-, '-');
    GEOMETRIX_EXPRESSION_HELPER(+, '+');
    GEOMETRIX_EXPRESSION_HELPER(*, '*');
    GEOMETRIX_EXPRESSION_HELPER(^, '^');
    GEOMETRIX_EXPRESSION_HELPER(/ , '/');

    #undef GEOMETRIX_EXPRESSION_HELPER
}//! namespace geometrix;

#define GEOMETRIX_TYPEOF_BINARY_EXPR( op, A, B ) \
geometrix::expression_type_helper<A,B,op>::type
/***/

#define GEOMETRIX_TYPEOF_BINARY_EXPR_TPL( op, A, B ) \
typename geometrix::expression_type_helper<A,B,op>::type
/***/

#endif //GEOMETRIX_LINEAR_ALGEBRA_EXPRESSION_HPP
