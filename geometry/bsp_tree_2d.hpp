//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_BSPTREE2D_HPP
#define GENERATIVE_GEOMETRY_BSPTREE2D_HPP
#pragma once

#include "utilities.hpp"
#include "line_intersection.hpp"

#include <boost/foreach.hpp>
#include <vector>
#include <set>

namespace generative
{
namespace numeric
{
namespace geometry
{
    enum point_location_classification
    {
        e_inside = -1,
        e_boundary = 0,
        e_outside = 1
    };

    template <typename Segment, typename NumberComparisonPolicy>
    class bsp_tree_2d
    {
    public:

        bsp_tree_2d(){}

        template <typename InputSegmentIterator>
        bsp_tree_2d( InputSegmentIterator first, InputSegmentIterator last );

        template <typename InputSegmentIterator>
        bsp_tree_2d( InputSegmentIterator first, InputSegmentIterator last, const NumberComparisonPolicy& compare );

        template <typename Polygon>
        bsp_tree_2d( const Polygon& p, const NumberComparisonPolicy& compare );

        //! Destructor
        virtual ~bsp_tree_2d(){}

        //! Method to detect if a point is inside, on the boundary our outside the shape represented by the partition.
        template <typename Point>
        point_location_classification               locate_point( const Point& point ) const;

        //! Method to to a 'painters algorithm' traversal of the BSP for a specified point.
        template <typename Point, typename Visitor>
        void                                        painters_traversal( const Point& point, Visitor& visitor ) const;

        //! Method to get a partition of a line segment
        void                                        get_partition( const Segment& edge, 
                                                                   std::vector< Segment >& positive,
                                                                   std::vector< Segment >& negative,
                                                                   std::vector< Segment >& coincidentSame,
                                                                   std::vector< Segment >& coincidentDiff ) const;

        //! Method to return a negated (edges reversed) copy of the bsp tree.
        boost::shared_ptr< bsp_tree_2d< Segment, NumberComparisonPolicy > > negation() const;

    private:

        enum classification
        {
            e_crosses,
            e_positive,
            e_negative,
            e_coincident
        };

        ///Method to classify the characterization of the splitting line and an edge (left,right or split).
        classification  classify( const Segment& splittingLine, const Segment& edge, Segment& subPos, Segment& subNeg ) const;

        //! Method to get a positive partition of a line segment
        void get_positive_partition( const Segment& edge,
                                     std::vector< Segment >& positive,
                                     std::vector< Segment >& negative,
                                     std::vector< Segment >& coincidentSame,
                                     std::vector< Segment >& coincidentDifferent ) const;

        //! Method to get a negative partition of a line segment
        void get_negative_partition( const Segment& edge,
                                     std::vector< Segment >& positive,
                                     std::vector< Segment >& negative,
                                     std::vector< Segment >& coincidentSame,
                                     std::vector< Segment >& coincidentDifferent ) const;

        boost::shared_ptr< bsp_tree_2d< Segment, NumberComparisonPolicy > >   m_positiveChild;
        boost::shared_ptr< bsp_tree_2d< Segment, NumberComparisonPolicy > >   m_negativeChild;
        Segment                                                               m_splittingSegment;
        std::vector<Segment>                                                  m_coincidentEdges;
        NumberComparisonPolicy                                                m_compare;

    };

    template <typename Segment, typename NumberComparisonPolicy>
    template <typename InputSegmentIterator>
    bsp_tree_2d< Segment, NumberComparisonPolicy >::bsp_tree_2d( InputSegmentIterator first, InputSegmentIterator last )
    {
        m_splittingSegment = *first;

        std::vector< Segment > posList, negList;
        while( first != last )
        {
            const Segment& segment = *first++;

            Segment subNeg, subPos;
            classification type = classify( m_splittingSegment, segment, subPos, subNeg );

            if( type == e_crosses )
            {
                posList.push_back( subPos );
                negList.push_back( subNeg );
            }
            else if( type == e_positive )
            {
                posList.push_back( segment );
            }
            else if( type == e_negative )
            {
                negList.push_back( segment );
            }
            else
            {
                m_coincidentEdges.push_back( segment );
            }
        }

        if( !posList.empty() )
        {
            m_positiveChild.reset( new bsp_tree_2d( posList.begin(), posList.end() ) );
        }

        if( !negList.empty() )
        {
            m_negativeChild.reset( new bsp_tree_2d( negList.begin(), negList.end() ) );
        }
    }

    template <typename Segment, typename NumberComparisonPolicy>
    template <typename InputSegmentIterator>
    bsp_tree_2d< Segment, NumberComparisonPolicy >::bsp_tree_2d( InputSegmentIterator first, InputSegmentIterator last, const NumberComparisonPolicy& compare )
        : m_compare( compare )
    {
        m_splittingSegment = *first;

        std::vector< Segment > posList, negList;
        while( first != last )
        {
            const Segment& segment = *first++;

            Segment subNeg, subPos;
            classification type = classify( m_splittingSegment, segment, subPos, subNeg );

            if( type == e_crosses )
            {
                posList.push_back( subPos );
                negList.push_back( subNeg );
            }
            else if( type == e_positive )
            {
                posList.push_back( segment );
            }
            else if( type == e_negative )
            {
                negList.push_back( segment );
            }
            else
            {
                m_coincidentEdges.push_back( segment );
            }
        }

        if( !posList.empty() )
        {
            m_positiveChild.reset( new bsp_tree_2d( posList.begin(), posList.end() ) );
        }

        if( !negList.empty() )
        {
            m_negativeChild.reset( new bsp_tree_2d( negList.begin(), negList.end() ) );
        }
    }

    template <typename Segment, typename NumberComparisonPolicy>
    template <typename Polygon>
    bsp_tree_2d< Segment, NumberComparisonPolicy >::bsp_tree_2d( const Polygon& p, const NumberComparisonPolicy& compare )
        : m_compare( compare )
    {
        std::vector< Segment > posList, negList;

        point_sequence_traits< Polygon >::const_iterator pNext = point_sequence_traits< Polygon >::begin( p ); 
        point_sequence_traits< Polygon >::const_iterator pIt = pNext++;
        point_sequence_traits< Polygon >::const_iterator pEnd = point_sequence_traits< Polygon >::end( p );        
        m_splittingSegment = segment_access_traits< Segment >::construct( *pIt, *pNext );
        while( pNext != pEnd )
        {
            Segment segment = segment_access_traits< Segment >::construct( *pIt++, *pNext++ );
            
            Segment subNeg, subPos;
            classification type = classify( m_splittingSegment, segment, subPos, subNeg );

            if( type == e_crosses )
            {
                posList.push_back( subPos );
                negList.push_back( subNeg );
            }
            else if( type == e_positive )
            {
                posList.push_back( segment );
            }
            else if( type == e_negative )
            {
                negList.push_back( segment );
            }
            else
            {
                m_coincidentEdges.push_back( segment );
            }
        }

        if( !posList.empty() )
        {
            m_positiveChild.reset( new bsp_tree_2d( posList.begin(), posList.end() ) );
        }

        if( !negList.empty() )
        {
            m_negativeChild.reset( new bsp_tree_2d( negList.begin(), negList.end() ) );
        }
    }
    
    template <typename Segment, typename NumberComparisonPolicy>
    typename bsp_tree_2d< Segment, NumberComparisonPolicy >::classification  bsp_tree_2d< Segment, NumberComparisonPolicy >::classify( const Segment& splittingLine, const Segment& edge, Segment& subPos, Segment& subNeg ) const
    {
        typedef Segment                                              segment_type;
        typedef typename segment_traits< segment_type >::point_type  point_type;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        typedef segment_access_traits< segment_type >                segment_access;
        typedef cartesian_access_traits< point_type >                point_access;

        orientation_type orientation_start = get_orientation( segment_access::get_start( splittingLine ), segment_access::get_end( splittingLine ), segment_access::get_start( edge ), m_compare );
        orientation_type orientation_end = get_orientation( segment_access::get_start( splittingLine ), segment_access::get_end( splittingLine ), segment_access::get_end( edge ), m_compare );
        
        if( (orientation_start == oriented_left && orientation_end == oriented_right ) ||
            (orientation_start == oriented_right && orientation_end == oriented_left ) )
        {
            point_type xPoint;
            intersection_type iType = line_intersect( segment_access::get_start( splittingLine ), segment_access::get_end( splittingLine ), edge, xPoint, m_compare );
            if( iType == e_crossing )
            {
                if( orientation_end == oriented_left )
                {
                    if( equals( segment_access::get_start( edge ), xPoint, m_compare ) )
                    {
                        return e_positive;
                    }
                    else if( equals( segment_access::get_end( edge ), xPoint, m_compare ) )
                    {
                        return e_negative;
                    }
                    else
                    {
                        subNeg = construction_traits< Segment >::construct( segment_access::get_start( edge ), xPoint );
                        subPos = construction_traits< Segment >::construct( xPoint, segment_access::get_end( edge ) );
                    }
                }
                else
                {
                    if( equals( segment_access::get_start( edge ), xPoint, m_compare ) )
                    {
                        return e_negative;
                    }
                    else if( equals( segment_access::get_end( edge ), xPoint, m_compare ) )
                    {
                        return e_positive;
                    }
                    else
                    {
                        subPos = construction_traits< Segment >::construct( segment_access::get_start( edge ), xPoint );
                        subNeg = construction_traits< Segment >::construct( xPoint, segment_access::get_end( edge ) );
                    }                    
                }

                return e_crosses;
            }
            else if( iType == e_overlapping )
            {
                return e_coincident;
            }
        }
        
        if( orientation_start == oriented_left || orientation_end == oriented_left )
        {
            return e_positive;
        }
        else if( orientation_start == oriented_right || orientation_end == oriented_right )
        {
            return e_negative;
        }
        else
        {
            return e_coincident;
        }
    }

    template <typename Segment, typename NumberComparisonPolicy>
    template <typename Point>
    point_location_classification bsp_tree_2d< Segment, NumberComparisonPolicy >::locate_point( const Point& point ) const
    {
        typedef Segment                                              segment_type;
        typedef typename segment_traits< segment_type >::point_type  point_type;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        typedef segment_access_traits< segment_type >                segment_access;
        typedef cartesian_access_traits< point_type >                point_access;

        orientation_type orientation_point = get_orientation( segment_access::get_start( splittingLine ), segment_access::get_end( splittingLine ), point, m_compare );
   
        if( orientation_point == oriented_left )
        {
            if( m_positiveChild != 0 )
            {
                return m_positiveChild->locate_point( point );
            }
            else
            {
                return e_outside;
            }
        }
        else if( orientation_point == oriented_right )
        {
            if( m_negativeChild != 0 )
            {
                return m_negativeChild->locate_point( point );
            }
            else
            {
                return e_inside;
            }
        }
        else
        {
            BOOST_FOREACH( const Segment& segment, m_coincidentEdges )
            {
                if( is_between( segment_access::get_start( segment ), segment_access::get_end( segment ), point, true, m_compare ) ) 
                {
                    return e_boundary;
                }
            }

            if( m_positiveChild != 0 )
            {
                return m_positiveChild->locate_point( point );
            }
            else if( m_negativeChild != 0 )
            {
                return m_negativeChild->locate_point( point );
            }
            else
            {
                ///This case should not happen.. but may due to numerical errors. The algorithms suggest noting this in debug mode but returning boundary in 
                ///release (most likely due to round-off on a nearly collinear point.
                BOOST_ASSERT( false );
                return e_boundary;
            }
        }
    }

    ///This concept is used to check if the edge properties conform to a numeric weight
    ///type which can be used in graph calculations.
    template <typename Visitor, typename VisitedType>
    struct IsVisitorTypeConcept
    {
        void constraints()
        {                
            Visitor* visitor = 0;
            VisitedType* visitee = 0;
            (*visitor)( *visitee ); ///needs to be able to call this.
        }
    };

    template <typename Segment, typename NumberComparisonPolicy>
    template <typename Point, typename Visitor>
    void bsp_tree_2d< Segment, NumberComparisonPolicy >::painters_traversal( const Point& point, Visitor& visitor ) const
    {
        boost::function_requires< IsVisitorTypeConcept<Visitor,Segment> >();      ///must conform to the visitor concept.
        typedef Segment                                              segment_type;
        typedef typename segment_traits< segment_type >::point_type  point_type;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        typedef segment_access_traits< segment_type >                segment_access;
        typedef cartesian_access_traits< point_type >                point_access;
        
        if( m_negativeChild == 0 && m_positiveChild == 0 )
        {
            BOOST_FOREACH( const segment_type& segment, m_coincidentEdges )
            {
                visitor( segment );
            }
        }
        else 
        {
            orientation_type orientation_point = get_orientation( segment_access::get_start( splittingLine ), segment_access::get_end( splittingLine ), point, m_compare );
   
            if( orientation_point == oriented_left )
            {             
                if( m_negativeChild != 0 )
                {
                    m_negativeChild->painters_traversal( point, visitor );
                }

                BOOST_FOREACH( const segment_type& segment, m_coincidentEdges )
                {
                    visitor( segment );
                }
                
                if( m_positiveChild != 0 )
                {
                    m_positiveChild->painters_traversal( point, visitor );
                }   
            }
            else if( orientation_point == oriented_right )
            {
                if( m_positiveChild != 0 )
                {
                    m_positiveChild->painters_traversal( point, visitor );
                }

                BOOST_FOREACH( const segment_type& segment, m_coincidentEdges )
                {
                    visitor( segment );
                }

                if( m_negativeChild != 0 )
                {
                    m_negativeChild->painters_traversal( point, visitor );
                }    
            }
            else
            {
                if( m_positiveChild != 0 )
                {
                    m_positiveChild->painters_traversal( point, visitor );
                }
                if( m_negativeChild != 0 )
                {
                    m_negativeChild->painters_traversal( point, visitor );
                }
            }
        }
    }

    template <typename Segment, typename NumberComparisonPolicy>
    boost::shared_ptr< bsp_tree_2d< Segment, NumberComparisonPolicy > > bsp_tree_2d< Segment, NumberComparisonPolicy >::negation() const
    {
        typedef segment_access_traits< Segment > segment_access;

        boost::shared_ptr< bsp_tree2d< Segment > > pNegatedTree( new bsp_tree_2d< Segment >() );
        
        BOOST_FOREACH( const Segment& edge, m_coincidentEdges )
        {
            Segment reversedEdge = segment_access::construct( segment_access::get_end( edge ), segment_access::get_start( edge ) );
            pNegatedTree.m_coincidendEdges.push_back( reversedEdge );
        }

        if( m_positiveChild )
        {
            m_pNegatedTree.m_negativeChild = m_positiveChild->negation();
        }

        if( m_negativeChild )
        {
            m_pNegatedTree.m_positiveChild = m_negativeChild->negation();
        }

        return pNegatedTree;
    }

    //! Method to get a partition of a line segment
    template <typename Segment, typename NumberComparisonPolicy>
    void bsp_tree_2d< Segment, NumberComparisonPolicy >::get_partition( const Segment& edge, std::vector< Segment >& positive, std::vector< Segment >& negative, std::vector< Segment >& coincidentSame, std::vector< Segment >& coincidentDifferent ) const
    {
        typedef Segment                                              segment_type;
        typedef typename segment_traits< segment_type >::point_type  point_type;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;
        typedef segment_access_traits< segment_type >                segment_access;
        typedef cartesian_access_traits< point_type >                point_access;
        
        Segment subNeg, subPos;
        classification type = classify( m_splittingSegment, edge, subPos, subNeg );
        if( type == e_crosses )
        {
            get_positive_partition( subPos, positive, negative, coincidentSame, coincidentDifferent );
            get_negative_partition( subNeg, positive, negative, coincidentSame, coincidentDifferent );
        }
        else if( type == e_positive )
        {
            get_positive_partition( edge, positive, negative, coincidentSame, coincidentDifferent );
        }
        else if( type == e_negative )
        {
            get_negative_partition( edge, positive, negative, coincidentSame, coincidentDifferent );
        }
        else
        {
            typedef std::set< Segment, lexicographical_segment_compare< NumberComparisonPolicy > > UniqueSegmentSet;
            lexicographical_segment_compare< NumberComparisonPolicy > lexSegCompare( m_compare );
            UniqueSegmentSet overlappingSegments( lexSegCompare );
            typedef std::set< Segment, segment_interval_compare< NumberComparisonPolicy > > IntervalSegmentSet;
            segment_interval_compare< NumberComparisonPolicy > intervalSegCompare( m_compare );
            IntervalSegmentSet nonoverlappingSegments( intervalSegCompare );
                
            overlappingSegments.insert( edge );
            const point_type& edgeStart = segment_access::get_start( edge );
            const point_type& edgeEnd  = segment_access::get_end( edge );
            bool noneOverlaps = true;
            BOOST_FOREACH( const Segment& edgeC, m_coincidentEdges )
            {
                std::vector< Segment > overlapping;
                BOOST_FOREACH( const Segment edgeA, overlappingSegments )
                {
                    point_type xPoints[2];
                    intersection_type iType = intersect( edgeC, edgeA, xPoints, m_compare );
                    if( iType == e_overlapping )
                    {
                        noneOverlaps = false;
                        Segment AB = construction_traits< Segment >::construct( xPoints[0], xPoints[1] );
                        overlapping.push_back( AB );                                             
                    }
                }
                overlappingSegments.insert( overlapping.begin(), overlapping.end() );
            }

            if( noneOverlaps )
            {
                //clear edge from the overlappingSegments set.
                overlappingSegments.clear();
            }

            BOOST_FOREACH( const Segment edgeA, overlappingSegments )
            {
               //! Instead of using arctan2 and thus complicating user defined types... I am going to attempt using a comparison of slopes
               //! Compare the delta x and y to see sign difference.
               coordinate_type splitDeltaY = point_access::get<1>( segment_access::get_start( m_splittingSegment ) ) - point_access::get<1>( segment_access::get_end( m_splittingSegment ) );
               coordinate_type edgeADeltaY = point_access::get<1>( segment_access::get_start( edgeA ) ) - point_access::get<1>( segment_access::get_end( edgeA ) );

               coordinate_type splitDeltaX = point_access::get<0>( segment_access::get_start( m_splittingSegment ) ) - point_access::get<0>( segment_access::get_end( m_splittingSegment ) );
               coordinate_type edgeADeltaX = point_access::get<0>( segment_access::get_start( edgeA ) ) - point_access::get<0>( segment_access::get_end( edgeA ) );
                    
               //Check if opposite directions
               if( m_compare.less_than( splitDeltaY * edgeADeltaY, coordinate_type( 0 ) ) || m_compare.less_than( splitDeltaX * edgeADeltaX, coordinate_type( 0 ) ) )
               {
                   coincidentDifferent.push_back( edgeA );
               }
               else
               {
                   coincidentSame.push_back( edgeA );
               }
            }

            //! Construct segments on edge which do not overlap with any in m_coincident
            nonoverlappingSegments.insert( edge );
            BOOST_FOREACH( const Segment& AB, overlappingSegments )
            {
                if( nonoverlappingSegments.empty() )
                {
                    break;
                }
                collinear_segment_difference( nonoverlappingSegments, AB, m_compare );
            }

            BOOST_FOREACH( const Segment s, nonoverlappingSegments )
            {
                get_positive_partition( s, positive, negative, coincidentSame, coincidentDifferent );
                get_negative_partition( s, positive, negative, coincidentSame, coincidentDifferent );
            }
        }
    }

    //! Method to get a partition of a line segment
    template <typename Segment, typename NumberComparisonPolicy>
    void bsp_tree_2d< Segment, NumberComparisonPolicy >::get_positive_partition( const Segment& edge, std::vector< Segment >& positive, std::vector< Segment >& negative, std::vector< Segment >& coincidentSame, std::vector< Segment >& coincidentDifferent ) const
    {
        if( m_positiveChild )
        {
            m_positiveChild->get_partition( edge, positive, negative, coincidentSame, coincidentDifferent );
        }
        else
        {
            positive.push_back( edge );
        }
    }

    //! Method to get a partition of a line segment
    template <typename Segment, typename NumberComparisonPolicy>
    void bsp_tree_2d< Segment, NumberComparisonPolicy >::get_negative_partition( const Segment& edge, std::vector< Segment >& positive, std::vector< Segment >& negative, std::vector< Segment >& coincidentSame, std::vector< Segment >& coincidentDifferent ) const
    {
        if( m_negativeChild )
        {
            m_negativeChild->get_partition( edge, positive, negative, coincidentSame, coincidentDifferent );
        }
        else
        {
            negative.push_back( edge );
        }
    }   

}}}//namespace generative::numeric::geometry;

#endif //GENERATIVE_GEOMETRY_BSPTREE2D_HPP
