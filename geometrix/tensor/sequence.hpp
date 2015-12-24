//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SEQUENCE_HPP
#define GEOMETRIX_SEQUENCE_HPP

#include <geometrix/geometric_traits.hpp>
#include <geometrix/space/dimension.hpp>
#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>

#include <boost/preprocessor.hpp>

namespace geometrix {

//! \brief Tag to check if a type is a sequence
template <typename Sequence, typename EnableIf=void>
struct is_sequence : boost::false_type{};

template <typename Sequence>
struct is_sequence<Sequence, typename geometric_traits<Sequence>::is_sequence> : boost::true_type{};

//! \brief A SequenceConcept concept check for a type which models a Sequence.

//! A Sequence is defined as an ordered sequence of values with a static tensor_order.
//! The SequenceConcept checks that valid geometric_traits have been defined for a Sequence type.
template <typename Sequence>
struct SequenceConcept
{
    void constraints() const
    {
        BOOST_STATIC_ASSERT((is_sequence<Sequence>::value));
        typedef typename geometric_traits< Sequence >::dimension_type dimension_type;
		ignore_unused_warning_of<dimension_type>();
    }
};

//! Index type for sequences.
template <std::size_t Index>
struct sequence_index : boost::mpl::int_<Index> {};

}//namespace geometrix;

#endif //GEOMETRIX_SEQUENCE_HPP
