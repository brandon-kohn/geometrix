//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_BENTLEY_OTTMANN_SEGMENT_INTERSECTION_HPP
#define GEOMETRIX_BENTLEY_OTTMANN_SEGMENT_INTERSECTION_HPP
#pragma once

#include <geometrix/algorithm/bentley_ottmann_sweep.hpp>
#include <geometrix/utility/utilities.hpp>
#include <geometrix/numeric/rational_utilities.hpp>
#include <geometrix/primitive/point_traits.hpp>

#include <boost/concept_check.hpp>

#include <map>
#include <memory>

namespace geometrix {
 
    template <typename Point, typename Segment, typename NumberComparisonPolicy>
    class sweepline_ordinate_compare
    {
    public:

        typedef Point                                                point_type;
        typedef Segment                                              segment_type;
        typedef cartesian_access_traits< point_type >                point_access;
        typedef segment_access_traits< segment_type >                segment_access;
        typedef typename geometric_traits< point_type >::arithmetic_type coordinate_type;

        sweepline_ordinate_compare()
            : m_point( std::make_shared<point_type>() )
        {}

        sweepline_ordinate_compare( const NumberComparisonPolicy& compare )
			: m_point( std::make_shared<point_type>() )
            , m_compare( compare )
        {}

        template <typename SegmentIterator>
        bool operator()( SegmentIterator s1, SegmentIterator s2 ) const  
        {
            if( s1 != s2 )
            {
                const point_type& currentPoint = get_current_event();

                const point_type& s1_start = get_start( *s1 );
                const point_type& s1_end= get_end( *s1 );

                const point_type& s2_start = get_start( *s2 );
                const point_type& s2_end= get_end( *s2 );

                if( numeric_sequence_equals( s1_start, s2_start, m_compare ) && numeric_sequence_equals( s1_end, s2_end, m_compare ) )
                    return false;

                bool s1Vertical = is_vertical( s1_start, s1_end, m_compare );	
                bool s2Vertical = is_vertical( s2_start, s2_end, m_compare );

                if( s1Vertical && s2Vertical )
                    return s1 < s2;
                else if ( s1Vertical && is_between( s2_start, s2_end, currentPoint, true, m_compare ) )
                    return false;
                else if ( s2Vertical && is_between( s1_start, s1_end, currentPoint, true, m_compare ) )
                    return true;
                else
                    return segment_compare<coordinate_type>::compare( s1Vertical, s2Vertical, s1_start, s1_end, s2_start, s2_end, s1, s2, *m_point, m_compare );                
            }
            else
                return false;
        }

        const point_type& get_current_event() const { return *m_point; }
        void              set_current_event( const point_type& p ) { *m_point = p; }

    private:

        template <typename NumericType, typename EnableIf = void>
        struct segment_compare
        {};

        template <typename NumericType>
        struct segment_compare< NumericType, typename boost::enable_if< typename numeric_traits< NumericType >::is_integral >::type >
        {
            typedef NumericType coordinate_type;
            
            template <typename SegmentIterator>
            static bool compare( bool s1IsVertical, bool s2IsVertical, const point_type& s1_start, const point_type& s1_end, const point_type& s2_start, const point_type& s2_end, SegmentIterator s1, SegmentIterator s2, const Point& _point, const NumberComparisonPolicy& _compare )
            {            
                typedef typename rational_promotion_policy< coordinate_type >::rational_type rational_type;
                coordinate_type xEvent = get<0>( _point );
                coordinate_type one( 1 );
                coordinate_type zero( 0 );
                rational_type y1(zero,one), slope1(zero,one);
                if( s1IsVertical )
                {
                    coordinate_type one = 1;
                    y1 = boost::rational<coordinate_type>( get<1>( _point ), one );
                }
                else
                    y1 = rational_y_of_x( s1_start, s1_end, xEvent, slope1, _compare );
                
                rational_type y2(zero,one), slope2(zero,one);
                if( s2IsVertical )
                {   
                    coordinate_type one = 1;
                    y2 = rational_type( get<1>( _point ), one );
                }
                else
                    y2 = rational_y_of_x( s2_start, s2_end, xEvent, slope2, _compare );

                if( _compare.less_than( y1, y2 ) )
                    return true;
                else if( _compare.greater_than( y1, y2 ) )
                    return false;

                ///This is the case where the segments intersect in the event point. 
                ///Neither segment is vertical at this point as both segments contain the point
                ///and that case is dealt with in the previous calling function. 
                ///It remains to compare the slopes of the two segments (thus reversing their order in the sweep line).
                ///If the slopes are equal, sort by the iterator ordering in the original segment list.
                if( _compare.equals( y1, y2 ) )
                {
                    //The points are equal.. so compare slopes.
                    BOOST_ASSERT( !s1IsVertical && !s2IsVertical ); //should be no verticals here as that case is dealt with above.
                    if( _compare.less_than( slope1, slope2 ) )
                        return true;
                    else if( _compare.greater_than( slope1, slope2 ) )
                        return false;
                }

                //compare iterators as a last ditch effort.
                return s1 < s2;
            }
            
        };

        template <typename NumericType>
        struct segment_compare< NumericType, typename boost::enable_if< typename numeric_traits< NumericType >::is_float >::type >
        {
            typedef NumericType coordinate_type;

            template <typename SegmentIterator>
            static bool compare( bool s1IsVertical, bool s2IsVertical, const point_type& s1_start, const point_type& s1_end, const point_type& s2_start, const point_type& s2_end, SegmentIterator s1, SegmentIterator s2, const Point& _point, const NumberComparisonPolicy& _compare )
            {            
                coordinate_type xEvent = get<0>( _point );            
                coordinate_type y1, slope1;
                if( s1IsVertical )
                    y1 = get<1>( _point );
                else
                    y1 = y_of_x( s1_start, s1_end, xEvent, slope1 );
                
                coordinate_type y2, slope2;
                if( s2IsVertical )
                    y2 = get<1>( _point );
                else
                    y2 = y_of_x( s2_start, s2_end, xEvent, slope2 );
                
                if( _compare.less_than( y1, y2 ) )
                    return true;
                else if( _compare.greater_than( y1, y2 ) )
                    return false;
                
                ///This is the case where the segments intersect in the event point. 
                ///Neither segment is vertical at this point as both segments contain the point
                ///and that case is dealt with in the previous calling function. 
                ///It remains to compare the slopes of the two segments (thus reversing their order in the sweep line).
                ///If the slopes are equal, sort by the iterator ordering in the original segment list.
                if( _compare.equals( y1, y2 ) )
                {
                    //The points are equal.. so compare slopes.
                    BOOST_ASSERT( !s1IsVertical && !s2IsVertical ); //should be no verticals here as that case is dealt with above.
                    if( _compare.less_than( slope1, slope2 ) )
                        return true;
                    else if( _compare.greater_than( slope1, slope2 ) )
                        return false;
                }

                //compare iterators as a last ditch effort.
                return s1 < s2;
            }
        };

        std::shared_ptr< point_type > m_point; 
        NumberComparisonPolicy        m_compare;        

    };

    template< typename Point, typename Segment, typename NumberComparisonPolicy>
    struct process_new_events
    {
        typedef Point point_type;
        typedef Segment segment_type;
        typedef typename geometric_traits<point_type>::arithmetic_type coordinate_type;
        typedef cartesian_access_traits< point_type > point_access;
        typedef segment_access_traits< segment_type > segment_access;

        process_new_events( const NumberComparisonPolicy& compare = NumberComparisonPolicy() )
            : m_compare( compare )
        {}

        template <typename EventQueue, typename SweepLine>
		void operator()( EventQueue& eventQueue, SweepLine& sweepLine, const typename SweepLine::sweep_item_type* s1, const typename SweepLine::sweep_item_type* s2 )
        {            
			if( s1 != s2 )
			{
				const point_type& point = sweepLine.get_current_event();
				coordinate_type positionX = get<0>( point );
				coordinate_type positionY = get<1>( point );
				point_type xPoint[2];
				intersection_type iType = calculate_intersection( *s1, *s2, xPoint, m_compare );
				if( iType != e_non_crossing )
				{
					//segments intersect at xPoint
					coordinate_type x = get<0>( xPoint[0] );
					coordinate_type y = get<1>( xPoint[0] );

					//Add the event if it is to the right of the sweep line.
					if( m_compare.greater_than( x, positionX ) || (m_compare.equals( x, positionX ) && m_compare.greater_than( y, positionY )) )
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
            typedef Event     point_type;

            const point_type& segment_end = get_end( sweepItem );
            return numeric_sequence_equals( event, segment_end, compare );
        }

        template <typename SweepItem, typename Event>
        bool sweep_item_starts_with( const SweepItem& sweepItem, const Event& event )
        {
            typedef Event     point_type;

            const point_type& segment_end = get_start( sweepItem );
            return numeric_sequence_equals( event, segment_end, compare );
        }

        template <typename SweepItem, typename Event>
        bool sweep_item_overlaps( const SweepItem& sweepItem, const Event& event )
        {
            typedef Event     point_type;
            
            const point_type& segment_start = get_start( sweepItem );
            const point_type& segment_end = get_end( sweepItem );

            return is_between( segment_start, segment_end, event, true, compare );
        }

        template <typename SweepLine, typename Event>
        typename SweepLine::iterator lower_bound_for_event( SweepLine& sweepLine, Event& event )
        {
            typedef typename SweepLine::sweep_item_type                      segment_type;
            typedef typename geometric_traits< segment_type >::point_type    point_type;
            typedef typename geometric_traits< point_type >::arithmetic_type coordinate_type;

            point_type just_right_of_event = construct<point_type>( get<0>( event ) + coordinate_type(1), get<1>( event )  );
            
            segment_type segment = construct<segment_type>( event, just_right_of_event );
            return sweepLine.lower_bound( &segment );
        }

        template <typename EventQueue, typename SweepLine>
        void handle_event( EventQueue& eventQueue, SweepLine& sweepLine, typename EventQueue::iterator& event )
        {            
            typedef typename SweepLine::iterator sweep_item_iterator;
            typedef typename SweepLine::sweep_item_type sweep_item_type;
            sweepLine.set_current_event( event->first );
            std::set<sweep_item_type*> L;
            std::set<sweep_item_type*> C;
            std::set<sweep_item_type*>& U = event->second;
            
            //Sweep the scan line and classify sweep_items as ending with this event (L structure), beginning with the current event (U structure) or overlapping the current event (C structure)
            sweep_item_iterator sweepIter( sweepLine.begin() );
            sweep_item_iterator sEnd( sweepLine.end() );
            while( sweepIter != sEnd )
            {   
                if( sweep_item_ends_with( **sweepIter, event->first ) ) //if the sweep item ends in the event.
                    L.insert( *sweepIter );
                else if( sweep_item_overlaps( **sweepIter, event->first ) && !sweep_item_starts_with( **sweepIter, event->first ) )
                    C.insert( *sweepIter );
                
                ++sweepIter;
            }
            
            typename std::set<sweep_item_type*>::iterator iter;
            typename std::set<sweep_item_type*>::iterator end;

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
                sweepLine.remove( *iter );
            
            //Insert U and C again.
            for ( iter = UC.begin(); iter != UC.end(); ++iter )
                sweepLine.insert( *iter );

            //visitor.debug_post_order( sweepLine.begin(), sweepLine.end(), event->first );
            
            if(sweepLine.size() < 2)
                return;

            if ( UC.empty() )
            {
                auto swpIt = lower_bound_for_event( sweepLine, event->first );
                if ( swpIt != sweepLine.end() && swpIt != sweepLine.begin() )
                {
                    auto s1 = swpIt;                    
                    if( --s1 != sweepLine.end() )
                        process_new_events( eventQueue, sweepLine, *s1, *swpIt );
                }
            }
            else
            {
                for ( auto swpIt = sweepLine.begin(); swpIt != sweepLine.end(); ++swpIt )
                {
                    if ( UC.find( *swpIt ) != UC.end() && swpIt != sweepLine.begin() )
                    {
						auto s1 = swpIt;
						if( --s1 != sweepLine.end() )
							process_new_events( eventQueue, sweepLine, *s1, *swpIt );
                        break;
                    }
                }
                                                
                for ( auto swpIt = sweepLine.rbegin(); swpIt != sweepLine.rend(); ++swpIt )
                {
                    if ( UC.find( *swpIt ) != UC.end() )
                    {
						auto spp = swpIt;
                        if( ++spp != sweepLine.rend() )
							process_new_events( eventQueue, sweepLine, *swpIt, *spp );
                        break;
                    }
                }   
            }            
        }//handle_event

        NewEventProcessor      process_new_events;
        Visitor                visitor;
        NumberComparisonPolicy compare;
    };
    
    template <typename Segment, typename Visitor, typename NumberComparisonPolicy>
    inline void bentley_ottmann_segment_intersection( const std::vector< Segment >& segments, Visitor& visitor, const NumberComparisonPolicy& compare )
    {
        typedef Segment                                        segment_type;
        typedef typename geometric_traits<Segment>::point_type point_type;
        
        BOOST_CONCEPT_ASSERT((SegmentConcept<segment_type>));
        BOOST_CONCEPT_ASSERT((Point2DConcept<point_type>));

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
        typename std::vector< segment_type >::iterator sIter( ordered_segs.begin() );
        typename std::vector< segment_type >::iterator theEnd( ordered_segs.end() );        
        while(sIter != theEnd)
        {
            segment_type& segment = *sIter;
            if ( !lexi_comp( get_start( segment ), get_end( segment ) ) )
                segment = construct<segment_type>( get_end( segment ), get_start( segment ) );

            eventQueue[ get_start( segment ) ].insert( &*sIter );
            eventQueue[ get_end( segment ) ];//simply ensure the latter event exists.. no need to associate the segment.
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

}//namespace geometrix;

#endif //GEOMETRIX_BENTLEY_OTTMANN_SEGMENT_INTERSECTION_HPP
