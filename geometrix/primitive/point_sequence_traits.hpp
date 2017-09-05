//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_POINT_SEQUENCE_TRAITS_HPP
#define GEOMETRIX_POINT_SEQUENCE_TRAITS_HPP

#include <geometrix/primitive/point_traits.hpp>

namespace geometrix {

//! \brief Tag to check if a type is a point sequence type.

//! \ingroup Type Traits
template <typename PointSequence, typename Enable = void>
struct is_point_sequence : boost::false_type{};

//! \brief point_sequence_traits define the typedef and access interface to an ordered collection of points.

//! \ingroup PrimitiveTraits
//! \ingroup Primitives
//! The point_sequence_traits interface requires an stl conforming iterator iterface for the sequence type.
template <typename PointSequence, typename EnableIf=void>
struct point_sequence_traits
{
    BOOST_MPL_ASSERT_MSG( 
          ( false )
        , POINT_SEQUENCE_TRAITS_NOT_DEFINED
        , (PointSequence) );	
};

//! \brief Concept of a sequence of points.

//! \ingroup Concepts
//! \ingroup PrimitiveConcepts
//! Point sequences are constrained to define a point_type as well as to provide an iterator interface via the point_sequence_traits specialization.
template <typename PointSequence>
struct PointSequenceConcept
{
    //! traits must define point type
    typedef typename point_sequence_traits< PointSequence >::point_type     point_type;

    //! Check that is is indeed a point.
    BOOST_CONCEPT_ASSERT((PointConcept< point_type >));

    //! traits define iterator access?.. or should there be access traits?
    typedef typename point_sequence_traits< PointSequence >::iterator               iterator;
    typedef typename point_sequence_traits< PointSequence >::const_iterator         const_iterator;

#if defined(GEOMETRIX_USE_REVERSIBLE_POINT_SEQUENCE_TRAITS)
    typedef typename point_sequence_traits< PointSequence >::reverse_iterator       reverse_iterator;
    typedef typename point_sequence_traits< PointSequence >::const_reverse_iterator const_reverse_iterator;
#endif

    BOOST_CONCEPT_USAGE(PointSequenceConcept<PointSequence>)
    {        
        //! Check the access interface.
        PointSequence* pSequence = 0;

        //! iterator access must be defined for both const_iterator and iterator types
        iterator it = point_sequence_traits< PointSequence >::begin( *pSequence );
        it = point_sequence_traits< PointSequence >::end( *pSequence );

        const_iterator cit = point_sequence_traits< PointSequence >::begin( *pSequence );
        cit = point_sequence_traits< PointSequence >::end( *pSequence );

#if defined(GEOMETRIX_USE_REVERSIBLE_POINT_SEQUENCE_TRAITS)
        //! iterator access must be defined for both reverse_const_iterator and reverse_iterator types
        reverse_iterator rit = point_sequence_traits< PointSequence >::rbegin( *pSequence );
        rit = point_sequence_traits< PointSequence >::rend( *pSequence );

        const_reverse_iterator rcit = point_sequence_traits< PointSequence >::rbegin( *pSequence );
        rcit = point_sequence_traits< PointSequence >::rend( *pSequence );
#endif

        //! random access.
        const point_type& point1 = point_sequence_traits< PointSequence >::get_point( *pSequence, 0 );
        boost::ignore_unused_variable_warning(point1);

        //! access the front
        const point_type& point2 = point_sequence_traits< PointSequence >::front( *pSequence );
        boost::ignore_unused_variable_warning(point2);

        //! access the back
        const point_type& point3 = point_sequence_traits< PointSequence >::back( *pSequence );
        boost::ignore_unused_variable_warning(point3);

        //! stl type stuff
        std::size_t s = point_sequence_traits< PointSequence >::size( *pSequence );
        boost::ignore_unused_variable_warning(s);
        bool empty = point_sequence_traits< PointSequence >::empty( *pSequence );
        boost::ignore_unused_variable_warning(empty);
    }
};

enum class polygon_winding { clockwise, counterclockwise };

//! Specialization for pulling arithmetic types out of geometric_traits.
template <typename T>
struct arithmetic_type_of<T, typename boost::enable_if<typename is_point_sequence<T>::type>::type>
: arithmetic_type_of<typename point_sequence_traits<T>::point_type>
{};

template <typename T>
struct dimension_of<T, typename boost::enable_if<typename is_point_sequence<T>::type>::type>
: dimension_of<typename point_sequence_traits<T>::point_type>
{};

}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEQUENCE_TRAITS_HPP
