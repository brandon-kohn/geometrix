//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_FIXED_POINT_HPP
#define GEOMETRIX_NUMERIC_FIXED_POINT_HPP

#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/utility/utilities.hpp>

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/operators.hpp>
#include <boost/concept_check.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/cstdint.hpp>
#include <limits>
#include <cmath>

namespace geometrix {

    template <typename Base>
    struct binary_format_traits
    {
        typedef Base                format_type;
        typedef boost::mpl::int_<2> radix_type;

        BOOST_CONCEPT_ASSERT( (boost::is_integral<format_type>) );
    };

    template <typename Base>
    struct decimal_format_traits
    {
        typedef Base                 format_type;        
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
        static B scale_up( T v ) 
        {
            return boost::numeric_cast<B>( widen_cast( v ) * power<Radix, scale::value>::value );
        }
        
        //! Reverse the scale up operation.
        template <typename T, typename B>
        static T scale_down( B v ) 
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / power<Radix, scale::value>::value );
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
    };

    //! Note: numeric_limits and boost::numeric_cast (range checking) do not work properly with runtime scaling.
    template <int Radix>
    struct generic_run_time_scale_policy
    {
        typedef run_time_category category;

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
        static typename boost::enable_if< boost::is_integral<T>, B >::type scale_up( T v ) 
        {
            return boost::numeric_cast<B>( widen_cast( v ) << scale::value );
        }

        template <typename B, typename T>
        static typename boost::enable_if< boost::is_float<T>, B >::type scale_up( T v )
        {
            return boost::numeric_cast<B>( widen_cast( v ) * power<2, scale::value>::value );
        }

        //! Reverse the scale up operation.
        template <typename T, typename B>
        static typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v ) 
        {
            return boost::numeric_cast<T>( v >> scale::value );
        }

        template <typename T, typename B>
        static typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v ) 
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / power<2, scale::value>::value );
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
        static typename boost::enable_if< boost::is_integral<T>, T >::type scale_down( B v, int F ) 
        {
            return boost::numeric_cast<T>( v >> F );
        }

        template <typename T, typename B>
        static typename boost::enable_if< boost::is_float<T>, T >::type scale_down( B v, int F ) 
        {
            return boost::numeric_cast<T>( boost::numeric_cast<typename widen<T>::type>( v ) / F );
        }
    };

    //! Note: numeric_limits and boost::numeric_cast (range checking) do not work properly with runtime scaling.
    struct binary_run_time_scale_policy
    {
        typedef run_time_category category;

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

    template <typename T>
    struct is_run_time< fixed_point<T>, typename boost::enable_if< is_run_time< typename resolve_scale_policy<fixed_point<T> >::type> >::type > : boost::true_type{};
    template <typename T>
    struct is_compile_time
        <
            fixed_point<T>
          , typename boost::enable_if
            < 
                is_compile_time<typename resolve_scale_policy< fixed_point<T> >::type > 
            >::type 
        > : boost::true_type{};

    template< typename Traits >
    class fixed_point
        : Traits::scale_policy
        , boost::ordered_field_operators
        <
            fixed_point< Traits >
          , boost::unit_steppable< fixed_point< Traits >
          , boost::ordered_field_operators2< fixed_point< Traits >, long double
          , boost::ordered_field_operators2< fixed_point< Traits >, double
          , boost::ordered_field_operators2< fixed_point< Traits >, float
          , boost::ordered_field_operators2< fixed_point< Traits >, int
          , boost::ordered_field_operators2< fixed_point< Traits >, unsigned int
          , boost::ordered_field_operators2< fixed_point< Traits >, long
          , boost::ordered_field_operators2< fixed_point< Traits >, unsigned long
          , boost::ordered_field_operators2< fixed_point< Traits >, boost::int64_t
          , boost::ordered_field_operators2< fixed_point< Traits >, unsigned boost::int64_t
          , boost::ordered_field_operators2< fixed_point< Traits >, char
          , boost::ordered_field_operators2< fixed_point< Traits >, unsigned char
          , boost::ordered_field_operators2< fixed_point< Traits >, short
          , boost::ordered_field_operators2< fixed_point< Traits >, unsigned short
        > > > > > > > > > > > > > > >
    {
    public:
        typedef Traits                                  traits_type;
        typedef typename traits_type::scale_policy      scale_policy;
        typedef typename traits_type::rounding_policy   rounding_policy;
        typedef typename traits_type::format_type       base_type;        

    private:

        BOOST_CONCEPT_ASSERT( (boost::is_integral<base_type>) );

        friend class fixed_point;
        friend class std::numeric_limits< fixed_point< traits_type > >;

        //! Run time.
        template <typename T>
        base_type convert_to_format( const scale_policy& p, T value, bosot::false_type&, boost::false_type& ) const
        {
            return boost::numeric_cast<base_type>( rounding_policy::round( p.scale_up<T>( value ) ) );
        }

        //! Compile time.
        template <typename T>
        base_type convert_to_format( const scale_policy&, T value, boost::true_type&, boost::false_type& ) const
        {
            return boost::numeric_cast<base_type>( rounding_policy::round( scale_policy::scale_up<T>( value ) ) );
        }

        template <int a, int b>
        struct abs_diff
        {
            typedef boost::mpl::int_<a> A;
            typedef boost::mpl::int_<b> B;
            static const int value = boost::mpl::if_c
                <
                    boost::mpl::greater<A,B>::value,
                    boost::mpl::int_<a - b>,
                    boost::mpl::int_<b - a> 
                >::value;
        };

        template < typename T, typename U, typename EnableIf = void >
        struct is_same_scale : boost::false_type {};

        template < typename T >
        struct is_same_scale
            <
                scale_policy,
                T, 
                typename boost::enable_if_c
                <
                    is_compile_time<scale_policy>::value && is_compile_time< T >::value &&
                    boost::is_same< scale_policy, T >::value
                >::type 
            > : boost::true_type
        {};

        //! Helper struct for initializing from another fixed_point whose trait type is different.
        template < typename T, typename EnableIf = void >
        struct Initializer{};

        //! Initialize from the same type.
        template <typename T>
        struct Initializer
            <
                T
            ,   typename boost::enable_if_c
                <
                    boost::is_same< traits_type, T >::value &&
                    is_compile_time< scale_policy >::value
                >::type
            >
        {
            Initializer( const scale_policy& )
            {}

            base_type operator()( const fixed_point<traits_type>& value )
            {
                return value.m_value;
            }               
        };

        //! Initialize with same radix but different fractional precision.
        template <typename T>
        struct Initializer
            <
                T
            ,   typename boost::lazy_enable_if_c
                <
                    is_compile_time< scale_policy >::value &&
                    is_compile_time< typename T::scale_policy >::value &&
                    !is_same_scale< scale_policy, typename T::scale_policy >::value &&
                    traits_type::radix_type::value == T::radix_type::value,
                    void
                >::type
            >
        {
            Initializer( const scale_policy& )
            {}

            base_type scale( const fixed_point<T>& other, boost::mpl::bool_<true>&, compile_time_category& )
            {
                return scale_policy::scale_up< base_type, abs_diff<T::scale::value, scale::value >::value >( other.m_value );
            }

            base_type scale( const fixed_point<T>& other, boost::mpl::bool_<false>&, compile_time_category& )
            {
                return scale_policy::scale_down< base_type, abs_diff<scale::value, T::scale::value>::value >( other.m_value );
            }

            base_type operator()( const fixed_point<T>& other )
            {
                return scale( other, boost::mpl::greater< scale, T::scale >::type(), scale_policy::category );                    
            }
        };

        //! Initialize with same radix but different fractional precision.
        template <typename T>
        struct Initializer
            <
                T
            ,   typename boost::enable_if_c
                <
                    ( is_run_time< scale_policy >::value ||
                      is_run_time< typename T::scale_policy >::value ) &&
                    traits_type::radix_type::value == T::radix_type::value
                >::type
            >
        {
            Initializer( const scale_policy& scalePolicy )
                : m_scale( scalePolicy )
            {}

            base_type operator()( const fixed_point<T>& other )
            {
                if( m_scale.get_scale() > other.get_scale() )
                    return m_scale.scale_up< base_type >( other.m_value, std::abs( signed_cast( m_scale.get_scale() - other.get_scale() ) ) );
                else
                    return m_scale.scale_down< base_type >( other.m_value, std::abs( signed_cast( m_scale.get_scale() - other.get_scale() ) ) );
            }

            const scale_policy& m_scale;
        };

        //! Initialize with different radix.
        template <typename T>
        struct Initializer
            <
                T
            ,   typename boost::enable_if_c
                <
                    traits_type::radix_type::value != T::radix_type::value
                >::type
            >
        {
            Initializer( const scale_policy& scalePolicy )
                : m_scale( scalePolicy )
            {}

            base_type operator()( const fixed_point<T>& other )
            {
                BOOST_STATIC_ASSERT( traits_type::radix_type::value != T::radix_type::value );
                return boost::numeric_cast<base_type>( rounding_policy::round( m_scale.scale_up< long double >( other.convert_to<long double>() ) ) );
            }

            const scale_policy& m_scale;
        };

        template <typename T, typename Category>
        base_type convert_to_format( const scale_policy& scalePolicy, const fixed_point<T>& other, Category&, boost::true_type& ) const
        {
            return Initializer<T>( scalePolicy )( other );
        }

        template <typename T>
        typename boost::enable_if_c
            < 
                boost::is_same< T, fixed_point< traits_type > >::value && 
                is_run_time< typename resolve_scale_policy< T >::type >::value,
                scale_policy&
            >::type scale_init( T value )
        {
            return static_cast<scale_policy&>( value );
        }

        template <typename T>
        typename boost::disable_if_c
            < 
                boost::is_same< T, fixed_point< traits_type > >::value && 
                is_run_time< typename resolve_scale_policy< T >::type >::value,
                scale_policy
            >::type scale_init( T value )
        {
            return scale_policy();
        }

    public:

        fixed_point( const scale_policy& p = scale_policy() )
            : scale_policy( p )
            , m_value( base_type() )
        {}
        
        template< typename V >
        fixed_point( V value, const scale_policy& p )
            : scale_policy( p )
            , m_value( convert_to_format( p, value, typename is_compile_time<scale_policy>::type(), is_fixed_point<V>() ) )
        {}

        template< typename T >
        fixed_point( const fixed_point<T>& value )
            : scale_policy( scale_init( value ) )
            , m_value( convert_to_format( scale_init( value ), value, typename is_compile_time<scale_policy>::type(), boost::true_type() ) )
        {}

        template< typename V >
        fixed_point( V value, typename boost::enable_if_c< boost::is_float<V>::value || boost::is_integral<V>::value >::type* dummy = 0 )
            : scale_policy( scale_init( value ) )
            , m_value( convert_to_format( scale_init( value ), value, typename is_compile_time<scale_policy>::type(), is_fixed_point<V>() ) )
        {}

        template< typename V >
        fixed_point<traits_type>& operator =( const V& rhs )
        {
            base_type temp = convert_to_format( *this, rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<V>() );
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
            return m_value < convert_to_format( *this, rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
        }

        template <typename T>
        bool operator > ( T rhs ) const
        {
            return m_value > convert_to_format( *this, rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
        }

        bool operator ==( const fixed_point<traits_type>& rhs ) const
        {
            return m_value == rhs.m_value; 
        }

        template <typename T>
        bool operator == ( T rhs ) const
        {
            return m_value == convert_to_format( *this, rhs, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
        }

        bool operator !() const
        {
            return m_value == 0; 
        }

        fixed_point<traits_type> operator -() const
        {
            fixed_point<traits_type> result(*this);
            result.m_value = -m_value;
            return result;
        }

        fixed_point<traits_type>& operator ++()
        {
            m_value += scale_policy::scale_up< base_type >( 1 );
            return *this;
        }

        fixed_point<traits_type>& operator --()
        {
            m_value -= scale_policy::scale_up< base_type >( 1 );
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
            m_value += convert_to_format( *this, v, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
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
            m_value -= convert_to_format( *this, v, typename is_compile_time<scale_policy>::type(), is_fixed_point<T>() );
            return *this;
        }

        fixed_point<traits_type>& operator *= ( const fixed_point<traits_type>& factor )
        {
            m_value = scale_policy::scale_down< base_type >( widen_cast(m_value) * factor.m_value );
            return *this;
        }

        template <typename T>
        fixed_point<traits_type>& operator *=( T v )
        {
            return (*this) *= fixed_point<traits_type>(v, *this);
        }

        fixed_point<traits_type>& operator /= (const fixed_point<traits_type>& divisor)
        {
            m_value = boost::numeric_cast< base_type >( scale_policy::scale_up< widen<base_type>::type >( m_value ) / widen_cast( divisor.m_value ) );
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

        //! Access the epsilon (useful for runtime scales).
        fixed_point< traits_type > epsilon() const
        {
            fixed_point< traits_type > one( 1, *this );
            fixed_point< traits_type > one_plus_epsilon( one, *this );
            one_plus_epsilon.m_value += 1;
            return fixed_point< traits_type >( one_plus_epsilon - one, *this );
        }

        //! Useful for runtimes
        fixed_point< traits_type > min BOOST_PREVENT_MACRO_SUBSTITUTION () const
        {
            fixed_point<traits_type> minimum( *this );
            minimum.m_value = (std::numeric_limits<base_type>::min)();
            return minimum;
        }

        //! Useful for runtimes
        fixed_point< traits_type > max BOOST_PREVENT_MACRO_SUBSTITUTION () const
        {
            fixed_point< traits_type > maximum( *this );
            maximum.m_value = (std::numeric_limits<base_type>::max)();
            return maximum;
        }

        int get_scale() const
        {
            return scale_policy::get_scale();
        }

        operator bool() const
        {
            return !m_value;
        }

    private:

        base_type m_value;

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
            std::numeric_limits<typename fixed_point_type::base_type>::is_signed;
        static const bool is_specialized = true;
        static const bool tinyness_before = false;
        static const bool traps = false;
        static const float_round_style round_style = fixed_point_type::traits_type::rounding_policy::round_style::value;
        static const int digits = std::numeric_limits<fixed_point_type::base_type>::digits;
        static const int digits10 = std::numeric_limits<fixed_point_type::base_type>::digits10;
        static const int max_exponent = 0;
        static const int max_exponent10 = 0;
        static const int min_exponent = 0;
        static const int min_exponent10 = 0;
        static const int radix = fixed_point_type::traits_type::radix_type::value;

        static fixed_point_type (min)()
        {
            fixed_point_type minimum;
            minimum.m_value = (std::numeric_limits<typename fixed_point_type::base_type>::min)();
            return minimum;
        }

        static fixed_point_type (max)()
        {
            fixed_point_type maximum;
            maximum.m_value = (std::numeric_limits<typename fixed_point_type::base_type>::max)();
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
