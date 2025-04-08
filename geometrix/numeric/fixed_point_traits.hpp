//
//! Copyright © 2008-2013
//! Brandon Kohn, James Stewart
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_FIXED_POINT_TRAITS_HPP
#define GEOMETRIX_NUMERIC_FIXED_POINT_TRAITS_HPP

#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/utility/compile.hpp>
#include <geometrix/numeric/detail/friend_ordered_field_operators.hpp>
#include <geometrix/numeric/detail/arithmetic_operators.hpp>

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/utility/enable_if.hpp>
#include <cstdint>
#include <limits>
#include <cmath>
#include <iomanip>
#include <locale>
#include <boost/lexical_cast.hpp>
#include <string>
#include <type_traits>

namespace geometrix {

    template <int Radix, int Exp>
    struct generic_compile_time_scale_policy
    {
        typedef void compile_time_category;

        typedef boost::mpl::int_<Radix> radix_type;
        typedef boost::mpl::int_<Exp> exp_type;

        int get_scale() const { return Exp; }
        
        //! Reverse the scale up operation.
        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * power_c<Radix, Exp>::value );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR T scale_down( B v ) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / power_c<Radix, Exp>::value );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v, int ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow( Radix, Exp ) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR T scale_down( B v, int ) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / integral_pow( Radix, Exp ) );
        }
    };

    //! Note: numeric_limits and boost::numeric_cast (range checking) do not work properly with runtime scaling.
    template <int Radix>
    struct generic_run_time_scale_policy
    {
        typedef void run_time_category;
		using radix_type = boost::mpl::int_<Radix>;

        BOOST_CONSTEXPR generic_run_time_scale_policy( unsigned int scale = 2 )
            : m_scale( scale )
        {}

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow( Radix, m_scale ) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR T scale_down( B v ) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / integral_pow( Radix, m_scale ) );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v, int Exp ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow( Radix, Exp ) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR T scale_down( B v, int Exp ) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / integral_pow( Radix, Exp ) );
        }
        
        int get_scale() const { return m_scale; }

        int m_scale;
    };

    template <int Exp>
    struct binary_compile_time_scale_policy
    {
        typedef void compile_time_category;

        typedef boost::mpl::int_<Exp> exp_type;
        int get_scale() const { return Exp; }

        //! Reverse the scale up operation.
        template <typename B, typename T>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_integral<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << Exp );
        }

        template <typename B, typename T>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_float<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * power_c<2, Exp>::value );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v ) const
        {
            return boost::numeric_cast<T>( v >> Exp );
        }

        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v ) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / power_c<2, Exp>::value );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v, int, typename boost::enable_if< boost::is_integral<T> >::type* = 0 ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << Exp );
        }

        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v, int, typename boost::enable_if< boost::is_float<T> >::type* = 0 ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * power_c<2, Exp>::value );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v, int) const
        {
            return boost::numeric_cast<T>( v >> Exp );
        }

        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v, int) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / power_c<2, Exp>::value );
        }
    };

    //! Note: numeric_limits and boost::numeric_cast (range checking) do not work properly with runtime scaling.
    struct binary_run_time_scale_policy
    {
        typedef void run_time_category;

        BOOST_CONSTEXPR binary_run_time_scale_policy( unsigned int scale = 2 )
            : m_scale( scale )
        {}

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_integral<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << m_scale );
        }

        template <typename B, typename T>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_float<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow(2,m_scale) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v ) const
        {
            return boost::numeric_cast<T>( v >> m_scale );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v ) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / m_scale );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v, int Exp, typename boost::enable_if< boost::is_integral<T> >::type* = 0 ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << Exp );
        }

        template <typename B, typename T>
        BOOST_CONSTEXPR B scale_up( T v, int Exp, typename boost::enable_if< boost::is_float<T> >::type* = 0 ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow(2, Exp) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v, int Exp ) const
        {
            return boost::numeric_cast<T>( v >> Exp );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        BOOST_CONSTEXPR typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v, int Exp ) const
        {
            return boost::numeric_cast<T>( arithmetic_promote( v ) / Exp );
        }

        int get_scale() const { return m_scale; }

        int m_scale;
    };

    //! This is also known as the Banker's Round.
    struct round_half_to_even
    {
        typedef boost::mpl::integral_c<std::float_round_style,std::round_to_nearest> round_style;

        template <typename T>
        BOOST_CONSTEXPR static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {
            using std::floor;
            using std::ceil;
            T prev = floor(v);
            T next = ceil(v);
            T rt = (v - prev) - (next - v);
            const T zero(0.0);
            const T two(2.0);

            if ( rt < zero )
                return prev;
            else if ( rt > zero )
                return next;
            else
            {
                bool even = two * floor(prev / two) == prev;
                return ( even ? prev : next );
            }
        }
    };

    struct round_toward_zero
    {
        template <typename T>
        BOOST_CONSTEXPR static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        BOOST_CONSTEXPR static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {
            using std::floor;
            using std::ceil;
            return v >= static_cast<T>(0) ? floor(v) : ceil(v) ;
        }

        typedef boost::mpl::integral_c<std::float_round_style,std::round_toward_zero> round_style;
    } ;

    template<class S>
    struct round_toward_infinity
    {
        template <typename T>
        BOOST_CONSTEXPR static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        BOOST_CONSTEXPR static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {
            using std::ceil;
            return ceil(v);
        }

        typedef boost::mpl::integral_c<std::float_round_style,std::round_toward_infinity> round_style;
    } ;

    template<class S>
    struct round_toward_neg_infinity
    {
        template <typename T>
        BOOST_CONSTEXPR static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        BOOST_CONSTEXPR static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {
            using std::floor;
            return floor(v);
        }

        typedef boost::mpl::integral_c<std::float_round_style,std::round_toward_neg_infinity> round_style;
    };

    template <typename FormatType, typename ScalePolicy, typename RoundingPolicy>
    struct fixed_point_traits
    {
        typedef FormatType                                format_type;
        typedef ScalePolicy                               scale_policy;
        typedef RoundingPolicy                            rounding_policy;
    };
}//! namespace geometrix;

#endif //GEOMETRIX_NUMERIC_FIXED_POINT_TRAITS_HPP

