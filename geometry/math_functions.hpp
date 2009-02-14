//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_MATH_FUNCTIONS_HPP
#define _GENERATIVE_GEOMETRY_MATH_FUNCTIONS_HPP
#pragma once

#include <cmath>

namespace generative
{
namespace numeric
{
namespace geometry
{
    namespace detail
    {
        template <typename T>
        inline T abs( T iValue )
        {
            return ( ( iValue >= T(0) ) ? iValue : -iValue );
        }

        template <typename FloatType>
        class sqrt_function_evaluator
        {
        public:

	        sqrt_function_evaluator(const FloatType& value)
                : a(value)
                , two( 2 )
            {}

            inline void operator()(const FloatType& x, FloatType& f2, FloatType& f3) const
            {
                f2 = x*x - a;//x^2 -a solve for root = sqrt(a)
                f3 = two*x;//2*x d(f'(x))/dx derivative
            }

        private:
            FloatType two;
            FloatType a;
        };

        template <typename FloatType, int ITERATIONS, typename Function>
        FloatType newton_root_safe(const Function& func, const FloatType& x1, const FloatType& x2, const FloatType& xacc)
        {
            FloatType df,dx,dxold,f,fh,fl;
            FloatType temp,xh,xl,rts;
            func(x1,fl,df);
            func(x2,fh,df);
            FloatType zero( 0 );
            if( (fl > zero && fh > zero) || (fl < zero && fh < zero) )
            {
                return zero;
            }
            if( fl == zero ) 
                return x1;
            if( fh == zero )
                return x2;
            if( fl < zero )
        	{ 
                xl = x1;
                xh = x2;
            }
            else
            {
                xh = x1;
                xl = x2;
            }
            FloatType two( 2 );
            rts = (x1+x2)/two;
            dxold=abs(x2-x1);
            dx=dxold; 
            func(rts,f,df);
            for( int j=1;j<= ITERATIONS; ++j )
            {
                // Loop over allowed iterations.
                if ((((rts-xh)*df-f)*((rts-xl)*df-f) > zero) || (abs(two*f) > abs(dxold*df) ) )
                {
                    // or not decreasing fast enough.
                    dxold=dx;
                    dx=(xh-xl)/two;
                    rts=xl+dx;
                    if (xl == rts)
                        return rts; //Change in root is negligible.
                }
                else
                {
                    // Newton step acceptable. Take it.
                    dxold=dx;
                    dx=f/df;
                    temp=rts;
                    rts -= dx;
                    if (temp == rts)
                        return rts;
                }
                if (abs(dx) < xacc)
                    return rts;// Convergence criterion.
                func(rts,f,df);
                if (f < zero )// Maintain the bracket on the root.
                    xl=rts;
                else
                    xh=rts;
            }
            assert(false);
            return zero;// Never get here.
        }
        
        template <typename FloatType, int ITERATIONS>
        FloatType sqrt_newton(const FloatType& x, const FloatType& accuracyCutoff = 1e-50 )
        {
            ///solve for the root of x^2 - a (gives you sqrt(a) using the Newton Method with an accuracy cutoff of 1.0e-50.
            return newton_root_safe<FloatType, ITERATIONS>( sqrt_function_evaluator<FloatType>(x), FloatType(0), x, accuracyCutoff );
        }
    }

    //! TODO: Need to come up with a generative Taylor series type with convergence traits to synthesize the transcendents when not available.
    template <typename CoordinateType>
    struct math_functions
    {
        static inline CoordinateType sqrt( CoordinateType v ) { return CoordinateType( std::sqrt( boost::numeric_cast<double>( v ) ) ); }
        static inline CoordinateType atan2( CoordinateType y, CoordinateType x ) { return CoordinateType( atan2( boost::numeric_cast<double>(y), boost::numeric_cast<double>(x) ) ); }
        static inline CoordinateType sin( CoordinateType v ) { return CoordinateType( std::sin( boost::numeric_cast<double>( v ) ) ); }
        static inline CoordinateType asin( CoordinateType v ) { return CoordinateType( std::asin( boost::numeric_cast<double>( v ) ) ); }
        static inline CoordinateType cos( CoordinateType v ) { return CoordinateType( std::cos( boost::numeric_cast<double>( v ) ) ); }
        static inline CoordinateType acos( CoordinateType v ) { return CoordinateType( std::acos( boost::numeric_cast<double>( v ) ) ); }
        static inline CoordinateType tan( CoordinateType v ) { return CoordinateType( std::tan( boost::numeric_cast<double>( v ) ) ); }   
        static inline CoordinateType atan( CoordinateType v ) { return CoordinateType( std::atan( boost::numeric_cast<double>( v ) ) ); }
        static inline CoordinateType floor( CoordinateType v ) { return CoordinateType( std::floor( boost::numeric_cast<double>( v ) ) ); }
        static inline CoordinateType ceil( CoordinateType v ) { return CoordinateType( std::ceil( boost::numeric_cast<double>( v ) ) ); }
    };

    template <>
    struct math_functions<double>
    {
        static inline double sqrt( double v ) { return std::sqrt( v ); }
        static inline double atan2( double y, double x ) { return std::atan2( y, x ); }
        static inline double sin( double v ) { return std::sin( v ); }
        static inline double cos( double v ) { return std::cos( v ); }
        static inline double asin( double v ) { return std::asin( v ); }
        static inline double acos( double v ) { return std::acos( v ); }
        static inline double tan( double v ) { return std::tan( v ); }
        static inline double atan( double v ) { return std::atan( v ); }
        static inline double floor( double v ) { return std::floor( v ); }
        static inline double ceil( double v ) { return std::ceil( v ); }
    };

    template <>
    struct math_functions<float>
    {
        static inline float sqrt( float v ) { return std::sqrt( v ); }
        static inline float atan2( float y, float x ) { return std::atan2( y, x ); }
        static inline float sin( float v ) { return std::sin( v ); }
        static inline float cos( float v ) { return std::cos( v ); }
        static inline float asin( float v ) { return std::asin( v ); }
        static inline float acos( float v ) { return std::acos( v ); }
        static inline float tan( float v ) { return std::tan( v ); }
        static inline float atan( float v ) { return std::atan( v ); }
        static inline float floor( float v ) { return std::floor( v ); }
        static inline float ceil( float v ) { return std::ceil( v ); }
    };

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_MATH_FUNCTIONS_HPP
