//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_GEOMETRY_FLOODFILL_GRID_TRAVERSAL_HPP_
#define BOOST_GEOMETRY_FLOODFILL_GRID_TRAVERSAL_HPP_
#pragma once

#include "geometric_concepts.hpp"
#include <cassert>

namespace boost
{
namespace numeric
{
namespace geometry
{
   
//! \class floodfill_grid_traversal
//* This class encapsulates a color grid which can be used to perform
//* flood-fill traversals on any positive index based grid type.( x and y >= 0)
//* The traversal is based on code found on free code found on wikipedia.
template <typename GridTraits>
class floodfill_grid_traversal
{
public:

    typedef GridTraits grid_traits;

    floodfill_grid_traversal( const grid_traits& conversionPolicy )
        : m_colorGrid( conversionPolicy.get_width(), std::vector<size_t>( conversionPolicy.get_height(), 0 ) ),
          m_height( conversionPolicy.get_height() ),
          m_width( conversionPolicy.get_width() ),
          m_lastColor(0),
          m_conversionPolicy( conversionPolicy ),
          BOUNDARY( (std::numeric_limits<size_t>::max)() )
    {}

    virtual ~floodfill_grid_traversal(){}

    //! \brief Method to perform a flood traversal.
    //* Note: The visitor class must hold a reference and do the actual cell extraction from the grid.
    //* This allows this interface to remain generic with respect to all grid access details (these become details the visitor should know however).    
    template <typename Visitor>
    void traversal( int x, int y, Visitor& visitor );

    template <typename Segment, typename Visitor>
    void traverse_segment( const Segment& segment, Visitor& visitor );

    template <typename Segment>
    void mark_segment( const Segment& segment );

    template <typename PointSequence>
    void mark_point_sequence( const PointSequence& polyline );

    void reset_non_boundary_fills();

private:

    struct color_marker_visitor
    {
        color_marker_visitor( std::vector< std::vector< size_t > >& grid, size_t color )
            : m_grid( grid ), m_color( color )
        {}

        void visit( size_t x, size_t y ) 
        {
            m_grid[x][y]=m_color;
        }

        std::vector< std::vector< size_t > >& m_grid;
        size_t m_color;
    };

    size_t m_lastColor;
    typedef std::vector< size_t >     matrix_row;
    typedef std::vector< matrix_row > matrix;
    matrix m_colorGrid;
    grid_traits m_conversionPolicy;
    size_t m_height;
    size_t m_width;
    const size_t BOUNDARY;

};

template <typename PointSequence>
inline void floodfill_grid_traversal::mark_point_sequence( const PointSequence& polyline )
{
    typedef point_sequence_traits< PointSequence >::point_type point_type;
    typedef segment< point_type > segment_type;
    color_marker_visitor visitor( m_colorGrid, BOUNDARY );
    size_t numberSegments = point_sequence_traits<PointSequence>::size( polyline ) - 1;
    for( int i=0;i < numberSegments; ++i )
    {
        segment_type segment( point_sequence_traits< PointSequence >::get_point( polyline, i ), point_sequence_traits< PointSequence >::get_point( polyline, i + 1 ) );
        traverse_segment( segment, visitor );
    }
}

inline void floodfill_grid_traversal::mark_segment( const Segment& segment )
{
    color_marker_visitor visitor( m_colorGrid, BOUNDARY );
    traverse_segment( segment, visitor );
}

template <typename Visitor>
inline void floodfill_grid_traversal::traversal( int x, int y, Visitor& visitor )
{
    assert( x >= 0 && x < m_width );
    assert( y >= 0 && y < m_height );

    try
    {
        int y1; 
        bool spanLeft, spanRight;
        size_t oldColor = m_colorGrid[x][y];
        size_t newColor = m_lastColor + 1;///Note: if this ever wraps.. might cause invalid floods... 

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

template <typename Visitor>
inline void floodfill_grid_traversal::traverse_segment( const Segment& segment, Visitor& visitor )
{
    typedef typename segment_traits< Segment >::point_type point_type;
    typedef typename segment_access_traits< Segment >      segment_access;
    typedef typename cartesian_access_traits< point_type > point_access;
    
    ///First find the start and end cell index addresses.
    const point_type& sPoint = segment_access_traits< Segment >::get_start( segment );
    const point_type& tPoint = segment_access_traits< Segment >::get_end( segment );

    int x1 = m_conversionPolicy.get_x_index( point_access::get<0>( sPoint ) );
    int y1 = m_conversionPolicy.get_y_index( point_access::get<1>( sPoint ) );
    int x2 = m_conversionPolicy.get_x_index( point_access::get<0>( tPoint ) );
    int y2 = m_conversionPolicy.get_y_index( point_access::get<1>( tPoint ) );
    int dx, dy, bx, by, xsign, ysign, p, const1, const2;

    bx = x1;
    by = y1;
    dx = (x2 - x1);
    dy = (y2 - y1);
    if( (dx == 0) && (dy == 0) ) // line fits into one grid cell
    {
        visitor( bx, by );
    }
    else if ( dy == 0 ) // have a horizontal line
    {
        xsign = dx / abs(dx);

        visitor( bx, by );
        while( bx != x2 )
        {
            bx += xsign;
            visitor( bx, by );
        }
    }
    else if ( dx == 0 ) // have vertical line
    {
        ysign = dy / abs(dy);
        visitor( bx, by );
        while( by != y2 )
        {
            by += ysign;
            visitor( bx, by );
        }
    }
    else // use Bresenham algorithm
    {
        xsign = dx / abs(dx);
        ysign = dy / abs(dy);
        dx = abs(dx);
        dy = abs(dy);
        // set initial point on line
        visitor( bx, by );

        if(dx < dy ) // line more vertical than horizontal
        {
            p = 2 * dx - dy;
            const1 = 2 * dx;
            const2 = 2 * (dx - dy);
            while ( by != y2 )
            {
                by = by + ysign;
                if( p < 0 )
                {
                    p = p + const1;
                }
                else
                {
                    p = p + const2;
                    bx = bx + xsign;
                }

                visitor( bx, by );
            }
        }
        else // line more horizontal than vertical
        {
            p = 2 * dy - dx;
            const2 = 2 * (dy - dx);
            const1 = 2 * dy;
            while ( bx != x2 )
            {
                bx = bx + xsign;
                if ( p < 0 )
                {
                    p = p + const1;
                }
                else
                {
                    p = p + const2;
                    by = by + ysign;
                }

                visitor( bx, by );
            }
        }
    }
}

inline void floodfill_grid_traversal::reset_non_boundary_fills()
{    
    for( Matrix::iterator column( m_colorGrid.begin() ), end( m_colorGrid.end() ); column != end; ++column )
    {
        for( MatrixRow::iterator row( column->begin() ), rEnd( column->end() ); row != rEnd; ++row )
        {
            if( *row != BOUNDARY )
            {
                *row = 0;
            }
        }
    }
}

}}}///namespace 

#endif // BOOST_GEOMETRY_FLOODFILL_GRID_TRAVERSAL_HPP_
