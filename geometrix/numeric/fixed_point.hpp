//
//! Copyright © 2008-2013
//! Brandon Kohn, James Stewart
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_FIXED_POINT_HPP
#define GEOMETRIX_NUMERIC_FIXED_POINT_HPP

#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/utility/compile.hpp>

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/cstdint.hpp>
#include <limits>
#include <cmath>
#include <iomanip>
#include <locale>

namespace geometrix {

    template <typename T, typename EnableIf=void>
    struct is_compile_time : boost::false_type{};
    template <typename T>
    struct is_compile_time<T, typename T::compile_time_category > : boost::true_type
    {};

    template <typename T, typename EnableIf=void>
    struct is_run_time : boost::false_type{};
    template <typename T>
    struct is_run_time<T, typename T::run_time_category > : boost::true_type
    {};

    template <typename Format>
    struct binary_format_traits
    {
        typedef Format              format_type;
        typedef boost::mpl::int_<2> radix_type;

        BOOST_CONCEPT_ASSERT( (boost::is_integral<format_type>) );
    };

    template <typename Format>
    struct decimal_format_traits
    {
        typedef Format               format_type;        
        typedef boost::mpl::int_<10> radix_type;

        BOOST_CONCEPT_ASSERT( (boost::is_integral<format_type>) );        
    };

    template <int Radix, int F>
    struct generic_compile_time_scale_policy
    {
        typedef void compile_time_category;

        typedef boost::mpl::int_<F> scale;

        int get_scale() const { return scale::value; }

        //! Reverse the scale up operation.
        template <typename B, typename T>
        B scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * power_c<Radix, F>::value );
        }
        
        //! Reverse the scale up operation.
        template <typename T, typename B>
        T scale_down( B v ) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / power_c<Radix, F>::value );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        B scale_up( T v, int f ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow( Radix, f ) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        T scale_down( B v, int f ) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / integral_pow( Radix, f ) );
        }
    };

    //! Note: numeric_limits and boost::numeric_cast (range checking) do not work properly with runtime scaling.
    template <int Radix>
    struct generic_run_time_scale_policy
    {
        typedef void run_time_category;

        generic_run_time_scale_policy( unsigned int scale = 2 )
            : m_scale( scale )
        {}

        int get_scale() const { return m_scale; }
        
        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        B scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow( Radix, m_scale ) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        T scale_down( B v ) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / integral_pow( Radix, m_scale ) );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        B scale_up( T v, int F ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow( Radix, F ) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        T scale_down( B v, int F ) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / integral_pow( Radix, F ) );
        }

        int m_scale;
    };

    template <int F>
    struct binary_compile_time_scale_policy
    {
        typedef void compile_time_category;

        typedef boost::mpl::int_<F> scale;

        int get_scale() const { return scale::value; }

        //! Reverse the scale up operation.
        template <typename B, typename T>
        typename boost::enable_if< boost::is_integral<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << scale::value );
        }

        template <typename B, typename T>
        typename boost::enable_if< boost::is_float<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * power_c<2, scale::value>::value );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v ) const 
        {
            return boost::numeric_cast<T>( v >> scale::value );
        }

        template <typename T, typename B>
        typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v ) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / power_c<2, scale::value>::value );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        B scale_up( T v, int, typename boost::enable_if< boost::is_integral<T> >::type* d = 0 ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << F );
        }

        template <typename B, typename T>
        B scale_up( T v, int, typename boost::enable_if< boost::is_float<T> >::type* d = 0 ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow(2,F) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v, int) const
        {
            return boost::numeric_cast<T>( v >> F );
        }

        template <typename T, typename B>
        typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v, int) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / F );
        }
    };

    //! Note: numeric_limits and boost::numeric_cast (range checking) do not work properly with runtime scaling.
    struct binary_run_time_scale_policy
    {
        typedef void run_time_category;

        binary_run_time_scale_policy( unsigned int scale = 2 )
            : m_scale( scale )
        {}

        int get_scale() const { return m_scale; }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        typename boost::enable_if< boost::is_integral<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << m_scale );
        }

        template <typename B, typename T>
        typename boost::enable_if< boost::is_float<T>, B >::type scale_up( T v ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow(2,m_scale) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v ) const
        {
            return boost::numeric_cast<T>( v >> m_scale );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v ) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / m_scale );
        }

        //! Scale from T to B by a factor of Radix^scale.
        template <typename B, typename T>
        B scale_up( T v, int F, typename boost::enable_if< boost::is_integral<T> >::type* d = 0 ) const
        {
            return boost::numeric_cast<B>( widen_cast( v ) << F );
        }

        template <typename B, typename T>
        B scale_up( T v, int F, typename boost::enable_if< boost::is_float<T> >::type* d = 0 ) const 
        {
            return boost::numeric_cast<B>( widen_cast( v ) * integral_pow(2,F) );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v, int F ) const
        {
            return boost::numeric_cast<T>( v >> F );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v, int F ) const
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / F );
        }

        int m_scale;
    };

    //! This is also known as the Banker's Round.
    struct round_half_to_even
    {
        typedef boost::mpl::integral_c<std::float_round_style,std::round_to_nearest> round_style;

        template <typename T>
        static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {            
            T prev = std::floor(v);
            T next = std::ceil(v);
            T rt = (v - prev) - (next - v);
            const T zero(0.0);
            const T two(2.0);

            if ( rt < zero )
                return prev;
            else if ( rt > zero )
                return next;
            else
            {
                bool even = two * std::floor(prev / two) == prev;
                return ( even ? prev : next );
            }
        }
    };

    struct round_toward_zero
    {
        template <typename T>
        static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {
            return v >= static_cast<T>(0) ? std::floor(v) : std::ceil(v) ;
        }

        typedef boost::mpl::integral_c<std::float_round_style,std::round_toward_zero> round_style;
    } ;

    template<class S>
    struct round_toward_infinity
    {
        template <typename T>
        static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {
            return return std::ceil(v);
        }

        typedef boost::mpl::integral_c<std::float_round_style,std::round_toward_infinity> round_style;
    } ;

    template<class S>
    struct round_toward_neg_infinity
    {
        template <typename T>
        static typename boost::enable_if< typename boost::is_integral<T>::type, T >::type round( T v )
        {
            return v;
        }

        template <typename T>
        static typename boost::enable_if< typename boost::is_float<T>::type, T >::type round( T v )
        {
            return return std::floor(v);
        }

        typedef boost::mpl::integral_c<std::float_round_style,std::round_toward_neg_infinity> round_style;
    } ;

    template <typename FormatTraits, typename ScalePolicy, typename RoundingPolicy>
    struct fixed_point_traits
    {
        typedef FormatTraits                              format_traits;
        typedef ScalePolicy                               scale_policy;
        typedef RoundingPolicy                            rounding_policy;
        typedef typename format_traits::format_type       format_type;
        typedef typename format_traits::radix_type        radix_type;        
    };

    template< typename Traits >
    class fixed_point;

    template <typename T, typename EnableIf=void>
    struct is_fixed_point : boost::false_type{};

    template <typename Traits>
    struct is_fixed_point< fixed_point<Traits>, void > : boost::true_type{};

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

    //! Comparison
    #define GEOMETRIX_LESS_THAN_COMPARABLE_OPERATORS(T, U)                               \
    friend bool operator<=(const T& x, const U& y) { return !static_cast<bool>(x > y); } \
    friend bool operator>=(const T& x, const U& y) { return !static_cast<bool>(x < y); } \
    friend bool operator>(const U& x, const T& y)  { return y < x; }                     \
    friend bool operator<(const U& x, const T& y)  { return y > x; }                     \
    friend bool operator<=(const U& x, const T& y) { return !static_cast<bool>(y < x); } \
    friend bool operator>=(const U& x, const T& y) { return !static_cast<bool>(y > x); } \
    /***/

    #define GEOMETRIX_LESS_THAN_COMPARABLE_SELF_OPERATORS(T)                             \
    friend bool operator>(const T& x, const T& y)  { return y < x; }                     \
    friend bool operator<=(const T& x, const T& y) { return !static_cast<bool>(y < x); } \
    friend bool operator>=(const T& x, const T& y) { return !static_cast<bool>(x < y); } \
    /***/

    #define GEOMETRIX_EQUALITY_COMPARABLE_OPERATORS(T, U)                                \
    friend bool operator==(const U& y, const T& x) { return x == y; }                    \
    friend bool operator!=(const U& y, const T& x) { return !static_cast<bool>(x == y); }\
    friend bool operator!=(const T& y, const U& x) { return !static_cast<bool>(y == x); }\
    /***/

    #define GEOMETRIX_EQUALITY_COMPARABLE_SELF_OPERATORS(T)                  \
        friend bool operator!=(const T& x, const T& y)                       \
        { return !static_cast<bool>(x == y); }                               \
    /***/

    //!Arithmetic
    #define GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE( T, U, OP )                \
        friend T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }\
        friend T operator OP( const U& lhs, T rhs ) { return rhs OP##= lhs; }\
    /***/    

    #define GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE_SELF( T, OP )              \
        friend T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }\
    /***/

    #define GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE( T, U, OP )            \
        friend T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }\
        friend T operator OP( const U& lhs, const T& rhs )                   \
        { return T( lhs ) OP##= rhs; }                                       \
    /***/

    #define GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE_SELF( T, OP )          \
        friend T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }\
    /***/

    #define GEOMETRIX_INCREMENTABLE_OPERATOR(T)                              \
        friend T operator++(T& x, int)                                       \
        {                                                                    \
            T nrv(x);                                                        \
            ++x;                                                             \
            return nrv;                                                      \
        }                                                                    \
    /***/
    #define GEOMETRIX_DECREMENTABLE_OPERATOR(T)                              \
        friend T operator--(T& x, int)                                       \
        {                                                                    \
            T nrv(x);                                                        \
            --x;                                                             \
            return nrv;                                                      \
        }                                                                    \
    /***/

    #define GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(T, U)                \
    GEOMETRIX_LESS_THAN_COMPARABLE_OPERATORS(T, U)                           \
    GEOMETRIX_EQUALITY_COMPARABLE_OPERATORS(T, U)                            \
    GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE( T, U, * )                         \
    GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE( T, U, + )                         \
    GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE( T, U, - )                     \
    GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE(T, U, / )                      \
    /***/

    #define GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS_SELF(T)              \
    GEOMETRIX_LESS_THAN_COMPARABLE_SELF_OPERATORS(T)                         \
    GEOMETRIX_EQUALITY_COMPARABLE_SELF_OPERATORS(T)                          \
    GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE_SELF( T, * )                       \
    GEOMETRIX_BINARY_OPERATOR_COMMUTATIVE_SELF( T, + )                       \
    GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE_SELF( T, - )                   \
    GEOMETRIX_BINARY_OPERATOR_NON_COMMUTATIVE_SELF( T, / )                   \
    /***/

    namespace detail
    {
        //! Helper struct for initializing from another fixed_point whose trait type is different.

        //! The default case uses conversion to long double. This should happen if the radix or format is different.
        template <typename ToTraits, typename FromTraits, typename EnableIf = void>
        struct fixed_point_copy_ctor_helper
        {
            fixed_point_copy_ctor_helper( const typename ToTraits::scale_policy& scalePolicy )
                : m_scale( scalePolicy )
            {}

            typename ToTraits::format_type operator()( const fixed_point<FromTraits>& other )
            {
                //GEOMETRIX_STATIC_ASSERT( traits_type::radix_type::value != T::radix_type::value );
                return boost::numeric_cast<typename ToTraits::format_type>( 
                    ToTraits::rounding_policy::round( m_scale.scale_up< long double >( other.convert_to<long double>() ) ) );
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
                is_compile_time<typename Traits::scale_policy>::value
            >::type
        >
        {
            fixed_point_copy_ctor_helper( const typename Traits::scale_policy& )
            {}

            typename Traits::format_type operator()( const fixed_point<Traits>& value )
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
                is_compile_time< typename ToTraits::scale_policy >::value &&
                is_compile_time< typename FromTraits::scale_policy >::value &&
                boost::is_same< typename ToTraits::scale_policy, typename FromTraits::scale_policy >::value == false &&
                ToTraits::radix_type::value == FromTraits::radix_type::value
            >::type
        >
        {
            fixed_point_copy_ctor_helper( const typename ToTraits::scale_policy& scalePolicy )
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

            typename ToTraits::format_type scale( const fixed_point<FromTraits>& other, boost::mpl::bool_<true>&, boost::mpl::bool_<true>& )
            {
                return m_scale.scale_up
                    < 
                        typename ToTraits::format_type                      
                    >( other.m_value, abs_diff<FromTraits::scale_policy::scale::value, ToTraits::scale_policy::scale::value>::value  );
            }

            typename ToTraits::format_type scale( const fixed_point<FromTraits>& other, boost::mpl::bool_<false>&, boost::mpl::bool_<true>& )
            {
                return m_scale.scale_down
                    < 
                        typename ToTraits::format_type                      
                    >( other.m_value, abs_diff<ToTraits::scale_policy::scale::value, FromTraits::scale_policy::scale::value>::value );
            }

            typename ToTraits::format_type operator()( const fixed_point<FromTraits>& other )
            {
                return scale( other
                    , boost::mpl::greater
                      < 
                          typename ToTraits::scale_policy::scale
                        , typename FromTraits::scale_policy::scale 
                      >::type()
                    , is_compile_time<typename ToTraits::scale_policy>::type() );
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
                ( is_run_time< typename ToTraits::scale_policy >::value ||
                  is_run_time< typename FromTraits::scale_policy >::value ) &&
                  ToTraits::radix_type::value == FromTraits::radix_type::value
            >::type
        >
        {
            fixed_point_copy_ctor_helper( const typename ToTraits::scale_policy& scalePolicy )
                : m_scale( scalePolicy )
            {}

            typename ToTraits::format_type operator()( const fixed_point<FromTraits>& other )
            {
                if( m_scale.get_scale() > other.get_scale() )
                    return m_scale.scale_up<typename ToTraits::format_type>( other.m_value, std::abs( signed_cast( m_scale.get_scale() - other.get_scale() ) ) );
                else
                    return m_scale.scale_down<typename ToTraits::format_type>( other.m_value, std::abs( signed_cast( m_scale.get_scale() - other.get_scale() ) ) );
            }

            const typename ToTraits::scale_policy& m_scale;
        };
    }//namespace geometrix::detail;

    template< typename Traits >
    class fixed_point
        : public Traits::scale_policy
    {
    public:
        typedef Traits                                  traits_type;
        typedef typename traits_type::scale_policy      scale_policy;
        typedef typename traits_type::rounding_policy   rounding_policy;
        typedef typename traits_type::format_type       format_type;        

    private:

        //! Bool implicit conversion idiom
        typedef void (fixed_point<traits_type>::*bool_type)() const;
        void this_type_does_not_support_comparisons() const {}

        BOOST_CONCEPT_ASSERT( (boost::is_integral<format_type>) );

        friend class fixed_point;
        friend class std::numeric_limits< fixed_point< traits_type > >;

        //! Run time.
        template <typename T>
        format_type convert_to_format( T value, boost::false_type&, boost::false_type& ) const
        {
            return boost::numeric_cast<format_type>( rounding_policy::round( scale_up<T>( value ) ) );
        }

        //! Compile time.
        template <typename T>
        format_type convert_to_format( T value, boost::true_type&, boost::false_type& ) const
        {
            return boost::numeric_cast<format_type>( rounding_policy::round( scale_policy::scale_up<T>( value ) ) );
        }

        //! Conversion from another fixed_point type with possibly different traits.
        template <typename T1, typename T2, typename T3> friend struct detail::fixed_point_copy_ctor_helper;
        template <typename T, typename Category>
        format_type convert_to_format( const fixed_point<T>& other, Category&, boost::true_type& ) const
        {
            detail::fixed_point_copy_ctor_helper<traits_type, T> initer( (const scale_policy&)*this );
            return initer( other );
        }
        
        //! Access the scale policy as a reference (used for run time policies which have state). 
        template <typename T>
        typename boost::enable_if_c
            < 
                boost::is_same< typename boost::remove_const<T>::type, fixed_point< traits_type > >::value && 
                is_run_time< typename resolve_scale_policy< T >::type >::value,
                scale_policy&
            >::type scale_init( T value )
        {
            return static_cast<scale_policy&>( value );
        }

        //! For compile time scale policies there is no state, so just construct one.
        template <typename T>
        typename boost::disable_if_c
            < 
                boost::is_same< typename boost::remove_const<T>::type, fixed_point< traits_type > >::value && 
                is_run_time< typename resolve_scale_policy< T >::type >::value,
                scale_policy
            >::type scale_init( T value )
        {
            return scale_policy();
        }

    public:

        fixed_point( const scale_policy& p = scale_policy() )
            : scale_policy( p )
            , m_value( format_type() )
        {}
        
        template< typename V >
        fixed_point( V value, const scale_policy& p )
            : scale_policy( p )
            , m_value( convert_to_format( value, typename is_compile_time<scale_policy>::type(), is_fixed_point<V>() ) )
        {}

        template< typename T >
        fixed_point( const fixed_point<T>& value )
            : scale_policy( scale_init( value ) )
            , m_value( convert_to_format( value, typename is_compile_time<scale_policy>::type(), boost::true_type() ) )
        {}

        #define GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(T) \
        fixed_point(T value)                              \
            : scale_policy()                              \
            , m_value( convert_to_format(                 \
                  value,                                  \
                  typename is_compile_time                \
                  <                                       \
                      scale_policy                        \
                  >::type(), boost::false_type() ) )      \
        {}                                                \
        /***/

        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(unsigned char);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(signed char);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(char);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(unsigned short);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(short);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(unsigned int);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(int);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(unsigned long);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(long);
#if defined(BOOST_HAS_LONG_LONG)
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(unsigned long long);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(long long);
#endif
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(float);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(double);
        GEOMETRIX_FIXED_POINT_FUNDAMENTAL_CTOR(long double);

        template< typename V >
        fixed_point<traits_type>& operator =( const V& rhs )
        {
            format_type temp = convert_to_format( rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<V>() );
            std::swap(m_value,temp);
            return *this;
        }

        void swap( fixed_point<traits_type>& rhs )
        {
            std::swap( static_cast<scale_policy&>(*this), static_cast<scale_policy&>(rhs) );
            std::swap( m_value, rhs.m_value );
        }

        bool operator < ( const fixed_point<traits_type>& rhs ) const
        {
            return m_value < rhs.m_value;
        }

        template <typename T>
        bool operator < ( T rhs ) const
        {
            return m_value < convert_to_format( rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
        }

        template <typename T>
        bool operator > ( T rhs ) const
        {
            return m_value > convert_to_format( rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
        }

        bool operator ==( const fixed_point<traits_type>& rhs ) const
        {
            return m_value == rhs.m_value; 
        }

        template <typename T>
        bool operator == ( T rhs ) const
        {
            return m_value == convert_to_format( rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
        }

        fixed_point<traits_type> operator -() const
        {
            fixed_point<traits_type> result(*this);
            result.m_value = -m_value;
            return result;
        }

        fixed_point<traits_type>& operator ++()
        {
            m_value += scale_policy::scale_up< format_type >( 1 );
            return *this;
        }

        fixed_point<traits_type>& operator --()
        {
            m_value -= scale_policy::scale_up< format_type >( 1 );
            return *this;
        }

        fixed_point<traits_type>& operator +=( const fixed_point<traits_type>& v )
        {
            m_value += v.m_value;
            return *this;
        }

        template <typename T>
        fixed_point<traits_type>& operator +=( T v )
        {
            m_value += convert_to_format( v, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
            return *this;
        }

        fixed_point<traits_type>& operator -=( const fixed_point<traits_type>& v )
        {
            m_value -= v.m_value;
            return *this;
        }

        template <typename T>
        fixed_point<traits_type>& operator -=( T v )
        {
            m_value -= convert_to_format( v, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
            return *this;
        }

        fixed_point<traits_type>& operator *= ( const fixed_point<traits_type>& factor )
        {
            m_value = scale_policy::scale_down< format_type >( widen_cast(m_value) * factor.m_value );
            return *this;
        }

        template <typename T>
        fixed_point<traits_type>& operator *=( T v )
        {
            return (*this) *= fixed_point<traits_type>(v, *this);
        }

        fixed_point<traits_type>& operator /= (const fixed_point<traits_type>& divisor)
        {
            m_value = boost::numeric_cast< format_type >( scale_policy::scale_up< widen<format_type>::type >( m_value ) / widen_cast( divisor.m_value ) );
            return *this;
        }

        template <typename T>
        fixed_point<traits_type>& operator /=( T v )
        {
            return (*this) /= fixed_point<traits_type>(v, *this);
        }

        template <typename T> 
        T convert_to() const
        {			
            return scale_policy::scale_down<T>( m_value );
        }

		template <> 
		bool convert_to<bool>() const
		{
			return m_value != 0;
		}

        //! Access the epsilon (useful for runtime scales).
        fixed_point< traits_type > epsilon() const
        {
            fixed_point< traits_type > e( *this );
            e.m_value = 1;
            return e;
        }

        //! Useful for runtimes
        fixed_point< traits_type > min BOOST_PREVENT_MACRO_SUBSTITUTION () const
        {
            fixed_point<traits_type> minimum( *this );
            minimum.m_value = (std::numeric_limits<format_type>::min)();
            return minimum;
        }

        //! Useful for runtimes
        fixed_point< traits_type > max BOOST_PREVENT_MACRO_SUBSTITUTION () const
        {
            fixed_point< traits_type > maximum( *this );
            maximum.m_value = (std::numeric_limits<format_type>::max)();
            return maximum;
        }

        int get_scale() const
        {
            return scale_policy::get_scale();
        }

#if defined(GEOMETRIX_ALLOW_FIXEDPOINT_IMPLICIT_CONVERSIONS)
		template <typename T>
		operator T() const 
		{
			#pragma GEOMETRIX_WARNING("Implicitly converting fixed_point to another numeric type.")
            int ImplicitlyConvertingFixedPointWarning = (double)0.0;//Allow the compiler to put a stack warning for this implicit conversion.
			return convert_to<T>();
		}
#endif

        bool operator !() const
        {
            return m_value == 0; 
        }

        operator bool_type() const 
        {
            return m_value ? &fixed_point::this_type_does_not_support_comparisons : 0;
        }

		//! For when it's useful to have the true scaled value (an example is assigning discrete ranges to user interface elements)
		format_type get_scaled_value() const
		{
			return m_value;
		}

		//!
		//! Convert to a string, avoiding precision loss inherent with a conversion to double before formatting
		//!
		//! Precision limit allows for the decimal precision of the resulting string to be controlled regardless of scale. Default -1 means precision matches scale.
		//! Locale-sensitive so the decimal point is correct according to the specified locale. If locale is not specified the global locale is assumed.
		//! Leading zero is added by default when the decimal point would be placed at the start of the resulting string.
		std::string to_string( std::string::size_type precisionLimit = -1, const std::locale& localeInfo = std::locale(), bool addLeadingZero = true ) const
		{
			// Format the inital string with the decimal representation of the value
			std::string output( boost::str( boost::format( "%d" ) % m_value ) );

			// Pad with zeros as required e.g. where scale is 3, but value is 5 we want to output to return as (assuming leading zero) 0.005 rather than 0.5
			std::string::size_type scale = ( std::string::size_type )get_scale();
			if( output.size() < scale )
			{
				output.insert( 0, scale - output.size(), '0' );
			}

			// Insert decimal point character appropriate to locale
			std::string::size_type decimalInsertPos = output.size() - scale;
			GEOMETRIX_ASSERT( decimalInsertPos >= 0 );
			if( decimalInsertPos >=0 )
			{
				std::string decimalPoint( 1, std::use_facet< std::numpunct< char > >( localeInfo ).decimal_point() );
				output.insert( decimalInsertPos, decimalPoint );

				// Add a leading zero if placing a decimal at the start of the string
				if( decimalInsertPos == 0 && addLeadingZero )
				{
					output.insert( 0, "0" );
				}
			}

			// Handle precision limit if required
			if( precisionLimit != -1 )
			{
				if( precisionLimit > scale )
				{
					// add trailing zeros to increase precision
					output.insert( output.size(), "0", precisionLimit - scale );
				}
				else if( precisionLimit < ( int )scale )
				{
					// trim string to remove excess precision
					output.erase( output.size() - ( scale - precisionLimit ), scale - precisionLimit );
				}
				// no changes necessary where precisionLimit == scale
			}

			return output;
		}

        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, long double);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, double);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, float);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, char);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, signed char);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, unsigned char);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, short);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, unsigned short);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, int);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, unsigned int);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, long);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, unsigned long);
        #if defined(BOOST_HAS_LONG_LONG)
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, long long);
        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS(fixed_point<Traits>, unsigned long long);
        #endif

        GEOMETRIX_IMPLEMENT_ORDERED_FIELD_OPERATORS_SELF(fixed_point<Traits>);
        GEOMETRIX_INCREMENTABLE_OPERATOR(fixed_point<Traits>);
        GEOMETRIX_DECREMENTABLE_OPERATOR(fixed_point<Traits>);

    private:

		//friend class boost::serialization::access;

		//template <typename Archive>
		//void serialize(Archive& ar, unsigned int v)
		//{
		//	ar & m_value;
		//}

        format_type m_value;

    };

    template<typename Traits>
    std::istream& operator >> ( std::istream& is, fixed_point<Traits>& v )
    {
        double value=0.;
        is >> value;
        if(is)
            v = value;
        return is;
    }

    template<typename Traits>
    std::ostream& operator << ( std::ostream& os, const fixed_point<Traits>& v )
    {
        double value = v.convert_to<double>();
        os << std::setprecision( std::numeric_limits< fixed_point<Traits> >::digits10 ) << value;
        return os;
    }

}//namespace geometrix;

namespace std
{
	#define GEOMETRIX_DEFINE_STD_MATH_FUNCTION(fn)                                 \
		template <typename Traits>                                                 \
		geometrix::fixed_point<Traits> fn(const geometrix::fixed_point<Traits>& v) \
		{                                                                          \
			double vd = v.convert_to<double>();                                    \
			vd = std:: fn (vd);                                                    \
			return geometrix::fixed_point<Traits>(vd);                             \
		}                                                                          \
	/***/

	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(sqrt)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(cos)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(sin)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(tan)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(atan)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(acos)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(asin)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(exp)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(log10)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(log)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(ceil)
	GEOMETRIX_DEFINE_STD_MATH_FUNCTION(floor)

	#undef GEOMETRIX_DEFINE_STD_MATH_FUNCTION

	template <typename Traits>
	geometrix::fixed_point<Traits> fabs(const geometrix::fixed_point<Traits>& v)
	{
		if( v > 0 )
			return v;
		else
			return -v;
	}

	template <typename Traits>
	geometrix::fixed_point<Traits> abs(const geometrix::fixed_point<Traits>& v)
	{
		if( v > 0 )
			return v;
		else
			return -v;
	}

    template< typename Traits >
    class numeric_limits< geometrix::fixed_point<Traits> >
    {
    public:

        typedef geometrix::fixed_point<Traits> fixed_point_type;
        static const float_denorm_style has_denorm = denorm_absent;
        static const bool has_denorm_loss = false;
        static const bool has_infinity = false;
        static const bool has_quiet_NaN = false;
        static const bool has_signaling_NaN = false;
        static const bool is_bounded = true;
        static const bool is_exact = true;
        static const bool is_iec559 = false;
        static const bool is_integer = false;
        static const bool is_modulo = false;
        static const bool is_signed = 
            std::numeric_limits<typename fixed_point_type::format_type>::is_signed;
        static const bool is_specialized = true;
        static const bool tinyness_before = false;
        static const bool traps = false;
        static const float_round_style round_style = fixed_point_type::traits_type::rounding_policy::round_style::value;
        static const int digits = std::numeric_limits<fixed_point_type::format_type>::digits;
        static const int digits10 = std::numeric_limits<fixed_point_type::format_type>::digits10;
        static const int max_exponent = 0;
        static const int max_exponent10 = 0;
        static const int min_exponent = 0;
        static const int min_exponent10 = 0;
        static const int radix = fixed_point_type::traits_type::radix_type::value;

        static fixed_point_type (min)()
        {
            fixed_point_type minimum;
            minimum.m_value = (std::numeric_limits<typename fixed_point_type::format_type>::min)();
            return minimum;
        }

        static fixed_point_type (max)()
        {
            fixed_point_type maximum;
            maximum.m_value = (std::numeric_limits<typename fixed_point_type::format_type>::max)();
            return maximum;
        }

        static fixed_point_type epsilon()
        {
            fixed_point_type one( 1 );
            fixed_point_type one_plus_epsilon( 1 );
            one_plus_epsilon.m_value += 1;
            return one_plus_epsilon - one;
        }

        static fixed_point_type round_error()
        {
            return (fixed_point_type)(0.5);
        }

        static fixed_point_type denorm_min()
        {
            return (fixed_point_type)(0);
        }

        static fixed_point_type infinity()
        {
            return (fixed_point_type)(0);
        }

        static fixed_point_type quiet_NaN()
        {
            return (fixed_point_type)(0);
        }

        static fixed_point_type signaling_NaN()
        {
            return (fixed_point_type)(0);
        }
    };
}

namespace boost 
{
    namespace numeric 
    {
        template<typename Traits>
        struct bounds< geometrix::fixed_point<Traits> >
        {
            static geometrix::fixed_point<Traits> lowest  () { return (std::numeric_limits< geometrix::fixed_point<Traits> >::min)(); }
            static geometrix::fixed_point<Traits> highest () { return (std::numeric_limits< geometrix::fixed_point<Traits> >::max)(); }
            static geometrix::fixed_point<Traits> smallest() { return std::numeric_limits< geometrix::fixed_point<Traits> >::epsilon(); }
        };

        template<typename Traits, typename T>
        struct raw_converter< conversion_traits< T, geometrix::fixed_point<Traits> > >
        {
            static T low_level_convert ( const geometrix::fixed_point<Traits>& n )
            {
                return n.convert_to<T>(); 
            }
        } ;

        template<typename Traits, typename T>
        struct raw_converter< conversion_traits< geometrix::fixed_point<Traits>, T > >
        {
            static geometrix::fixed_point<Traits> low_level_convert ( T n )
            {
                return geometrix::fixed_point<Traits>( n );
            }
        } ;
    } // namespace numeric
} // namespace boost

#endif //GEOMETRIX_NUMERIC_FIXED_POINT_HPP
