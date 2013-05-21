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
#include <geometrix/utility/construction_policy.hpp>

namespace geometrix {

//! \class sphere
//! \brief A class for specifying a sphere in N dimensions. (n-sphere).
template <typename Vector>
class sphere
{
    BOOST_CLASS_REQUIRE( Vector, geometrix, VectorConcept );

public:

    typedef Vector                                               vector_type;
    typedef typename geometric_traits<vector_type>::coordinate_type coordinate_type;
    typedef typename geometric_traits<vector_type>::dimension_type  dimension_type;

    sphere()
    {}

    sphere( const vector_type& center, const coordinate_type& radius )
        : m_center( center )
        , m_radius( radius )
    {}
    
    ~sphere(){}

    const vector_type&     get_center() const { return m_center; }	
    const coordinate_type& get_radius() const { return m_radius; }

private:

    //! Sphere defined by a central point and a radius.
    vector_type     m_center;
    coordinate_type m_radius;

};

//! Specialize the coordinate accessors
#define GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( Sphere )                                       \
namespace geometrix {                                                                         \
template <>                                                                                   \
struct sphere_access_traits< Sphere >                                                         \
{                                                                                             \
    typedef Sphere                                            sphere_type;                    \
    typedef typename sphere_traits< Sphere >::vector_type     vector_type;                    \
    typedef typename sphere_traits< Sphere >::coordinate_type coordinate_type;                \
    typedef typename sphere_traits< Sphere >::dimension_type  dimension_type;                 \
                                                                                              \
    static const vector_type&     get_center( const sphere_type& s ){ return s.get_center(); }\
    static const coordinate_type& get_radius( const sphere_type& s ){ return s.get_radius(); }\
};                                                                                            \
}                                                                                             \
/***/

template <typename Vector>
struct construction_policy< sphere< Vector > >
{    
    typedef typename sphere_traits< Sphere >::coordinate_type coordinate_type;
    static sphere< Vector > construct( const Vector& center, const coordinate_type& radius ) 
    {
        return sphere< Vector >( center, radius );
    }
};

}//namespace geometrix;

//! Define some default traits.
GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_double_2d, sphere< vector_double_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_double_3d, sphere< vector_double_3d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_float_2d, sphere< vector_float_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_float_3d, sphere< vector_float_3d > );

GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_double_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_double_3d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_float_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_float_3d > );

GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_int_2d, sphere< vector_int_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_int_3d, sphere< vector_int_3d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_int64_2d, sphere< vector_int64_2d > );
GEOMETRIX_DEFINE_SPHERE_TRAITS( vector_int64_3d, sphere< vector_int64_3d > );

GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int_3d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int64_2d > );
GEOMETRIX_DEFINE_SPHERE_ACCESS_TRAITS( sphere< vector_int64_3d > );

#endif //GEOMETRIX_SPHERE_HPP
