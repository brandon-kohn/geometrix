//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_DISTANCE_COMPARE_DISTANCE_HPP
#define GEOMETRIX_DISTANCE_COMPARE_DISTANCE_HPP

#include <geometrix/algorithm/distance/point_point_distance.hpp>

namespace geometrix {

    //! \struct distance_compare
    //! \brief A predicate to compare two numeric sequences by their distance from a fixed origin.    
    template <typename CoordinateSequence, typename NumberComparisonPolicy>
    struct distance_compare
    {
        distance_compare(const CoordinateSequence& p, const NumberComparisonPolicy& compare)
            : m_compare(compare)
            , m_p(p)
        {}

        template <typename CoordinateSequence1, typename CoordinateSequence2>
        bool operator()(const CoordinateSequence1& lhs, const CoordinateSequence2& rhs) const
        {
            typename result_of::point_point_distance_sqrd
                <
                CoordinateSequence
                , CoordinateSequence1
                >::type distanceLHS = point_point_distance_sqrd(m_p, lhs);
            typename result_of::point_point_distance_sqrd
                <
                CoordinateSequence
                , CoordinateSequence2
                >::type distanceRHS = point_point_distance_sqrd(m_p, rhs);
            return m_compare.less_than(distanceLHS, distanceRHS);
        }

        NumberComparisonPolicy m_compare;
        CoordinateSequence     m_p;
    };
}//namespace geometrix;

#endif //GEOMETRIX_DISTANCE_COMPARE_DISTANCE_HPP
