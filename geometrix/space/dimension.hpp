//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DIMENSION_HPP
#define GEOMETRIX_DIMENSION_HPP

#include <geometrix/geometric_traits.hpp>

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>

namespace geometrix {

    //! A type trait specialized on dimension.
	template <std::size_t Dimension>
    struct dimension : boost::mpl::int_< Dimension >{};

    template <typename T, typename EnableIf=void>
    struct dimension_of 
    {
        typedef typename geometric_traits<typename geometrix::remove_const_ref<T>::type>::dimension_type type;
		BOOST_STATIC_CONSTANT( std::size_t, value = type::value );
    };

    //! A concept type to express membership in a dimension.
    //! 
    //! Membership to a dimension means the item requires a number of linearly independent vectors to be described
    //! (no more and no less.)
    //! (e.g. A 2-dimensional point has coordinates which are described by a basis of 2 linearly independent 
    //!  vectors.)
	template <typename T, std::size_t Dimension>
    struct DimensionConcept
    {
		GEOMETRIX_STATIC_ASSERT( (dimension_of<T>::value == Dimension) );

        void constraints() const
        {
            
        }
    };

    //! \brief A meta-function for comparing the dimensional cardinality of two types via their specialized dimension_traits to find the lesser of the two.
    template <typename T1, typename T2, typename EnableIf=void>
    struct dimension_less : 
            boost::mpl::less
            <
                typename dimension_of< T1 >::type
              , typename dimension_of< T2 >::type
            >
    {};

    template <typename T1, typename T2, typename EnableIf=void>
    struct dimensions_equal :
            boost::mpl::equal
            <
                typename dimension_of< T1 >::type
              , typename dimension_of< T2 >::type
            >
    {};

    //! Given two numeric types, decide on the wider type to be used in arithmetic.
    template <typename T1, typename T2, typename EnableIf=void>
    struct max_dimension_of_2
        : boost::mpl::deref
            <
                typename boost::mpl::max_element
                < 
                    boost::mpl::vector< T1, T2 >
                  , dimension_less<boost::mpl::_1, boost::mpl::_2> 
                >::type
            >
    {};

    //! A concept to check if a dimension holds an index.
    template <typename Dimension, std::size_t Index>
    struct DimensionContainsIndexConcept
    {
        GEOMETRIX_STATIC_ASSERT(( static_cast<int>(Index) < Dimension::value ));
    };

}//namespace geometrix;

#endif //GEOMETRIX_DIMENSION_HPP
