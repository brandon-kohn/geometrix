//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_BOUNDING_BOX_INTERSECTION_HPP
#define GENERATIVE_GEOMETRY_BOUNDING_BOX_INTERSECTION_HPP
#pragma once

#include <geometry\indexed_sequence_traversal.hpp>
#include <boost\algorithm\minmax.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! Bounding box intersection test between two boxes (defined by segment diagonals p1->p2 and p3->p4.)
    template <typename NumberComparisonPolicy, typename Point>
    bool bounding_box_intersection( const Point& p1, const Point& p2, const Point& p3, const Point& p4, const NumberComparisonPolicy& compare ) 
    {
        typedef typename point_traits<Point>::coordinate_type coordinate_type;
        typedef indexed_access_traits< Point >                access_traits;

        boost::function_requires< Point2DConcept< Point > >();

//         coordinate_type x1 = access_traits::get<0>( p1 );
//         coordinate_type y1 = access_traits::get<1>( p1 );
//         coordinate_type x2 = access_traits::get<0>( p2 );
//         coordinate_type y2 = access_traits::get<1>( p2 );
//         
//         coordinate_type x3 = access_traits::get<0>( p3 );
//         coordinate_type y3 = access_traits::get<1>( p3 );
//         coordinate_type x4 = access_traits::get<0>( p4 );
//         coordinate_type y4 = access_traits::get<1>( p4 );
// 
//         coordinate_type xmin1 = (std::min)( x1, x2 );
//         coordinate_type ymin1 = (std::min)( y1, y2 );
//         coordinate_type xmax1 = (std::max)( x1, x2 );
//         coordinate_type ymax1 = (std::max)( y1, y2 );
//         
//         coordinate_type xmin2 = (std::min)( x3, x4 );
//         coordinate_type ymin2 = (std::min)( y3, y4 );
//         coordinate_type xmax2 = (std::max)( x3, x4 );
//         coordinate_type ymax2 = (std::max)( y3, y4 );

        coordinate_type xmax1, xmax2, ymax1, ymax2, xmin1, xmin2, ymin1, ymin2;
        boost::tie( xmin1, xmax1 ) = boost::minmax( access_traits::get<0>( p1 ), access_traits::get<0>( p2 ) );
        boost::tie( ymin1, ymax1 ) = boost::minmax( access_traits::get<1>( p1 ), access_traits::get<1>( p2 ) );
        boost::tie( xmin2, xmax2 ) = boost::minmax( access_traits::get<0>( p3 ), access_traits::get<0>( p4 ) );
        boost::tie( ymin2, ymax2 ) = boost::minmax( access_traits::get<1>( p3 ), access_traits::get<1>( p4 ) );

        return ( compare.greater_than_or_equal( xmax1, xmin2 ) &&
                 compare.greater_than_or_equal( xmax2, xmin1 ) &&
                 compare.greater_than_or_equal( ymax1, ymin2 ) &&
                 compare.greater_than_or_equal( ymax2, ymin1 ) );
    }

    //! Bounding box test between two segments.
    template <typename NumberComparisonPolicy, typename Segment>
    bool bounding_box_intersection( const Segment& s1, const Segment& s2, const NumberComparisonPolicy& compare ) 
    {        
        typedef segment_access_traits< Segment > segment_access;
        return bounding_box_intersection( segment_access::get_start( s1 ), segment_access::get_end( s1 ), segment_access::get_start( s2 ), segment_access::get_end( s2 ), compare );
    }

    //! \brief a class to define a bounding_range in N dimensional space. 
    //! Constructs from two points in space which define the min bound and max bound of a(in 2D the lower left and upper right corner of a square are these.)
    namespace detail
    {       
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

            template <typename NumericType>
            void operator() ( const NumericType& n ) const
            {
                if( m_compare.less_than( n, m_low[m_it] ) )
                    m_low[m_it] = n;
                if( m_compare.greater_than( n, m_high[m_it] ) )
                    m_high[m_it] = n;

                m_it = (m_it + 1) % Dimension;
            }

            NumberComparisonPolicy m_compare;
            mutable size_t         m_it;
            mutable numeric_array& m_low;
            mutable numeric_array& m_high;

        };
    }
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
            typedef typename point_traits< point_type >::coordinate_type            coordinate_type;
            typedef typename point_sequence_traits< PointSequence >::const_iterator iterator;
            
            iterator pIt = point_sequence_traits< PointSequence >::begin( pointSequence ); 
            iterator pEnd = point_sequence_traits< PointSequence >::end( pointSequence );   
            boost::array< coordinate_type, numeric_sequence_traits< point_type >::dimension_type::value > low;
            indexed_sequence_traversal::for_each( *pIt, assign_to_array< boost::array< coordinate_type, numeric_sequence_traits< point_type >::dimension_type::value > >( low ) );
            boost::array< coordinate_type, numeric_sequence_traits< point_type >::dimension_type::value > high = low;
            
            //! check bounds against each point in the sequence updating the limits on each dimension.
            while( pIt != pEnd )
            {
                const point_type& p = *pIt++;
                indexed_sequence_traversal::for_each( p,
                    detail::numeric_sequence_bounds< coordinate_type,
                                                     numeric_sequence_traits< point_type >::dimension_type::value,
                                                     NumberComparisonPolicy >( low, high, compare ) );
            }

            m_low = construction_traits< sequence_type >::construct( low );
            m_high = construction_traits< sequence_type >::construct( high );
        }

        //! Check if a numeric_sequence type T intersects this range.
        template <typename T, typename NumberComparisonPolicy>
        bool intersects( const T& t, const NumberComparisonPolicy& compare, typename boost::enable_if< is_numeric_sequence<T> >::type* = 0 ) const
        {
            return comparer<T>::compare( t, m_low, m_high, compare );
        }

        //! Check if a coordinate at a particular dimension intersects.
        template <unsigned int D, typename NumericType, typename NumberComparisonPolicy>
        bool intersects( const NumericType& t, const NumberComparisonPolicy& compare, typename boost::enable_if< is_numeric<NumericType> >::type* = 0 ) const
        {
            coordinate_type& lowD = indexed_access_traits< sequence_type >::get<D>( m_low );
            coordinate_type& highD = indexed_access_traits< sequence_type >::get<D>( m_high );            
            bool lessThanHigh = compare.less_than( highD, t );
            bool greaterThanLow = compare.greater_than( lowD, t );
            return ( lessThanHigh && greaterThanLow );
        }

        //! Check if another range intersects this range.
        template <typename N, typename NumberComparisonPolicy>
        bool intersects( const orthogonal_range<N>& range, const NumberComparisonPolicy& compare ) const
        {
            bool upperIntersects = comparer<N>::compare( range.get_upper_bound(), m_low, m_high, compare );
            if( upperIntersects )
                return true;
            else 
                return comparer<N>::compare( range.get_lower_bound(), m_low, m_high, compare );
        }

        //! Check if another range is contained by this range.
        template <typename N, typename NumberComparisonPolicy>
        bool contains( const orthogonal_range<N>& range, const NumberComparisonPolicy& compare ) const
        {
            bool upperIntersects = comparer<N>::compare( range.get_upper_bound(), m_low, m_high, compare );
            bool lowerIntersects = comparer<N>::compare( range.get_lower_bound(), m_low, m_high, compare );
            return ( upperIntersects && lowerIntersects );
        }

        const sequence_type& get_lower_bound() const { return m_low; }
        const sequence_type& get_upper_bound() const { return m_high; }

    private:

        template <typename NumericSequence, typename Enable = void>
        struct comparer
        {};

        template <typename NumericSequence>
        struct comparer<NumericSequence, typename boost::enable_if< typename should_use_compile_time_access1< NumericSequence >::type >::type >
        {
            template <typename NumericSequence, unsigned int D>
            struct dimension_processor
            {
                template <typename NumberComparisonPolicy>
                static bool compare( const NumericSequence& n, const NumericSequence& low, const NumericSequence& high, const NumberComparisonPolicy& nCompare )
                {   
                    typedef indexed_access_traits< NumericSequence > access;                    
                    if( nCompare.less_than( access::get<D>( n ), access::get<D>( low ) ) ||
                        nCompare.greater_than( access::get<D>( n ), access::get<D>( high ) ) )                    
                        return false;
                    return dimension_processor<NumericSequence, D-1>::compare( n, low, high, nCompare );
                }
            };

            template <typename NumericSequence>
            struct dimension_processor<NumericSequence, 0>
            {
                template <typename NumberComparisonPolicy>
                static bool compare( const NumericSequence& n, const NumericSequence& low, const NumericSequence& high, const NumberComparisonPolicy& nCompare )
                {   
                    typedef indexed_access_traits< NumericSequence > access;                    
                    if( nCompare.less_than( access::get<0>( n ), access::get<0>( low ) ) ||
                        nCompare.greater_than( access::get<0>( n ), access::get<0>( high ) ) )                    
                        return false;
                    else
                        return true;
                }
            };

            template <typename NumberComparisonPolicy>
            static bool compare( const NumericSequence& n, const NumericSequence& low, const NumericSequence& high, const NumberComparisonPolicy& nCompare )
            {
                return dimension_processor<NumericSequence, numeric_sequence_traits<NumericSequence>::dimension_type::value - 1>::compare( n, low, high, nCompare );
            }
        };

        template <typename NumericSequence>
        struct comparer<NumericSequence, typename boost::enable_if< typename should_use_run_time_access1< NumericSequence >::type >::type >
        {
            template <typename NumberComparisonPolicy>
            static bool compare( const NumericSequence& lhs, const NumericSequence& rhs, const NumberComparisonPolicy& nCompare )
            {
                typedef indexed_access_traits< NumericSequence > access;
                for( size_t i=0;i < access::dimension_type::value; ++i )
                {
                    if( nCompare.less_than( access::get( n, i ), access::get( low, i ) ) ||
                    nCompare.greater_than( access::get( n, i ), access::get( high, i ) ) )                    
                    return false;
                }

                return true;//all in range.
            }
        };

        NumericSequence m_low;
        NumericSequence m_high;

    };

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_BOUNDING_BOX_INTERSECTION_HPP
