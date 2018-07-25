//
//! Copyright © 2008-2011
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
// CLASS numeric_sequence
//! A template class for specifying numeric_sequence which can be compile time
//! and runtime indexed conforming to random access for both and can support
//! vector and scalar arithmetic.
//!
template <typename NumericType>
class numeric_sequence<NumericType,DIMENSION>
{
    //! define the constructors via the preprocessor.
    template <typename T, BOOST_PP_ENUM_PARAMS(DIMENSION, typename U)>
    BOOST_CONSTEXPR static boost::array<T, DIMENSION> to_array(BOOST_PP_ENUM_BINARY_PARAMS(DIMENSION, U, const& a))
    {
        return { BOOST_PP_ENUM_PARAMS(DIMENSION, a) };
    }

public:

    typedef NumericType                             numeric_type;
    typedef dimension<DIMENSION>                    dimension_type;
    typedef boost::array< NumericType, DIMENSION >  numeric_array;
    typedef typename numeric_array::value_type      value_type;
    typedef typename numeric_array::reference       reference;
    typedef typename numeric_array::const_reference const_reference;

    numeric_sequence() = default;

    //! define the constructors via the preprocessor.
    template <typename A0, typename A1, typename... Args>
    numeric_sequence(A0&& a0, A1&& a1, Args&&...a)
        : m_sequence(to_array<NumericType>(std::forward<A0>(a0), std::forward<A1>(a1), std::forward<Args>(a)...))
    {
        static_assert((sizeof...(Args) + 2) == DIMENSION, "call to construct a numeric_sequence with wrong number of arguments.");
    }

    #define GEOMETRIX_ACCESS_EXPR_( z, i, e ) \
        geometrix::get<i>( e )                \
    /***/

    template <typename Expr>
    numeric_sequence( const Expr& e )
        : m_sequence(to_array<NumericType>( BOOST_PP_ENUM(DIMENSION, GEOMETRIX_ACCESS_EXPR_, e) ))
    {
        using expr_t = typename std::decay<Expr>::type;
        //! The type at each dimension of the expression e should be the same type, and should be convertible to the numeric_type of this numeric_sequence.
        //! If you get an error here, it likely means you are trying to construct a point or vector from an expression whose elements are not compatible with the numeric type of the point/vector.
        static_assert(is_homogeneous<expr_t>::value, "Cannot construct a point/vector/numeric_sequence from a non-homogeneous expression");
        using elem_t = typename std::decay<typename type_at<expr_t, 0>::type>::type;
        static_assert(std::is_convertible<elem_t, numeric_type>::value, "Cannot construct a point/vector/numeric_sequence from an expression whose element type is not convertible to the numeric type of the point/vector/numeric_sequence");
    }
    #undef GEOMETRIX_ACCESS_EXPR_

    numeric_sequence( const numeric_array& a )
        : m_sequence( a )
    {}

    template <std::size_t D>
    const numeric_type& get() const
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension< D >::value >= 0 && dimension< D >::value < dimension_type::value )
           , NUMERIC_SEQUENCE_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
           , ( dimension< D > )
        );

        return m_sequence[D];
    }

    template <std::size_t D>
    numeric_type& get()
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension< D >::value >= 0 && dimension< D >::value < dimension_type::value )
           , NUMERIC_SEQUENCE_GET_CALLED_WITH_INDEX_OUT_OF_BOUNDS
           , ( dimension< D > )
        );

        return m_sequence[D];
    }

    const numeric_type& get( std::size_t i ) const
    {
        GEOMETRIX_ASSERT( static_cast<int>(i) < dimension_type::value );
        return m_sequence[i];
    }

    numeric_type& get( std::size_t i )
    {
        GEOMETRIX_ASSERT( static_cast<int>(i) < dimension_type::value );
        return m_sequence[i];
    }

    template <std::size_t Index>
    void set(const numeric_type& v)
    {
        GEOMETRIX_STATIC_ASSERT(Index < DIMENSION);
        m_sequence[Index] = v;
    }

    void set(std::size_t index, const numeric_type& v)
    {
        GEOMETRIX_ASSERT(index < DIMENSION);
        m_sequence[index] = v;
    }

    const numeric_type& operator[]( std::size_t i ) const
    {
        GEOMETRIX_ASSERT( static_cast<int>(i) < dimension_type::value );
        return m_sequence[i];
    }

    numeric_type& operator[]( std::size_t i )
    {
        GEOMETRIX_ASSERT( static_cast<int>(i) < dimension_type::value );
        return m_sequence[i];
    }

    template <typename Expr>
    numeric_sequence& operator=( const Expr& e )
    {
        //! Helper macro to build access traits code.
        #define GEOMETRIX_ASSIGN_ACCESS_EXPR_( z, i, e ) \
            (*this)[i] = geometrix::get<i>( e );         \
        /***/
        BOOST_PP_REPEAT( DIMENSION, GEOMETRIX_ASSIGN_ACCESS_EXPR_, e )
        return *this;
        #undef GEOMETRIX_ASSIGN_ACCESS_EXPR
    }

protected:

    //! Operator interface
    template <typename Expr>
    numeric_sequence& operator+= ( const Expr& p )
    {
        typedef boost::fusion::vector<numeric_array&, const Expr&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( m_sequence, p ) ), make_fused_procedure( boost::lambda::_1 += boost::lambda::_2 ) );
        return *this;
    }

    // numeric_sequence operator+(numeric_sequence, const numeric_sequence&) automatically
    // generated by addable.
    template <typename Expr>
    numeric_sequence& operator-= ( const Expr& p )
    {
        typedef boost::fusion::vector<numeric_array&, const Expr&> sequences;
        boost::fusion::for_each( boost::fusion::zip_view<sequences>( sequences( m_sequence, p ) ), make_fused_procedure( boost::lambda::_1 -= boost::lambda::_2 ) );
        return *this;
    }
    // numeric_sequence operator-(numeric_sequence, const numeric_sequence&) automatically
    // generated by subtractable.
    template <typename Expr>
    numeric_sequence& operator*= ( const Expr& v )
    {
        boost::fusion::for_each( m_sequence, boost::lambda::_1 *= v );
        return *this;
    }
    // numeric_sequence operator*(numeric_sequence, const T&) and
    // numeric_sequence operator*(const T&, numeric_sequence) auto-generated
    // by multipliable.
    template <typename Expr>
    numeric_sequence& operator/= ( const Expr& v )
    {
        boost::fusion::for_each( m_sequence, boost::lambda::_1 /= v );
        return *this;
    }
    // numeric_sequence operator/(numeric_sequence, const T&) auto-generated
    // by dividable.

    numeric_array m_sequence;

};

}//namespace geometrix;

#undef DIMENSION
