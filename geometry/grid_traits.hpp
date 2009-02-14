//
//! Copyright © 2008
//! Brandon Kohn
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

namespace generative
{
namespace numeric
{
namespace geometry
{
    //! TODO: Need to treat this to N-dimensions.
    template <typename Coordinate>
    class grid_traits
    {
    public:

        typedef Coordinate coordinate_type;

        grid_traits( const coordinate_type& xmin, const coordinate_type& xmax, const coordinate_type& ymin, const coordinate_type& ymax, const coordinate_type& cellWidth )
            : m_xmin( xmin ), m_xmax( xmax ), m_ymin( ymin ), m_ymax( ymax ), m_cellWidth( cellWidth )
        {
            m_numberXCells = boost::numeric_cast<size_t>( (m_xmax - m_xmin) / cellWidth ) + 1;
            m_numberYCells = boost::numeric_cast<size_t>( (m_ymax - m_ymin) / cellWidth ) + 1;
        }

        inline coordinate_type get_min_x() const { return m_xmin; }
        inline coordinate_type get_min_y() const { return m_ymin; }
        inline coordinate_type get_max_x() const { return m_xmax; }
        inline coordinate_type get_max_y() const { return m_ymax; }
        inline coordinate_type get_cell_size() const { return m_cellWidth; }
        inline size_t          get_x_index( coordinate_type x ) const { return boost::numeric_cast<size_t>( (x-m_xmin)/m_cellWidth ); }
        inline size_t          get_y_index( coordinate_type y ) const { return boost::numeric_cast<size_t>( (y-m_ymin)/m_cellWidth ); }
        inline size_t          get_width() const { return m_numberXCells; }
        inline size_t          get_height() const { return m_numberYCells; }

        inline std::pair<size_t, size_t> get_indices( coordinate_type x, coordinate_type y )
        {
            return std::make_pair( boost::numeric_cast<size_t>( (x-m_xmin)/m_cellWidth ), boost::numeric_cast<size_t>( (y-m_ymin)/m_cellWidth ) );
        }

    private:

        coordinate_type m_xmin;
        coordinate_type m_xmax;
        coordinate_type m_ymin;
        coordinate_type m_ymax;
        coordinate_type m_cellWidth;
        size_t          m_numberXCells;
        size_t          m_numberYCells;

    };

}}}///namespace 

#endif // BOOST_GEOMETRY_GRID_TRAITS_HPP_
