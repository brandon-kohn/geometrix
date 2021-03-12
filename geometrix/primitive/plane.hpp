//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_PLANE_HPP
#define GEOMETRIX_PLANE_HPP

#include <geometrix/primitive/plane_traits.hpp>
#include <geometrix/primitive/point.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/algebra/algebra.hpp>

namespace geometrix {

//! \class plane
//! \brief A class for specifying a plane with two vectors in N dimensions.
template <typename Point>
class plane
{
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    using point_type = Point;
    using dimension_type = typename dimension_of<point_type>::type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;
    using dimensionless_type = typename dimensionless_type_of<point_type>::type;
    using vector_type = vector<dimensionless_type, dimension_type::value>;

    plane()
    {}

    plane( const point_type& a, const vector_type& u, const vector_type& v )
        : m_a( a )
        , m_u( normalize(u) )
        , m_v( normalize(v) )
        , m_n( normalize( cross_product( m_u, m_v ) ) )
        , m_d( scalar_projection( as_vector(m_a), m_n ) )
    {}

    plane( const point_type& a, const point_type& b, const point_type& c )
        : m_a( a )
        , m_u( normalize(b-a) )
        , m_v( normalize(c-a) )
        , m_n( normalize( cross_product( m_u, m_v ) ) )
        , m_d( scalar_projection( as_vector(m_a), m_n ) )
    {}

    ~plane(){}

    const point_type&    get_reference_point() const { return m_a; }
    const vector_type&   get_u() const { return m_u; }
    const vector_type&   get_parallel_vector() const { return m_v; }
    const vector_type&   get_normal_vector() const { return m_n; }
    arithmetic_type      get_distance_to_origin() const { return m_d; }

private:

    //! Plane defined by a + su + tv
    point_type  m_a;
    vector_type m_u;
    vector_type m_v;
    vector_type m_n;
    arithmetic_type m_d;

};

template <typename Point>
struct is_plane< plane<Point> > : boost::true_type{};
template <typename Point>
struct geometric_traits<plane<Point>>
{
    using hyperplane_dimension = dimension<3>;
    typedef void                                             is_plane;
    typedef Point                                            point_type;
    typedef plane<Point>                                     plane_type;
    typedef typename dimension_of<point_type>::type          dimension_type;
    using vector_type = typename plane<Point>::vector_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;
};

//! Specialize the coordinate accessors
template <typename Point>
struct plane_access_traits<plane<Point>>
{
    typedef Point  point_type;
    typedef typename plane<Point>::dimension_type dimension_type;
    using vector_type = typename plane<Point>::vector_type;
    using arithmetic_type = typename geometric_traits<point_type>::arithmetic_type;

    static const point_type&  get_reference_point( const plane<Point>& l ){ return l.get_reference_point(); }
    static const vector_type& get_u( const plane<Point>& l ){ return l.get_u(); }
    static const vector_type& get_parallel_vector( const plane<Point>& l ){ return l.get_parallel_vector(); }
    static arithmetic_type    get_distance_to_origin( const plane<Point>& l ){ return l.get_distance_to_origin(); }
};

template <typename Point>
struct construction_policy<plane<Point>>
{
    template <typename V1, typename V2>
    static plane<Point> construct( const Point& a, const V1& u, const V2& v )
    {
        return plane<Point>( a, u, v );
    }
};

namespace result_of {
    template <typename Point>
    struct make_plane
    {
    private:
        using length_t = typename arithmetic_type_of<Point>::type;
        using point_t = point<length_t, dimension_of<Point>::value>;
    public:
        using type = plane<point_t>;
    };
}//! namespace result_of;

template <typename Point, typename U, typename V>
inline typename result_of::make_plane<Point>::type make_plane(const Point& a, const U& u, const V& v)
{
    using plane_t = typename result_of::make_plane<Point>::type;
    return plane_t(a, u, v);
}

}//namespace geometrix;

#endif //GEOMETRIX_PLANE_HPP
