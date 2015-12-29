//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_GRID_TRAITS_HPP
#define GEOMETRIX_GRID_TRAITS_HPP
#pragma once

#include <geometrix/primitive/point.hpp>
#include <geometrix/primitive/point_sequence_utilities.hpp>

#include <cassert>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/cstdint.hpp>

namespace geometrix
{
    template <typename Coordinate>
    class grid_traits
    {
    public:

        typedef Coordinate coordinate_type;

        grid_traits( const coordinate_type& xmin, const coordinate_type& xmax, const coordinate_type& ymin, const coordinate_type& ymax, const coordinate_type& cellWidth )
            : m_xmin(xmin)
            , m_xmax(xmax)
            , m_ymin(ymin)
            , m_ymax(ymax)
            , m_cellWidth(cellWidth)
            , m_cellWidthDivisor(1/cellWidth)
        {
			BOOST_ASSERT( cellWidth > 0 );
			BOOST_ASSERT( xmin < xmax && ymin < ymax );

            m_numberXCells = boost::numeric_cast<boost::uint32_t>( (m_xmax - m_xmin) * m_cellWidthDivisor ) + 1;
            m_numberYCells = boost::numeric_cast<boost::uint32_t>((m_ymax - m_ymin) * m_cellWidthDivisor ) + 1;
        }

		grid_traits( const boost::tuple<coordinate_type, coordinate_type, coordinate_type, coordinate_type>& bounds, const coordinate_type& cellWidth )
			: m_xmin( boost::get<e_xmin>(bounds) )
			, m_xmax( boost::get<e_xmax>( bounds ) )
			, m_ymin( boost::get<e_ymin>( bounds ) )
			, m_ymax( boost::get<e_ymax>( bounds ) )
			, m_cellWidth( cellWidth )
			, m_cellWidthDivisor( 1 / cellWidth )
		{
			BOOST_ASSERT( cellWidth > 0 );
			BOOST_ASSERT( m_xmin < m_xmax && m_ymin < m_ymax );

			m_numberXCells = boost::numeric_cast<boost::uint32_t>((m_xmax - m_xmin) * m_cellWidthDivisor) + 1;
			m_numberYCells = boost::numeric_cast<boost::uint32_t>((m_ymax - m_ymin) * m_cellWidthDivisor) + 1;
		}
		
        coordinate_type get_min_x() const { return m_xmin; }
        coordinate_type get_min_y() const { return m_ymin; }
        coordinate_type get_max_x() const { return m_xmax; }
        coordinate_type get_max_y() const { return m_ymax; }
        coordinate_type get_cell_size() const { return m_cellWidth; }
        
        boost::uint32_t     get_x_index(coordinate_type x) const
		{
			BOOST_ASSERT( x >= m_xmin && x <= m_xmax );
			return static_cast<boost::uint32_t>((x - m_xmin) * m_cellWidthDivisor); 
		}

        boost::uint32_t     get_y_index(coordinate_type y) const 
		{
			BOOST_ASSERT( y >= m_ymin && y <= m_ymax );
			return static_cast<boost::uint32_t>((y - m_ymin) * m_cellWidthDivisor);
		}

        boost::uint32_t     get_width() const { return m_numberXCells; }
        boost::uint32_t     get_height() const { return m_numberYCells; }

		template <typename Point>
		bool is_contained( const Point& p ) const 
		{
			BOOST_CONCEPT_ASSERT( (Point2DConcept<Point>) );
			return get<0>( p ) > m_xmin && get<0>( p ) < m_xmax && get<1>( p ) > m_ymin && get<1>( p ) < m_ymax; 
		}

        point<coordinate_type, 2> get_cell_centroid(boost::uint32_t i, boost::uint32_t j) const
        {
            return point<coordinate_type, 2>(m_xmin + coordinate_type(i + .5) * m_cellWidth, m_ymin + coordinate_type(j + .5) * m_cellWidth);
        }

        std::vector< point<coordinate_type, 2> > get_cell_polygon(boost::uint32_t i, boost::uint32_t j) const
        {
            std::vector< point<coordinate_type, 2> > points(4);
                     
            coordinate_type xmax = m_xmin + (i + 1) * m_cellWidth;
            coordinate_type ymax = m_ymin + (j + 1) * m_cellWidth;
            coordinate_type xmin = m_xmin + i * m_cellWidth;
            coordinate_type ymin = m_ymin + j * m_cellWidth;
            assign(points[0], xmin, ymin);
            assign(points[1], xmax, ymin);
            assign(points[2], xmax, ymax);
            assign(points[3], xmin, ymax);

            return points;
        }

        //! Get the corner point for a cell starting from the lower left-hand point and working around in a counterclockwise winding.
        
        //! Access the lower left-hand corner point of the specified cell.
        point<coordinate_type, 2> get_cell_corner0(boost::uint32_t i, boost::uint32_t j) const
        {
            return point<coordinate_type, 2>(m_xmin + i * m_cellWidth, m_ymin + j * m_cellWidth);
        }

        //! Access the lower right-hand corner point of the specified cell.
        point<coordinate_type, 2> get_cell_corner1(boost::uint32_t i, boost::uint32_t j) const
        {
            return point<coordinate_type, 2>(m_xmin + (i + 1) * m_cellWidth, m_ymin + j * m_cellWidth);
        }

        //! Access the upper right-hand corner point of the specified cell.
        point<coordinate_type, 2> get_cell_corner2(boost::uint32_t i, boost::uint32_t j) const
        {
            return point<coordinate_type, 2>(m_xmin + (i + 1) * m_cellWidth, m_ymin + (j + 1) * m_cellWidth);
        }

        //! Access the upper left-hand corner point of the specified cell.
        point<coordinate_type, 2> get_cell_corner3(boost::uint32_t i, boost::uint32_t j) const
        {
            return point<coordinate_type, 2>(m_xmin + i * m_cellWidth, m_ymin + (j + 1) * m_cellWidth);
        }
        
        //! Access the x coordinate translated to the grid origin and scaled to grid units where a unit is the cell width.
        coordinate_type get_scaled_grid_coordinate_x(coordinate_type x) const { return (x - m_xmin) * m_cellWidthDivisor; }

        //! Access the y coordinate translated to the grid origin and scaled to grid units where a unit is the cell width.
        coordinate_type get_scaled_grid_coordinate_y(coordinate_type y) const { return (y - m_ymin) * m_cellWidthDivisor; }

        point<coordinate_type, 2> get_origin() const { return point<coordinate_type, 2>(m_xmin, m_ymin); }
        
    private:

        coordinate_type m_xmin;
        coordinate_type m_xmax;
        coordinate_type m_ymin;
        coordinate_type m_ymax;
        coordinate_type m_cellWidth;
        coordinate_type m_cellWidthDivisor;
        boost::uint32_t     m_numberXCells;
        boost::uint32_t     m_numberYCells;

    };

}//! namespace geometrix;

#endif // GEOMETRIX_GRID_TRAITS_HPP
