//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SPHERE_HPP
#define GEOMETRIX_SPHERE_HPP

#include <geometrix/primitive/sphere_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/construction_policy.hpp>

namespace geometrix {

//! \class sphere
//! \brief A class for specifying a sphere in N dimensions. (n-sphere).
template <typename Point>
class sphere
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Point                                                  point_type;
    typedef typename geometric_traits<point_type>::arithmetic_type radius_type;
    typedef typename geometric_traits<point_type>::dimension_type  dimension_type;

    sphere()
    {}

    sphere( const point_type& center, const radius_type& radius )
        : m_center( center )
        , m_radius( radius )
    {}
    
    ~sphere(){}

    const point_type&      get_center() const { return m_center; }	
    const radius_type& get_radius() const { return m_radius; }

private:

    //! Sphere defined by a central point and a radius.
    point_type     m_center;
    radius_type m_radius;

};

//! Specialize the coordinate accessors
#define GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( Sphere )                                 \
namespace geometrix {                                                                   \
template <>                                                                             \
struct sphere_access_traits< Sphere >                                                   \
{                                                                                       \
    typedef Sphere                                     sphere_type;                     \
    typedef geometric_traits< Sphere >::point_type     point_type;                      \
    typedef geometric_traits< Sphere >::radius_type    radius_type;                     \
    typedef geometric_traits< Sphere >::dimension_type dimension_type;                  \
                                                                                        \
    static point_type      get_center( const sphere_type& s ){ return s.get_center(); } \
    static radius_type     get_radius( const sphere_type& s ){ return s.get_radius(); } \
};                                                                                      \
}                                                                                       \
/***/

template <typename Point>
struct construction_policy< sphere< Point > >
{    
    typedef typename geometric_traits< sphere<Point> >::radius_type radius_type;
    static sphere< Point > construct( const Point& center, const radius_type& radius ) 
    {
        return sphere< Point >( center, radius );
    }
};

typedef sphere< point_double_2d > sphere_double_2d;
typedef sphere< point_double_3d > sphere_double_3d;

typedef sphere_double_2d          circle_double_2d;

}//namespace geometrix;

//! Define some default traits.
GEOMETRIX_DEFINE_SPHERE_TRAITS( point_double_2d, sphere< point_double_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( point_double_3d, sphere< point_double_3d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( point_float_2d, sphere< point_float_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( point_float_3d, sphere< point_float_3d > );

GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_double_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_double_3d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_float_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_float_3d > );

GEOMETRIX_DEFINE_SPHERE_TRAITS( point_int_2d, sphere< point_int_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( point_int_3d, sphere< point_int_3d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( point_int64_2d, sphere< point_int64_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( point_int64_3d, sphere< point_int64_3d > );

GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_int_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_int_3d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_int64_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< point_int64_3d > );

#endif //GEOMETRIX_SPHERE_HPP
