//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_NUMERIC_TRAITS_HPP
#define GEOMETRIX_NUMERIC_TRAITS_HPP

#include <geometrix/utility/construction_policy.hpp>

#include <boost/cstdint.hpp>
#include <climits>

namespace geometrix {
    
//! Default numeric traits struct. 
//! NOTE: must be specialized for user types.
template <typename NumericType, typename EnableIf=void>
struct numeric_traits
{};

//! \brief Tag to check if a type is a numeric type.
template <typename NumericType, typename Enable = void>
struct is_numeric : boost::false_type{};

template <typename NumericType>
struct is_numeric<NumericType, typename numeric_traits<NumericType>::is_numeric> : boost::true_type{};

template <typename T1, typename T2, typename EnableIf=void>
struct numeric_width_compare 
    : boost::mpl::less
        <
            boost::mpl::int_<numeric_traits< T1 >::digits>
          , boost::mpl::int_<numeric_traits< T2 >::digits> 
        >
{};

template <typename T1, typename T2>
struct numeric_width_compare
    < 
        T1
      , T2
      , typename boost::enable_if
        < 
            boost::mpl::and_
            < 
                typename numeric_traits<T1>::is_integral
              , typename numeric_traits<T2>::is_float 
            > 
        >::type 
    > : boost::true_type {};

template <typename T1, typename T2>
struct numeric_width_compare
    < 
        T1
      , T2
      , typename boost::enable_if
        <
            boost::mpl::and_
            < 
                typename numeric_traits<T1>::is_float
              , typename numeric_traits<T2>::is_integral 
            > 
        >::type
    > : boost::false_type {};

template <typename NumericType>                                                                           
struct numeric_traits< NumericType, typename boost::enable_if< boost::is_arithmetic<NumericType> >::type >                                                  
{                                                                                     
    typedef NumericType                                       numeric_type;                    
    typedef typename boost::is_float< numeric_type >::type    is_float;                        
    typedef typename boost::is_integral< numeric_type >::type is_integral;                     
    typedef void                                              is_numeric;     
        
    BOOST_STATIC_CONSTANT                                                             
    (                                                                                 
        unsigned int                                                                  
      , digits = std::numeric_limits<numeric_type>::digits10                          
    );                                                                                
                                                                                      
    BOOST_MPL_ASSERT_MSG(                                                             
        ( !( is_float::value && is_integral::value ) )                                
        , NUMERIC_TRAITS_CANNOT_DEFINE_FLOAT_AND_INTEGRAL
        , (NumericType) );                                                            
};                                                                                    

template <typename NumericType>                                                                           
struct construction_policy< NumericType, typename boost::enable_if< boost::is_arithmetic<NumericType> >::type  >                                             
{
    static const NumericType& construct( const NumericType& t ){ return t; } 
};

//! Macro for native types which work with boost::is_float/is_integral for automatic determination of type.
#define GEOMETRIX_DEFINE_NUMERIC_TRAITS( NumericType )                                \
namespace geometrix {                                                                 \
template <>                                                                           \
struct numeric_traits< NumericType >                                                  \
{                                                                                     \
    typedef NumericType                              numeric_type;                    \
    typedef boost::is_float< numeric_type >::type    is_float;                        \
    typedef boost::is_integral< numeric_type >::type is_integral;                     \
    BOOST_STATIC_CONSTANT                                                             \
    (                                                                                 \
        unsigned int                                                                  \
      , digits = std::numeric_limits<numeric_type>::digits10                          \
    );                                                                                \
    typedef void is_numeric;                                                          \
                                                                                      \
    BOOST_MPL_ASSERT_MSG(                                                             \
        ( !( is_float::value && is_integral::value ) )                                \
        , NUMERIC_TRAITS_CANNOT_DEFINE_FLOAT_AND_INTEGRAL                             \
        , (NumericType) );                                                            \
};                                                                                    \
template <>                                                                           \
struct construction_policy< NumericType >                                             \
{ static const NumericType& construct( const NumericType& t ){ return t; } };         \
}                                                                                     \
/***/

//! Macro for defining numeric_traits for integral types.
#define GEOMETRIX_DEFINE_INTEGRAL_NUMERIC_TRAITS( NumericType, Digits )               \
namespace geometrix {                                                                 \
template <>                                                                           \
struct numeric_traits< NumericType >                                                  \
{                                                                                     \
    typedef NumericType       numeric_type;                                           \
    BOOST_STATIC_CONSTANT                                                             \
    (                                                                                 \
        unsigned int                                                                  \
      , digits = Digits                                                               \
    );                                                                                \
    typedef boost::false_type is_float ;                                              \
    typedef boost::true_type  is_integral;                                            \
    typedef void is_numeric;                                                          \
};                                                                                    \
}                                                                                     \
/***/

//! Macro for defining numeric_traits for floating point types.
#define GEOMETRIX_DEFINE_FLOATING_POINT_NUMERIC_TRAITS(Numeric, Precision)            \
namespace geometrix {                                                                 \
template <>                                                                           \
struct numeric_traits< Numeric >                                                      \
{                                                                                     \
    typedef Numeric       numeric_type;                                               \
    BOOST_STATIC_CONSTANT                                                             \
    (                                                                                 \
        unsigned int                                                                  \
      , digits = Precision                                                            \
    );                                                                                \
    typedef void is_numeric;                                                          \
    typedef boost::true_type  is_float;                                               \
    typedef boost::false_type is_integral;                                            \
};                                                                                    \
}                                                                                     \
/***/

}//namespace geometrix;

#endif //GEOMETRIX_NUMERIC_TRAITS_HPP
