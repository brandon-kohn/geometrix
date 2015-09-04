//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

namespace geometrix {

#define DIMENSION BOOST_PP_ITERATION()

/////////////////////////////////////////////////////////////////////////////
//
// CLASS point
//! A template class for specifying point which can be compile time
//! and runtime indexed conforming to random access for both and can support
//! vector arithmetic.
//!
template <typename NumericType>
class point<NumericType, DIMENSION>
    : public numeric_sequence< NumericType, DIMENSION >        
#if defined( GEOMETRIX_DEFINE_POINT_OPERATORS )
    , boost::additive< point<NumericType, DIMENSION>, vector< NumericType, DIMENSION >     // point +- numeric_sequence
    >
#endif
{
public:

    typedef numeric_sequence< NumericType, DIMENSION > sequence_type;
    typedef typename sequence_type::numeric_type       coordinate_type;

    point(){}

    //! define the constructors via the preprocessor.
    point( BOOST_PP_ENUM_PARAMS(DIMENSION, const coordinate_type& a) )
        : sequence_type( BOOST_PP_ENUM_PARAMS(DIMENSION, a) )
    {
    }

    template <typename NumericSequence>
    point( const NumericSequence& a )
        : sequence_type( a )
    {}
#if defined( GEOMETRIX_DEFINE_POINT_OPERATORS )
    //! Operator interface    
    point operator+= ( const sequence_type& p )
    {
        sequence_type::operator +=( p );
        return *this;
    }

    // numeric_sequence operator+(numeric_sequence, const numeric_sequence&) automatically
    // generated by addable.

    point operator-= ( const sequence_type& p )
    {
        sequence_type::operator -=( p );
        return *this;
    }
    // numeric_sequence operator-(numeric_sequence, const numeric_sequence&) automatically
    // generated by subtractable.
#endif
};

//! Helper macro to build access traits code.
#define GEOMETRIX_ACCESS_ARG_POINT_( z, i, _ ) \
    geometrix::get<i>( args )                  \
/***/

template <typename N>
struct construction_policy< point<N, DIMENSION> >
{    
    static point<N, DIMENSION> construct( BOOST_PP_ENUM_PARAMS(DIMENSION, const N& a) )
    {
        return point<N, DIMENSION>( BOOST_PP_ENUM_PARAMS(DIMENSION, a) );
    }
    
    template <typename NumericSequence>                                                                         
    static point<N,DIMENSION> construct( const NumericSequence& args )                                                    
    {                                                                                                           
        return point<N, DIMENSION>( BOOST_PP_ENUM( DIMENSION, GEOMETRIX_ACCESS_ARG_POINT_, NumericSequence ) );
    }                                                                                                       
};

template <typename T>
struct assignment_policy < point< T, DIMENSION> >
{
    static void assign(point<T, DIMENSION>& v, BOOST_PP_ENUM_PARAMS(DIMENSION, const T& a))
    {
        BOOST_PP_REPEAT(DIMENSION, GEOMETRIX_SET_ARG_VALUE_AT_INDEX_, _);
    }

    template <typename NumericSequence>
    static void assign(point<T, DIMENSION>& v, const NumericSequence& args)
    {
        BOOST_PP_REPEAT(DIMENSION, GEOMETRIX_SET_VALUE_AT_INDEX_, args);
    }
};

#undef GEOMETRIX_ACCESS_ARG_POINT_
          
}//namespace geometrix;

#undef DIMENSION
