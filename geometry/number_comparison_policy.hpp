//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_NUMBER_COMPARISON_POLICY_HPP
#define _BOOST_GEOMETRY_NUMBER_COMPARISON_POLICY_HPP
#pragma once

#include <boost/test/floating_point_comparison.hpp>

namespace boost
{
namespace numeric
{
namespace geometry
{


template <typename Number>
inline Number number_absolute_value( const Number& v ) 
{
    const Number zero( 0 );
    return v < zero ? -v : v;
}

template<typename NumberType, typename ToleranceType>
inline bool check_is_close_to_tolerance( const NumberType& v, const ToleranceType& tolerance )
{
    return number_absolute_value( v ) < number_absolute_value( tolerance );
}

//! A policy concept for floating point comparisons
//! which enforces the required interface.
template <typename ComparisonPolicy, typename Number>
struct NumberComparisonPolicyConcept
{
    void constraints()
    {
        Number a, b;
        bool eq = m_policy->equals( a, b );
        bool lt = m_policy->less_than( a, b );
        bool lte = m_policy->less_than_or_equals( a, b );
        bool gt = m_policy->greater_than( a, b );        
        bool gte = m_policy->greater_than_or_equals( a, b );
    }

    ComparisonPolicy* m_policy;
};

//! Specifies a comparison within an absolute tolerance
template <typename Float, typename ToleranceType>
inline bool equals_within_absolute_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    return ( number_absolute_value( u - v ) <= e );
};

//! Specifies a comparison within an tolerance which is a fraction of each quantity.
template <typename Float, typename ToleranceType>
inline bool equals_within_fraction_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    using namespace boost::test_tools;
    
    if( u == 0 )
    {
        return check_is_close_to_tolerance( v, e );
    }
    else if( v == 0 )
    {
        return check_is_close_to_tolerance( u, e );
    }
    else
    {
        return check_is_close_t()( u, v, fraction_tolerance_t<ToleranceType>( e ) );
    }     
};

//! Less than subject to tolerance
template <typename Float, typename ToleranceType>
inline bool less_than_with_absolute_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{    
    return ( ( u - v ) < -e );
};

template <typename Float, typename ToleranceType>
inline bool less_than_with_fraction_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    using namespace boost::test_tools;

    if( u == 0 )
    {
        return u < v && !check_is_close_to_tolerance( v, e );
    }
    else if( v == 0 )
    {
        return u < v && !check_is_close_to_tolerance( u, e );
    }
    else
    {
        return u < v && !check_is_close_t()( u, v, fraction_tolerance_t<ToleranceType>( e ) );
    }
};

//! Less Than or Equal
template <typename Float, typename ToleranceType>
inline bool less_than_or_equals_with_absolute_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    return ( ( u - v ) <= e );
};

template <typename Float, typename ToleranceType>
inline bool less_than_or_equals_with_fraction_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    using namespace boost::test_tools;

    if( u == 0 )
    {
        return u <= v || check_is_close_to_tolerance( v, e );
    }
    else if( v == 0 )
    {
        return u <= v || check_is_close_to_tolerance( u, e );
    }
    else
    {
        return u <= v || check_is_close_t()( u, v, fraction_tolerance_t<ToleranceType>( e ) );
    }        
};

//! Greater than subject to tolerance
template <typename Float, typename ToleranceType>
inline bool greater_than_with_absolute_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    return ( ( u - v ) > e );
};

template <typename Float, typename ToleranceType>
inline bool greater_than_with_fraction_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    using namespace boost::test_tools;

    if( u == 0 )
    {
        return u > v && !check_is_close_to_tolerance( v, e );
    }
    else if( v == 0 )
    {
        return u > v && !check_is_close_to_tolerance( u, e );
    }
    else
    {
        return u > v && !check_is_close_t()( u, v, fraction_tolerance_t<ToleranceType>( e ) );
    }
};

template <typename Float, typename ToleranceType>
inline bool greater_than_or_equals_with_absolute_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    return ( ( u - v ) >= -e ); 
};

template <typename Float, typename ToleranceType>
inline bool greater_than_or_equals_with_fraction_tolerance( const Float& u, const Float& v, const ToleranceType& e )
{
    using namespace boost::test_tools;

    if( u == 0 )
    {
        return u >= v || check_is_close_to_tolerance( v, e );
    }
    else if( v == 0 )
    {
        return u >= v || check_is_close_to_tolerance( u, e );
    }
    else
    {
        return u >= v || check_is_close_t()( u, v, fraction_tolerance_t<ToleranceType>( e ) );
    }
};

//! A comparison policy based on a fraction of each quantity.
template <typename ToleranceType>
class fraction_tolerance_comparison_policy
{
public:

    fraction_tolerance_comparison_policy( ToleranceType e = 1e-10 )
        : m_tolerance( e )
    {}

    //! Specifies a comparison within an tolerance which is a fraction of each quantity.
    template <typename Float>
    inline bool equals( const Float& u, const Float& v) const
    {
        using namespace boost::test_tools;

        if( u == 0 )
        {
            return check_is_close_to_tolerance( v, m_tolerance.m_value );
        }
        else if( v == 0 )
        {
            return check_is_close_to_tolerance( u, m_tolerance.m_value );
        }
        else
        {
            return check_is_close_t()( u, v, m_tolerance );
        }
    };

    template <typename Float>
    inline bool less_than( const Float& u, const Float& v ) const
    {
        using namespace boost::test_tools;

        if( u == 0 )
        {
            return u < v && !check_is_close_to_tolerance( v, m_tolerance.m_value );
        }
        else if( v == 0 )
        {
            return u < v && !check_is_close_to_tolerance( u, m_tolerance.m_value );
        }
        else
        {
            return u < v && !check_is_close_t()( u, v, m_tolerance );
        }        
    };

    template <typename Float>
    inline bool less_than_or_equals( const Float& u, const Float& v ) const
    {
        using namespace boost::test_tools;

        if( u == 0 )
        {
            return u <= v || check_is_close_to_tolerance( v, m_tolerance.m_value );
        }
        else if( v == 0 )
        {
            return u <= v || check_is_close_to_tolerance( u, m_tolerance.m_value );
        }
        else
        {
            return u <= v || check_is_close_t()( u, v, m_tolerance );
        }     
    };

    template <typename Float>
    inline bool greater_than( const Float& u, const Float& v ) const
    {
        using namespace boost::test_tools;
        
        if( u == 0 )
        {
            return u > v && !check_is_close_to_tolerance( v, m_tolerance.m_value );
        }
        else if( v == 0 )
        {
            return u > v && !check_is_close_to_tolerance( u, m_tolerance.m_value );
        }
        else
        {
            return u > v && !check_is_close_t()( u, v, m_tolerance );
        }     
    };

    template <typename Float>
    inline bool greater_than_or_equals( const Float& u, const Float& v ) const
    {
        using namespace boost::test_tools;
        
        if( u == 0 )
        {
            return u >= v || check_is_close_to_tolerance( v, m_tolerance.m_value );
        }
        else if( v == 0 )
        {
            return u >= v || check_is_close_to_tolerance( u, m_tolerance.m_value );
        }
        else
        {
            return u >= v || check_is_close_t()( u, v, m_tolerance );
        }     
    };

private:

    boost::test_tools::fraction_tolerance_t<ToleranceType> m_tolerance;

};

//! A comparison policy based on an absolute tolerance threshold
template <typename ToleranceType>
class absolute_tolerance_comparison_policy
{
public:

    absolute_tolerance_comparison_policy( const ToleranceType& e = 1e-10 )
        : m_tolerance( e )
    {}

    template <typename Float>
    inline bool equals( const Float& u, const Float& v ) const
    {
        return ( number_absolute_value( u - v ) <= m_tolerance );
    };

    template <typename Float>
    inline bool less_than( const Float& u, const Float& v ) const
    {
        return ( ( u - v ) < -m_tolerance );
    };

    template <typename Float>
    inline bool less_than_or_equals( const Float& u, const Float& v ) const
    {
        return ( ( u - v ) <= m_tolerance );
    };

    template <typename Float>
    inline bool greater_than( const Float& u, const Float& v ) const
    {
        return ( ( u - v ) > m_tolerance );
    };

    template <typename Float>
    inline bool greater_than_or_equals( const Float& u, const Float& v ) const
    {
        return ( ( u - v ) >= -m_tolerance ); 
    };

private:

    ToleranceType m_tolerance;

};

//! A comparison policy to compare based only on the characteristics of the number types.
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
    inline bool less_than_or_equals( const Number& u, const Number& v ) const
    {
        return u <= v;
    };

    template <typename Number>
    inline bool greater_than( const Number& u, const Number& v ) const
    {
        return u > v;
    };

    template <typename Number>
    inline bool greater_than_or_equals( const Number& u, const Number& v ) const
    {
        return u >= v;
    };

};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_NUMBER_COMPARISON_POLICY_HPP
