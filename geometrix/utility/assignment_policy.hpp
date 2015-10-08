//
//! Copyright © 2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ASSIGNMENT_POLICY_HPP
#define GEOMETRIX_ASSIGNMENT_POLICY_HPP

#include <geometrix/utility/type_traits.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/preprocessor.hpp>

#if !defined( GEOMETRIX_ASSIGNMENT_POLICY_MAX_ARGS )
#define GEOMETRIX_ASSIGNMENT_POLICY_MAX_ARGS 10
#endif

namespace geometrix {

    //! \brief A trait type to assign a type T.
    //! The assignment_policy type should provide an interface for assigning the type on which it is specialized. 
    //! This should include all the inputs needed to call the assignor of the specialized type (or to act as a assignor.)
    //! The assignment_policy specialization should include a static member function that returns the assigned instance 
    //! from the required arguments.
    //! For example:
    //! \code
    //! struct A
    //! {
    //!     A( int a )
    //!     : m_a( a )
    //!     {}
    //!    
    //!     void set(int v) { m_a = v; }
    //! 
    //!     int m_a;
    //! };
    //!     
    //! template <>
    //! assignment_policy< A >
    //! {
    //!     static void assign( A& v, int a ) { v.set(a); }
    //! };
    //! \endcode
    template <typename T, typename EnableIf=void>
    struct assignment_policy
    {    	
        BOOST_MPL_ASSERT_MSG( 
          ( false )
        , ASSIGNMENT_POLICY_NOT_DEFINED
        , (T) );
    };      

//! Helper macro to build args like const T1& t1, const T2& t2 etc.
#define GEOMETRIX_ARG_AT_INDEX( z, i, Arg )      \
    const BOOST_PP_CAT(Arg,i)& BOOST_PP_CAT(a,i) \
/***/
        
#define GEOMETRIX_ASSIGNMENT_FUNCTION( z, i, _ )                                           \
template < typename T, BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(i),typename A) >                 \
inline void assign( T& v, BOOST_PP_ENUM( BOOST_PP_INC(i),GEOMETRIX_ARG_AT_INDEX,A) )       \
{                                                                                          \
    assignment_policy<T>::assign( v, BOOST_PP_ENUM_PARAMS( BOOST_PP_INC(i), a) );          \
}                                                                                          \
/***/

//! Generate assignment traits functions.
BOOST_PP_REPEAT(GEOMETRIX_ASSIGNMENT_POLICY_MAX_ARGS, GEOMETRIX_ASSIGNMENT_FUNCTION, _);

#define GEOMETRIX_SET_ARG_VALUE_AT_INDEX_(z, i, _) \
    geometrix::set<i>(v, BOOST_PP_CAT(a,i));       \
/***/

#define GEOMETRIX_SET_VALUE_AT_INDEX_(z, i, seq)  \
    geometrix::set<i>(v, geometrix::get<i>(seq)); \
/***/

#undef GEOMETRIX_ARG_AT_INDEX
#undef GEOMETRIX_CONSTRUCT_FUNCTION

}//namespace geometrix;

#endif //GEOMETRIX_ASSIGNMENT_POLICY_HPP
