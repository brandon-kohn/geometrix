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
#include <geometrix/arithmetic/cmath.hpp>
#include <geometrix/tensor/tensor_traits.hpp>

namespace geometrix {

    template <typename F>
    struct tensor_function 
    {};

    template <typename F>
    struct function_traits<tensor_function<F> > : function_traits<F>{};

    namespace tensor_functions
    {
        typedef tensor_function<abs_function> abs;
        typedef tensor_function<sin_function> sin;
        typedef tensor_function<asin_function> asin;
        typedef tensor_function<cos_function> cos;
        typedef tensor_function<acos_function> acos;
        typedef tensor_function<tan_function> tan;
        typedef tensor_function<atan_function> atan;
        typedef tensor_function<sqrt_function> sqrt;
        typedef tensor_function<floor_function> floor;
        typedef tensor_function<ceil_function> ceil;
    };

    //! Tensor functions are initiated when the object is multiplied by its arg type.
    template <typename F, typename Arg>
    inline typename boost::result_of<F(Arg)>::type operator * ( const tensor_function<F>&, const Arg& a )
    {
        return F()(a);
    }

    template <typename F, typename Arg>
    inline typename boost::result_of<F(Arg)>::type operator * ( const Arg& a, const tensor_function<F>& )
    {
        return F()(a);
    }
    
}//namespace geometrix;

#endif //GEOMETRIX_TENSOR_FUNCTION_HPP
