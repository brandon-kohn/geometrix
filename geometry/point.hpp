//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_POINT_HPP
#define _BOOST_GEOMETRY_POINT_HPP
#pragma once

#include "point_traits.hpp"
#include "math_functions.hpp"
#include "dimension_traits.hpp"
#include "construction_traits.hpp"
#include "numeric_sequence.hpp"
#include "detail/vector_generator.hpp"
#include "detail/point_generator.hpp"
#include "detail/pairwise_fusion_operations.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

template <typename NumericType, unsigned int Dimension>
struct has_compile_time_access< point< NumericType, Dimension > > : boost::true_type {};

template <typename NumericType, unsigned int Dimension>
struct has_run_time_access< point< NumericType, Dimension > > : boost::true_type {};

//! Concrete Point Types for some of the more common coordinate types.
typedef point<float, 2>       point_float_2d;
typedef point<float, 3>       point_float_3d;
typedef point<double, 2>      point_double_2d;
typedef point<double, 3>      point_double_3d;

typedef point<int, 2>         point_int_2d;
typedef point<int, 3>         point_int_3d;
typedef point<long long, 2>   point_int64_2d;
typedef point<long long, 3>   point_int64_3d;

//! Specialize the coordinate accessors for cartesian coords.
#define BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( Point )                                 \
template <>                                                                           \
struct cartesian_access_traits< Point > : public indexed_access_traits< Point >       \
{                                                                                     \
    typedef Point                                     point_type;                     \
    typedef point_traits<point_type>::coordinate_type coordinate_type;                \
    typedef point_traits<point_type>::dimension_type  dimension_type;                 \
};

template <unsigned int Index, typename Point, typename Dimension>
struct polar_coordinate_get
{};

template<typename Point>
struct polar_coordinate_get<0, Point, dimension_traits<2> >
{
    //! Access the radius.
    static typename point_traits<Point>::coordinate_type get( const Point& p )
    {
        typedef Point                                         point_type;                 
        typedef typename point_traits<Point>::coordinate_type coordinate_type;
        return math_functions< coordinate_type >::sqrt( p.get<e_x>() * p.get<e_x>() +
                                                        p.get<e_y>() * p.get<e_y>() );
    }

};

template<typename Point>
struct polar_coordinate_get<0, Point, dimension_traits<3> >
{
    //! Access the radius.
    static typename point_traits<Point>::coordinate_type get( const Point& p )
    {
        typedef Point                                         point_type;                     
        typedef typename point_traits<Point>::coordinate_type coordinate_type;
        return math_functions< coordinate_type >::sqrt( p.get<e_x>() * p.get<e_x>() +
                                                        p.get<e_y>() * p.get<e_y>() +
                                                        p.get<e_z>() * p.get<e_z>() ); 
    }     
};

template<typename Point, typename Dimension>
struct polar_coordinate_get<1, Point, Dimension>
{
    typedef Point                                              point_type;                     
    typedef typename point_traits<point_type>::coordinate_type coordinate_type;                
    typedef Dimension                                          dimension_type; 

    //! Access theta
    static inline coordinate_type get( const point_type& p ) { return math_functions< coordinate_type >::atan( p.get<1>() / p.get<0>() ); }  \
};

template<typename Point>
struct polar_coordinate_get<2, Point, dimension_traits<3> >
{
    typedef Point                                              point_type;                     
    typedef typename point_traits<point_type>::coordinate_type coordinate_type;                
    typedef typename point_traits<point_type>::dimension_type  dimension_type; 

    //! Access phi
    static inline coordinate_type get( const Point& p )
    {
        return math_functions< coordinate_type >::atan( math_functions< coordinate_type >::sqrt( p.get<e_x>()*p.get<e_x>() + p.get<e_y>() * p.get<e_y>() ) / p.get<e_z>() ); 
    } 
};

//! Specialize the coordinate accessors for polar coords.
#define BOOST_DEFINE_POLAR_ACCESS_TRAITS( Point )                                     \
template <>                                                                           \
struct polar_access_traits< Point >                                                   \
{                                                                                     \
    typedef Point                                     point_type;                     \
    typedef point_traits<point_type>::coordinate_type coordinate_type;                \
    typedef point_traits<point_type>::dimension_type  dimension_type;                 \
                                                                                      \
    template <unsigned int Index>                                                     \
    static inline coordinate_type get( const point_type& p ) { return polar_coordinate_get<Index, Point, dimension_type>::get( p ); } \
    template <typename T>                                                             \
    static inline typename boost::enable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<2> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t ) { return point_type( r*math_functions< coordinate_type >::cos(t), r*math_functions< coordinate_type >::sin(t) ); }                                                                                        \
    template <typename T>                                                             \
    static inline typename boost::disable_if< boost::is_same< typename point_traits<T>::dimension_type, dimension_traits<2> >, point_type >::type construct( const coordinate_type& r, const coordinate_type& t, const coordinate_type& phi ) { return point_type( r*math_functions< coordinate_type >::cos(t)*math_functions< coordinate_type >::sin(phi), r*math_functions< coordinate_type >::sin(t)*math_functions< coordinate_type >::sin(phi), r*math_functions< coordinate_type >::cos(phi) ); } \
};

BOOST_DEFINE_POINT_TRAITS( point_float_2d );
BOOST_DEFINE_POINT_TRAITS( point_float_3d );
BOOST_DEFINE_POINT_TRAITS( point_double_2d );
BOOST_DEFINE_POINT_TRAITS( point_double_3d );

BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_float_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_float_3d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_double_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_double_3d );

BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_float_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_float_3d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_double_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_double_3d );

BOOST_DEFINE_POINT_TRAITS( point_int_2d );
BOOST_DEFINE_POINT_TRAITS( point_int_3d );
BOOST_DEFINE_POINT_TRAITS( point_int64_2d );
BOOST_DEFINE_POINT_TRAITS( point_int64_3d );

BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int_3d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int64_2d );
BOOST_DEFINE_CARTESIAN_ACCESS_TRAITS( point_int64_3d );

BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int_3d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int64_2d );
BOOST_DEFINE_POLAR_ACCESS_TRAITS( point_int64_3d );

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_POINT_HPP
