//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_CONSTANTS_HPP
#define GENERATIVE_GEOMETRY_CONSTANTS_HPP
#pragma once

#include "rational_utilities.hpp"
#include "numeric_traits.hpp"
#include "detail/integral_constant_pi_calculator.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

    template <typename CoordinateType>
    struct constants
    {
        
    };

    template <>
    struct constants<double>
    {
        static const double pi() { return 3.1415926535897931; }
    };

    template <>
    struct constants<float>
    {
        static const float pi() { return 3.1415926535897931f; }
    };

    //! \brief Constants for integral types.
    template <>
    struct constants<int>
    {
        typedef int int_type;

        //! \brief Calculate the required digits of pi for type int.
        static const rational_promotion_traits< int_type >::rational_type pi() 
        {
            typedef rational_promotion_traits< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::precision >::pi();
            return _pi;
        }
    };

    template <>
    struct constants<long long>
    {
        typedef long long int_type;

        //! \brief Calculate the required digits of pi for type long long.
        static const rational_promotion_traits< int_type >::rational_type pi() 
        {
            typedef rational_promotion_traits< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::precision >::pi();
            return _pi;
        }
    };

    template <>
    struct constants<long>
    {
        typedef long int_type;

        //! \brief Calculate the required digits of pi for type long.
        static const rational_promotion_traits< int_type >::rational_type pi() 
        {
            typedef rational_promotion_traits< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::precision >::pi();
            return _pi;
        }
    };

    template <>
    struct constants<short>
    {
        typedef short int_type;

        //! \brief Calculate the required digits of pi for type short.
        static const rational_promotion_traits< int_type >::rational_type pi() 
        {
            typedef rational_promotion_traits< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::precision >::pi();
            return _pi;
        }
    };

    template <>
    struct constants<char>
    {
        typedef char int_type;

        //! \brief Calculate the required digits of pi for type char.
        static const rational_promotion_traits< int_type >::rational_type pi() 
        {
            typedef rational_promotion_traits< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::precision >::pi();
            return _pi;
        }
    };

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_CONSTANTS_HPP
