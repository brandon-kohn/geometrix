//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINE_HPP
#define GEOMETRIX_LINE_HPP

#include <geometrix/primitive/line_traits.hpp>
#include <geometrix/primitive/segment_traits.hpp>
#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/algebra/expression.hpp>

namespace geometrix {

//! \class line
//! \brief A class for specifying a line with two vectors in N dimensions.
template <typename Point, typename Vector>
class line
{
    BOOST_CLASS_REQUIRE( Vector, geometrix, VectorConcept );
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Vector                                           vector_type;
    typedef Point                                            point_type;
    typedef typename dimension_of< vector_type >::type       dimension_type;
    typedef typename geometric_traits<point_type>::arithmetic_type arithmetic_type;

    line()
    {}

    line( const point_type& u, const vector_type& v )
        : m_u( u )
        , m_v( normalize(v) )
        , m_n(left_normal( m_v ))
        , m_d(dot_product(m_n, as_vector(u)))
    {}

    line( const point_type& a, const point_type& b )
        : m_u( a )
        , m_v( normalize( b - a ) )
        , m_n( left_normal( m_v ) )
        , m_d( dot_product( m_n, as_vector( a ) ) )
    {}

    template <typename Segment>
    line( const Segment& segment )
        : m_u(get_start(segment))
        , m_v( normalize(get_end(segment) - get_start(segment) ) )
        , m_n( left_normal(m_v))
        , m_d( dot_product(m_n, as_vector(m_u)))
    {}

    const point_type&  get_reference_point() const { return m_u; }
    const vector_type& get_parallel_vector() const { return m_v; }
    const vector_type& get_normal_vector() const { return m_n; }
    const arithmetic_type& get_distance_to_origin() const { return m_d; }

private:

    //! Line defined by u + tv
    point_type  m_u;
    vector_type m_v;
    vector_type m_n;
    arithmetic_type m_d;

};

template <typename Point, typename Vector>
struct is_line< line<Point, Vector> > : boost::true_type{};
template <typename Point, typename Vector>
struct geometric_traits< line<Point, Vector> >
{
    using hyperplane_dimension = dimension<2>;
    
    typedef Vector                                           vector_type;
    typedef Point                                            point_type;
    typedef line<Point,Vector>                               line_type;
    typedef typename dimension_of< vector_type >::type       dimension_type;
};

//! Specialize the coordinate accessors
template <typename Point, typename Vector>
struct line_access_traits< line<Point, Vector> >
{
    typedef Point  point_type;
    typedef Vector vector_type;
    typedef typename line< Point, Vector >::dimension_type dimension_type;

    static const point_type& get_reference_point( const line<Point,Vector>& l ){ return l.get_reference_point(); }
    static const vector_type& get_parallel_vector( const line<Point,Vector>& l ){ return l.get_parallel_vector(); }
    static const vector_type& get_normal_vector( const line<Point,Vector>& l ){ return l.get_normal_vector(); }
};

template <typename Point, typename Vector>
struct construction_policy< line< Point, Vector > >
{
    static line< Point, Vector > construct( const Point& u, const Vector& v )
    {
        return line< Point, Vector >( u, v );
    }
};

template <typename Point, typename Vector>
inline line<Point, Vector> make_line( const Point& a, const Vector& direction )
{
    return line<Point, Vector>( a, direction );
}

namespace result_of {
    template <typename Segment>
    struct make_line
    {
    private:
        using point_t = typename geometric_traits<Segment>::point_type;
        using vector_t = vector<typename arithmetic_type_of<point_t>::type, dimension_of<point_t>::value>;
    public:
        using type = line<point_t, vector_t>;
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
