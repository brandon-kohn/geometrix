//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_NUMERIC_RATIONAL_HPP
#define _BOOST_NUMERIC_RATIONAL_HPP
#pragma once

#include "../../geometry/numeric_traits.hpp"
#include <boost/rational.hpp>
#include <boost/operators.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cmath>

namespace boost
{
namespace numeric
{

//Forward Declarations
template <typename IntType>
class rational_ext;

template <typename IntType>
rational_ext<IntType> abs( const rational_ext<IntType>& r );

// A class to extend the boost rational type to initialize from a floating point type.
template <typename IntType>
class rational_ext 
    : boost::less_than_comparable < rational_ext<IntType>
    , boost::equality_comparable < rational_ext<IntType>
    , boost::less_than_comparable2 < rational_ext<IntType>, IntType
    , boost::equality_comparable2 < rational_ext<IntType>, IntType
    , boost::addable < rational_ext<IntType>
    , boost::subtractable < rational_ext<IntType>
    , boost::multipliable < rational_ext<IntType>
    , boost::dividable < rational_ext<IntType>
    , boost::addable2 < rational_ext<IntType>, IntType
    , boost::subtractable2 < rational_ext<IntType>, IntType
    , boost::subtractable2_left < rational_ext<IntType>, IntType
    , boost::multipliable2 < rational_ext<IntType>, IntType
    , boost::dividable2 < rational_ext<IntType>, IntType
    , boost::dividable2_left < rational_ext<IntType>, IntType
    , boost::incrementable < rational_ext<IntType>
    , boost::decrementable < rational_ext<IntType>
    , boost::less_than_comparable2 < rational_ext<IntType>, boost::rational<IntType>
    , boost::equality_comparable2 < rational_ext<IntType>, boost::rational<IntType>
    , boost::addable2 < rational_ext<IntType>, boost::rational<IntType>
    , boost::subtractable2 < rational_ext<IntType>, boost::rational<IntType>
    , boost::subtractable2_left < rational_ext<IntType>, boost::rational<IntType>
    , boost::multipliable2 < rational_ext<IntType>, boost::rational<IntType>
    , boost::dividable2 < rational_ext<IntType>, boost::rational<IntType>
    , boost::dividable2_left < rational_ext<IntType>, boost::rational<IntType>
    , boost::less_than_comparable2 < rational_ext<IntType>, double
    , boost::equality_comparable2 < rational_ext<IntType>, double
    , boost::addable2 < rational_ext<IntType>, double
    , boost::subtractable2 < rational_ext<IntType>, double
    , boost::subtractable2_left < rational_ext<IntType>, double
    , boost::multipliable2 < rational_ext<IntType>, double
    , boost::dividable2 < rational_ext<IntType>, double
    , boost::dividable2_left < rational_ext<IntType>, double
    , boost::less_than_comparable2 < rational_ext<IntType>, float
    , boost::equality_comparable2 < rational_ext<IntType>, float
    , boost::addable2 < rational_ext<IntType>, float
    , boost::subtractable2 < rational_ext<IntType>, float
    , boost::subtractable2_left < rational_ext<IntType>, float
    , boost::multipliable2 < rational_ext<IntType>, float
    , boost::dividable2 < rational_ext<IntType>, float
    , boost::dividable2_left < rational_ext<IntType>, float
    > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > >
{
public:

    typedef IntType int_type;
    typedef typename boost::call_traits< int_type >::param_type int_param;

    rational_ext()
        : m_rational( int_type(0), int_type(1) )
    {}

    explicit rational_ext( int_param x )
        : m_rational(x)
    {}

    explicit rational_ext( int_param n, int_param d )
        : m_rational(n,d)
    {}

    explicit rational_ext( int x )
        : m_rational( x )
    {}

    explicit rational_ext( double x )
        : m_rational( init_from_floating<IntType>(x) )
    {}

    explicit rational_ext( float x )
        : m_rational( init_from_floating<IntType>(x) )
    {}

    rational_ext( const boost::rational<int_type>& other )
        : m_rational(other)
    {}

    //! Assignment from double.
    rational_ext& operator = ( double x )
    {
        m_rational = init_from_floating<int_type>(x);
        return *this;
    }

    // Add assignment from IntType
    rational_ext& operator=( int_param n ) { return assign(n, 1); }

    // Assign in place
    rational_ext& assign( int_param n, int_param d );

    // Access to representation
    int_type numerator() const { return m_rational.numerator(); }
    int_type denominator() const { return m_rational.denominator(); }

    // Arithmetic assignment operators
    rational_ext& operator+= (const rational_ext& r);
    rational_ext& operator-= (const rational_ext& r);
    rational_ext& operator*= (const rational_ext& r);
    rational_ext& operator/= (const rational_ext& r);

    rational_ext& operator+= ( int_param i );
    rational_ext& operator-= ( int_param i );
    rational_ext& operator*= ( int_param i );
    rational_ext& operator/= ( int_param i );

    rational_ext& operator+= ( double i );
    rational_ext& operator-= ( double i );
    rational_ext& operator*= ( double i );
    rational_ext& operator/= ( double i );

    rational_ext& operator+= ( float i );
    rational_ext& operator-= ( float i );
    rational_ext& operator*= ( float i );
    rational_ext& operator/= ( float i );

    // Increment and decrement
    const rational_ext& operator++();
    const rational_ext& operator--();

    // Operator not
    bool operator!() const { return m_rational.operator !(); }

    // Boolean conversion
    //operator bool_type() const { m_rational.bool_type(); }

    // Comparison operators
    bool operator< (const rational_ext& r) const;
    bool operator== (const rational_ext& r) const;

    bool operator<  ( int_param i ) const;
    bool operator>  ( int_param i ) const;
    bool operator== ( int_param i ) const;
    
    bool operator<  ( double i ) const;
    bool operator>  ( double i ) const;
    bool operator== ( double i ) const;

    bool operator<  ( float i ) const;
    bool operator>  ( float i ) const;
    bool operator== ( float i ) const;

private:

    boost::rational<IntType> m_rational;

};

//! Method to construct the rational from a floating point type representation
template <typename IntType, typename Float>
boost::rational<IntType> init_from_floating( Float xOriginal )
{
    Float x = xOriginal;

    unsigned int numDigitsInFloat = numeric_traits<Float>::precision;

    IntType num( 0 );
    IntType denom( 1 );

    int testNum(0);
    int testDenom(1);

    if( x != 0 )
    {   
        //! Store the negative sign
        bool isNegative = (x < 0);
        if (isNegative)
        {
            x = -x;
        }

        //! Get the significand and exponent calculate the numerator and denominator.
        int exponent;
        double wholeNumber;
        Float significand = std::frexp( x, &exponent );

        //! Get the width of the floating point number.
        Float width = std::pow( 2, numDigitsInFloat );

        while( significand != 0 )
        {
            significand *= width; // shift double significand
            significand = std::modf( significand, &wholeNumber );
            num <<= numDigitsInFloat;

            testNum <<= numDigitsInFloat;

            num += boost::numeric_cast<int>( wholeNumber );

            testNum += boost::numeric_cast<int>( wholeNumber );

            exponent -= numDigitsInFloat;
        }

        if( exponent > 0 )
        {
            num <<= boost::numeric_cast<unsigned long>( exponent );
            testNum <<= boost::numeric_cast<unsigned long>( exponent );
        }
        else if( exponent < 0 )
        {
            denom <<= boost::numeric_cast<unsigned long>( -exponent );
            testDenom <<= boost::numeric_cast<unsigned long>( -exponent );
        }

        if( isNegative )
        {
            num = -num;
            testNum = -testNum;
        }
    }

    return boost::rational<IntType>( num, denom );
}

// Assign in place
template <typename IntType>
inline rational_ext<IntType>& rational_ext<IntType>::assign( int_param n, int_param d )
{
    m_rational.assign( n, d );
    return *this;
}

// Unary plus and minus
template <typename IntType>
inline rational_ext<IntType> operator+ (const rational_ext<IntType>& r)
{
    return r;
}

template <typename IntType>
inline rational_ext<IntType> operator- (const rational_ext<IntType>& r)
{
    return rational_ext<IntType>(-r.numerator(), r.denominator());
}

// Arithmetic assignment operators
template <typename IntType>
rational_ext<IntType>& rational_ext<IntType>::operator+= (const rational_ext<IntType>& r)
{
    m_rational += r.m_rational;
    return *this;
}

template <typename IntType>
rational_ext<IntType>& rational_ext<IntType>::operator-= (const rational_ext<IntType>& r)
{
    m_rational -= r.m_rational;
    return *this;
}

template <typename IntType>
rational_ext<IntType>& rational_ext<IntType>::operator*= (const rational_ext<IntType>& r)
{
    m_rational *= r.m_rational;
    return *this;
}

template <typename IntType>
rational_ext<IntType>& rational_ext<IntType>::operator/= (const rational_ext<IntType>& r)
{
    m_rational /= r.m_rational;
    return *this;
}

// Mixed-mode operators
template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator+= ( int_param i )
{
    return operator+= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>& rational_ext<IntType>::operator-= ( int_param i )
{
    return operator-= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator*= (int_param i)
{
    return operator*= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator/= (int_param i)
{
    return operator/= (rational_ext<IntType>(i));
}

//double mix moad
template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator+= ( double i )
{
    return operator+= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>& rational_ext<IntType>::operator-= ( double i )
{
    return operator-= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator*= ( double i )
{
    return operator*= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator/= ( double i )
{
    return operator/= (rational_ext<IntType>(i));
}


//! float mixed mode
template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator+= ( float i )
{
    return operator+= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>& rational_ext<IntType>::operator-= ( float i )
{
    return operator-= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator*= ( float i)
{
    return operator*= (rational_ext<IntType>(i));
}

template <typename IntType>
inline rational_ext<IntType>&
rational_ext<IntType>::operator/= ( float i )
{
    return operator/= (rational_ext<IntType>(i));
}


// Increment and decrement
template <typename IntType>
inline const rational_ext<IntType>& rational_ext<IntType>::operator++()
{
    // This can never denormalise the fraction
    m_rational.operator ++();
    return *this;
}

template <typename IntType>
inline const rational_ext<IntType>& rational_ext<IntType>::operator--()
{
    // This can never denormalise the fraction
    m_rational.operator --();
    return *this;
}

// Comparison operators
template <typename IntType>
bool rational_ext<IntType>::operator< (const rational_ext<IntType>& r) const
{
    return m_rational < r.m_rational;
}

template <typename IntType>
bool rational_ext<IntType>::operator< (int_param i) const
{
    return m_rational < i;
}

template <typename IntType>
bool rational_ext<IntType>::operator> (int_param i) const
{
    return m_rational > i;
}

template <typename IntType>
bool rational_ext<IntType>::operator< ( double i ) const
{
    return m_rational < init_from_floating<IntType>( i );
}

template <typename IntType>
bool rational_ext<IntType>::operator> ( double i ) const
{
    return m_rational > init_from_floating<IntType>( i );
}

template <typename IntType>
bool rational_ext<IntType>::operator< ( float i ) const
{
    return m_rational < init_from_floating<IntType>( i );
}

template <typename IntType>
bool rational_ext<IntType>::operator> ( float i ) const
{
    return m_rational > init_from_floating<IntType>( i );
}

template <typename IntType>
inline bool rational_ext<IntType>::operator== (const rational_ext<IntType>& r) const
{
    return m_rational == r.m_rational;
}

template <typename IntType>
inline bool rational_ext<IntType>::operator== (int_param i) const
{
    return m_rational == i;
}


template <typename IntType>
inline bool rational_ext<IntType>::operator== (double i) const
{
    return m_rational == init_from_floating<IntType>( i );
}

template <typename IntType>
inline bool rational_ext<IntType>::operator== (float i) const
{
    return m_rational == init_from_floating<IntType>( i );
}

// Input and output
template <typename IntType>
std::istream& operator>> (std::istream& is, rational_ext<IntType>& r)
{
    IntType n = IntType(0), d = IntType(1);
    char c = 0;
    boost::detail::resetter sentry(is);

    is >> n;
    c = is.get();

    if (c != '/')
        is.clear(std::istream::badbit);  // old GNU c++ lib has no ios_base

#if !defined(__GNUC__) || (defined(__GNUC__) && (__GNUC__ >= 3)) || defined __SGI_STL_PORT
    is >> std::noskipws;
#else
    is.unsetf(ios::skipws); // compiles, but seems to have no effect.
#endif
    is >> d;

    if (is)
        r.assign(n, d);

    return is;
}

// Add manipulators for output format?
template <typename IntType>
std::ostream& operator<< (std::ostream& os, const rational_ext<IntType>& r)
{
    os << r.numerator() << '/' << r.denominator();
    return os;
}

// Type conversion
template <typename T, typename IntType>
inline T rational_cast( const rational_ext<IntType>& src BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(T) )
{
    return static_cast<T>(src.numerator())/static_cast<T>(src.denominator());
}

// Do not use any abs() defined on IntType - it isn't worth it, given the
// difficulties involved (Koenig lookup required, there may not *be* an abs()
// defined, etc etc).
template <typename IntType>
inline rational_ext<IntType> abs(const rational_ext<IntType>& r)
{
    if( r.numerator() >= IntType(0) )
        return r;

    return rational_ext<IntType>( -r.numerator(), r.denominator() );
}


}}//namespace boost::numeric;

#endif //_BOOST_NUMERIC_RATIONAL_HPP
