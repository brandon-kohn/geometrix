//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TENSOR_FUNCTION_HPP
#define GEOMETRIX_TENSOR_FUNCTION_HPP

#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/tensor/tensor_traits.hpp>

namespace geometrix {

    template <typename F>
    struct tensor_function 
    {};

    template <typename F>
    struct function_traits<tensor_function<F> > : function_traits<F>{};

    namespace algebra
    {
        typedef tensor_function<math::absolute_value> abs;
        typedef tensor_function<math::sine>           sin;
        typedef tensor_function<math::arcsine>        asin;
        typedef tensor_function<math::cosine>         cos;
        typedef tensor_function<math::arccosine>      acos;
        typedef tensor_function<math::tangent>        tan;
        typedef tensor_function<math::arctangent>     atan;
        typedef tensor_function<math::square_root>    sqrt;
        typedef tensor_function<math::round_down>     floor;
        typedef tensor_function<math::round_up>       ceil;
    };

    //! Tensor functions are initiated when the object is multiplied by its arg type.
    template <typename F, typename Arg>
    typename boost::result_of<F(Arg)>::type operator * ( const tensor_function<F>&, const Arg& a )
    {
        return F()(a);
    }

    template <typename F, typename Arg>
    typename boost::result_of<F(Arg)>::type operator * ( const Arg& a, const tensor_function<F>& )
    {
        return F()(a);
    }
    
}//namespace geometrix;

#endif //GEOMETRIX_TENSOR_FUNCTION_HPP
