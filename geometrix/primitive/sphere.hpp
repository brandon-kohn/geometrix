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
template <std::size_t N, typename Point>
class sphere
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Point                                                  point_type;
    typedef typename geometric_traits<point_type>::arithmetic_type radius_type;
    typedef dimension<N>                                           dimension_type;

    sphere()
    {}

    sphere( const point_type& center, const radius_type& radius )
        : m_center( center )
        , m_radius( radius )
    {}
    
    ~sphere(){}

    const point_type&  get_center() const { return m_center; }	
	template <typename T>
	void set_center(const T& p) 
	{
		BOOST_CONCEPT_ASSERT((PointConcept<T>));
		assign(m_center, p); 
	}
    
	const radius_type& get_radius() const { return m_radius; }
	void set_radius(const radius_type& r) { m_radius = r; }

private:

    //! Sphere defined by a central point and a radius.
    point_type  m_center;
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

template <std::size_t N, typename Point>
struct construction_policy< sphere< N, Point > >
{    
    typedef typename geometric_traits< sphere<N, Point> >::radius_type radius_type;
    static sphere< N, Point > construct( const Point& center, const radius_type& radius ) 
    {
        return sphere< N, Point >( center, radius );
    }
};

template <std::size_t N, typename Point, typename Radius>
inline sphere<N, Point> make_sphere( const Point& center, const Radius& radius )
{
	return sphere<N, Point>( center, radius );
}

typedef sphere< 2, point_double_2d > sphere2_double_2d;
typedef sphere< 2, point_double_3d > sphere2_double_3d;
typedef sphere< 3, point_double_3d > sphere3_double_3d;

typedef sphere2_double_2d          circle_double_2d;
typedef sphere2_double_3d          circle_double_3d;

}//namespace geometrix;

//! Define some default traits.
GEOMETRIX_DEFINE_SPHERE_TRAITS(2, point_double_2d, sphere2_double_2d);
GEOMETRIX_DEFINE_SPHERE_TRAITS(2, point_double_2d, sphere2_double_3d);
GEOMETRIX_DEFINE_SPHERE_TRAITS(3, point_double_3d, sphere3_double_3d);

GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS(sphere2_double_2d);
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS(sphere2_double_3d);
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS(sphere3_double_3d);


#endif //GEOMETRIX_SPHERE_HPP
