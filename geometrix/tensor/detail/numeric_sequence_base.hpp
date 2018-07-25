//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DETAIL_NUMERIC_SEQUENCE_BASE_HPP
#define GEOMETRIX_DETAIL_NUMERIC_SEQUENCE_BASE_HPP

#include <geometrix/tensor/detail/numeric_sequence_headers.hpp>
#include <geometrix/tensor/sequence.hpp>
#include <type_traits>

namespace geometrix {

    template <typename F>
    inline boost::fusion::fused_procedure< F > make_fused_procedure( const F& f = F() )
    {
        return boost::fusion::fused_procedure< F >( f );
    }

    template <typename F>
    inline boost::fusion::fused< F > make_fused( const F& f = F() )
    {
        return boost::fusion::fused< F >( f );
    }

    template <typename NumericType, std::size_t D>
    class numeric_sequence;

    //! Create a boost array initialized to some value.
    template <typename T, std::size_t D>
    inline boost::array<T,D> make_initialized_array( const T& initValue )
    {
        boost::array<T,D> numericSequence;
        boost::fusion::for_each( numericSequence, boost::lambda::_1 = initValue );
        return numericSequence;
    }

    //! Create a boost array initialized to some value.
    template <typename Sequence>
    inline boost::array< typename geometric_traits< Sequence >::value_type,
        geometric_traits< Sequence >::dimension_type::value > make_initialized_array( const Sequence& sequence )
    {
        typedef typename geometric_traits< Sequence >::value_type     value_type;
        typedef typename geometric_traits< Sequence >::dimension_type dimension_type;

        boost::array< value_type, dimension_type::value > arraySequence;

        typedef boost::fusion::vector< boost::array< value_type, dimension_type::value >&, const Sequence& > sequences;
        boost::fusion::for_each(
            boost::fusion::zip_view<sequences>(
                sequences( arraySequence, sequence ) ),
                make_fused_procedure( boost::lambda::_1 = boost::lambda::_2 ) );

        return arraySequence;
    }

    namespace detail {
        template <typename T, typename... Args>
        inline BOOST_CONSTEXPR boost::array<T,sizeof...(Args)> make_array(Args&&... a) 
        {
            return { std::forward<Args>(a)... };
        }
    }

    template <typename NumericType, std::size_t Dimension>
    class numeric_sequence
    {
            BOOST_MPL_ASSERT_MSG(
              ( false )
            , NUMERIC_SEQUENCE_DIMENSION_NOT_DEFINED
            , (NumericType) );
    };

    template <typename NumericType>
    class numeric_sequence<NumericType,0>
    {
        BOOST_MPL_ASSERT_MSG(
              ( false )
            , NUMERIC_SEQUENCE_CANNOT_HAVE_ZERO_DIMENSION
            , (NumericType) );
    };

}//namespace geometrix;

//GEOMETRIX_DETAIL_NUMERIC_SEQUENCE_BASE_HPP
#endif
