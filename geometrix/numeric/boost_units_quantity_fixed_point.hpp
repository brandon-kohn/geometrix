//
//! Copyright © 2016
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_BOOST_UNITS_QUANTITY_FIXED_POINT_HPP
#define GEOMETRIX_NUMERIC_BOOST_UNITS_QUANTITY_FIXED_POINT_HPP

#include <boost/units/quantity.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/numeric/fixed_point.hpp>
#include <type_traits>

namespace geometrix {

    template <typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR boost::units::quantity<Unit, geometrix::fixed_point<Traits>> operator * (const X& d, const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& l)
    {
        return boost::units::quantity<Unit, geometrix::fixed_point<Traits>>::from_value(d * l.value());
    }

    template <typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR boost::units::quantity<Unit, geometrix::fixed_point<Traits>> operator * (const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& l, const X& d)
    {
        return boost::units::quantity<Unit, geometrix::fixed_point<Traits>>::from_value(d * l.value());
    }

    template <typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR boost::units::quantity<Unit, geometrix::fixed_point<Traits>> operator + (const X& d, const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& l)
    {
        return boost::units::quantity<Unit, geometrix::fixed_point<Traits>>::from_value(d + l.value());
    }

    template <typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR boost::units::quantity<Unit, geometrix::fixed_point<Traits>> operator + (const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& l, const X& d)
    {
        return boost::units::quantity<Unit, geometrix::fixed_point<Traits>>::from_value(d + l.value());
    }

    template <typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR boost::units::quantity<Unit, geometrix::fixed_point<Traits>> operator - (const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& l, const X& d)
    {
        return boost::units::quantity<Unit, geometrix::fixed_point<Traits>>::from_value(l.value() - d);
    }

    template <typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR boost::units::quantity<Unit, geometrix::fixed_point<Traits>> operator - (const X& d, const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& l)
    {
        return boost::units::quantity<Unit, geometrix::fixed_point<Traits>>::from_value(d - l.value());
    }

    template<typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR typename boost::units::divide_typeof_helper<X, boost::units::quantity<Unit, geometrix::fixed_point<Traits>>>::type operator / (const X& lhs, const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& rhs)
    {
        using type = typename boost::units::divide_typeof_helper<X, boost::units::quantity<Unit, geometrix::fixed_point<Traits>>>::type;
        return type::from_value(lhs / rhs.value());
    }

    template <typename Unit, typename X, typename Traits, typename std::enable_if<std::is_arithmetic<X>::value, int>::type = 0>
    inline BOOST_CONSTEXPR boost::units::quantity<Unit, geometrix::fixed_point<Traits>> operator / (const boost::units::quantity<Unit, geometrix::fixed_point<Traits>>& l, const X& d)
    {
        return boost::units::quantity<Unit, geometrix::fixed_point<Traits>>::from_value(l.value()/d);
    }

}//namespace geometrix;

#endif //GEOMETRIX_NUMERIC_BOOST_UNITS_QUANTITY_FIXED_POINT_HPP
