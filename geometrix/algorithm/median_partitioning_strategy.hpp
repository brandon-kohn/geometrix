//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MEDIAN_PARTITIONING_STRATEGY_HPP
#define GEOMETRIX_MEDIAN_PARTITIONING_STRATEGY_HPP
#pragma once

#include <geometrix/primitive/point_sequence_utilities.hpp>
#include <geometrix/utility/utilities.hpp>
#include <algorithm>

namespace geometrix {
    //! \class median_partitioning_strategy
    //! \brief A class to partition a point sequence by sorting the sequence
    //! into points which lay left/collinear or right of a lined formed at
    //! the specified dimension.
    struct median_partitioning_strategy
    {
        template <std::size_t Dimension, typename PointSequence, typename NumberComparisonPolicy>
        std::size_t partition( PointSequence& sequence, const NumberComparisonPolicy& compare ) const
        {
            //! Sort on the dimension.
            std::size_t medianIndex = point_sequence_traits<PointSequence>::size( sequence )/2;
            point_sequence_traits< PointSequence >::iterator begin = point_sequence_traits<PointSequence>::begin( sequence );
            point_sequence_traits< PointSequence >::iterator median = point_sequence_traits<PointSequence>::begin( sequence );
            point_sequence_traits< PointSequence >::iterator end = point_sequence_traits<PointSequence>::end( sequence );
            std::advance( median, medianIndex );
            dimension_compare<Dimension, NumberComparisonPolicy> dCompare( compare );
            
            std::nth_element( begin, median, end, dCompare );

            //!To handle cases where the value at Dimension is the same at multiple points, iterate up until a value is found which is distinct from the last.
            //!This ensures that partitioning occurs at a boundary rather than splitting a sequence of equivalent coordinates.
            point_sequence_traits< PointSequence >::iterator next = median;
            ++next;
            while( next != end && !dCompare( *median, *next ) )
            {
                ++median;
                ++next;
                ++medianIndex;
            }
            
            return medianIndex;
        }
    };

}//namespace geometrix;

#endif //GEOMETRIX_MEDIAN_PARTITIONING_STRATEGY_HPP
