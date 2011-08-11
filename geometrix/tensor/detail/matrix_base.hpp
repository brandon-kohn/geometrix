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

namespace geometrix {
  
template <typename T, unsigned int Rows, unsigned int Columns>
struct matrix
{
    BOOST_STATIC_ASSERT(( Rows <= GEOMETRIX_MAX_MATRIX_ROWS && Columns <= GEOMETRIX_MAX_MATRIX_COLUMNS ));

    typedef T              value_type;
    typedef T&             reference;
    typedef const T&       const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;
        
    boost::array< boost::array<T, Columns>, Rows > m;

    boost::array<T,Columns>&       operator[]( std::size_t i ) { return m[i]; }
    const boost::array<T,Columns>& operator[]( std::size_t i ) const { return m[i]; }
    
    template <int Row, int Column>
    const_reference get() const { return m[Row][Column]; }
    template <int Row, int Column>
    reference get(){ return m[Row][Column]; }

    const_reference get( int Row, int Column ) const { return m[Row][Column]; }
    reference get( int Row, int Column ){ return m[Row][Column]; }
};

}//namespace geometrix;

//GEOMETRIX_MATRIX_BASE_HPP
#endif
