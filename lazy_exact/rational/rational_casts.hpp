//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_NUMERIC_RATIONAL_CASTS_HPP
#define _BOOST_NUMERIC_RATIONAL_CASTS_HPP
#pragma once

namespace boost
{
    template <unsigned int SignificantFigures, typename Rational, typename IntType>
    void get_significant_digits( const Rational& x, IntType& significand, IntType& exponent )
    {   
        typedef IntType int_type;
        int_type zero( 0 );
        int_type significantFigures( SignificantFigures );

        if( x == zero )
        {
            return;
        }

        bool neg = x < zero;
        
        int_type ten( 10 );
        int_type one( 1 );
        int_type q = x.numerator() / x.denominator();        
        int_type r = x.numerator() % x.denominator();
        int_type multiplier( one );        
        if( q != zero )
        {
            ///Add up the whole numbers.
            int_type e = q;            
            
            do
            {   
                int_type digit = (e % ten);
                significand += digit * multiplier;                
                if( significantFigures == zero )
                {
                    exponent += one;
                    significand /= ten;                    
                }
                else
                {
                    --significantFigures;
                    multiplier *= ten;
                }
                
                e /= ten;
            }
            while( e > zero );
        }

        //! mark a decimal here ;)
        if( r != zero && significantFigures > zero )
        { 
            do 
            {                
                r *= ten;
                q = r / x.denominator();                
                if( q > zero )
                {
                    int_type e = q;            
                    do
                    {                           
                        significand *= ten;
                        --significantFigures;
                        int_type digit = (e % ten);
                        significand += digit;
                        exponent -= one;
                        e /= ten;
                    }
                    while( e > zero && significantFigures > zero );
                    r = r % x.denominator();                    
                }
                else
                {
                    significand *= ten;
                    --significantFigures;
                    exponent -= one;                    
                }
            }
            while( r != zero && significantFigures > zero );            
        }

        if( neg ) 
            significand = -significand;
    }

    std::ostream& operator << ( std::ostream& os, const rational_ext< boost::mp_math::mp_int<> >& src )
    {
        typedef boost::mp_math::mp_int<> int_t;
        int_t significand( 0 ), exponent( 0 );
        get_significant_digits< 30 >( src, significand, exponent );
        os << significand << "E" << exponent;        
        return os;
    }

    template <typename IntType>
    double rational_cast( const boost::rational< IntType >& src )
    {
        typedef IntType int_t;
        int_t significand( 0 ), exponent( 0 );
        get_significant_digits< 15 >( src, significand, exponent );
        double result = boost::numeric_cast<double>( boost::numeric_cast< long long >( significand ) ) * pow( 10., boost::numeric_cast< double >( boost::numeric_cast< long long >( exponent ) ) );
        return result;
    }
    
    template <typename IntType>
    double rational_cast( const rational_ext< IntType >& src )
    {
        typedef boost::mp_math::mp_int<> int_t;
        int_t significand( 0 ), exponent( 0 );
        get_significant_digits< 15 >( src, significand, exponent );
        double result = boost::numeric_cast<double>( boost::numeric_cast< long long >( significand ) ) * pow( 10., boost::numeric_cast< double >( boost::numeric_cast< long long >( exponent ) ) );
        return result;
    }
    
    double rational_cast( const boost::rational< long long >& src )
    {
        return static_cast<double>( src.numerator() ) / static_cast<double>( src.denominator() );
    }

    double rational_cast( const rational_ext< long long >& src )
    {
        return static_cast<double>( src.numerator() ) / static_cast<double>( src.denominator() );
    }
}
#endif //_BOOST_NUMERIC_RATIONAL_CASTS_HPP
