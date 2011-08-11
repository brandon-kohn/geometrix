//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(GEOMETRIX_NUMERIC_SEQUENCE_HPP)
#define GEOMETRIX_NUMERIC_SEQUENCE_HPP

#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/detail/tensor_access_defines.hpp>
#include <geometrix/tensor/detail/numeric_sequence_limits.hpp>
#include <geometrix/tensor/detail/numeric_sequence_generator.hpp>
#include <geometrix/tensor/sequence.hpp>
#include <geometrix/numeric/numeric_traits.hpp>

#include <boost/mpl/max_element.hpp>
#include <boost/mpl/if.hpp>
#include <boost/fusion/sequence/intrinsic.hpp>
#include <boost/call_traits.hpp>

namespace geometrix {

//! \brief Tag to check if a type is a numeric_sequence
template <typename Sequence, typename Enable = void>
struct is_numeric_sequence : boost::false_type{};

template <typename Sequence>
struct is_numeric_sequence<Sequence, typename geometric_traits<Sequence>::is_numeric_sequence> : boost::true_type{};

//! \brief Concept check for a type which models a NumericSequence.

//! A NumericSequence is a model of a Sequence type with the additional 
//! constraint that it must have a numeric type as the value_type.
template <typename NumericSequence>
struct NumericSequenceConcept
{
    void constraints() const
    {
        boost::function_requires< SequenceConcept< NumericSequence > >();
    }
};

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename NumericSequence>
struct NumericSequence2DConcept
{
    void constraints() const
    {
        boost::function_requires< NumericSequenceConcept< NumericSequence > >();        
        BOOST_STATIC_ASSERT( dimension_of<NumericSequence>::value == 2 );
    }
};

//! \ingroup Concepts 
//! \ingroup PrimitiveConcepts
template <typename NumericSequence>
struct NumericSequence3DConcept
{
    void constraints() const
    {
        boost::function_requires< NumericSequenceConcept< NumericSequence > >();        
        BOOST_STATIC_ASSERT( dimension_of<NumericSequence>::value == 3 );
    }
};

//! Helper macro to build access traits code.
#define GEOMETRIX_ACCESS_ARG_( z, i, T ) \
geometrix::get<i>( args )                \
/***/

#define GEOMETRIX_PARAM_ARG_( z, i, T )       \
const BOOST_PP_CAT(A,i)& BOOST_PP_CAT( a, i ) \
/***/

//! Helper macro to build args like const T1& t1, const T2& t2 etc.
#define GEOMETRIX_ARG_AT_INDEX( z, i, Arg )      \
    BOOST_PP_CAT(Arg,i) BOOST_PP_CAT(a,i) \
/***/

//! Macro for defining a basic point/vector construction policy for types using constructors.
#define GEOMETRIX_DEFINE_NUMERIC_SEQUENCE_CONSTRUCTION_POLICY( Sequence, Dimension )            \
namespace geometrix {                                                                           \
template <>                                                                                     \
struct construction_policy< Sequence >                                                          \
{                                                                                               \
    template < BOOST_PP_ENUM_PARAMS( Dimension,typename A) >                                    \
    static Sequence construct( BOOST_PP_ENUM( Dimension, GEOMETRIX_PARAM_ARG_, Sequence ) )     \
    {                                                                                           \
        return Sequence( BOOST_PP_ENUM_PARAMS(Dimension, a) );                                  \
    }                                                                                           \
                                                                                                \
    template <typename NumericSequence>                                                         \
    static Sequence construct( const NumericSequence& args )                                    \
    {                                                                                           \
        boost::function_requires< CompileTimeAccessConcept< NumericSequence > >();              \
        return Sequence( BOOST_PP_ENUM( Dimension, GEOMETRIX_ACCESS_ARG_, NumericSequence ) );  \
    }                                                                                           \
};                                                                                              \
}                                                                                               \
/***/

//! Macro for defining a point/vector construction policy for POD types using static initialization.
#define GEOMETRIX_DEFINE_POD_NUMERIC_SEQUENCE_CONSTRUCTION_POLICY( Sequence, Dimension )        \
namespace geometrix {                                                                           \
template <>                                                                                     \
struct construction_policy< Sequence >                                                          \
{                                                                                               \
    template < BOOST_PP_ENUM_PARAMS( Dimension,typename A) >                                    \
    static Sequence construct( BOOST_PP_ENUM( Dimension, GEOMETRIX_PARAM_ARG_, Sequence ) )     \
    {                                                                                           \
        Sequence s={ BOOST_PP_ENUM_PARAMS(Dimension, a) };                                      \
        return s;                                                                               \
    }                                                                                           \
                                                                                                \
    template <typename NumericSequence>                                                         \
    static Sequence construct( const NumericSequence& args )                                    \
    {                                                                                           \
        boost::function_requires< CompileTimeAccessConcept< NumericSequence > >();              \
        Sequence s ={ BOOST_PP_ENUM( Dimension, GEOMETRIX_ACCESS_ARG_, NumericSequence ) };     \
        return s;                                                                               \
    }                                                                                           \
};                                                                                              \
}                                                                                               \
/***/

}//namespace geometrix;

#endif//GEOMETRIX_NUMERIC_SEQUENCE_HPP
