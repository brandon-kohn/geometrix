//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_GEOMETRY_GRID_TRAITS_HPP_
#define BOOST_GEOMETRY_GRID_TRAITS_HPP_
#pragma once

#include "geometric_concepts.hpp"
#include <cassert>

namespace boost
{
namespace numeric
{
namespace geometry
{

    class grid_traits
    {
    public:

        grid_traits( double xmin, double xmax, double ymin, double ymax, double tileSize = 1.0 )
            : m_xmin( xmin ), m_xmax( xmax ), m_ymin( ymin ), m_ymax( ymax ), m_tileSize( tileSize )
        {
            m_numberXTiles = static_cast<unsigned int>( (m_xmax - m_xmin) / tileSize ) + 1;
            m_numberYTiles = static_cast<unsigned int>( (m_ymax - m_ymin) / tileSize ) + 1;
        }

        inline double GetMinX() const { return m_xmin; }
        inline double GetMinY() const { return m_ymin; }
        inline double GetMaxX() const { return m_xmax; }
        inline double GetMaxY() const { return m_ymax; }
        inline double GetTileSize() const { return m_tileSize; }
        inline unsigned int GetXIndex( double x ) const { return static_cast<unsigned int>( (x-m_xmin)/m_tileSize ); }
        inline unsigned int GetYIndex( double y ) const { return static_cast<unsigned int>( (y-m_ymin)/m_tileSize ); }
        inline unsigned int GetWidth() const { return m_numberXTiles; }
        inline unsigned int get_height() const { return m_numberYTiles; }

        inline std::pair<unsigned int, unsigned int> get_indices( double x, double y )
        {
            return std::make_pair( static_cast<unsigned int>( (x-m_xmin)/m_tileSize ), static_cast<unsigned int>( (y-m_ymin)/m_tileSize ) );
        }

        inline CPoint2DArray GetCornerPoints( unsigned int i, unsigned int j ) const
        {
            CPoint2DArray points( 4 );
            double xmin = static_cast<double>( i ) * m_tileSize + m_xmin;
            double ymin = static_cast<double>( j ) * m_tileSize + m_ymin;
            double xmax = xmin + m_tileSize;
            double ymax = ymin + m_tileSize;
            points[0].Set( xmin, ymin );
            points[1].Set( xmax, ymin );
            points[2].Set( xmax, ymax );
            points[3].Set( xmin, ymax );
            return points;
        }

    private:

        double m_xmin;
        double m_xmax;
        double m_ymin;
        double m_ymax;
        double m_tileSize;
        unsigned int m_numberXTiles;
        unsigned int m_numberYTiles;

    };

//!
//! flood_scanline_traversal
//!
//! This class encapsulates a color grid which can be used to perform
//! flood-fill traversals on any positive index based grid type.( x and y >= 0)
//! The traversal is based on code found on free code found on wikipedia.
//!
template <typename GridTraits>
class flood_scanline_traversal
{
public:

    typedef GridTraits grid_traits;

    //! Default Constructor
    flood_scanline_traversal( const grid_traits& conversionPolicy )
        : m_colorGrid( conversionPolicy.GetWidth(), std::vector<size_t>( conversionPolicy.get_height(), 0 ) ),
          m_height( conversionPolicy.get_height() ),
          m_width( conversionPolicy.GetWidth() ),
          m_lastColor(0),
          m_conversionPolicy( conversionPolicy ),
          BOUNDARY( (std::numeric_limits<unsigned int>::max)() )
    {
        
    }

    //! Destructor
    virtual ~flood_scanline_traversal(){}

    // Public Member Functions

    ///Method to perform a traversal. Note: The visitor class must hold a reference and do the actual cell extraction from the grid.
    ///This allows this interface to remain generic with respect to all grid access details (these become details the visitor should know however).
    ///It was done this way because we have no common grid interface.
    template <typename visitor>
    void traversal( int x, int y, visitor& visitor );

    template <typename Segment, typename visitor>
    void traverse_segment( const Segment& segment, visitor& visitor );

    template <typename Segment>
    void mark_segment( const Segment& segment );

    template <typename PointSequence>
    void mark_pointsequence( const PointSequence& polyline );

    void reset_non_boundary_fills();

private:

    struct color_marker_visitor
    {
        color_marker_visitor( std::vector< std::vector< size_t > >& grid, unsigned int color )
            : m_grid( grid ), m_color( color )
        {

        }

        void visit( unsigned int x, unsigned int y ) 
        {
            m_grid[x][y]=m_color;
        }

        std::vector< std::vector< size_t > >& m_grid;
        unsigned int m_color;
    };

    unsigned int m_lastColor;
    typedef std::vector< size_t > matrix_row;
    typedef std::vector< MatrixRow > matrix;
    matrix m_colorGrid;
    grid_traits m_conversionPolicy;
    size_t m_height;
    size_t m_width;
    const unsigned int BOUNDARY;
    
};

/////////////////////////////////////////////////////////////////////////////
//
// Inline methods
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//! <b>Detailed Description:</b>\n
//! ---
//! 
//! <b>Return:</b>\n
//! inline void 
//! 
//! @param const PointSequence& polyline
//! 
//! <b>Modifications:</b>\n
//! 08-06-2007 : Brandon Kohn, Created\n
//! 
/////////////////////////////////////////////////////////////////////////////
inline void flood_scanline_traversal::mark_pointsequence( const PointSequence& polyline )
{
    color_marker_visitor visitor( m_colorGrid, BOUNDARY );
    int numberSegments = polyline.Ns();
    for( int i=0;i < numberSegments; ++i )
    {
        traverse_segment( polyline( i, polyline.NextIndex(i) ), visitor );
    }
}

inline void flood_scanline_traversal::mark_segment( const Segment& segment )
{
    color_marker_visitor visitor( m_colorGrid, BOUNDARY );
    traverse_segment( segment, visitor );
}

template <typename visitor>
inline void flood_scanline_traversal::traversal( int x, int y, visitor& visitor )
{
    assert( x >= 0 && x < m_width );
    assert( y >= 0 && y < m_height );

    try
    {
        int y1; 
        bool spanLeft, spanRight;
        unsigned int oldColor = m_colorGrid[x][y];
        unsigned int newColor = m_lastColor + 1;///Note: if this ever wraps.. might cause invalid floods... 

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

            //std::stringstream str;
            //std::cout << "Processing pair: " << x << "," << y << std::endl;
            //str << "Processed x: " << x << " y[0]: " << y1 << "... y[n]: ";
            spanLeft = spanRight = 0;
            while(y1 < m_height && m_colorGrid[x][y1] == oldColor )
            {
                ///Only change interior colors.
                if( m_colorGrid[x][y1] != BOUNDARY )
                {
                    m_colorGrid[x][y1] = newColor;
                }

                visitor.visit( x, y1 );
                if(!spanLeft && x > 0 && m_colorGrid[x - 1][y1] == oldColor) 
                {
                    //if(!push(x - 1, y1)) return;
                    //std::cout << "\t" << "Pushing: x: " << (x-1) << " y: " << y1 << std::endl;
                    stack.push( std::make_pair(x - 1, y1) );
                    spanLeft = 1;
                }
                else if(spanLeft && x > 0 && m_colorGrid[x - 1][y1] != oldColor)
                {
                    spanLeft = 0;
                }
                if(!spanRight && x < m_width - 1 && m_colorGrid[x + 1][y1] == oldColor) 
                {
                    //if(!push(x + 1, y1)) return;
                    //std::cout << "\t" << "Pushing: x: " << (x+1) << " y: " << y1 << std::endl;
                    stack.push( std::make_pair(x + 1, y1) );
                    spanRight = 1;
                }
                else if(spanRight && x < m_width - 1 && m_colorGrid[x + 1][y1] != oldColor)
                {
                    spanRight = 0;
                } 
                y1++;
            }

            //std::cout << str.str() << y1 << std::endl;           
        }
    }
    catch ( ... )
    {
        ///need to update the traversal state.
        ++m_lastColor;
        throw;    	
    }

    ++m_lastColor;
}

/////////////////////////////////////////////////////////////////////////////
//! <b>Detailed Description:</b>\n
//! ---
//! Improved Bresenham algorithm to draw a line in a grid with minimal special
//! case handling. Based on code by Steve Cunningham and Tim Worsham, October 
//! 1988, CSU Stanislaus.\n
//!
//! <b>Return:</b>\n
//! ---
//! Nothing.\n
//! 
//! <b>Modifications:</b>\n
//! 15/07/2005 : JAMOS, Created\n
//! 26/08/2005 : JAMOS, Moved here from CModelView.\n
//! 
/////////////////////////////////////////////////////////////////////////////
template <typename visitor>
inline void flood_scanline_traversal::traverse_segment( const Segment& segment, visitor& visitor )
{
    ///First find the start and end cell index addresses.
    int x1 = m_conversionPolicy.GetXIndex( segment.Ox() );
    int y1 = m_conversionPolicy.GetYIndex( segment.Oy() );
    int x2 = m_conversionPolicy.GetXIndex( segment.Dx() );
    int y2 = m_conversionPolicy.GetYIndex( segment.Dy() );
    int dx, dy, bx, by, xsign, ysign, p, const1, const2;

    bx = x1;
    by = y1;
    dx = (x2 - x1);
    dy = (y2 - y1);
    if( (dx == 0) && (dy == 0) ) // line fits into one grid cell
    {
        visitor.visit( bx, by );
    }
    else if ( dy == 0 ) // have a horizontal line
    {
        xsign = dx / abs(dx);

        visitor.visit( bx, by );
        while( bx != x2 )
        {
            bx += xsign;
            visitor.visit( bx, by );
        }
    }
    else if ( dx == 0 ) // have vertical line
    {
        ysign = dy / abs(dy);
        visitor.visit( bx, by );
        while( by != y2 )
        {
            by += ysign;
            visitor.visit( bx, by );
        }
    }
    else // use Bresenham algorithm
    {
        xsign = dx / abs(dx);
        ysign = dy / abs(dy);
        dx = abs(dx);
        dy = abs(dy);
        // set initial point on line
        visitor.visit( bx, by );

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

                visitor.visit( bx, by );
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

                visitor.visit( bx, by );
            }
        }
    }
}

inline void flood_scanline_traversal::reset_non_boundary_fills()
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

#endif // BOOST_GEOMETRY_GRID_TRAITS_HPP_
