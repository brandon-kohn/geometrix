//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SCALAR_HPP
#define GEOMETRIX_SCALAR_HPP

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/space/dimension.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/tensor/scalar_access_policy.hpp>

namespace geometrix {

    //! \brief Tag to check if a type is a numeric type.
    template <typename NumericType, typename Enable = void>
    struct is_scalar : boost::false_type{};

    //! Tag to mark scalar types.
    struct scalar_tag{};
    
    template <typename NumericType>
    struct is_scalar<NumericType, typename numeric_traits<typename remove_const_ref<NumericType>::type>::is_numeric> : boost::true_type {};

    template <typename NumericType>
    struct geometric_traits<NumericType, typename numeric_traits<typename remove_const_ref<NumericType>::type>::is_numeric>
    {
        typedef void is_scalar;
        typedef void is_homogeneous;
    };

    template <typename NumericType>
    struct dimension_of<NumericType, typename numeric_traits<NumericType>::is_numeric> 
    {
        typedef dimension<1> type;
    };

    template <typename NumericType>
    struct tensor_traits<NumericType, typename numeric_traits<NumericType>::is_numeric>
    {
        typedef null_access_policy<NumericType> access_policy;
        typedef boost::mpl::int_<0>             tensor_order;
        typedef void                            rank_0;
        typedef void                            is_tensor;
    };
    
}//namespace geometrix;

#endif //GEOMETRIX_SCALAR_HPP
