//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_CONSTRUCTION_TRAITS_HPP
#define GENERATIVE_GEOMETRY_CONSTRUCTION_TRAITS_HPP
#pragma once

#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! \brief A trait type to construct a type T.
    //! The construction_traits type should provide an interface for constructing the type on which it is specialized. 
    //! This should include all the inputs needed to call the constructor of the specialized type (or to act as a constructor.)
    //! The construction_traits specialization should include a static member function that returns the constructed instance 
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
    //! construction_traits< A >
    //! {
    //!     static A construct( int a ) { return A(a); }
    //! };
    //! \endcode
    template <typename T>
    struct construction_traits
    {    	
        BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, CONSTRUCTION_TRAITS_NOT_DEFINED
		, (T) );
    };

}}}//namespace generative::numeric::geometry;

//! Helper macro to build access traits code.
#define GENERATIVE_GEOMETRY_INDEXED_ACCESS_ARG_( z, i, T )                \
generative::numeric::geometry::indexed_access_traits< T >::get<i>( args ) \
/***/

//! Macro for defining a basic point/vector construction traits.
#define GENERATIVE_GEOMETRY_DEFINE_NUMERIC_SEQUENCE_CONSTRUCTION_TRAITS( Sequence, Dimension )                  \
namespace generative{namespace numeric{namespace geometry{                                                      \
template <>                                                                                                     \
struct construction_traits< Sequence >                                                                          \
{                                                                                                               \
    typedef numeric_sequence_traits< Sequence >::numeric_type numeric_type;                                     \
    static Sequence construct( BOOST_PP_ENUM_PARAMS(Dimension, const numeric_type& a) )                         \
    {                                                                                                           \
        return Sequence( BOOST_PP_ENUM_PARAMS(Dimension, a) );                                                  \
    }                                                                                                           \
                                                                                                                \
    template <typename NumericSequence>                                                                         \
    static Sequence construct( const NumericSequence& args )                                                    \
    {                                                                                                           \
        boost::function_requires< CompileTimeIndexedAccessConcept< NumericSequence > >();                       \
        return Sequence( BOOST_PP_ENUM( Dimension, GENERATIVE_GEOMETRY_INDEXED_ACCESS_ARG_, NumericSequence ) );\
    }                                                                                                           \
};                                                                                                              \
}}}                                                                                                             \
/***/

#endif //GENERATIVE_GEOMETRY_CONSTRUCTION_TRAITS_HPP
