//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_MATRIX_BASE_HPP
#define GEOMETRIX_MATRIX_BASE_HPP

#include <geometrix/utility/static_assert.hpp>

namespace geometrix {
  
template <typename T, std::size_t Rows, std::size_t Columns>
struct matrix
{
    GEOMETRIX_STATIC_ASSERT(( Rows <= GEOMETRIX_MAX_MATRIX_ROWS && Columns <= GEOMETRIX_MAX_MATRIX_COLUMNS ));

	BOOST_STATIC_CONSTANT( std::size_t, RowCount = Rows );
	BOOST_STATIC_CONSTANT( std::size_t, ColCount = Columns );

    typedef T              value_type;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;
        
    T elems[Rows][Columns];

    T* operator[]( std::size_t i ) { return elems[i]; }
    T const* operator[]( std::size_t i ) const { return elems[i]; }
    
    template <int Row, int Column>
    const_reference get() const { return elems[Row][Column]; }
    template <int Row, int Column>
    reference get(){ return elems[Row][Column]; }

    const_reference get( int Row, int Column ) const { return elems[Row][Column]; }
    reference get( int Row, int Column ){ return elems[Row][Column]; }
};

}//namespace geometrix;

//GEOMETRIX_MATRIX_BASE_HPP
#endif
