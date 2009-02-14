//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_NUMBER_COMPARISON_POLICY_HPP
#define _GENERATIVE_GEOMETRY_NUMBER_COMPARISON_POLICY_HPP
#pragma once

#include "detail/floating_point_comparison.hpp"
#include "construction_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \brief A policy concept for floating point comparisons which enforces the required interface.

//! A NumberComparisonPolicy must must define the following member functions for a templated 
//! numeric type:\n
//!
//! - bool equals( const numeric_type& lhs, const numeric_type rhs )
//! - bool less_than( const numeric_type& lhs, const numeric_type rhs )
//! - bool less_than_or_equal( const numeric_type& lhs, const numeric_type rhs )
//! - bool greater_than( const numeric_type& lhs, const numeric_type rhs )
//! - bool greater_than_or_equal( const numeric_type& lhs, const numeric_type rhs )
template <typename ComparisonPolicy, typename NumericType>
struct NumberComparisonPolicyConcept
{
    void constraints()
    {
        NumericType a, b;
        bool eq = m_policy->equals( a, b );
        bool lt = m_policy->less_than( a, b );
        bool lte = m_policy->less_than_or_equal( a, b );
        bool gt = m_policy->greater_than( a, b );        
        bool gte = m_policy->greater_than_or_equal( a, b );
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
    inline bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
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
    inline bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
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
    inline bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
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
    inline bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
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
    inline bool operator()( const NumericType& lhs, const NumericType& rhs ) const 
    {
        return m_compare.equals( lhs, rhs );
    }

    NumberComparisonPolicy m_compare;

};

//! Specifies a comparison within an absolute tolerance
template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool equals_within_absolute_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    return ( absolute_value( u - v ) <= e );
};

//! Specifies a comparison within an tolerance which is a fraction of each quantity.
template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool equals_within_fraction_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{    
    if( u == numeric_traits< NumericType1 >::zero )
    {
        return equals_zero( v, e );
    }
    else if( v == numeric_traits< NumericType2 >::zero )
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
inline bool less_than_with_absolute_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{    
    return ( ( u - v ) < -e );
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool less_than_with_fraction_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    if( u == numeric_traits< NumericType1 >::zero )
    {
        return u < v && !equals_zero( v, e );
    }
    else if( v == numeric_traits< NumericType2 >::zero )
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
inline bool less_than_or_equal_with_absolute_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    return ( ( u - v ) <= e );
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool less_than_or_equal_with_fraction_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    if( u == numeric_traits< NumericType1 >::zero )
    {
        return u <= v || equals_zero( v, e );
    }
    else if( v == numeric_traits< NumericType2 >::zero )
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
inline bool greater_than_with_absolute_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    return ( ( u - v ) > e );
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool greater_than_with_fraction_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    if( u == numeric_traits< NumericType1 >::zero )
    {
        return u > v && !equals_zero( v, e );
    }
    else if( v == numeric_traits< NumericType2 >::zero )
    {
        return u > v && !equals_zero( u, e );
    }
    else
    {
        return u > v && !equals_within_tolerance( u, v, e );
    }
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool greater_than_or_equal_with_absolute_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    return ( ( u - v ) >= -e ); 
};

template <typename NumericType1, typename NumericType2, typename ToleranceType>
inline bool greater_than_or_equal_with_fraction_tolerance( const NumericType1& u, const NumericType2& v, const ToleranceType& e )
{
    if( u == numeric_traits< NumericType1 >::zero )
    {
        return u >= v || equals_zero( v, e );
    }
    else if( v == numeric_traits< NumericType2 >::zero )
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
//! std::assert( (d1 < 0.01 && d2 < 0.01) == altb );
//! \endcode
template <typename FractionToleranceType, typename AbsoluteToleranceFactorType = FractionToleranceType>
class fraction_tolerance_comparison_policy
{
public:

    fraction_tolerance_comparison_policy( FractionToleranceType e = construction_traits<FractionToleranceType>::construct( 1.0e10 ) )
        : m_fractionTolerance( e )
        , m_absoluteToleranceFactor( static_cast<AbsoluteToleranceFactorType>( e ) )
    {}

    fraction_tolerance_comparison_policy( FractionToleranceType e, AbsoluteToleranceFactorType a )
        : m_fractionTolerance( e )
        , m_absoluteToleranceFactor( a )
    {}

    //! Specifies a comparison within an tolerance which is a fraction of each quantity.
    template <typename NumericType1, typename NumericType2>
    inline bool equals( const NumericType1& u, const NumericType2& v ) const
    {
        if( u == numeric_traits< NumericType1 >::zero )
        {
            return equals_zero( v, m_absoluteToleranceFactor );
        }
        else if( v == numeric_traits< NumericType2 >::zero )
        {
            return equals_zero( u, m_absoluteToleranceFactor );
        }
        else
        {
            return equals_within_tolerance( u, v, m_fractionTolerance );
        }
    };

    template <typename NumericType1, typename NumericType2>
    inline bool less_than( const NumericType1& u, const NumericType2& v ) const
    {
        if( u == numeric_traits< NumericType1 >::zero )
        {
            return u < v && !equals_zero( v, m_absoluteToleranceFactor );
        }
        else if( v == numeric_traits< NumericType2 >::zero )
        {
            return u < v && !equals_zero( u, m_absoluteToleranceFactor );
        }
        else
        {
            return u < v && !equals_within_tolerance( u, v, m_fractionTolerance );
        }        
    };

    template <typename NumericType1, typename NumericType2>
    inline bool less_than_or_equal( const NumericType1& u, const NumericType2& v ) const
    {
        if( u == numeric_traits< NumericType1 >::zero )
        {
            return u <= v || equals_zero( v, m_absoluteToleranceFactor );
        }
        else if( v == numeric_traits< NumericType2 >::zero )
        {
            return u <= v || equals_zero( u, m_absoluteToleranceFactor );
        }
        else
        {
            return u <= v || equals_within_tolerance( u, v, m_fractionTolerance );
        }     
    };

    template <typename NumericType1, typename NumericType2>
    inline bool greater_than( const NumericType1& u, const NumericType2& v ) const
    {        
        if( u == numeric_traits< NumericType1 >::zero )
        {
            return u > v && !equals_zero( v, m_absoluteToleranceFactor );
        }
        else if( v == numeric_traits< NumericType2 >::zero )
        {
            return u > v && !equals_zero( u, m_absoluteToleranceFactor );
        }
        else
        {
            return u > v && !equals_within_tolerance( u, v, m_fractionTolerance );
        }     
    };

    template <typename NumericType1, typename NumericType2>
    inline bool greater_than_or_equal( const NumericType1& u, const NumericType2& v ) const
    {        
        if( u == numeric_traits< NumericType1 >::zero )
        {
            return u >= v || equals_zero( v, m_absoluteToleranceFactor );
        }
        else if( v == numeric_traits< NumericType2 >::zero )
        {
            return u >= v || equals_zero( u, m_absoluteToleranceFactor );
        }
        else
        {
            return u >= v || equals_within_tolerance( u, v, m_fractionTolerance );
        }     
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
//! std::assert( (std::abs(a - b) < 0.01) == altb );
//! \endcode
template <typename ToleranceType>
class absolute_tolerance_comparison_policy
{
public:
    
    absolute_tolerance_comparison_policy( const ToleranceType& e = 1e-10 )
        : m_tolerance( e )
    {}

    template <typename NumericType1, typename NumericType2>
    inline bool equals( const NumericType1& u, const NumericType2& v ) const
    {
        return ( absolute_value( u - v ) <= m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    inline bool less_than( const NumericType1& u, const NumericType2& v ) const
    {
        return ( ( u - v ) < -m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    inline bool less_than_or_equal( const NumericType1& u, const NumericType2& v ) const
    {
        return ( ( u - v ) <= m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    inline bool greater_than( const NumericType1& u, const NumericType2& v ) const
    {
        return ( ( u - v ) > m_tolerance );
    };

    template <typename NumericType1, typename NumericType2>
    inline bool greater_than_or_equal( const NumericType1& u, const NumericType2& v ) const
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
//! std::assert( a < b == altb );
//! \endcode
//! Good for ints and exact types.

class direct_comparison_policy
{
public:

    direct_comparison_policy()        
    {}

    template <typename Number>
    inline bool equals( const Number& u, const Number& v ) const
    {
        return u == v;
    };

    template <typename Number>
    inline bool less_than( const Number& u, const Number& v ) const
    {
        return u < v;
    };

    template <typename Number>
    inline bool less_than_or_equal( const Number& u, const Number& v ) const
    {
        return u <= v;
    };

    template <typename Number>
    inline bool greater_than( const Number& u, const Number& v ) const
    {
        return u > v;
    };

    template <typename Number>
    inline bool greater_than_or_equal( const Number& u, const Number& v ) const
    {
        return u >= v;
    };

};

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_NUMBER_COMPARISON_POLICY_HPP
