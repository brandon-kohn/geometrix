//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_PROMOTION_POLICY_HPP
#define GEOMETRIX_NUMERIC_PROMOTION_POLICY_HPP

#include <geometrix/numeric/number_comparison_policy.hpp>
#include <geometrix/numeric/numeric_traits.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/version.hpp>
#include <boost/fusion/include/iter_fold.hpp>
#include <boost/mpl/max_element.hpp>

namespace geometrix {

    template <typename NumericType, typename EnableIf = void>
    struct arithmetic_promotion_policy
    {
        //! Define the promoted type.
        typedef NumericType type;

        static type          promote( const NumericType& i ){ return i; }
        static type          promote_div( const NumericType& numerator, const NumericType& denominator ){ return numerator / denominator; }
    
        template <typename T>
        static NumericType   demote( const T& real ) { return boost::numeric_cast<NumericType>(real); }
    };

#if !defined( GEOMETRIX_CUSTOM_INTEGRAL_ARITHMETIC_PROMOTION_POLICY )
    template <typename NumericType>
    struct arithmetic_promotion_policy
        <
           NumericType
         , typename boost::enable_if< typename numeric_traits<NumericType>::is_integral >::type 
        >
    {
        typedef double       type;

        static type          promote( const NumericType& i ){ return boost::numeric_cast<type>( i ); }
        static type          promote_div( const NumericType& numerator, const NumericType& denominator ){ return boost::numeric_cast<type>( numerator ) / boost::numeric_cast<type>( denominator ); }
        static NumericType   demote( const type& real ) { return boost::numeric_cast<NumericType>( real ); }
    };
#endif//GEOMETRIX_CUSTOM_INTEGRAL_ARITHMETIC_PROMOTION_POLICY

    //! Given a forward sequence of numeric types, decide on the type to use in arithmetic.
    template <typename NumericSequence>
    struct select_arithmetic_type_from_sequence
        : boost::mpl::max_element
            <
                NumericSequence
              , numeric_width_compare<boost::mpl::_1, boost::mpl::_2>
            >::type
    {};

    //! Given two numeric types, decide on the wider type to be used in arithmetic.
    template <typename T1, typename T2, typename EnableIf=void>
    struct select_arithmetic_type_from_2
        : boost::mpl::if_
            < 
                numeric_width_compare< T1, T2 > 
              , T1
              , T2
            >
    {};

    template <typename T>
    struct select_arithmetic_type_from_2< T, T >
    {
        typedef T type;
    };

    //! Given two numeric types, decide on the wider type to be used in arithmetic.
    template <typename T1, typename T2, typename EnableIf=void>
    struct select_arithmetic_type_from_sequences
    {
        typedef typename select_arithmetic_type_from_sequence<T1>::type A;
        typedef typename select_arithmetic_type_from_sequence<T2>::type B;
        typedef typename select_arithmetic_type_from_2<A,B>::type type;
    };

    //! \brief Access the arithmetic form of the specified coordinate from a numeric sequence of type T.
    template <typename T>
    inline typename arithmetic_promotion_policy<T>::type arithmetic_promote( const T& t )
    {        
        return arithmetic_promotion_policy<T>::promote(t);
    }

    template <typename T, typename U>
    inline T arithmetic_demote( const U& u )
    {        
        return arithmetic_promotion_policy<T>::demote(u);
    }
    
    template<typename T, typename EnableIf = void >
    struct widen
    {
        template <typename Type>
        struct NO_WIDEN_SPECIALIZATION_DEFINED
        {
            static const bool value = false;
        };
        BOOST_STATIC_ASSERT( NO_WIDEN_SPECIALIZATION_DEFINED<T>::value );
    };

    template<>
    struct widen<char>
    {
        typedef boost::int32_t type;
    };

    template<>
    struct widen<signed char>
    {
        typedef int type;
    };

    template<>
    struct widen<unsigned char>
    {
        typedef unsigned int type;
    };

    template<>
    struct widen<short>
    {
        typedef int type;
    };

    template<>
    struct widen<unsigned short>
    {
        typedef unsigned int type;
    };

    template<>
    struct widen<int> 
    {
        typedef long long type;
    };

    template<>
    struct widen<unsigned int>
    {
        typedef unsigned long long type;
    };

    //! Because of how boost::int32_t is defined in boost's cstdint.hpp ... have to define the opposite way here.
#if( BOOST_VERSION >= 104601 )
#define BOOST_INT32_T_IS_INT
#undef BOOST_INT32_T_IS_LONG
#else
#define BOOST_INT32_T_IS_LONG
#undef BOOST_INT32_T_IS_INT
#endif

#if defined( BOOST_INT32_T_IS_LONG )
    template<>
    struct widen<int> 
    {
        typedef long long type;
    };

    template<>
    struct widen<unsigned int>
    {
        typedef unsigned long long type;
    };
#endif

#if defined( BOOST_INT32_T_IS_INT )
    template<>
    struct widen<long> 
    {
        typedef long long type;
    };

    template<>
    struct widen<unsigned long>
    {
        typedef unsigned long long type;
    };
#endif

    template<>
    struct widen<long long>
    {
        typedef long long type;
    };

    template<>
    struct widen<unsigned long long>
    {
        typedef unsigned long long type;
    };

    template<>
    struct widen<float>
    {
        typedef double type;
    };

    template<>
    struct widen<double>
    {
        typedef long double type;
    };

    template<>
    struct widen<long double>
    {
        typedef long double type;
    };

    template <typename T>
    inline typename widen<T>::type widen_cast( T t ) { return boost::numeric_cast< typename widen<T>::type >( t ); }

    template <typename T>
    inline typename boost::make_signed<T>::type signed_cast( T t )
    {
        return static_cast< typename boost::make_signed<T>::type >( t );
    }

    template <typename T>
    inline typename boost::make_unsigned<T>::type unsigned_cast( T t )
    {
        return static_cast< typename boost::make_unsigned<T>::type >( t );
    }

}//namespace geometrix;

#endif //GEOMETRIX_NUMERIC_PROMOTION_POLICY_HPP
