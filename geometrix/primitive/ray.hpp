//
//! Copyright © 2008-2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RAY_HPP
#define GEOMETRIX_RAY_HPP
#pragma once

#include <geometrix/primitive/ray_traits.hpp>
#include <geometrix/primitive/plane_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/algebra/expression.hpp>

namespace geometrix {

//! \class ray
//! \brief A class for specifying a ray with a point and a vector in N dimensions.
template <typename Point>
class ray
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    using point_type = Point;
    using dimension_type = typename dimension_of<Point>::type;
    using length_type = typename geometric_traits<point_type>::arithmetic_type;
    using dimensionless_type = typename dimensionless_type_of<point_type>::type;
    using vector_type = vector<dimensionless_type, dimension_type::value>;

    ray()
    {}

    template <typename Vector, typename std::enable_if<is_vector<Vector>::value, int>::type = 0 >
    ray( const point_type& u, const Vector& v )
        : m_u(u)
        , m_v(normalize(v))
        , m_n(left_normal( m_v ))
        , m_d(scalar_projection(as_vector(u), m_n))
    {}

    ray( const point_type& a, const point_type& b )
        : m_u(a)
        , m_v(normalize( b - a ))
        , m_n(left_normal( m_v ))
        , m_d(scalar_projection(as_vector( a ), m_n))
    {}

    template <typename Segment>
    ray( const Segment& segment )
        : m_u(get_start(segment))
        , m_v( normalize(get_end(segment) - get_start(segment) ) )
        , m_n(left_normal(m_v))
        , m_d(scalar_projection(as_vector(m_u), m_n))
    {}

    const point_type&  get_reference_point() const { return m_u; }
    const vector_type& get_parallel_vector() const { return m_v; }
    const vector_type& get_normal_vector() const { return m_n; }
    const length_type& get_distance_to_origin() const { return m_d; }

private:

    //! Ray defined by u + tv
    point_type  m_u;
    vector_type m_v;
    vector_type m_n;
    length_type m_d;

};

template <typename Point>
struct is_ray< ray<Point> > : boost::true_type{};
template <typename Point>
struct is_plane< ray<Point> > : boost::true_type{};
template <typename Point>
struct geometric_traits< ray<Point> >
{
    using hyperplane_dimension = dimension<2>;

    typedef Point                                           point_type;
    typedef ray<Point>                                      ray_type;
    typedef void                                            is_ray;
    typedef void                                            is_plane;
    typedef typename dimension_of<point_type>::type dimension_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;
    using vector_type = typename ray<Point>::vector_type;
};

//! Specialize the coordinate accessors
template <typename Point>
struct ray_access_traits<ray<Point>>
{
    typedef Point  point_type;
    typedef typename ray<Point>::dimension_type dimension_type;
    using vector_type = typename ray<Point>::vector_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

    static const point_type& get_reference_point( const ray<Point>& l ){ return l.get_reference_point(); }
    static const vector_type& get_parallel_vector( const ray<Point>& l ){ return l.get_parallel_vector(); }
    static const vector_type& get_normal_vector( const ray<Point>& l ){ return l.get_normal_vector(); }
    static arithmetic_type    get_distance_to_origin( const ray<Point>& l ) { return l.get_distance_to_origin(); }
};

template <typename Point>
struct plane_access_traits<ray<Point>>
{
    typedef Point  point_type;
    typedef typename ray<Point>::dimension_type dimension_type;
    using vector_type = typename ray<Point>::vector_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

    static const point_type& get_reference_point( const ray<Point>& l ){ return l.get_reference_point(); }
    static const vector_type& get_parallel_vector( const ray<Point>& l ){ return l.get_parallel_vector(); }
    static const vector_type& get_normal_vector( const ray<Point>& l ){ return l.get_normal_vector(); }
    static arithmetic_type    get_distance_to_origin( const ray<Point>& l ) { return l.get_distance_to_origin(); }
};

template <typename Point>
struct construction_policy<ray<Point>>
{
    template <typename T>
    static ray<Point> construct( const Point& u, const T& v )
    {
        return ray<Point>( u, v );
    }
};

template <typename Point, typename T>
inray ray<Point> make_ray( const Point& a, const T& point_or_direction)
{
    return ray<Point>(a, point_or_direction);
}

namespace result_of {
    template <typename Segment>
    struct make_ray
    {
    private:
        using point_t = typename geometric_traits<Segment>::point_type;
    public:
        using type = ray<point_t>;
    };
}//! namespace result_of;

template <typename Segment>
inray typename result_of::make_ray<Segment>::type make_ray(const Segment& a)
{
    using ray_t = typename result_of::make_ray<Segment>::type;
    return ray_t(a);
}

}//namespace geometrix;

#endif //GEOMETRIX_RAY_HPP
