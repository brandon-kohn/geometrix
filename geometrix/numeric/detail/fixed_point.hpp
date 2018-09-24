//
//! Copyright © 2008-2013
//! Brandon Kohn, James Stewart
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_DETAIL_FIXED_POINT_HPP
#define GEOMETRIX_NUMERIC_DETAIL_FIXED_POINT_HPP

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

    template< typename Traits >
    class fixed_point;

	template <typename T, typename EnableIf = void>
	struct is_fixed_point : boost::false_type
	{};

	template <typename T>
	struct is_fixed_point<T, typename T::fixed_point_identifier> : boost::true_type
	{};

	template < typename T, typename EnableIf = void >
	struct resolve_scale_policy
	{
		typedef void type;
	};

	template < typename T >
	struct resolve_scale_policy< fixed_point<T> >
	{
		typedef typename T::scale_policy type;
	};

    template <typename T, typename EnableIf=void>
    struct is_static : boost::false_type{};
    template <typename T>
    struct is_static<T, typename T::compile_time_category > : boost::true_type
    {};

    namespace detail
    {
        //! Helper struct for initializing from another fixed_point whose trait type is different.

        //! The default case uses conversion to double. This should happen if the radix or format is different.
        template <typename ToTraits, typename FromTraits, typename EnableIf = void>
        struct fixed_point_copy_ctor_helper
        {
            BOOST_CONSTEXPR fixed_point_copy_ctor_helper( const typename ToTraits::scale_policy& scalePolicy )
                : m_scale( scalePolicy )
            {}

            BOOST_CONSTEXPR typename ToTraits::format_type operator()( const fixed_point<FromTraits>& other ) const
            {
                return boost::numeric_cast<typename ToTraits::format_type>( ToTraits::rounding_policy::round( m_scale.template scale_up<double>( other.template convert_to<double>() ) ) );
            }

            const typename ToTraits::scale_policy& m_scale;
        };

        //! Initialize from the same type.
        template <typename Traits>
        struct fixed_point_copy_ctor_helper
        <
            Traits
          , Traits
          , typename boost::enable_if_c
            <
                geometrix::is_static<typename Traits::scale_policy>::value
            >::type
        >
        {
            BOOST_CONSTEXPR fixed_point_copy_ctor_helper( const typename Traits::scale_policy& )
            {}

            BOOST_CONSTEXPR typename Traits::format_type operator()( const fixed_point<Traits>& value ) const
            {
                return value.m_value;
            }
        };

        //! Initialize where both are compile time scales with same radix but different precision.
        template <typename ToTraits, typename FromTraits>
        struct fixed_point_copy_ctor_helper
        <
            ToTraits
          , FromTraits
          , typename boost::enable_if_c
            <
                geometrix::is_static< typename ToTraits::scale_policy >::value &&
                geometrix::is_static< typename FromTraits::scale_policy >::value &&
                boost::is_same< typename ToTraits::scale_policy, typename FromTraits::scale_policy >::value == false &&
                ToTraits::radix_type::value == FromTraits::radix_type::value
            >::type
        >
        {
            BOOST_CONSTEXPR fixed_point_copy_ctor_helper( const typename ToTraits::scale_policy& scalePolicy )
                : m_scale( scalePolicy )
            {}

            template <int a, int b>
            struct abs_diff
                : boost::mpl::if_c
                  <
                      (a > b)
                    , boost::mpl::int_<a-b>
                    , boost::mpl::int_<b-a>
                  >::type
            {};

            BOOST_CONSTEXPR typename ToTraits::format_type scale( const fixed_point<FromTraits>& other, boost::mpl::bool_<true>&, boost::mpl::bool_<true>& ) const
            {
                return m_scale.template scale_up
                    <
                        typename ToTraits::format_type
                    >( other.m_value, abs_diff<FromTraits::scale_policy::scale::value, ToTraits::scale_policy::scale::value>::value  );
            }

            BOOST_CONSTEXPR typename ToTraits::format_type scale( const fixed_point<FromTraits>& other, boost::mpl::bool_<false>&, boost::mpl::bool_<true>& ) const
            {
                return m_scale.template scale_down
                    <
                        typename ToTraits::format_type
                    >( other.m_value, abs_diff<ToTraits::scale_policy::scale::value, FromTraits::scale_policy::scale::value>::value );
            }

            BOOST_CONSTEXPR typename ToTraits::format_type operator()( const fixed_point<FromTraits>& other ) const
            {
                return scale( other
                    , boost::mpl::greater
                      <
                          typename ToTraits::scale_policy::scale
                        , typename FromTraits::scale_policy::scale
                      >::type()
                    , geometrix::is_static<typename ToTraits::scale_policy>::type() );
            }

            const typename ToTraits::scale_policy& m_scale;
        };

        //! Initialize where either is runtime scaled with same radix but different precision.
        template <typename ToTraits, typename FromTraits>
        struct fixed_point_copy_ctor_helper
        <
            ToTraits
          , FromTraits
          , typename boost::enable_if_c
            <
                ( !geometrix::is_static< typename ToTraits::scale_policy >::value ||
                  !geometrix::is_static< typename FromTraits::scale_policy >::value ) &&
                  ToTraits::radix_type::value == FromTraits::radix_type::value
            >::type
        >
        {
            BOOST_CONSTEXPR fixed_point_copy_ctor_helper( const typename ToTraits::scale_policy& scalePolicy )
                : m_scale( scalePolicy )
            {}

            BOOST_CONSTEXPR typename ToTraits::format_type operator()( const fixed_point<FromTraits>& other ) const
            {
                return ( m_scale.get_scale() > other.get_scale() ) 
                    ? m_scale.template scale_up<typename ToTraits::format_type>( other.m_value, std::abs( signed_cast( m_scale.get_scale() - other.get_scale() ) ) )
                    : m_scale.template scale_down<typename ToTraits::format_type>( other.m_value, std::abs( signed_cast( m_scale.get_scale() - other.get_scale() ) ) );
            }

            const typename ToTraits::scale_policy& m_scale;
        };
    }//! namespace detail;
}//! namespace geometrix;
#endif //GEOMETRIX_NUMERIC_DETAIL_FIXED_POINT_HPP

