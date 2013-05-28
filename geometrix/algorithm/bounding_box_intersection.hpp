//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BOUNDING_BOX_INTERSECTION_HPP
#define GEOMETRIX_BOUNDING_BOX_INTERSECTION_HPP
#pragma once

#include <geometrix/utility/array.hpp>
#include <geometrix/primitive/point_sequence_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <boost/algorithm/minmax.hpp>

namespace geometrix {

    //! Bounding box intersection test between two boxes (defined by segment diagonals p1->p2 and p3->p4.)
    template <typename NumberComparisonPolicy, typename Point>
    bool bounding_box_intersection( const Point& p1, const Point& p2, const Point& p3, const Point& p4, const NumberComparisonPolicy& compare ) 
    {
        typedef typename geometric_traits<Point>::coordinate_type coordinate_type;
        BOOST_CONCEPT_ASSERT((Point2DConcept<Point>));

        coordinate_type xmax1, xmax2, ymax1, ymax2, xmin1, xmin2, ymin1, ymin2;
        boost::tie( xmin1, xmax1 ) = boost::minmax( get<0>( p1 ), get<0>( p2 ) );
        boost::tie( ymin1, ymax1 ) = boost::minmax( get<1>( p1 ), get<1>( p2 ) );
        boost::tie( xmin2, xmax2 ) = boost::minmax( get<0>( p3 ), get<0>( p4 ) );
        boost::tie( ymin2, ymax2 ) = boost::minmax( get<1>( p3 ), get<1>( p4 ) );

        return ( compare.greater_than_or_equal( xmax1, xmin2 ) &&
                 compare.greater_than_or_equal( xmax2, xmin1 ) &&
                 compare.greater_than_or_equal( ymax1, ymin2 ) &&
                 compare.greater_than_or_equal( ymax2, ymin1 ) );
    }

    //! Bounding box test between two segments.
    template <typename NumberComparisonPolicy, typename Segment>
    bool bounding_box_intersection( const Segment& s1, const Segment& s2, const NumberComparisonPolicy& compare ) 
    {        
        return bounding_box_intersection( get_start( s1 ), get_end( s1 ), get_start( s2 ), get_end( s2 ), compare );
    }

    //! \brief a class to define a bounding_range in N dimensional space. 
    //! Constructs from two points in space which define the min bound and max bound of a(in 2D the lower left and upper right corner of a square are these.)
    namespace bounding_box { namespace detail {

        template <typename NumericType, unsigned int Dimension, typename NumberComparisonPolicy>
        struct numeric_sequence_bounds
        {
            typedef boost::array< NumericType, Dimension > numeric_array;

            numeric_sequence_bounds( numeric_array& low, 
                                     numeric_array& high, 
                                     const NumberComparisonPolicy& compare )
                : m_low( low )
                , m_high( high )
                , m_compare( compare )
                , m_it(0)
            {}

            template <typename T>
            void operator() ( const T& n ) const
            {
                if( m_compare.less_than( n, m_low[m_it] ) )
                    m_low[m_it] = n;
                if( m_compare.greater_than( n, m_high[m_it] ) )
                    m_high[m_it] = n;

                m_it = (m_it + 1) % Dimension;
            }

            NumberComparisonPolicy m_compare;
            mutable std::size_t    m_it;
            mutable numeric_array& m_low;
            mutable numeric_array& m_high;

        };

        template <typename Sequence, unsigned int D>
        struct dimension_processor
        {
            template <typename NumberComparisonPolicy>
            static bool compare( const Sequence& n, const Sequence& low, const Sequence& high, const NumberComparisonPolicy& nCompare )
            {   
                if( nCompare.less_than( get<D>( n ), get<D>( low ) ) ||
                    nCompare.greater_than( get<D>( n ), get<D>( high ) ) )                    
                    return false;
                return dimension_processor<Sequence, D-1>::compare( n, low, high, nCompare );
            }
        };
        
        template <typename Sequence>
        struct dimension_processor<Sequence, 0>
        {
            template <typename NumberComparisonPolicy>
            static bool compare( const Sequence& n, const Sequence& low, const Sequence& high, const NumberComparisonPolicy& nCompare )
            {   
                if( nCompare.less_than( get<0>( n ), get<0>( low ) ) ||
                    nCompare.greater_than( get<0>( n ), get<0>( high ) ) )                    
                    return false;
                else
                    return true;
            }
        };
        
        template <typename Sequence, typename NumberComparisonPolicy>
        inline bool compare( const Sequence& n, const Sequence& low, const Sequence& high, const NumberComparisonPolicy& nCompare )
        {
            return dimension_processor<Sequence, geometric_traits<Sequence>::dimension_type::value - 1>::compare( n, low, high, nCompare );
        }        
    }}//namespace bounding_box::detail;

    template <typename NumericSequence>
    class orthogonal_range
    {
    public:

        typedef NumericSequence sequence_type;
        
        orthogonal_range( const sequence_type& low,
                          const sequence_type& high )
            : m_low( low )
            , m_high( high )
        {}

        //! Construct a range from a point sequence by finding the min/max values on each dimension.
        template <typename PointSequence, typename NumberComparisonPolicy>
        orthogonal_range( const PointSequence& pointSequence, const NumberComparisonPolicy& compare )
        {
            typedef typename point_sequence_traits< PointSequence >::point_type     point_type;
            typedef typename geometric_traits< point_type >::arithmetic_type        coordinate_type;
            typedef typename point_sequence_traits< PointSequence >::const_iterator iterator;
            
            iterator pIt = point_sequence_traits< PointSequence >::begin( pointSequence ); 
            iterator pEnd = point_sequence_traits< PointSequence >::end( pointSequence );   
            typedef boost::array< coordinate_type, dimension_of< point_type >::type::value > array_type;
            array_type low = construct<array_type>(*pIt);
            array_type high = low;
            
            //! check bounds against each point in the sequence updating the limits on each dimension.
            while( pIt != pEnd )
            {
                const point_type& p = *pIt++;
                boost::fusion::for_each( p,
                    bounding_box::detail::numeric_sequence_bounds
                    <
                        coordinate_type
                      , dimension_of< point_type >::type::value
                      , NumberComparisonPolicy 
                    >( low, high, compare ) );
            }

            m_low = construct<sequence_type>( low );
            m_high = construct<sequence_type>( high );
        }

        //! Check if a numeric_sequence type T intersects this range.
        template <typename T, typename NumberComparisonPolicy>
        bool intersects( const T& t, const NumberComparisonPolicy& compare, typename boost::enable_if< is_numeric_sequence<T> >::type* = 0 ) const
        {
            return bounding_box::detail::compare( t, m_low, m_high, compare );
        }

        //! Check if a coordinate at a particular dimension intersects.
        template <unsigned int D, typename NumericType, typename NumberComparisonPolicy>
        bool intersects( const NumericType& t, const NumberComparisonPolicy& compare, typename boost::enable_if< is_numeric<NumericType> >::type* = 0 ) const
        {
            typedef typename geometric_traits<NumericSequence>::coordinate_type coordinate_type;
            coordinate_type& lowD = get<D>( m_low );
            coordinate_type& highD = get<D>( m_high );            
            bool lessThanHigh = compare.less_than( highD, t );
            bool greaterThanLow = compare.greater_than( lowD, t );
            return ( lessThanHigh && greaterThanLow );
        }

        //! Check if another range intersects this range.
        template <typename Sequence, typename NumberComparisonPolicy>
        bool intersects( const orthogonal_range<Sequence>& range, const NumberComparisonPolicy& compare ) const
        {
            bool upperIntersects = bounding_box::detail::compare( range.get_upper_bound(), m_low, m_high, compare );
            if( upperIntersects )
                return true;
            else 
                return bounding_box::detail::compare( range.get_lower_bound(), m_low, m_high, compare );
        }

        //! Check if another range is contained by this range.
        template <typename Sequence, typename NumberComparisonPolicy>
        bool contains( const orthogonal_range<Sequence>& range, const NumberComparisonPolicy& compare ) const
        {
            bool upperIntersects = bounding_box::detail::compare( range.get_upper_bound(), m_low, m_high, compare );
            bool lowerIntersects = bounding_box::detail::compare( range.get_lower_bound(), m_low, m_high, compare );
            return ( upperIntersects && lowerIntersects );
        }

        const sequence_type& get_lower_bound() const { return m_low; }
        const sequence_type& get_upper_bound() const { return m_high; }

    private:
        
        NumericSequence m_low;
        NumericSequence m_high;

    };

}//namespace geometrix;

#endif //GEOMETRIX_BOUNDING_BOX_INTERSECTION_HPP
