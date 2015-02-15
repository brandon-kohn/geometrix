//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CONSTRUCTION_POLICY_HPP
#define GEOMETRIX_CONSTRUCTION_POLICY_HPP

#include <geometrix/utility/type_traits.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/preprocessor.hpp>

#if !defined( GEOMETRIX_CONSTRUCTION_POLICY_MAX_ARGS )
#define GEOMETRIX_CONSTRUCTION_POLICY_MAX_ARGS 10
#endif

namespace geometrix {

    //! \brief A trait type to construct a type T.
    //! The construction_policy type should provide an interface for constructing the type on which it is specialized. 
    //! This should include all the inputs needed to call the constructor of the specialized type (or to act as a constructor.)
    //! The construction_policy specialization should include a static member function that returns the constructed instance 
    //! from the required arguments.
    //! For example:
    //! \code
    //! struct A
    //! {
    //!     A( int a )
    //!     : m_a( a )
    //!     {}
    //! 
    //!     int m_a;
    //! };
    //!     
    //! template <>
    //! construction_policy< A >
    //! {
    //!     static A construct( int a ) { return A(a); }
    //! };
    //! \endcode
    template <typename T, typename EnableIf=void>
    struct construction_policy
    {    	
        BOOST_MPL_ASSERT_MSG( 
          ( false )
        , CONSTRUCTION_POLICY_NOT_DEFINED
        , (T) );
    };      

//! Helper macro to build args like const T1& t1, const T2& t2 etc.
#define GEOMETRIX_ARG_AT_INDEX( z, i, Arg )      \
    const BOOST_PP_CAT(Arg,i)& BOOST_PP_CAT(a,i) \
/***/
        
#define GEOMETRIX_CONSTRUCT_FUNCTION( z, i, _ )                                            \
template < typename T, BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(i),typename A) >                 \
inline T construct( BOOST_PP_ENUM( BOOST_PP_INC(i),GEOMETRIX_ARG_AT_INDEX,A) )             \
{                                                                                          \
    return construction_policy<T>::construct( BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(i), a) ); \
}                                                                                          \
/***/

//! Generate construction traits functions.
BOOST_PP_REPEAT( GEOMETRIX_CONSTRUCTION_POLICY_MAX_ARGS, GEOMETRIX_CONSTRUCT_FUNCTION, _ );

#undef GEOMETRIX_ARG_AT_INDEX
#undef GEOMETRIX_CONSTRUCT_FUNCTION

}//namespace geometrix;

#endif //GEOMETRIX_CONSTRUCTION_POLICY_HPP
