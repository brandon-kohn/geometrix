//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_BENTLEY_OTTMANN_SWEEP_LINE_HPP
#define _GENERATIVE_GEOMETRY_BENTLEY_OTTMANN_SWEEP_LINE_HPP
#pragma once

#include <vector>
#include <set>
#include <functional>

namespace generative
{
namespace numeric
{
namespace geometry
{
    template <typename EventItem, typename SweepItem, typename SweepCompare>
    class sweep_line
    {
    public:

        typedef EventItem                                          event_item_type;
        typedef SweepItem                                          sweep_item_type;
        typedef SweepCompare                                       sweep_key_compare;
        typedef std::multiset<sweep_item_type*, sweep_key_compare> sweep_events;
        typedef typename sweep_events::iterator	 	               iterator; 
        typedef typename sweep_events::const_iterator	           const_iterator;        

        inline sweep_line(){}

        inline sweep_line( const sweep_key_compare& compare )
            : m_set( compare )
        {}

        inline ~sweep_line(){}

        inline iterator			      insert( sweep_item_type* item ){ return m_set.insert( item ); }

        inline iterator			      remove( sweep_item_type* item )
        {
            ///A linear search over the set comparing the value of the items (pointers usually) is likely cheaper than 
            ///the binary search using the comparison operator.
            
            iterator iter(m_set.begin());
            iterator theEnd(m_set.end());
            while(iter != theEnd)
            {
                if(*iter == item)
                {
                    m_set.erase(iter++);
                    return iter;
                }
                ++iter;
            }
            return theEnd;
        }

        inline void				      erase( iterator iter ) { m_set.erase(iter); }

        inline iterator			      begin() { return m_set.begin(); }

        inline iterator			      end() { return m_set.end(); }

        inline const_iterator	      begin() const { return m_set.begin(); }

        inline const_iterator	      end() const { return m_set.end(); }

        inline iterator			      lower_bound( sweep_item_type* item ) { return m_set.lower_bound(item); }

        inline iterator			      upper_bound( sweep_item_type* item ) { return m_set.upper_bound(item); }

        inline void				      set_current_event( const event_item_type& event ) { m_set.key_comp().set_current_event(event); }

        inline const event_item_type& get_current_event() const { return m_set.key_comp().get_current_event(); }

        inline size_t		          size() const { return m_set.size(); }

        inline bool                   empty() const { return m_set.empty(); }

    private:

        sweep_events m_set;

    };

    //! The Bentley-Ottmann Sweep Line algorithm. 
    //!	
    //!
    template <typename EventQueue, typename SweepLine, typename EventHandler>
    inline void bentley_ottmann_sweep( EventQueue& events, SweepLine& sweepLine, EventHandler& eventHandler )
    {
        while ( !events.empty() )
        {
            EventQueue::iterator iter = events.begin();
            eventHandler.handle_event( events, sweepLine, iter );
            events.erase( iter );
        }    
    }

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_BENTLEY_OTTMANN_SWEEP_LINE_HPP
