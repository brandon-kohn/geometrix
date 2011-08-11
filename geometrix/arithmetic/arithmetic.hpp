//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATH_FUNCTIONS_HPP
#define GEOMETRIX_MATH_FUNCTIONS_HPP

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/function_traits.hpp>

#include <boost/typeof/typeof.hpp>
#include <boost/utility/result_of.hpp>

#include <cmath>

namespace geometrix {

    namespace result_of 
    {
        template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
        struct multiplies{};

        template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
        struct plus
        {};

        template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
        struct minus{};

        template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
        struct divides{};
    }
    
    template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
    struct multiplies{};
    
    template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
    struct plus
    {};
    
    template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
    struct minus{};
    
    template <typename LHS, typename RHS, typename LeftType=void, typename RightType=void>
    struct divides{};
        
    namespace math
    {
        namespace tag
        {
            struct sqrt{};
            struct sin{};
            struct cos{};
            struct asin{};
            struct acos{};
            struct tan{};
            struct atan{};
            struct floor{};
            struct ceil{};
        }

        namespace detail
        {
            template <typename T>
            T abs( T iValue )
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

                void operator()(const FloatType& x, FloatType& f2, FloatType& f3) const
                {
                    f2 = x*x - a;//x^2 -a solve for root = sqrt(a)
                    f3 = two*x;//2*x d(f'(x))/dx derivative
                }

            private:
                FloatType two;
                FloatType a;
            };

            template <typename FloatType, int ITERATIONS, typename Function>
            inline FloatType newton_root_safe(const Function& func, const FloatType& x1, const FloatType& x2, const FloatType& xacc)
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
                BOOST_ASSERT(false);
                return zero;// Never get here.
            }
        
            template <typename FloatType, int ITERATIONS>
            inline FloatType sqrt_newton(const FloatType& x, const FloatType& accuracyCutoff = 1e-50 )
            {
                ///solve for the root of x^2 - a (gives you sqrt(a) using the Newton Method with an accuracy cutoff of 1.0e-50.
                return newton_root_safe<FloatType, ITERATIONS>( sqrt_function_evaluator<FloatType>(x), FloatType(0), x, accuracyCutoff );
            }
        }

        #define GEOMETRIX_STD_UNARY_FUNCTION( F )                     \
            template <typename Arg>                                   \
            struct BOOST_PP_CAT(F,_function)                          \
            {                                                         \
                BOOST_TYPEOF_NESTED_TYPEDEF_TPL                       \
                (                                                     \
                    result                                            \
                  , ::std::F(Arg())                                   \
                );                                                    \
                typedef typename result::type result_type;            \
                result_type operator()( Arg a ) const                 \
                {                                                     \
                    return ::std::F(a);                               \
                }                                                     \
            };                                                        \
        /***/ 

        #define GEOMETRIX_STD_BINARY_FUNCTION( F )                    \
            template <typename Arg1, typename Arg2>                   \
            struct BOOST_PP_CAT(F,_function)                          \
            {                                                         \
                BOOST_TYPEOF_NESTED_TYPEDEF_TPL                       \
                (                                                     \
                    result                                            \
                  , ::std::F(Arg1(),Arg2())                           \
                );                                                    \
                typedef typename result::type result_type;            \
                result_type operator()( Arg1 a, Arg2 b ) const        \
                {                                                     \
                    return ::std::F(a,b);                             \
                }                                                     \
            };                                                        \
        /***/ 

        GEOMETRIX_STD_UNARY_FUNCTION( abs );
        GEOMETRIX_STD_UNARY_FUNCTION( sqrt );
        GEOMETRIX_STD_UNARY_FUNCTION( sin );
        GEOMETRIX_STD_UNARY_FUNCTION( asin );
        GEOMETRIX_STD_UNARY_FUNCTION( cos );
        GEOMETRIX_STD_UNARY_FUNCTION( acos );
        GEOMETRIX_STD_UNARY_FUNCTION( tan );
        GEOMETRIX_STD_UNARY_FUNCTION( atan );
        GEOMETRIX_STD_UNARY_FUNCTION( floor );
        GEOMETRIX_STD_UNARY_FUNCTION( ceil );

        GEOMETRIX_STD_BINARY_FUNCTION( pow );
        GEOMETRIX_STD_BINARY_FUNCTION( atan2 );
    
        struct absolute_value
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename abs_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<absolute_value(Arg)>::type operator()( Arg a ) const
            {
                return abs_function<Arg>()( a );
            }
        };
        
        struct sine
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename sin_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<sine(Arg)>::type operator()( Arg a ) const
            {
                return sin_function<Arg>()(a);
            }
        };

        struct arcsine
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename asin_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<arcsine(Arg)>::type operator()( Arg a ) const
            {
                return asin_function<Arg>()(a);
            }
        };

        struct cosine
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename cos_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<cosine(Arg)>::type operator()( Arg a ) const
            {
                return cos_function<Arg>()(a);
            }
        };

        struct arccosine
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename acos_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<arccosine(Arg)>::type operator()( Arg a ) const
            {
                return acos_function<Arg>()(a);
            }
        };
        
        struct square_root
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename sqrt_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<square_root(Arg)>::type operator()( Arg a ) const
            {
                return sqrt_function<Arg>()(a);
            }
        };
        
        struct tangent
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename tan_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<tangent(Arg)>::type operator()( Arg a ) const
            {
                return tan_function<Arg>()(a);
            }
        };

        struct arctangent
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename atan_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<arctangent(Arg)>::type operator()( Arg a ) const
            {
                return atan_function<Arg>()(a);
            }

            template <typename This, typename ArgY, typename ArgX>
            struct result<This(ArgY,ArgX)>
            {
                typedef typename atan2_function<ArgY, ArgX>::result_type type;
            };

            template <typename ArgY, typename ArgX>
            typename result<arctangent(ArgY,ArgX)>::type operator()( ArgY y, ArgX x ) const
            {
                return atan2_function<ArgY,ArgX>()(y,x);
            }
        };

        struct power
        {
            template<typename Sig>
            struct result{};
            
            template <typename This, typename Base, typename Exp>
            struct result<This(Base,Exp)>
            {
                typedef typename typename pow_function<Base,Exp>::result_type type;
            };

            template <typename Base, typename Exp>
            typename result<power(Base,Exp)>::type operator()( Base b, Exp e ) const
            {
                return pow_function<Base,Exp>()(b,e);
            }
        };
        
        struct round_down
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename floor_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<round_down(Arg)>::type operator()( Arg a ) const
            {
                return floor_function<Arg>()(a);
            }
        };

        struct round_up
        {
            template<typename Sig>
            struct result{};

            template <typename This, typename Arg>
            struct result<This(Arg)>
            {
                typedef typename ceil_function<Arg>::result_type type;
            };

            template <typename Arg>
            typename result<round_up(Arg)>::type operator()( Arg a ) const
            {
                return ceil_function<Arg>()(a);
            }
        };
        
        //! Compile time calculation of N^P (integral).
        template< int N, unsigned int P >
        struct power_c
        {
            static const boost::int64_t value = N * power_c<N,P-1>::value;
        };

        template < int N >
        struct power_c<N, 0>
        {
            static const boost::int64_t value = 1;
        };

        template <typename T>
        inline T integral_pow( T radix, T exponent )
        {        
            if( exponent < T(0) )
                return T(0);

            T p(1);
            while( true )
            {
                if( exponent == T(0) )
                    break;

                p *= radix;
                --exponent;            
            }

            return p;
        }
        
        template <typename N>
        typename boost::result_of<absolute_value(N)>::type abs( N v ) { return absolute_value()(v); }

        template <typename N>
        typename boost::result_of<square_root(N)>::type sqrt( N v ) { return square_root()(v); }

        template <typename Base, typename Exp>
        typename boost::result_of<power(Base, Exp)>::type pow( Base b, Exp e ) { return power()(b,e); }

        template <typename N>
        typename boost::result_of<sine(N)>::type sin( N v ) { return sine()(v); }
        
        template <typename N>
        typename boost::result_of<arcsine(N)>::type asin( N v ) { return arcsine()(v); }
        
        template <typename N>
        typename boost::result_of<cosine(N)>::type cos( N v ) { return cosine()(v); }
                
        template <typename N>
        typename boost::result_of<arccosine(N)>::type acos( N v ) { return arccosine()(v); }
        
        template <typename N>
        typename boost::result_of<tangent(N)>::type tan( N v ) { return tangent()(v); }
        
        template <typename N>
        typename boost::result_of<arctangent(N)>::type atan( N v ) { return arctangent()(v); }

        template <typename Y, typename X>
        typename boost::result_of<arctangent(Y, X)>::type atan2( Y y, X x ) { return arctangent()(y,x); }
                
        template <typename N>
        typename boost::result_of<round_down(N)>::type floor( N v ) { return round_down()(v); }
                
        template <typename N>
        typename boost::result_of<round_up(N)>::type ceil( N v ) { return round_up()(v); }
        
    }//namespace math;

    template <>
    struct function_traits<geometrix::math::absolute_value>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::sine>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::cosine>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::square_root>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::tangent>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::arcsine>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::arccosine>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::arctangent>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::round_up>
    {
        typedef void is_unary_function;
    };

    template <>
    struct function_traits<geometrix::math::round_down>
    {
        typedef void is_unary_function;
    };

}//namespace geometrix;

#endif //GEOMETRIX_MATH_FUNCTIONS_HPP
