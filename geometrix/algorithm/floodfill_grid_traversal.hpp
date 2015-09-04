//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FLOODFILL_GRID_TRAVERSAL_HPP
#define GEOMETRIX_FLOODFILL_GRID_TRAVERSAL_HPP
#pragma once

#include <geometrix\algorithm\bresenham_segment_traversal.hpp>
#include <cassert>

namespace geometrix
{

//! \class floodfill_grid_traversal
//! This class encapsulates a color grid which can be used to perform
//! flood-fill traversals on any positive index based grid type.( x and y >= 0)
template <typename GridTraits>
class floodfill_grid_traversal
{
public:

    typedef GridTraits grid_traits;

    floodfill_grid_traversal( const grid_traits& gridTraits )
        : m_colorGrid( gridTraits.get_width(), std::vector<std::size_t>( gridTraits.get_height(), 0 ) ),
          m_height( gridTraits.get_height() ),
          m_width( gridTraits.get_width() ),
          m_lastColor(0),
          m_gridTraits( gridTraits ),
          BOUNDARY( (std::numeric_limits<std::size_t>::max)() )
    {}

    virtual ~floodfill_grid_traversal(){}

    //! \brief Method to perform a flood traversal.
    //! Note: The visitor class must hold a reference and do the actual cell extraction from the grid.
    //! This allows this interface to remain generic with respect to all grid access details (these become details the visitor should know however).    
    template <typename Visitor>
    void traversal( int x, int y, Visitor& visitor );

    template <typename Segment>
    void mark_segment( const Segment& segment );

    template <typename PointSequence>
    void mark_point_sequence( const PointSequence& polyline );

    void reset_non_boundary_fills()
    {
        for (grid::iterator column(m_colorGrid.begin()), end(m_colorGrid.end()); column != end; ++column)
        {
            for (grid_row::iterator row(column->begin()), rEnd(column->end()); row != rEnd; ++row)
            {
                if (*row != BOUNDARY)
                {
                    *row = 0;
                }
            }
        }
    }

private:

    struct color_marker_visitor
    {
        color_marker_visitor( std::vector< std::vector< std::size_t > >& grid, std::size_t color )
            : m_grid( grid ), m_color( color )
        {}

        void visit( std::size_t x, std::size_t y ) 
        {
            m_grid[x][y]=m_color;
        }

        std::vector< std::vector< std::size_t > >& m_grid;
        std::size_t m_color;
    };

    std::size_t m_lastColor;
    typedef std::vector< std::size_t > grid_row;
    typedef std::vector< grid_row > grid;
    grid m_colorGrid;
    grid_traits m_gridTraits;
    std::size_t m_height;
    std::size_t m_width;
    const std::size_t BOUNDARY;

};

template <typename PointSequence>
inline void floodfill_grid_traversal::mark_point_sequence( const PointSequence& polyline )
{
    typedef point_sequence_traits< PointSequence >::point_type point_type;
    typedef segment< point_type > segment_type;
    color_marker_visitor visitor( m_colorGrid, BOUNDARY );
    std::size_t numberSegments = point_sequence_traits<PointSequence>::size( polyline ) - 1;
    for( int i=0;i < numberSegments; ++i )
    {
        segment_type segment( point_sequence_traits< PointSequence >::get_point( polyline, i ), point_sequence_traits< PointSequence >::get_point( polyline, i + 1 ) );
        bresenham_segment_traversal( segment, visitor );
    }
}

template <typename Segment>
inline void floodfill_grid_traversal::mark_segment( const Segment& segment )
{
    color_marker_visitor visitor( m_colorGrid, BOUNDARY );
    bresenham_segment_traversal( segment, visitor );
}

template <typename Visitor>
inline void floodfill_grid_traversal::traversal( int x, int y, Visitor& visitor )
{
    BOOST_ASSERT( x >= 0 && x < m_width );
    BOOST_ASSERT( y >= 0 && y < m_height );

    try
    {
        int y1; 
        bool spanLeft, spanRight;
        std::size_t oldColor = m_colorGrid[x][y];
        std::size_t newColor = m_lastColor + 1;///Note: if this ever wraps.. might cause invalid floods... 

        std::stack< std::pair<int,int> > stack;
        stack.push( std::make_pair(x,y) );

        while( !stack.empty() )
        {    
            std::pair<int,int>& top = stack.top();
            x = top.first;
            y = top.second;
            stack.pop();

            y1 = y;
            while( y1 >= 0 && m_colorGrid[x][y1] == oldColor ) 
            {
                y1--;
            }
            ++y1;

            spanLeft = spanRight = 0;
            while(y1 < m_height && m_colorGrid[x][y1] == oldColor )
            {
                ///Only change interior colors.
                if( m_colorGrid[x][y1] != BOUNDARY )
                {
                    m_colorGrid[x][y1] = newColor;
                }

                visitor( x, y1 );
                if(!spanLeft && x > 0 && m_colorGrid[x - 1][y1] == oldColor) 
                {
                    stack.push( std::make_pair(x - 1, y1) );
                    spanLeft = 1;
                }
                else if(spanLeft && x > 0 && m_colorGrid[x - 1][y1] != oldColor)
                {
                    spanLeft = 0;
                }
                if(!spanRight && x < m_width - 1 && m_colorGrid[x + 1][y1] == oldColor) 
                {
                    stack.push( std::make_pair(x + 1, y1) );
                    spanRight = 1;
                }
                else if(spanRight && x < m_width - 1 && m_colorGrid[x + 1][y1] != oldColor)
                {
                    spanRight = 0;
                } 
                y1++;
            }
        }
    }
    catch ( ... )
    {
        ++m_lastColor;
        throw;    	
    }

    ++m_lastColor;
}

}//! namespace geometrix;

#endif // GEOMETRIX_FLOODFILL_GRID_TRAVERSAL_HPP
