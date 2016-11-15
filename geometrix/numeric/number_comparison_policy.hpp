//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMBER_COMPARISON_POLICY_HPP
#define GEOMETRIX_NUMBER_COMPARISON_POLICY_HPP

#include <geometrix/utility/floating_point_comparison.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/numeric/constants.hpp>
#include <boost/concept_check.hpp>

#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/as_map.hpp>
#include <boost/fusion/include/has_key.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>

namespace geometrix {

//! \brief A policy concept for floating point comparisons which enforces the required interface.

//! A NumberComparisonPolicy must must define the following member functions for a templated 
//! numeric type:\n
//!
//! - bool equals( const numeric_type& lhs, const numeric_type rhs )
//! - bool less_than( const numeric_type& lhs, const numeric_type rhs )
//! - bool less_than_or_equal( const numeric_type& lhs, const numeric_type rhs )
//! - bool greater_than( const numeric_type& lhs, const numeric_type rhs )
//! - bool greater_than_or_equal( const numeric_type& lhs, const numeric_type rhs )
template <typename ComparisonPolicy, typename NumericType = double>
struct NumberComparisonPolicyConcept
{
    void constraints() const
    {
		using namespace boost;

        NumericType a, b;
        bool eq = m_policy->equals( a, b );
		ignore_unused_variable_warning( eq );
        
		bool lt = m_policy->less_than( a, b );
		ignore_unused_variable_warning( lt );

		bool lte = m_policy->less_than_or_equal( a, b );
		ignore_unused_variable_warning( lte );

        bool gt = m_policy->greater_than( a, b );        
		ignore_unused_variable_warning( gt );

        bool gte = m_policy->greater_than_or_equal( a, b );
		ignore_unused_variable_warning( gte );
    }

    ComparisonPolicy* m_policy;
};

//! \brief Functor to adapt a number comparison policy to less_than operator.
template <typename NumberComparisonPolicy>
struct less_than
{
    less_than( const NumberComparisonPolicy& compare )
        : m_compare( compare )
    {}

    template <typename NumericType>
    bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
    {
        return m_compare.less_than( lhs, rhs );
    }

    NumberComparisonPolicy m_compare;

};

//! \brief Functor to adapt a number comparison policy to greater_than operator.
template <typename NumberComparisonPolicy>
struct greater_than
{
    greater_than( const NumberComparisonPolicy& compare )
        : m_compare( compare )
    {}

    template <typename NumericType>
    bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
    {
        return m_compare.greater_than( lhs, rhs );
    }

    NumberComparisonPolicy m_compare;

};


//! \brief Functor to adapt a number comparison policy to less_than_or_equal operator.
template <typename NumberComparisonPolicy>
struct less_than_or_equal
{
    less_than_or_equal( const NumberComparisonPolicy& compare )
        : m_compare( compare )
    {}

    template <typename NumericType>
    bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
    {
        return m_compare.less_than_or_equal( lhs, rhs );
    }

    NumberComparisonPolicy m_compare;

};

//! \brief Functor to adapt a number comparison policy to greater_than_or_equal operator.
template <typename NumberComparisonPolicy>
struct greater_than_or_equal
{
    greater_than_or_equal( const NumberComparisonPolicy& compare )
        : m_compare( compare )
    {}

    template <typename NumericType>
    bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
    {
        return m_compare.greater_than_or_equal( lhs, rhs );
    }

    NumberComparisonPolicy m_compare;

};

//! \brief Functor to adapt a number comparison policy to an equals operator.
template <typename NumberComparisonPolicy>
struct equals
{
    equals( const NumberComparisonPolicy& compare )
        : m_compare( compare )
    {}

    template <typename NumericType>
    bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
    {
        return m_compare.equals( lhs, rhs );
    }

    NumberComparisonPolicy m_compare;

};

//! Specifies a comparison within an absolute tolerance
template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool equals_within_absolute_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{
    return ( absolute_value( u - v ) <= e );
};

//! Specifies a comparison within an tolerance which is a fraction of each quantity.
template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool equals_within_fraction_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{    
    if( u == constants::zero<NumericType1>() )
    {
        return equals_zero( v, e );
    }
    else if( v == constants::zero<NumericType2>() )
    {
        return equals_zero( u, e );
    }
    else
    {
        return equals_within_tolerance( u, v, e );
    }     
};

//! Less than subject to tolerance
template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool less_than_with_absolute_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{    
    return ( ( u - v ) < -e );
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool less_than_with_fraction_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    if( u == constants::zero<NumericType1>() )
    {
        return u < v && !equals_zero( v, e );
    }
    else if( v == constants::zero<NumericType2>() )
    {
        return u < v && !equals_zero( u, e );
    }
    else
    {
        return u < v && !equals_within_tolerance( u, v, e );
    }
};

//! Less Than or Equal
template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool less_than_or_equal_with_absolute_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{
    return ( ( u - v ) <= e );
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool less_than_or_equal_with_fraction_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{
    if( u == constants::zero<NumericType1>() )
    {
        return u <= v || equals_zero( v, e );
    }
    else if( v == constants::zero<NumericType2>() )
    {
        return u <= v || equals_zero( u, e );
    }
    else
    {
        return u <= v || equals_within_tolerance( u, v, e );
    }        
};

//! Greater than subject to tolerance
template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool greater_than_with_absolute_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{
    return ( ( u - v ) > e );
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool greater_than_with_fraction_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{
    if( u == constants::zero<NumericType1>() )
    {
        return u > v && !equals_zero( v, e );
    }
    else if( v == constants::zero<NumericType2>() )
    {
        return u > v && !equals_zero( u, e );
    }
    else
    {
        return u > v && !equals_within_tolerance( u, v, e );
    }
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool greater_than_or_equal_with_absolute_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{
    return ( ( u - v ) >= -e ); 
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool greater_than_or_equal_with_fraction_tolerance(const NumericType1& u, const NumericType2& v, const ToleranceType& e)
{
    if( u == constants::zero<NumericType1>() )
    {
        return u >= v || equals_zero( v, e );
    }
    else if( v == constants::zero<NumericType2>() )
    {
        return u >= v || equals_zero( u, e );
    }
    else
    {
        return u >= v || equals_within_tolerance( u, v, e );
    }
};

//! A comparison policy based on a fraction of each quantity.

//! fraction_tolerance_comparison_policy implements a model of the NumberComparisonPolicyConcept.\n
//! fraction_tolerance_comparison_policy performs a comparison by comparing\n
//! the fractional relative difference of each operand to the tolerance threshold.\n
//!
//! For example:
//! \code
//! bool altb = fraction_tolerance_comparison_policy<double>( 0.01 ).less_than( a, b );
//! //! Is equivalent to:
//! double diff = std::abs(a - b);
//! double d1 = diff / std::abs( a );
//! double d2 = diff / std::abs( b );
//! std::GEOMETRIX_ASSERT( (d1 < 0.01 && d2 < 0.01) == altb );
//! \endcode
template <typename FractionToleranceType, typename AbsoluteToleranceFactorType = FractionToleranceType>
class fraction_tolerance_comparison_policy
{
public:

    fraction_tolerance_comparison_policy( FractionToleranceType e = construct<FractionToleranceType>( 1.0e-10 ) )
        : m_fractionTolerance( e )
        , m_absoluteToleranceFactor( static_cast<AbsoluteToleranceFactorType>( e ) )
    {}

    fraction_tolerance_comparison_policy( FractionToleranceType e, AbsoluteToleranceFactorType a )
        : m_fractionTolerance( e )
        , m_absoluteToleranceFactor( a )
    {}

    //! Specifies a comparison within an tolerance which is a fraction of each quantity.
    template <typename NumericType1, typename NumericType2>
    bool equals( const NumericType1& u, const NumericType2& v ) const
    {
        if( u == constants::zero< NumericType1 >() )
            return equals_zero( v, m_absoluteToleranceFactor );
        else if( v == constants::zero< NumericType2 >() )
            return equals_zero( u, m_absoluteToleranceFactor );
        else
            return equals_within_tolerance( u, v, m_fractionTolerance );        
    };

    template <typename NumericType1, typename NumericType2>
    bool less_than( const NumericType1& u, const NumericType2& v ) const
    {
        if( u == constants::zero< NumericType1 >() )
            return u < v && !equals_zero( v, m_absoluteToleranceFactor );
        else if( v == constants::zero< NumericType2 >() )
            return u < v && !equals_zero( u, m_absoluteToleranceFactor );
        else
            return u < v && !equals_within_tolerance( u, v, m_fractionTolerance );                
    };

    template <typename NumericType1, typename NumericType2>
    bool less_than_or_equal( const NumericType1& u, const NumericType2& v ) const
    {
        if( u == constants::zero<NumericType1>() )
            return u <= v || equals_zero( v, m_absoluteToleranceFactor );
        else if( v == constants::zero<NumericType2>() )
            return u <= v || equals_zero( u, m_absoluteToleranceFactor );
        else
            return u <= v || equals_within_tolerance( u, v, m_fractionTolerance );             
    };

    template <typename NumericType1, typename NumericType2>
    bool greater_than( const NumericType1& u, const NumericType2& v ) const
    {        
        if( u == constants::zero<NumericType1>() )
            return u > v && !equals_zero( v, m_absoluteToleranceFactor );
        else if( v == constants::zero<NumericType2>() )
            return u > v && !equals_zero( u, m_absoluteToleranceFactor );
        else
            return u > v && !equals_within_tolerance( u, v, m_fractionTolerance );             
    };

    template <typename NumericType1, typename NumericType2>
    bool greater_than_or_equal( const NumericType1& u, const NumericType2& v ) const
    {        
        if( u == constants::zero<NumericType1>() )
            return u >= v || equals_zero( v, m_absoluteToleranceFactor );
        else if( v == constants::zero<NumericType2>() )
            return u >= v || equals_zero( u, m_absoluteToleranceFactor );
        else
            return u >= v || equals_within_tolerance( u, v, m_fractionTolerance );             
    };

private:

    FractionToleranceType       m_fractionTolerance;
    AbsoluteToleranceFactorType m_absoluteToleranceFactor;

};

//! A comparison policy based on an absolute tolerance threshold

//! absolute_tolerance_comparison_policy implements a model of the NumberComparisonPolicyConcept.\n
//! absolute_tolerance_comparison_policy performs a comparison using an\n
//! absolute tolerance as an error threshold.
//!
//! For example:
//! \code
//! bool altb = absolute_tolerance_comparison_policy<double>( 0.01 ).less_than( a, b );
//! //! Is equivalent to:
//! std::GEOMETRIX_ASSERT( (std::abs(a - b) < 0.01) == altb );
//! \endcode
template <typename ToleranceType>
class absolute_tolerance_comparison_policy
{
public:

    typedef ToleranceType numeric_type;
    
    absolute_tolerance_comparison_policy( const ToleranceType& e = construct<ToleranceType>(1e-10) )
        : m_tolerance( e )
    {}

    template <typename NumericType1, typename NumericType2>
    bool equals( const NumericType1& u, const NumericType2& v ) const
    {
        return ( absolute_value( u - v ) <= m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    bool less_than( const NumericType1& u, const NumericType2& v ) const
    {
        return ( ( u - v ) < -m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    bool less_than_or_equal( const NumericType1& u, const NumericType2& v ) const
    {
        return ( ( u - v ) <= m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    bool greater_than( const NumericType1& u, const NumericType2& v ) const
    {
        return ( ( u - v ) > m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    bool greater_than_or_equal( const NumericType1& u, const NumericType2& v ) const
    {
        return ( ( u - v ) >= -m_tolerance ); 
    };

private:

    ToleranceType m_tolerance;

};

//! \brief A comparison policy to compare based only on the characteristics of the number types.

//! direct_comparison_policy implements a model of the NumberComparisonPolicyConcept.\n
//! direct_comparison_policy performs a direct comparison using the corresponding operator.\n
//!
//! For example:
//! \code
//! bool altb = direct_comparison_policy().less_than( a, b );
//! //! Is equivalent to:
//! std::GEOMETRIX_ASSERT( a < b == altb );
//! \endcode
//! Good for ints and exact types.

class direct_comparison_policy
{
public:

    direct_comparison_policy()        
    {}

    template <typename Number1, typename Number2>
    bool equals( const Number1& u, const Number2& v ) const
    {
        return u == v;
    };

    template <typename Number1, typename Number2>
    bool less_than( const Number1& u, const Number2& v ) const
    {
        return u < v;
    };

    template <typename Number1, typename Number2>
    bool less_than_or_equal( const Number1& u, const Number2& v ) const
    {
        return u <= v;
    };

    template <typename Number1, typename Number2>
    bool greater_than( const Number1& u, const Number2& v ) const
    {
        return u > v;
    };

    template <typename Number1, typename Number2>
    bool greater_than_or_equal( const Number1& u, const Number2& v ) const
    {
        return u >= v;
    };

};


//! A comparison policy based on an absolute tolerance threshold

//! relative_tolerance_comparison_policy implements a model of the NumberComparisonPolicyConcept.\n
//! relative_tolerance_comparison_policy performs a comparison using an\n
//! absolute tolerance as an error threshold.
//!
//! For example:
//! \code
//! bool altb = relative_tolerance_comparison_policy<double>( 0.01 ).less_than( a, b );
//! //! Is equivalent to:
//! std::GEOMETRIX_ASSERT( (std::abs(a - b) < 0.01) == altb );
//! \endcode
template <typename ToleranceType>
class relative_tolerance_comparison_policy
{
public:

	typedef ToleranceType numeric_type;

	relative_tolerance_comparison_policy(const ToleranceType& e = 1e-10)
		: m_tolerance(e)
	{}

	template <typename NumericType1, typename NumericType2>
	bool equals(const NumericType1& u, const NumericType2& v) const
	{
		using std::abs;
		return absolute_value(u - v) <= m_tolerance * (abs(u) + abs(v) + 1);
	};

	template <typename NumericType1, typename NumericType2>
	bool less_than(const NumericType1& u, const NumericType2& v) const
	{
		using std::sqrt;
		return (u - v) < -m_tolerance * (abs(u) + abs(v) + 1);
	};

	template <typename NumericType1, typename NumericType2>
	bool less_than_or_equal(const NumericType1& u, const NumericType2& v) const
	{
		using std::sqrt;
		return (u - v) <= m_tolerance * (abs(u) + abs(v) + 1);
	};

	template <typename NumericType1, typename NumericType2>
	bool greater_than(const NumericType1& u, const NumericType2& v) const
	{
		using std::sqrt;
		return (u - v) > m_tolerance * (abs(u) + abs(v) + 1);
	};

	template <typename NumericType1, typename NumericType2>
	bool greater_than_or_equal(const NumericType1& u, const NumericType2& v) const
	{
		using std::sqrt;
		return (u - v) >= -m_tolerance * (abs(u) + abs(v) + 1);
	};

private:

	ToleranceType m_tolerance;

};

template <typename ...Policies>
struct compound_comparison_policy
{
private:

	template <typename ComparisonPolicy>
	struct compare_policy_to_pair
		: boost::mpl::identity
		<
		    boost::fusion::pair<typename ComparisonPolicy::numeric_type, ComparisonPolicy>
		>
	{};

	typedef typename boost::fusion::result_of::as_map
		<
	    	typename boost::mpl::transform<boost::mpl::vector<Policies...>, compare_policy_to_pair<boost::mpl::_1>>::type
		>::type policy_map;
public:

	compound_comparison_policy() = default;
	compound_comparison_policy(Policies&&... p)
		: m_policy_map(boost::fusion::as_map(boost::fusion::make_vector(boost::fusion::make_pair<typename Policies::numeric_type>(p)...)))
	{}

	template <typename NumericType1, typename NumericType2>
	bool equals(const NumericType1& u, const NumericType2& v) const
	{
		return boost::fusion::at_key<NumericType1>(m_policy_map).equals(u, v);
	};

	template <typename NumericType1, typename NumericType2>
	bool less_than(const NumericType1& u, const NumericType2& v) const
	{
		return boost::fusion::at_key<NumericType1>(m_policy_map).less_than(u, v);
	};

	template <typename NumericType1, typename NumericType2>
	bool less_than_or_equal(const NumericType1& u, const NumericType2& v) const
	{
		return boost::fusion::at_key<NumericType1>(m_policy_map).less_than_or_equal(u, v);
	};

	template <typename NumericType1, typename NumericType2>
	bool greater_than(const NumericType1& u, const NumericType2& v) const
	{
		return boost::fusion::at_key<NumericType1>(m_policy_map).greater_than(u, v);
	};

	template <typename NumericType1, typename NumericType2>
	bool greater_than_or_equal(const NumericType1& u, const NumericType2& v) const
	{
		return boost::fusion::at_key<NumericType1>(m_policy_map).greater_than_or_equal(u, v);
	};

private:

	policy_map m_policy_map;

};

template <typename DefaultPolicy, typename ...Policies>
class mapped_tolerance_comparison_policy
{
	using default_policy = DefaultPolicy;

	using policy_map = typename boost::fusion::map<Policies...>;

	template <typename Key>
	typename boost::fusion::result_of::at_key<policy_map const, Key>::type get_policy(boost::mpl::true_) const
	{
		return boost::fusion::at_key<Key>(m_policy_map);
	}

	template <typename Key>
	const default_policy& get_policy(boost::mpl::false_) const
	{
		return m_default;
	}

	template <typename T>
	T	get_underlying_value(T v) const { return v; }

	template <typename Unit, typename T>
	T   get_underlying_value(const boost::units::quantity<Unit, T>& v) const { return v.value(); }

public:

	mapped_tolerance_comparison_policy() = default;

	mapped_tolerance_comparison_policy(const default_policy& defaultPolicy, const Policies&... p)
		: m_default(defaultPolicy)
		, m_policy_map(p...)
	{}

	template <typename NumericType1, typename NumericType2>
	bool equals(const NumericType1& u, const NumericType2& v) const
	{
		static_assert(std::is_convertible<NumericType1, NumericType2>::value, "types are not comparable");
		return get_policy<NumericType1>(boost::fusion::result_of::has_key<policy_map, NumericType1>::type()).equals(get_underlying_value(u), get_underlying_value(v));
	};

	template <typename NumericType1, typename NumericType2>
	bool less_than(const NumericType1& u, const NumericType2& v) const
	{
		static_assert(std::is_convertible<NumericType1, NumericType2>::value, "types are not comparable");
		return get_policy<NumericType1>(boost::fusion::result_of::has_key<policy_map, NumericType1>::type()).less_than(get_underlying_value(u), get_underlying_value(v));
	};

	template <typename NumericType1, typename NumericType2>
	bool less_than_or_equal(const NumericType1& u, const NumericType2& v) const
	{
		static_assert(std::is_convertible<NumericType1, NumericType2>::value, "types are not comparable");
		return get_policy<NumericType1>(boost::fusion::result_of::has_key<policy_map, NumericType1>::type()).less_than_or_equal(get_underlying_value(u), get_underlying_value(v));
	};

	template <typename NumericType1, typename NumericType2>
	bool greater_than(const NumericType1& u, const NumericType2& v) const
	{
		static_assert(std::is_convertible<NumericType1, NumericType2>::value, "types are not comparable");
		return get_policy<NumericType1>(boost::fusion::result_of::has_key<policy_map, NumericType1>::type()).greater_than(get_underlying_value(u), get_underlying_value(v));
	};

	template <typename NumericType1, typename NumericType2>
	bool greater_than_or_equal(const NumericType1& u, const NumericType2& v) const
	{
		static_assert(std::is_convertible<NumericType1, NumericType2>::value, "types are not comparable");
		return get_policy<NumericType1>(boost::fusion::result_of::has_key<policy_map, NumericType1>::type()).greater_than_or_equal(get_underlying_value(u), get_underlying_value(v));
	};

private:

	default_policy	m_default{ 1e-10 };
	policy_map		m_policy_map;

};

template <typename DefaultPolicy, typename ...Policies>
inline mapped_tolerance_comparison_policy<DefaultPolicy, Policies...> make_mapped_tolerance_comparison_policy(const DefaultPolicy& dflt, const Policies& ... p)
{
	return mapped_tolerance_comparison_policy<DefaultPolicy, Policies...>(dflt, p...);
}

}//namespace geometrix;

#endif //GEOMETRIX_NUMBER_COMPARISON_POLICY_HPP
