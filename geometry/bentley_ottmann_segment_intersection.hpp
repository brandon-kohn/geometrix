//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_BENTLEY_OTTMANN_SEGMENT_INTERSECTION_HPP
#define _BOOST_GEOMETRY_BENTLEY_OTTMANN_SEGMENT_INTERSECTION_HPP
#pragma once

#include "bentley_ottmann_sweep.hpp"
#include "utilities.hpp"
#include "rational_utilities.hpp"
#include <map>

namespace boost
{
namespace numeric
{
namespace geometry
{
 
    template <typename Point, typename Segment, typename NumberComparisonPolicy>
    class sweepline_ordinate_compare
    {
    public:

        typedef Point                                                point_type;
        typedef Segment                                              segment_type;
        typedef cartesian_access_traits< point_type >                point_access;
        typedef segment_access_traits< segment_type >                segment_access;
        typedef typename point_traits< point_type >::coordinate_type coordinate_type;

        sweepline_ordinate_compare()
            : m_point( new point_type() )
        {}

        sweepline_ordinate_compare( const NumberComparisonPolicy& compare )
            : m_point( new point_type() ),
            m_compare( compare )
        {}

        template <typename SegmentIterator>
        inline bool operator()( SegmentIterator s1, SegmentIterator s2 ) const  
        {
            if( s1 != s2 )
            {
                const point_type& currentPoint = get_current_event();

                const point_type& s1_start = segment_access::get_start( *s1 );
                const point_type& s1_end= segment_access::get_end( *s1 );

                const point_type& s2_start = segment_access::get_start( *s2 );
                const point_type& s2_end= segment_access::get_end( *s2 );

                if( numeric_sequence_equals( s1_start, s2_start, m_compare ) && numeric_sequence_equals( s1_end, s2_end, m_compare ) )
                {
                    return false;
                }

                bool s1Vertical = is_vertical( s1_start, s1_end, m_compare );	
                bool s2Vertical = is_vertical( s2_start, s2_end, m_compare );

                if( s1Vertical && s2Vertical )
                {
                    return s1 < s2;
                }
                else if ( s1Vertical && is_between( s2_start, s2_end, currentPoint, true, m_compare ) )
                {
                    return false;
                }
                else if ( s2Vertical && is_between( s1_start, s1_end, currentPoint, true, m_compare ) )
                {
                    return true;
                }
                else
                {
                    return segment_compare<coordinate_type>::compare( s1Vertical, s2Vertical, s1_start, s1_end, s2_start, s2_end, s1, s2, *m_point, m_compare );
                }
            }
            else
            {
                return false;
            }
        }

        inline const point_type& get_current_event() const { return *m_point; }
        inline void              set_current_event( const point_type& p ) { *m_point = p; }

    private:

        template <typename NumericType, class Enable = void>
        struct segment_compare
        {

        };

        template <typename NumericType>
        struct segment_compare< NumericType, typename boost::enable_if< typename numeric_traits< NumericType >::is_integral >::type >
        {
            typedef NumericType coordinate_type;
            
            template <typename SegmentIterator, typename NumberComparisonPolicy>
            inline static bool compare( bool s1IsVertical, bool s2IsVertical, const point_type& s1_start, const point_type& s1_end, const point_type& s2_start, const point_type& s2_end, SegmentIterator s1, SegmentIterator s2, const Point& _point, const NumberComparisonPolicy& _compare )
            {            
                typedef rational_promotion_traits< coordinate_type >::rational_type ratonal_type;
                coordinate_type xEvent = cartesian_access_traits< point_type >::get<0>( _point );
                coordinate_type one( 1 );
                coordinate_type zero( 0 );
                rational_type y1(zero,one), slope1(zero,one);
                if( s1IsVertical )
                {
                    coordinate_type one = 1;
                    y1 = boost::rational<coordinate_type>( cartesian_access_traits< point_type >::get<1>( _point ), one );
                }
                else
                {
                    y1 = rational_y_of_x( s1_start, s1_end, xEvent, slope1, _compare );
                }
                
                rational_type y2(zero,one), slope2(zero,one);
                if( s2IsVertical )
                {   
                    coordinate_type one = 1;
                    y2 = rational_type( cartesian_access_traits< point_type >::get<1>( _point ), one );
                }
                else
                {
                    y2 = rational_y_of_x( s2_start, s2_end, xEvent, slope2, _compare );
                }

                if( _compare.less_than( y1, y2 ) )
                {
                    return true;
                }
                else if( _compare.greater_than( y1, y2 ) )
                {
                    return false;
                }

                ///This is the case where the segments intersect in the event point. 
                ///Neither segment is vertical at this point as both segments contain the point
                ///and that case is dealt with in the previous calling function. 
                ///It remains to compare the slopes of the two segments (thus reversing their order in the sweep line).
                ///If the slopes are equal, sort by the iterator ordering in the original segment list.
                if( _compare.equals( y1, y2 ) )
                {
                    //The points are equal.. so compare slopes.
                    assert( !s1IsVertical && !s2IsVertical ); //should be no verticals here as that case is dealt with above.
                    if( _compare.less_than( slope1, slope2 ) )
                    {
                        return true;
                    }
                    else if( _compare.greater_than( slope1, slope2 ) )
                    {
                        return false;
                    }
                }

                //compare iterators as a last ditch effort.
                return s1 < s2;

            }
            
        };

        template <typename NumericType>
        struct segment_compare< NumericType, typename boost::enable_if< typename numeric_traits< NumericType >::is_float >::type >
        {
            typedef NumericType coordinate_type;

            template <typename SegmentIterator, typename NumberComparisonPolicy>
            inline static bool compare( bool s1IsVertical, bool s2IsVertical, const point_type& s1_start, const point_type& s1_end, const point_type& s2_start, const point_type& s2_end, SegmentIterator s1, SegmentIterator s2, const Point& _point, const NumberComparisonPolicy& _compare )
            {            
                coordinate_type xEvent = cartesian_access_traits< point_type >::get<0>( _point );            
                coordinate_type y1, slope1;
                if( s1IsVertical )
                {
                    y1 = cartesian_access_traits< point_type >::get<1>( _point );
                }
                else
                {
                    y1 = y_of_x( s1_start, s1_end, xEvent, slope1, _compare );
                }
                
                coordinate_type y2, slope2;
                if( s2IsVertical )
                {   
                    y2 = cartesian_access_traits< point_type >::get<1>( _point );
                }
                else
                {
                    y2 = y_of_x( s2_start, s2_end, xEvent, slope2, _compare );
                }

                if( _compare.less_than( y1, y2 ) )
                {
                    return true;
                }
                else if( _compare.greater_than( y1, y2 ) )
                {
                    return false;
                }

                ///This is the case where the segments intersect in the event point. 
                ///Neither segment is vertical at this point as both segments contain the point
                ///and that case is dealt with in the previous calling function. 
                ///It remains to compare the slopes of the two segments (thus reversing their order in the sweep line).
                ///If the slopes are equal, sort by the iterator ordering in the original segment list.
                if( _compare.equals( y1, y2 ) )
                {
                    //The points are equal.. so compare slopes.
                    assert( !s1IsVertical && !s2IsVertical ); //should be no verticals here as that case is dealt with above.
                    if( _compare.less_than( slope1, slope2 ) )
                    {
                        return true;
                    }
                    else if( _compare.greater_than( slope1, slope2 ) )
                    {
                        return false;
                    }
                }

                //compare iterators as a last ditch effort.
                return s1 < s2;
            }
        };

        NumberComparisonPolicy          m_compare;
        boost::shared_ptr< point_type > m_point; 

    };

    template< typename Point, typename Segment, typename NumberComparisonPolicy>
    struct process_new_events
    {
        typedef Point point_type;
        typedef Segment segment_type;
        typedef typename point_traits<point_type>::coordinate_type coordinate_type;
        typedef cartesian_access_traits< point_type > point_access;
        typedef segment_access_traits< segment_type > segment_access;

        process_new_events( const NumberComparisonPolicy& compare = NumberComparisonPolicy() )
            : m_compare( compare )
        {}

        template <typename EventQueue, typename SweepLine>
        inline void operator()( EventQueue& eventQueue, SweepLine& sweepLine, typename SweepLine::iterator& s1, typename SweepLine::iterator& s2 )
        {            
            const point_type& point = sweepLine.get_current_event();
            coordinate_type positionX = point_access::get<0>( point );
            coordinate_type positionY = point_access::get<1>( point );
            point_type xPoint[2];
            intersection_type iType = intersect( **s1, **s2, xPoint, m_compare );
            if( iType != e_non_crossing )
            {
                //segments intersect at xPoint
                coordinate_type x = point_access::get<0>( xPoint[0] );
                coordinate_type y = point_access::get<1>( xPoint[0] );
            
                //Add the event if it is to the right of the sweep line.
                if( m_compare.greater_than( x, positionX ) || ( m_compare.equals( x, positionX ) && m_compare.greater_than( y, positionY ) ) )
                {                    
                    eventQueue[xPoint[0]];
                }                
            }
        }

        NumberComparisonPolicy m_compare;

    };

    template <typename NewEventProcessor, typename Visitor, typename NumberComparisonPolicy>
    struct sweep_event_handler
    {
        sweep_event_handler( NewEventProcessor& newEventProcessor, Visitor& visitor_, const NumberComparisonPolicy& floatingPointCompare )
            : process_new_events( newEventProcessor ),
              visitor( visitor_ ),
              compare( floatingPointCompare )
        {}

        template <typename SweepItem, typename Event>
        bool sweep_item_ends_with( const SweepItem& sweepItem, const Event& event )
        {
            typedef typename SweepItem segment_type;
            typedef Event              point_type;

            const point_type& segment_end = segment_access_traits< segment_type >::get_end( sweepItem );
            return numeric_sequence_equals( event, segment_end, compare );
        }

        template <typename SweepItem, typename Event>
        bool sweep_item_starts_with( const SweepItem& sweepItem, const Event& event )
        {
            typedef typename SweepItem segment_type;
            typedef Event              point_type;

            const point_type& segment_end = segment_access_traits< segment_type >::get_start( sweepItem );
            return numeric_sequence_equals( event, segment_end, compare );
        }

        template <typename SweepItem, typename Event>
        bool sweep_item_overlaps( const SweepItem& sweepItem, const Event& event )
        {
            typedef typename SweepItem segment_type;
            typedef Event              point_type;
            
            const point_type& segment_start = segment_access_traits< segment_type >::get_start( sweepItem );
            const point_type& segment_end = segment_access_traits< segment_type >::get_end( sweepItem );

            return is_between( segment_start, segment_end, event, true, compare );
        }

        template <typename SweepLine, typename Event>
        typename SweepLine::iterator lower_bound_for_event( SweepLine& sweepLine, Event& event )
        {
            typedef typename SweepLine::sweep_item_type                      segment_type;
            typedef typename segment_traits< segment_type >::point_type      point_type;
            typedef typename segment_traits< segment_type >::coordinate_type coordinate_type;

            point_type just_right_of_event = construction_traits< point_type >::construct( cartesian_access_traits< point_type >::get<0>( event ) + coordinate_type(1), cartesian_access_traits< point_type >::get<1>( event )  );
            
            segment_type segment = construction_traits< segment_type >::construct( event, just_right_of_event );
            return sweepLine.lower_bound( &segment );
        }

        template <typename EventQueue, typename SweepLine>
        inline void handle_event( EventQueue& eventQueue, SweepLine& sweepLine, typename EventQueue::iterator& event )
        {            
            typedef typename SweepLine::iterator sweep_item_iterator;
            typedef typename SweepLine::sweep_item_type sweep_item_type;
            sweepLine.set_current_event( event->first );
            std::set<sweep_item_type*> L;
            std::set<sweep_item_type*> C;
            std::set<sweep_item_type*>& U = event->second;
            
            //Sweep the scan line and classify sweep_items as ending with this event (L structure), beginning with the current event (U structure) or overlapping the current event (C structure)
            typename sweep_item_iterator sweepIter( sweepLine.begin() );
            typename sweep_item_iterator sEnd( sweepLine.end() );
            while( sweepIter != sEnd )
            {   
                if( sweep_item_ends_with( **sweepIter, event->first ) ) //if the sweep item ends in the event.
                {
                    L.insert( *sweepIter );
                }
                else if( sweep_item_overlaps( **sweepIter, event->first ) && !sweep_item_starts_with( **sweepIter, event->first ) )
                {
                    C.insert( *sweepIter );
                }  

                ++sweepIter;
            }
            
            std::set<sweep_item_type*>::iterator iter;
            std::set<sweep_item_type*>::iterator end;

            std::set<sweep_item_type*> UC;
            std::set_union( U.begin(), U.end(), C.begin(), C.end(), std::inserter( UC, UC.begin() ) ); 
            
            std::set<sweep_item_type*> LUC;
            std::set_union( UC.begin(), UC.end(), L.begin(), L.end(), std::inserter( LUC, LUC.begin() ) );

            end = LUC.end();
            if ( LUC.size() > 1 )
            {
                //Report the sweep_items in the event.
                visitor( event->first, LUC.begin(), LUC.end() );
            }

            //visitor.debug_pre_order( sweepLine.begin(), sweepLine.end(), event->first );

            std::set<sweep_item_type*> LC;
            std::set_union( L.begin(), L.end(), C.begin(), C.end(), std::inserter( LC, LC.begin() ) );
            for ( iter = LC.begin(); iter != LC.end(); ++iter )
            {
                sweepLine.remove( *iter );
            }            
            
            //Insert U and C again.
            for ( iter = UC.begin(); iter != UC.end(); ++iter )
            {
                sweepLine.insert( *iter );
            }

            //visitor.debug_post_order( sweepLine.begin(), sweepLine.end(), event->first );
            
            //If the scan line has less than two segments... return
            if(sweepLine.size() < 2)
            {
                return;
            }
            
            typename sweep_item_iterator s1;
            typename sweep_item_iterator s2;
            typename sweep_item_iterator sp;
            typename sweep_item_iterator spp;
            typename sweep_item_iterator scanEnd(sweepLine.end());
            if ( UC.empty() )
            {
                sweepIter = lower_bound_for_event( sweepLine, event->first );
                if ( sweepIter != scanEnd )
                {
                    s1 = sweepIter;
                    --s1;
                    s2 = sweepIter;
                    if( s1 != scanEnd && s1 != s2 )
                    {
                        process_new_events( eventQueue, sweepLine, s1, s2 );
                    }
                }
            }
            else
            {
                for ( sweepIter = sweepLine.begin(); sweepIter != scanEnd; ++sweepIter )
                {
                    if ( UC.find( *sweepIter ) != UC.end() )
                    {
                        sp = sweepIter;
                        break;
                    }
                }
                s1 = sp;
                --s1;

                if ( s1 != scanEnd && sp != scanEnd && s1 != sp )
                {
                    process_new_events( eventQueue, sweepLine, s1, sp );	
                }
                
                sweepIter = scanEnd;				
                for ( --sweepIter; sweepIter != scanEnd; --sweepIter )
                {
                    if ( UC.find( *sweepIter ) != UC.end() )
                    {
                        spp = sweepIter;
                        break;
                    }
                }
                s2 = spp;
                ++s2;

                if( s2 != scanEnd && spp != scanEnd && s2 != spp)
                {
                    process_new_events( eventQueue, sweepLine, spp, s2 );
                }
                
            }            
        }//handle_event

        NewEventProcessor             process_new_events;
        Visitor                       visitor;
        NumberComparisonPolicy compare;
    };
    
    template <typename Segment, typename Visitor, typename NumberComparisonPolicy>
    inline void bentley_ottmann_segment_intersection( const std::vector< Segment >& segments, Visitor& visitor, const NumberComparisonPolicy& compare )
    {
        typedef Segment                                      segment_type;
        typedef typename segment_traits<Segment>::point_type point_type;
        
        boost::function_requires< SegmentConcept<segment_type> >();
        boost::function_requires< Point2DConcept< point_type > >();

        typedef std::vector< segment_type >::iterator                                          segment_iterator;
        typedef std::set< segment_type* >         						                       segment_ptr_set;
        typedef std::map< point_type, segment_ptr_set, 
                          lexicographical_compare< NumberComparisonPolicy > >                  event_queue;
        typedef sweepline_ordinate_compare< point_type, segment_type, NumberComparisonPolicy > segment_compare;
        typedef sweep_line< point_type, segment_type, segment_compare >                        scan_line;

        //! copy the segments as we must order them lexicographically.
        std::vector< segment_type >  ordered_segs( segments );
        
        //first build the event_queue
        lexicographical_compare< NumberComparisonPolicy > lexi_comp( compare );
        event_queue eventQueue( lexi_comp );
        std::vector< segment_type >::iterator sIter( ordered_segs.begin() );
        std::vector< segment_type >::iterator theEnd( ordered_segs.end() );        
        while(sIter != theEnd)
        {
            segment_type& segment = *sIter;
            if ( !lexi_comp( segment_access_traits< segment_type >::get_start( segment ), segment_access_traits< segment_type >::get_end( segment ) ) )
            {
                *sIter = construction_traits< segment_type >::construct( segment_access_traits< segment_type >::get_end( segment ), segment_access_traits< segment_type >::get_start( segment ) );
            }

            eventQueue[ segment_access_traits< segment_type >::get_start( segment ) ].insert( &*sIter );
            eventQueue[ segment_access_traits< segment_type >::get_end( segment ) ];//simply ensure the latter event exists.. no need to associate the segment.
            ++sIter;
        }

        segment_compare sweepCompare( compare );
        scan_line sweepLine( sweepCompare );

        typedef process_new_events<point_type, segment_type, NumberComparisonPolicy> new_event_processor;
        new_event_processor newEventProcessor( compare );
        sweep_event_handler<new_event_processor, Visitor, NumberComparisonPolicy> eventHandler( newEventProcessor, visitor, compare );

        //run the algorithm.
        bentley_ottmann_sweep( eventQueue, sweepLine, eventHandler );
    }

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_BENTLEY_OTTMANN_SEGMENT_INTERSECTION_HPP
