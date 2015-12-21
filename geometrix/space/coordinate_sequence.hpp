//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_COORDINATE_SEQUENCE_TRAITS_HPP
#define GEOMETRIX_COORDINATE_SEQUENCE_TRAITS_HPP

#include <geometrix/geometric_traits.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>

namespace geometrix {

//! \brief Tag to check if a type is a coordinate_sequence
template <typename Sequence, typename EnableIf = void>
struct is_coordinate_sequence : boost::false_type{};

//! \brief A traits type to define a sequence of a coordinate type with a static tensor_order.
//! A coordinate sequence is a numeric sequence with an additional property of having a coordinate frame of reference.
template <typename Sequence>
struct is_coordinate_sequence<Sequence, typename geometric_traits<typename geometrix::remove_const_ref<Sequence>::type>::is_coordinate_sequence> : boost::true_type{};

//! \brief Concept check for a type which models a CoordinateSequence.

//! A CoordinateSequence is a model of a NumericSequence type with
//! additional information about the metrics of the numeric type.
template <typename CoordinateSequence>
struct CoordinateSequenceConcept
{
    void constraints() const
    {
        BOOST_CONCEPT_ASSERT((NumericSequenceConcept< CoordinateSequence >));
		typedef typename geometric_traits<typename geometrix::remove_const_ref<CoordinateSequence>::type>::reference_frame reference_frame;
    }
};

//! \brief helper types to figure out the frame type between reference_frame_adaptor types and regular sequences.
template <typename T, typename EnableIf=void>
struct reference_frame_of
{
	typedef typename geometric_traits<typename geometrix::remove_const_ref<T>::type>::reference_frame type;
};

}//namespace geometrix;

#endif //GEOMETRIX_COORDINATE_SEQUENCE_TRAITS_HPP
