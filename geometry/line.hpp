//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_LINE_HPP
#define _BOOST_GEOMETRY_LINE_HPP
#pragma once

#include "geometric_concepts.hpp"
#include "vector_traits.hpp"
#include "line_traits.hpp"
#include "construction_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

/////////////////////////////////////////////////////////////////////////////
//
// CLASS line
// A template class for specifying a line with two vectors in N dimensions.
template <typename Vector>
class line
{
    BOOST_CLASS_REQUIRE( Vector, boost::numeric::geometry, VectorConcept );

public:

	typedef Vector                                               vector_type;
	typedef typename vector_traits<vector_type>::coordinate_type coordinate_type;
    typedef typename vector_traits<vector_type>::dimension_type  dimension_type;

    line()
    {}

	line( const vector_type& u, const vector_type& v, const coordinate_type& t )
		: m_u( u )
		, m_v( v )
        , m_t( t )
	{}
	
	~line(){}

	inline const vector_type&     get_u() const { return m_u; }
	inline const vector_type&     get_v()const { return m_v; }
    inline const coordinate_type& get_t() const { return m_t; }

private:

    //! Line defined by u + tv
	vector_type     m_u;
	vector_type     m_v;
    coordinate_type m_t;

};

//! Specialize the coordinate accessors
#define BOOST_DEFINE_LINE_ACCESS_TRAITS( Line )                                          \
template <>                                                                              \
struct line_access_traits< Line >                                                        \
{                                                                                        \
    typedef Line                                          line_type;                     \
    typedef typename line_traits< Line >::vector_type     vector_type;                   \
    typedef typename line_traits< Line >::coordinate_type coordinate_type;               \
    typedef typename line_traits< Line >::dimension_type  dimension_type;                \
                                                                                         \
    static inline const vector_type&     get_u( const line_type& l ){ return l.get_u(); }\
	static inline const vector_type&     get_v( const line_type& l ){ return l.get_v(); }\
    static inline const coordinate_type& get_t( const line_type& l ){ return l.get_t(); }\
};

template <typename Vector>
struct construction_traits< line< Vector > >
{    
    typedef typename line_traits< Line >::coordinate_type coordinate_type;
    static inline line< Vector > construct( const Vector& u, const Vector& v, const coordinate_type& t ) 
    {
        return line< Vector >( u, v, t );
    }
};

//! Define some default traits.
BOOST_DEFINE_LINE_TRAITS( vector_double_2d, line< vector_double_2d > );
BOOST_DEFINE_LINE_TRAITS( vector_double_3d, line< vector_double_3d > );
BOOST_DEFINE_LINE_TRAITS( vector_float_2d, line< vector_float_2d > );
BOOST_DEFINE_LINE_TRAITS( vector_float_3d, line< vector_float_3d > );

BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_double_2d > );
BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_double_3d > );
BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_float_2d > );
BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_float_3d > );

BOOST_DEFINE_LINE_TRAITS( vector_int_2d, line< vector_int_2d > );
BOOST_DEFINE_LINE_TRAITS( vector_int_3d, line< vector_int_3d > );
BOOST_DEFINE_LINE_TRAITS( vector_int64_2d, line< vector_int64_2d > );
BOOST_DEFINE_LINE_TRAITS( vector_int64_3d, line< vector_int64_3d > );

BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_int_2d > );
BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_int_3d > );
BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_int64_2d > );
BOOST_DEFINE_LINE_ACCESS_TRAITS( line< vector_int64_3d > );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_LINE_HPP
