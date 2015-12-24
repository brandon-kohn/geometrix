//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_INTEGRAL_CONSTANTS_PI_CALCULATOR_HPP
#define GEOMETRIX_INTEGRAL_CONSTANTS_PI_CALCULATOR_HPP

#include <geometrix/numeric/rational_utilities.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace geometrix {
namespace detail
{

template <unsigned int T>
struct number_value
{
    static const unsigned int value = T;
};

//! \brief Quick spigot algorithm to calculate pi to a specified precision.
template <typename T, unsigned int Precision>
struct calculate_pi
{
    static T pi()
    {
        GEOMETRIX_STATIC_ASSERT
        ( 
            ( Precision > 0 && Precision < 54900 )
            //, PRECISION_MUST_BE_LESS_THAN_54900
            //, (number_value<Precision>) 
        );

        //! count the remaining digits to process.
        unsigned int nDigits = Precision;
        
        int size = boost::numeric_cast< int >( std::floor( boost::numeric_cast<double>( Precision*14/4 ) ) );
        T numerator( 0 );
        T denominator( 1 );        
        T zeros[4] = { T(10), T(100), T(1000), T(10000) };
        int x, a, b, c, d, e, g, m;
        std::vector< int > f( size+1 );
        a = 10000;
        b = 1;
        c = size;
        e = 0;

        while( b != c )
        {
            f[b] = boost::numeric_cast< int >( std::floor( boost::numeric_cast<double>( a/5 ) ) );
            ++b;
        } 
        f[c] = 0;
        
        bool first = true;
        m = 0;
        while( c > 0 && nDigits > 0 )
        {
            g = 2*c;
            d = 0;
            b = c;
            while( b > 0 )
            {
                d = d + f[b]*a;
                g = g - 1;
                f[b] = d % g;
                d = boost::numeric_cast< int >( std::floor( boost::numeric_cast<double>( d/g ) ) );
                g = g - 1;
                b = b - 1;
                if (b != 0)
                    d = d*b;
            }
            c = c - 14;
            
            x = ( e + boost::numeric_cast< int >( std::floor( boost::numeric_cast<double>( d/a ) ) ) );
            if( x < 10 )
            {
                x *= 1000;
            }
            if( x < 100 )
            {
                x *= 100;
            }
            if( x < 1000 )
            {
                x *= 10;
            }
          
            if( nDigits >= 4 )
            {
                numerator *= zeros[3];
                numerator += T(x);
                if( first )
                {
                    denominator *= zeros[2];
                    first = false;
                }
                else
                {
                    denominator *= zeros[3];
                }
                nDigits -= 4;            
            }
            else if( nDigits > 0 )
            {
                numerator *= zeros[nDigits-1];
                numerator += T(x/1000);            
                denominator *= zeros[nDigits-1];
                nDigits = 0;
            }

            ++m;
            e = d % a;
        }

        return T( numerator / denominator );
    }
};

template <typename T>
struct calculate_pi<T,1>
{
    static T pi() 
    {
        return T(3);
    }        
};

template <typename T>
struct calculate_pi<T,2>
{
    static T pi()
    {
        return T(31)/T(10);        
    }        
};

template <typename T>
struct calculate_pi<T,3>
{
    static T pi()
    {
        return T(314)/T(100);
    }        
};

template <typename T>
struct calculate_pi<T,4>
{
    static T pi()
    {
        return T(3141)/T(1000);        
    }        
};

template <typename T>
struct calculate_pi<T,5>
{
    static T pi()
    {
        return T(31415)/T(10000);        
    }        
};

template <typename T>
struct calculate_pi<T,6>
{
    static T pi()
    {
        return T(314159)/T(100000);
    }
};

}}//namespace geometrix::detail;

#endif //GEOMETRIX_INTEGRAL_CONSTANTS_PI_CALCULATOR_HPP
