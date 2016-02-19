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

    typedef Vector                                                  vector_type;
    typedef Point                                                   point_type;
    typedef typename dimension_of< vector_type >::type              dimension_type;
	typedef typename geometric_traits<vector_type>::arithmetic_type arithmetic_type;
    plane()
    {}

    plane( const point_type& a, const vector_type& u, const vector_type& v )
        : m_a( a )
		, m_u( u )
        , m_v( v )
		, m_n( normalize<vector_type>( cross_product( m_u, m_v ) ) )
		, m_d( dot_product(m_n, as_vector(m_a)) )
    {}

	plane( const point_type& a, const point_type& b, const point_type& c )
		: m_a( a )
		, m_u( b-a )
		, m_v( c-a )
		, m_n( normalize<vector_type>( cross_product( m_u, m_v ) ) )
		, m_d( dot_product( m_n, as_vector( m_a ) ) )
	{}
    
    ~plane(){}

	const point_type&  get_a() const { return m_a; }
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
    
	static const point_type& get_a( const plane<Point, Vector>& l ){ return l.get_a(); }
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

}//namespace geometrix;

#endif //GEOMETRIX_PLANE_HPP
