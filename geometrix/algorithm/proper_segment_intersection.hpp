//
//! Copyright © 2026
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0.
//
#ifndef GEOMETRIX_PROPER_SEGMENT_INTERSECTION_HPP
#define GEOMETRIX_PROPER_SEGMENT_INTERSECTION_HPP

#include <geometrix/algorithm/intersection/segment_segment_intersection.hpp>
#include <geometrix/primitive/segment_traits.hpp>

namespace geometrix {

//! Return true only when two 2D segments cross in their interiors.
//!
//! Endpoint contact and collinear overlap are deliberately excluded. This is
//! useful for visibility tests where a sight segment may terminate exactly at
//! a navigation/boundary vertex without that vertex occluding itself.
//!
//! This predicate uses the classification-only segment intersection path. It
//! does not compute an intersection point and therefore avoids the divisions
//! and construction required by the full segment_segment_intersection API.
template <typename Segment1, typename Segment2, typename NumberComparisonPolicy>
inline bool proper_segment_intersection(
    const Segment1& segment1,
    const Segment2& segment2,
    const NumberComparisonPolicy& cmp)
{
    BOOST_CONCEPT_ASSERT((SegmentConcept<Segment1>));
    BOOST_CONCEPT_ASSERT((SegmentConcept<Segment2>));
    BOOST_CONCEPT_ASSERT((NumberComparisonPolicyConcept<NumberComparisonPolicy>));

    return classify_segment_segment_intersection(
        get_start(segment1),
        get_end(segment1),
        get_start(segment2),
        get_end(segment2),
        cmp) == e_crossing;
}

}//! namespace geometrix

#endif//! GEOMETRIX_PROPER_SEGMENT_INTERSECTION_HPP
