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

template <typename PointSequence>
struct is_point_sequence<PointSequence, typename geometric_traits<typename std::decay<PointSequence>::type>::is_point_sequence> : boost::true_type{};

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
    using type_t = typename std::decay<PointSequence>::type;
    
    //! traits must define point type
    typedef typename point_sequence_traits< type_t >::point_type     point_type;
    
    //! Should identify as a point sequence.
    using is_point_sequence = typename geometric_traits<type_t>::is_point_sequence;

    //! Check that is is indeed a point.
    BOOST_CONCEPT_ASSERT((PointConcept< point_type >));

    //! traits define iterator access?.. or should there be access traits?
    typedef typename point_sequence_traits< type_t >::iterator               iterator;
    typedef typename point_sequence_traits< type_t >::const_iterator         const_iterator;

#if defined(GEOMETRIX_USE_REVERSIBLE_POINT_SEQUENCE_TRAITS)
    typedef typename point_sequence_traits< type_t >::reverse_iterator       reverse_iterator;
    typedef typename point_sequence_traits< type_t >::const_reverse_iterator const_reverse_iterator;
#endif

    BOOST_CONCEPT_USAGE(PointSequenceConcept<PointSequence>)
    {
        //! Check the access interface.
        type_t* pSequence = 0;

        //! iterator access must be defined for both const_iterator and iterator types
        iterator bit = point_sequence_traits< type_t >::begin( *pSequence );
        iterator eit = point_sequence_traits< type_t >::end( *pSequence );

        const_iterator cit = point_sequence_traits< type_t >::begin( *pSequence );
        const_iterator ecit = point_sequence_traits< type_t >::end( *pSequence );

#if defined(GEOMETRIX_USE_REVERSIBLE_POINT_SEQUENCE_TRAITS)
        //! iterator access must be defined for both reverse_const_iterator and reverse_iterator types
        reverse_iterator rit = point_sequence_traits< type_t >::rbegin( *pSequence );
        reverse_iterator erit = point_sequence_traits< type_t >::rend( *pSequence );

        const_reverse_iterator rcit = point_sequence_traits< type_t >::rbegin( *pSequence );
        const_reverse_iterator ercit = point_sequence_traits< type_t >::rend( *pSequence );
#endif

        //! random access.
        const point_type& point1 = point_sequence_traits< type_t >::get_point( *pSequence, 0 );
        boost::ignore_unused_variable_warning(point1);

        //! access the front
        const point_type& point2 = point_sequence_traits< type_t >::front( *pSequence );
        boost::ignore_unused_variable_warning(point2);

        //! access the back
        const point_type& point3 = point_sequence_traits< type_t >::back( *pSequence );
        boost::ignore_unused_variable_warning(point3);

        //! stl type stuff
        std::size_t s = point_sequence_traits< type_t >::size( *pSequence );
        boost::ignore_unused_variable_warning(s);
        bool empty = point_sequence_traits< type_t >::empty( *pSequence );
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

template <typename T, typename std::enable_if<is_point_sequence<T>::value, int>::type = 0>
inline std::size_t number_vertices(const T& s)
{
    BOOST_CONCEPT_ASSERT((PointSequenceConcept<T>));
    return point_sequence_traits<T>::size(s);
}

template <typename T, typename std::enable_if<is_point_sequence<T>::value, int>::type = 0>
inline typename point_sequence_traits<T>::point_type get_vertex(const T& s, std::size_t i)
{
    BOOST_CONCEPT_ASSERT((PointSequenceConcept<T>));
    return point_sequence_traits<T>::get_point(s, i);
}

}//namespace geometrix;

#endif //GEOMETRIX_POINT_SEQUENCE_TRAITS_HPP
