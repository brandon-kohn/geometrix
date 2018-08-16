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
#include <geometrix/numeric/detail/friend_ordered_field_operators.hpp>
#include <geometrix/numeric/detail/arithmetic_operators.hpp>
#include <geometrix/numeric/detail/fixed_point.hpp>
#include <geometrix/numeric/fixed_point_traits.hpp>

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

    template<typename Traits>
    class fixed_point : public Traits::scale_policy
    {
    public:
        using traits_type = Traits;
        using scale_policy = typename traits_type::scale_policy;
        using rounding_policy = typename traits_type::rounding_policy;
        using format_type = typename traits_type::format_type;
        using fixed_point_identifier = void;

    private:

        //! Bool implicit conversion idiom
        typedef void (fixed_point<traits_type>::*bool_type)() const;
        void this_type_does_not_support_comparisons() const {}

        BOOST_CONCEPT_ASSERT( (boost::is_integral<format_type>) );

        friend class std::numeric_limits< fixed_point< traits_type > >;

        //! Run time.
        template <typename T, typename std::enable_if<std::is_arithmetic<typename std::decay<T>::type>::value, int>::type = 0>
        BOOST_CONSTEXPR format_type convert_to_format(T value, boost::false_type) const
        {
            return boost::numeric_cast<format_type>( rounding_policy::round( scale_policy::template scale_up<T>( value ) ) );
        }

        //! Compile time.
        template <typename T, typename std::enable_if<std::is_arithmetic<typename std::decay<T>::type>::value, int>::type = 0>
        BOOST_CONSTEXPR format_type convert_to_format(T value, boost::true_type) const
        {
            return boost::numeric_cast<format_type>( rounding_policy::round( scale_policy::template scale_up<T>( value ) ) );
        }

        //! Conversion from another fixed_point type with possibly different traits.
        template <typename T1, typename T2, typename T3> friend struct detail::fixed_point_copy_ctor_helper;
        template <typename T, typename Category>
        BOOST_CONSTEXPR format_type convert_to_format(const fixed_point<T>& other, Category) const
        {
            detail::fixed_point_copy_ctor_helper<traits_type, T> initer( (const scale_policy&)*this );
            return initer( other );
        }

        //! Access the scale policy as a reference (used for run time policies which have state).
        template <typename T>
        BOOST_CONSTEXPR typename boost::enable_if_c
            <
                boost::is_same< typename boost::remove_const<T>::type, fixed_point< traits_type > >::value &&
                !geometrix::is_static< typename resolve_scale_policy< T >::type >::value,
                scale_policy&
            >::type scale_init( T&& value )
        {
            return static_cast<scale_policy&>( value );
        }

        //! For compile time scale policies there is no state, so just construct one.
        template <typename T>
        BOOST_CONSTEXPR typename boost::disable_if_c
            <
                boost::is_same< typename boost::remove_const<T>::type, fixed_point< traits_type > >::value &&
                !geometrix::is_static< typename resolve_scale_policy< T >::type >::value,
                scale_policy
            >::type scale_init( T value )
        {
            return scale_policy();
        }

        struct from_format_tag{};

        BOOST_CONSTEXPR fixed_point(from_format_tag, const format_type& v, const scale_policy& p = scale_policy() )
            : scale_policy( p )
            , m_value( v )
        {}

    public:

        BOOST_CONSTEXPR static fixed_point from_format_value(const format_type& v, const scale_policy&p = scale_policy())
        {
            return fixed_point(from_format_tag(), v, p);
        }

        BOOST_CONSTEXPR fixed_point()
            : scale_policy()
            , m_value()
        {}

        BOOST_CONSTEXPR fixed_point( const scale_policy& p )
            : scale_policy( p )
            , m_value()
        {}

        template< typename V >
        BOOST_CONSTEXPR fixed_point(const V& value, const scale_policy& p )
            : scale_policy( p )
            , m_value(convert_to_format(value, boost::false_type()))
        {}

        template< typename T >
        BOOST_CONSTEXPR fixed_point( const fixed_point<T>& value )
            : scale_policy( scale_init( value ) )
            , m_value(convert_to_format(value, typename geometrix::is_static<scale_policy>::type()))
        {}

        template <typename T, typename std::enable_if<std::is_arithmetic<typename std::decay<T>::type>::value, int>::type = 0>
        BOOST_CONSTEXPR fixed_point(const T& value)
            : scale_policy()
            , m_value(convert_to_format(value, typename geometrix::is_static<scale_policy>::type()))
        {}

        template< typename V >
        BOOST_NO_CXX14_CONSTEXPR fixed_point<traits_type>& operator =( const V& rhs )
        {
            using std::swap;
            format_type temp = convert_to_format( rhs, typename geometrix::is_static<scale_policy>::type() );
            swap(m_value,temp);
            return *this;
        }

        BOOST_CONSTEXPR void swap( fixed_point<traits_type>& rhs )
        {
            using std::swap;
            swap( static_cast<scale_policy&>(*this), static_cast<scale_policy&>(rhs) );
            swap( m_value, rhs.m_value );
        }

        BOOST_CONSTEXPR bool operator < ( const fixed_point<traits_type>& rhs ) const
        {
            return m_value < rhs.m_value;
        }

        template <typename T>
        BOOST_CONSTEXPR bool operator < ( T rhs ) const
        {
            return m_value < convert_to_format( rhs, typename geometrix::is_static<scale_policy>::type() );
        }

        template <typename T>
        BOOST_CONSTEXPR bool operator > ( T rhs ) const
        {
            return m_value > convert_to_format( rhs, typename geometrix::is_static<scale_policy>::type() );
        }

        BOOST_CONSTEXPR bool operator ==( const fixed_point<traits_type>& rhs ) const
        {
            return m_value == rhs.m_value;
        }

        template <typename T>
        BOOST_CONSTEXPR bool operator == ( T rhs ) const
        {
            return m_value == convert_to_format( rhs, typename geometrix::is_static<scale_policy>::type() );
        }

        BOOST_CONSTEXPR fixed_point<traits_type> operator -() const
        {
            fixed_point<traits_type> result(*this);
            result.m_value = -m_value;
            return result;
        }

        BOOST_CONSTEXPR fixed_point<traits_type>& operator ++()
        {
            m_value += scale_policy::template scale_up< format_type >( 1 );
            return *this;
        }

        BOOST_CONSTEXPR fixed_point<traits_type>& operator --()
        {
            m_value -= scale_policy::template scale_up< format_type >( 1 );
            return *this;
        }

        BOOST_CONSTEXPR fixed_point<traits_type>& operator +=( const fixed_point<traits_type>& v )
        {
            m_value += v.m_value;
            return *this;
        }

        template <typename T>
        BOOST_CONSTEXPR fixed_point<traits_type>& operator +=( T v )
        {
            m_value += convert_to_format( v, typename geometrix::is_static<scale_policy>::type() );
            return *this;
        }

        BOOST_CONSTEXPR fixed_point<traits_type>& operator -=( const fixed_point<traits_type>& v )
        {
            m_value -= v.m_value;
            return *this;
        }

        template <typename T>
        BOOST_CONSTEXPR fixed_point<traits_type>& operator -=( T v )
        {
            m_value -= convert_to_format( v, typename geometrix::is_static<scale_policy>::type() );
            return *this;
        }

        BOOST_CONSTEXPR fixed_point<traits_type>& operator *= ( const fixed_point<traits_type>& factor )
        {
            m_value = scale_policy::template scale_down< format_type >( geometrix::widen_cast(m_value) * factor.m_value );
            return *this;
        }

        template <typename T>
        BOOST_CONSTEXPR fixed_point<traits_type>& operator *=( T v )
        {
            return (*this) *= fixed_point<traits_type>(v, *this);
        }

        BOOST_CONSTEXPR fixed_point<traits_type>& operator /= (const fixed_point<traits_type>& divisor)
        {
            m_value = boost::numeric_cast< format_type >( scale_policy::template scale_up< typename geometrix::widen<format_type>::type >( m_value ) / geometrix::widen_cast( divisor.m_value ) );
            return *this;
        }

        template <typename T>
        BOOST_CONSTEXPR fixed_point<traits_type>& operator /=( T v )
        {
            return (*this) /= fixed_point<traits_type>(v, *this);
        }

        template <typename T>
        BOOST_CONSTEXPR T convert_to() const
        {
            return scale_policy::template scale_down<T>( m_value );
        }

        //! Access the epsilon (useful for runtime scales).
        BOOST_CONSTEXPR fixed_point< traits_type > epsilon() const
        {
            fixed_point< traits_type > e( *this );
            e.m_value = 1;
            return e;
        }

        //! Useful for runtimes
        BOOST_CONSTEXPR fixed_point< traits_type > min BOOST_PREVENT_MACRO_SUBSTITUTION () const
        {
            fixed_point<traits_type> minimum( *this );
            minimum.m_value = (std::numeric_limits<format_type>::min)();
            return minimum;
        }

        //! Useful for runtimes
        BOOST_CONSTEXPR fixed_point< traits_type > max BOOST_PREVENT_MACRO_SUBSTITUTION () const
        {
            fixed_point< traits_type > maximum( *this );
            maximum.m_value = (std::numeric_limits<format_type>::max)();
            return maximum;
        }

#if defined(GEOMETRIX_ALLOW_FIXEDPOINT_IMPLICIT_CONVERSIONS)
        template <typename T>
        BOOST_CONSTEXPR operator T() const
        {
            #pragma GEOMETRIX_WARNING("Implicitly converting fixed_point to another numeric type.")
            int ImplicitlyConvertingFixedPointWarning = (double)0.0;//Allow the compiler to put a stack warning for this implicit conversion.
            return convert_to<T>();
        }
#endif

        BOOST_CONSTEXPR bool operator !() const
        {
            return m_value == 0;
        }

        BOOST_CONSTEXPR operator bool_type() const
        {
            return m_value ? &fixed_point::this_type_does_not_support_comparisons : 0;
        }

        //! For when it's useful to have the true scaled value (an example is assigning discrete ranges to user interface elements)
        BOOST_CONSTEXPR format_type get_scaled_value() const
        {
            return m_value;
        }

		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, long double);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, double);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, float);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, char);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, signed char);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, unsigned char);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, short);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, unsigned short);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, int);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, unsigned int);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, long);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, unsigned long);
		#if defined(BOOST_HAS_LONG_LONG)
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, long long);
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS(fixed_point<Traits>, unsigned long long);
		#endif
		GEOMETRIX_FRIEND_IMPLEMENT_ORDERED_FIELD_RELATIONAL_OPERATORS_SELF(fixed_point<Traits>);
		GEOMETRIX_FRIEND_INCREMENTABLE_OPERATOR(fixed_point<Traits>);
        GEOMETRIX_FRIEND_DECREMENTABLE_OPERATOR(fixed_point<Traits>);

        friend fixed_point fmod(const fixed_point& numer, const fixed_point& denom)
        {
            return fixed_point{from_format_tag(), numer.m_value % denom.m_value, numer};
        }

    private:

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
        double value = v.template convert_to<double>();
        os << std::setprecision( std::numeric_limits< fixed_point<Traits> >::digits10 ) << value;
        return os;
    }

	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator *(fixed_point<Traits> lhs, const U& rhs)
	{
		return lhs *= rhs;
	}
	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator *(const U& lhs, fixed_point<Traits> rhs)
	{
		return rhs *= lhs;
	}
	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator +(fixed_point<Traits> lhs, const U& rhs)
	{
		return lhs += rhs;
	}
	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator +(const U& lhs, fixed_point<Traits> rhs)
	{
		return rhs += lhs;
	}
	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator -(fixed_point<Traits> lhs, const U& rhs)
	{
		return lhs -= rhs;
	}
	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator -(const U& lhs, const fixed_point<Traits>& rhs)
	{
		return fixed_point<Traits>(lhs) -= rhs;
	}
	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator /(fixed_point<Traits> lhs, const U& rhs)
	{
		return lhs /= rhs;
	}

	template<typename Traits, typename U, typename std::enable_if<std::is_arithmetic<U>::value, int>::type = 0>
	inline fixed_point<Traits> operator /(const U& lhs, const fixed_point<Traits>& rhs)
	{
		return fixed_point<Traits>(lhs) /= rhs;
	}

	template<typename Traits> 
	inline fixed_point<Traits> operator *(fixed_point<Traits> lhs, const fixed_point<Traits>& rhs)
	{
		return lhs *= rhs;
	}
	
	template<typename Traits>
	inline fixed_point<Traits> operator +(fixed_point<Traits> lhs, const fixed_point<Traits>& rhs)
	{
		return lhs += rhs;
	}
	
	template<typename Traits>
	inline fixed_point<Traits> operator -(fixed_point<Traits> lhs, const fixed_point<Traits>& rhs)
	{
		return lhs -= rhs;
	}
	
	template<typename Traits>
	inline fixed_point<Traits> operator /(fixed_point<Traits> lhs, const fixed_point<Traits>& rhs)
	{
		return lhs /= rhs;
	}
}//! namespace not_geometrix;

namespace std
{
    #define GEOMETRIX_DEFINE_STD_MATH_FUNCTION(fn)                                        \
        template <typename Traits>                                                        \
        inline geometrix::fixed_point<Traits> fn(const geometrix::fixed_point<Traits>& v) \
        {                                                                                 \
            double vd = v.template convert_to<double>();                                  \
            vd = std:: fn (vd);                                                           \
            return geometrix::fixed_point<Traits>(vd);                                    \
        }                                                                                 \
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
    inline geometrix::fixed_point<Traits> fabs(const geometrix::fixed_point<Traits>& v)
    {
        if( v > 0 )
            return v;
        else
            return -v;
    }

    template <typename Traits>
    inline geometrix::fixed_point<Traits> abs(const geometrix::fixed_point<Traits>& v)
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
        BOOST_STATIC_CONSTEXPR float_denorm_style has_denorm = denorm_absent;
        BOOST_STATIC_CONSTEXPR bool has_denorm_loss = false;
        BOOST_STATIC_CONSTEXPR bool has_infinity = false;
        BOOST_STATIC_CONSTEXPR bool has_quiet_NaN = false;
        BOOST_STATIC_CONSTEXPR bool has_signaling_NaN = false;
        BOOST_STATIC_CONSTEXPR bool is_bounded = true;
        BOOST_STATIC_CONSTEXPR bool is_exact = true;
        BOOST_STATIC_CONSTEXPR bool is_iec559 = false;
        BOOST_STATIC_CONSTEXPR bool is_integer = false;
        BOOST_STATIC_CONSTEXPR bool is_modulo = false;
        BOOST_STATIC_CONSTEXPR bool is_signed = std::numeric_limits<typename fixed_point_type::format_type>::is_signed;
        BOOST_STATIC_CONSTEXPR bool is_specialized = true;
        BOOST_STATIC_CONSTEXPR bool tinyness_before = false;
        BOOST_STATIC_CONSTEXPR bool traps = false;
        BOOST_STATIC_CONSTEXPR float_round_style round_style = fixed_point_type::traits_type::rounding_policy::round_style::value;
        BOOST_STATIC_CONSTEXPR int digits = std::numeric_limits<typename fixed_point_type::format_type>::digits;
        BOOST_STATIC_CONSTEXPR int digits10 = std::numeric_limits<typename fixed_point_type::format_type>::digits10;
#ifndef BOOST_NO_CXX11_NUMERIC_LIMITS
        BOOST_STATIC_CONSTEXPR int max_digits10 = std::numeric_limits<typename fixed_point_type::format_type>::max_digits10;
#endif
        BOOST_STATIC_CONSTEXPR int max_exponent = 0;
        BOOST_STATIC_CONSTEXPR int max_exponent10 = 0;
        BOOST_STATIC_CONSTEXPR int min_exponent = 0;
        BOOST_STATIC_CONSTEXPR int min_exponent10 = 0;
        BOOST_STATIC_CONSTEXPR int radix = fixed_point_type::traits_type::scale_policy::radix_type::value;

        BOOST_STATIC_CONSTEXPR fixed_point_type (min)()
        {
            fixed_point_type minimum;
            minimum.m_value = (std::numeric_limits<typename fixed_point_type::format_type>::min)();
            return minimum;
        }

        BOOST_STATIC_CONSTEXPR fixed_point_type (max)()
        {
            fixed_point_type maximum;
            maximum.m_value = (std::numeric_limits<typename fixed_point_type::format_type>::max)();
            return maximum;
        }

        BOOST_STATIC_CONSTEXPR fixed_point_type epsilon()
        {
            fixed_point_type one( 1 );
            fixed_point_type one_plus_epsilon( 1 );
            one_plus_epsilon.m_value += 1;
            return one_plus_epsilon - one;
        }

        BOOST_STATIC_CONSTEXPR fixed_point_type round_error()
        {
            return (fixed_point_type)(0.5);
        }

        BOOST_STATIC_CONSTEXPR fixed_point_type denorm_min()
        {
            return (fixed_point_type)(0);
        }

        BOOST_STATIC_CONSTEXPR fixed_point_type infinity()
        {
            return (fixed_point_type)(0);
        }

        BOOST_STATIC_CONSTEXPR fixed_point_type quiet_NaN()
        {
            return (fixed_point_type)(0);
        }

        BOOST_STATIC_CONSTEXPR fixed_point_type signaling_NaN()
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
        struct raw_converter<conversion_traits<T, geometrix::fixed_point<Traits>>>
        {
            static T low_level_convert ( const geometrix::fixed_point<Traits>& n )
            {
                return n.template convert_to<T>();
            }
        } ;

        template<typename Traits, typename T>
        struct raw_converter<conversion_traits<geometrix::fixed_point<Traits>, T>>
        {
            static geometrix::fixed_point<Traits> low_level_convert ( T n )
            {
                return geometrix::fixed_point<Traits>( n );
            }
        } ;
    } // namespace numeric
} // namespace boost

#endif //GEOMETRIX_NUMERIC_FIXED_POINT_HPP

