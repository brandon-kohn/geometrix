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
#include <geometrix/utility/construction_policy.hpp>

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
	typedef typename geometric_traits<vector_type>::arithmetic_type arithmetic_type;

    line()
    {}

    line( const point_type& u, const vector_type& v )
        : m_u( u )
        , m_v( normalize(v) )
		, m_n(left_normal( m_v ))
		, m_d(dot_product(m_n, as_vector(u)))
    {}
    
    ~line(){}

    const point_type&  get_u() const { return m_u; }
    const vector_type& get_v() const { return m_v; }
	const vector_type& get_n() const { return m_n; }
	const arithmetic_type& get_d() const { return m_d; }

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
                                                                               
    static const point_type& get_u( const line<Point,Vector>& l ){ return l.get_u(); }  
    static const vector_type& get_v( const line<Point,Vector>& l ){ return l.get_v(); } 
};                                                                             

template <typename Point, typename Vector>
struct construction_policy< line< Point, Vector > >
{    
    static line< Point, Vector > construct( const Point& u, const Vector& v ) 
    {
        return line< Point, Vector >( u, v );
    }
};

}//namespace geometrix;

#endif //GEOMETRIX_LINE_HPP
