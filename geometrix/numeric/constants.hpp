//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CONSTANTS_HPP
#define GEOMETRIX_CONSTANTS_HPP

#include <geometrix/numeric/rational_utilities.hpp>
#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/integral_constant_pi_calculator.hpp>

namespace geometrix {

    template <typename CoordinateType>
    struct constants
    {
        
    };

    template <>
    struct constants<double>
    {
        static double pi() { return 3.1415926535897931; }
    };

    template <>
    struct constants<float>
    {
        static float pi() { return 3.1415926535897931f; }
    };

    //! \brief Constants for integral types.
    template <>
    struct constants<boost::int32_t>
    {
        typedef boost::int32_t int_type;

        //! \brief Calculate the required digits of pi for type int.
        static rational_promotion_policy< int_type >::rational_type pi() 
        {
            typedef rational_promotion_policy< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::digits >::pi();
            return _pi;
        }
    };

    template <>
    struct constants<boost::int64_t>
    {
        typedef boost::int64_t int_type;

        //! \brief Calculate the required digits of pi for type boost::int64_t.
        static rational_promotion_policy< int_type >::rational_type pi() 
        {
            typedef rational_promotion_policy< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::digits >::pi();
            return _pi;
        }
    };
    
    template <>
    struct constants<boost::int16_t>
    {
        typedef boost::int16_t int_type;

        //! \brief Calculate the required digits of pi for type short.
        static rational_promotion_policy< int_type >::rational_type pi() 
        {
            typedef rational_promotion_policy< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::digits >::pi();
            return _pi;
        }
    };

    template <>
    struct constants<boost::int8_t>
    {
        typedef boost::int8_t int_type;

        //! \brief Calculate the required digits of pi for type char.
        static rational_promotion_policy< int_type >::rational_type pi() 
        {
            typedef rational_promotion_policy< int_type >::rational_type rational_type;
            static rational_type _pi = detail::calculate_pi< rational_type, numeric_traits< int_type >::digits >::pi();
            return _pi;
        }
    };

}//namespace geometrix;

#endif //GEOMETRIX_CONSTANTS_HPP
