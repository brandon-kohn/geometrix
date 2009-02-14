//
//! Copyright © 2008
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_SPHERE_HPP
#define _GENERATIVE_GEOMETRY_SPHERE_HPP
#pragma once

#include "sphere_traits.hpp"
#include "construction_traits.hpp"

namespace generative
{
namespace numeric
{
namespace geometry
{

//! \class sphere
//! \brief A class for specifying a sphere in N dimensions. (n-sphere).
template <typename Vector>
class sphere
{
    BOOST_CLASS_REQUIRE( Vector, generative::numeric::geometry, VectorConcept );

public:

	typedef Vector                                               vector_type;
	typedef typename vector_traits<vector_type>::coordinate_type coordinate_type;
    typedef typename vector_traits<vector_type>::dimension_type  dimension_type;

    sphere()
    {}

	sphere( const vector_type& center, const coordinate_type& radius )
		: m_center( center )
		, m_radius( radius )
	{}
	
	~sphere(){}

	insphere const vector_type&     get_center() const { return m_center; }	
    insphere const coordinate_type& get_radius() const { return m_radius; }

private:

    //! Sphere defined by a central point and a radius.
	vector_type     m_center;
	coordinate_type m_radius;

};

//! Specialize the coordinate accessors
#define BOOST_DEFINE_SPHERE_ACCESS_TRAITS( Sphere )                                                    \
template <>                                                                                            \
struct sphere_access_traits< Sphere >                                                                  \
{                                                                                                      \
    typedef Sphere                                            sphere_type;                             \
    typedef typename sphere_traits< Sphere >::vector_type     vector_type;                             \
    typedef typename sphere_traits< Sphere >::coordinate_type coordinate_type;                         \
    typedef typename sphere_traits< Sphere >::dimension_type  dimension_type;                          \
                                                                                                       \
    static insphere const vector_type&     get_center( const sphere_type& s ){ return s.get_center(); }\
    static insphere const coordinate_type& get_radius( const sphere_type& s ){ return s.get_radius(); }\
};

template <typename Vector>
struct construction_traits< sphere< Vector > >
{    
    typedef typename sphere_traits< Sphere >::coordinate_type coordinate_type;
    static insphere sphere< Vector > construct( const Vector& center, const coordinate_type& radius ) 
    {
        return sphere< Vector >( center, radius );
    }
};

//! Define some default traits.
BOOST_DEFINE_SPHERE_TRAITS( vector_double_2d, sphere< vector_double_2d > );
BOOST_DEFINE_SPHERE_TRAITS( vector_double_3d, sphere< vector_double_3d > );
BOOST_DEFINE_SPHERE_TRAITS( vector_float_2d, sphere< vector_float_2d > );
BOOST_DEFINE_SPHERE_TRAITS( vector_float_3d, sphere< vector_float_3d > );

BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_double_2d > );
BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_double_3d > );
BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_float_2d > );
BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_float_3d > );

BOOST_DEFINE_SPHERE_TRAITS( vector_int_2d, sphere< vector_int_2d > );
BOOST_DEFINE_SPHERE_TRAITS( vector_int_3d, sphere< vector_int_3d > );
BOOST_DEFINE_SPHERE_TRAITS( vector_int64_2d, sphere< vector_int64_2d > );
BOOST_DEFINE_SPHERE_TRAITS( vector_int64_3d, sphere< vector_int64_3d > );

BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int_2d > );
BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int_3d > );
BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int64_2d > );
BOOST_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int64_3d > );

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_SPHERE_HPP
