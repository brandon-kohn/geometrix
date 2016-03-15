//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ORIENTED_BOUNDING_BOX_HPP
#define GEOMETRIX_ORIENTED_BOUNDING_BOX_HPP

#include <geometrix/primitive/point.hpp>
#include <geometrix/tensor/vector.hpp>
#include <geometrix/algebra/algebra.hpp>

namespace geometrix {

//! \class oriented_bounding_box
//! \brief A class for specifying a oriented_bounding_box with two vectors in 2 dimensions.
template <typename Point, typename Vector>
class oriented_bounding_box
{
    BOOST_CLASS_REQUIRE( Vector, geometrix, VectorConcept );
    BOOST_CLASS_REQUIRE( Point, geometrix, PointConcept );

public:

    typedef Vector                                                  vector_type;
    typedef Point                                                   point_type;
    typedef typename dimension_of< vector_type >::type              dimension_type;
	typedef typename geometric_traits<vector_type>::arithmetic_type arithmetic_type;
    
    oriented_bounding_box( const point_type& a, const vector_type& u, const vector_type& v, const arithmetic_type& uHalfWidth, const arithmetic_type& vHalfWidth )
        : m_center( a )
		, m_u( u )
        , m_v( v )
		, m_uHalfWidth(uHalfWidth)
		, m_vHalfWidth(vHalfWidth)
    {}
	    
    ~oriented_bounding_box(){}

	const point_type&  get_center() const { return m_center; }
    const vector_type& get_u() const { return m_u; }
    const vector_type& get_v() const { return m_v; }
	const arithmetic_type& get_u_half_width() { return m_uHalfWidth; }
	const arithmetic_type& get_v_half_width() { return m_vHalfWidth; }
	point_type get_left_forward_point() const { return m_center + m_uHalfWidth * m_u + m_vHalfWidth * m_v; }
	point_type get_right_backward_point() const { return m_center - m_uHalfWidth * m_u - m_vHalfWidth * m_v; }
	point_type get_right_forward_point() const { return m_center + m_uHalfWidth * m_u - m_vHalfWidth * m_v; }
	point_type get_left_backward_point() const { return m_center - m_uHalfWidth * m_u + m_vHalfWidth * m_v; }

private:

    //! Oriented bounding box defined by a center, and two orthogonal unit vectors defining the orientation in R2.
    point_type  m_center;
	vector_type m_u;
    vector_type m_v;
	arithmetic_type m_uHalfWidth;//! The distance from center to border along u.
	arithmetic_type m_vHalfWidth;//! The distance from center to border along v.

};

typedef oriented_bounding_box<point_double_2d, vector_double_2d> obb_double_2d;

}//namespace geometrix;

#endif //GEOMETRIX_ORIENTED_BOUNDING_BOX_HPP
