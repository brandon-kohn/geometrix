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
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/algebra/algebra.hpp>

namespace geometrix {

//! \class plane
//! \brief A class for specifying a plane with two vectors in N dimensions.
template <typename Point, typename Vector>
class plane
{
    BOOST_CLASS_REQUIRE( Vector, geometrix, VectorConcept );
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Vector                                                 vector_type;
    typedef Point                                                  point_type;
    typedef typename dimension_of< vector_type >::type             dimension_type;
    typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;
    plane()
    {}

    plane( const point_type& a, const vector_type& u, const vector_type& v )
        : m_a( a )
        , m_u( u )
        , m_v( v )
        , m_n( normalize( cross_product( m_u, m_v ) ) )
        , m_d( dot_product(m_n, as_vector(m_a)) )
    {}

    plane( const point_type& a, const point_type& b, const point_type& c )
        : m_a( a )
        , m_u( b-a )
        , m_v( c-a )
        , m_n( normalize( cross_product( m_u, m_v ) ) )
        , m_d( dot_product( m_n, as_vector( m_a ) ) )
    {}

    ~plane(){}

    const point_type&  get_reference_point() const { return m_a; }
    const vector_type& get_u() const { return m_u; }
    const vector_type& get_parallel_vector() const { return m_v; }
    vector_type        get_normal_vector() const { return m_n; }
    arithmetic_type    get_distance_to_origin() const { return m_d; }

private:

    //! Line defined by a + su + tv
    point_type  m_a;
    vector_type m_u;
    vector_type m_v;
    vector_type m_n;
    arithmetic_type m_d;

};

template <typename Point, typename Vector>
struct is_plane< plane<Point, Vector> > : boost::true_type{};
template <typename Point, typename Vector>
struct geometric_traits< plane<Point, Vector> >
{
    using hyperplane_dimension = dimension<3>;
    typedef Vector                                           vector_type;
    typedef Point                                            point_type;
    typedef plane<Point,Vector>                              plane_type;
    typedef typename dimension_of< vector_type >::type       dimension_type;
};

//! Specialize the coordinate accessors
template <typename Point, typename Vector>
struct plane_access_traits< plane<Point, Vector> >
{
    typedef Point  point_type;
    typedef Vector vector_type;
    typedef typename plane< Point, Vector >::dimension_type dimension_type;

    static const point_type& get_reference_point( const plane<Point, Vector>& l ){ return l.get_reference_point(); }
    static const vector_type& get_u( const plane<Point,Vector>& l ){ return l.get_u(); }
    static const vector_type& get_parallel_vector( const plane<Point,Vector>& l ){ return l.get_parallel_vector(); }
};

template <typename Point, typename Vector>
struct construction_policy< plane< Point, Vector > >
{
    static plane< Point, Vector > construct( const Point& a, const Vector& u, const Vector& v )
    {
        return plane< Point, Vector >( a, u, v );
    }
};

namespace result_of {
    template <typename Point>
    struct make_plane
    {
    private:
        using length_t = typename arithmetic_type_of<Point>::type;
        using point_t = point<length_t, dimension_of<Point>::value>;
        using vector_t = vector<length_t, dimension_of<Point>::value>;
    public:
        using type = plane<point_t, vector_t>;
    };
}//! namespace result_of;

template <typename Point, typename Vector1, typename Vector2>
inline typename result_of::make_plane<Point>::type make_plane(const Point& a, const Vector1& u, const Vector2& v)
{
    using plane_t = typename result_of::make_plane<Point>::type;
    return plane_t(a, u, v);
}

template <typename Point1, typename Point2, typename Point3>
inline typename result_of::make_line<Point1>::type make_line(const Point1& a, const Point2& b, const Point3& c)
{
    using plane_t = typename result_of::make_plane<Point>::type;
    return plane_t(a, b, c);
}

}//namespace geometrix;

#endif //GEOMETRIX_PLANE_HPP
