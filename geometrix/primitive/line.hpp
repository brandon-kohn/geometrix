//
//! Copyright � 2008-2021
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINE_HPP
#define GEOMETRIX_LINE_HPP
#pragma once

#include <geometrix/primitive/line_traits.hpp>
#include <geometrix/primitive/plane_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/algebra/expression.hpp>

namespace geometrix {

//! \class line
//! \brief A class for specifying a line with a point and a vector in N dimensions.
template <typename Point>
class line
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    using point_type = Point;
    using dimension_type = typename dimension_of<Point>::type;
    using length_type = typename geometric_traits<point_type>::arithmetic_type;
    using dimensionless_type = typename dimensionless_type_of<point_type>::type;
    using vector_type = vector<dimensionless_type, dimension_type::value>;

    line()
    {}

    template <typename Vector, typename std::enable_if<is_vector<Vector>::value, int>::type = 0 >
    line( const point_type& u, const Vector& v )
        : m_u(u)
        , m_v(normalize(v))
        , m_n(left_normal( m_v ))
        , m_d(scalar_projection(as_vector(u), m_n))
    {}

    line( const point_type& a, const point_type& b )
        : m_u(a)
        , m_v(normalize( b - a ))
        , m_n(left_normal( m_v ))
        , m_d(scalar_projection(as_vector( a ), m_n))
    {}

    template <typename Segment>
    line( const Segment& segment )
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

    //! Line defined by u + tv
    point_type  m_u;
    vector_type m_v;
    vector_type m_n;
    length_type m_d;

};

template <typename Point>
struct is_line< line<Point> > : boost::true_type{};
template <typename Point>
struct is_plane< line<Point> > : boost::true_type{};
template <typename Point>
struct geometric_traits< line<Point> >
{
    using hyperplane_dimension = dimension<2>;

    typedef Point                                   point_type;
    typedef line<Point>                             line_type;
    typedef void                                            is_line;
    typedef void                                            is_plane;
    typedef typename dimension_of<point_type>::type dimension_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;
    using vector_type = typename line<Point>::vector_type;
};

//! Specialize the coordinate accessors
template <typename Point>
struct line_access_traits<line<Point>>
{
    typedef Point  point_type;
    typedef typename line<Point>::dimension_type dimension_type;
    using vector_type = typename line<Point>::vector_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

    static const point_type& get_reference_point( const line<Point>& l ){ return l.get_reference_point(); }
    static const vector_type& get_parallel_vector( const line<Point>& l ){ return l.get_parallel_vector(); }
    static const vector_type& get_normal_vector( const line<Point>& l ){ return l.get_normal_vector(); }
    static arithmetic_type    get_distance_to_origin( const line<Point>& l ) { return l.get_distance_to_origin(); }
};

template <typename Point>
struct plane_access_traits<line<Point>>
{
    typedef Point  point_type;
    typedef typename line<Point>::dimension_type dimension_type;
    using vector_type = typename line<Point>::vector_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

    static const point_type& get_reference_point( const line<Point>& l ){ return l.get_reference_point(); }
    static const vector_type& get_parallel_vector( const line<Point>& l ){ return l.get_parallel_vector(); }
    static const vector_type& get_normal_vector( const line<Point>& l ){ return l.get_normal_vector(); }
    static arithmetic_type    get_distance_to_origin( const line<Point>& l ) { return l.get_distance_to_origin(); }
};

template <typename Point>
struct construction_policy<line<Point>>
{
    template <typename T>
    static line<Point> construct( const Point& u, const T& v )
    {
        return line<Point>( u, v );
    }
};

template <typename Point, typename T>
inline line<Point> make_line( const Point& a, const T& point_or_direction)
{
    return line<Point>(a, point_or_direction);
}

namespace result_of {
    template <typename Segment>
    struct make_line
    {
    private:
        using point_t = typename geometric_traits<Segment>::point_type;
    public:
        using type = line<point_t>;
    };
}//! namespace result_of;

template <typename Segment>
inline typename result_of::make_line<Segment>::type make_line(const Segment& a)
{
    using line_t = typename result_of::make_line<Segment>::type;
    return line_t(a);
}

}//namespace geometrix;

#endif //GEOMETRIX_LINE_HPP
