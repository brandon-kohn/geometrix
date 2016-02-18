//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SPHERE_TRAITS_HPP
#define GEOMETRIX_SPHERE_TRAITS_HPP

#include <geometrix/primitive/point_traits.hpp>

namespace geometrix {
//! \brief Tag to check if a type is a sphere
template <typename Sphere, typename EnableIf=void>
struct is_sphere : boost::false_type{};

//! \brief Macro for defining a sphere type traits.
#define GEOMETRIX_DEFINE_SPHERE_TRAITS( Point, Sphere )                      \
namespace geometrix {                                                        \
template <> struct is_sphere< Sphere > : boost::true_type{};                 \
template <>                                                                  \
struct geometric_traits< Sphere >                                            \
{                                                                            \
    typedef Point                                           point_type;      \
    typedef Sphere                                          sphere_type;     \
    typedef geometric_traits< point_type >::dimension_type  dimension_type;  \
    typedef geometric_traits< point_type >::arithmetic_type radius_type;     \
};                                                                           \
}                                                                            \
/***/

//! \brief sphere access traits struct
//! NOTE: must be specialized for user types.
template <typename Sphere>
struct sphere_access_traits
{
    BOOST_MPL_ASSERT_MSG( 
          ( false )
        , SPHERE_ACCESS_TRAITS_NOT_DEFINED
        , (Sphere) );	
 
    typedef Sphere                                                   sphere_type;                                           
    typedef typename geometric_traits< sphere_type >::point_type     point_type;   
    typedef typename geometric_traits< sphere_type >::radius_type    radius_type;
    typedef typename geometric_traits< sphere_type >::dimension_type dimension_type; 
                         
    static point_type  get_center( const sphere_type& s ){ return s.get_center(); }        	
    static radius_type get_radius( const sphere_type& s ){ return s.get_radius(); }

};

template <typename Sphere>
inline typename sphere_access_traits<typename remove_const_ref<Sphere>::type>::point_type get_center( const Sphere& s )
{
	return sphere_access_traits<typename remove_const_ref<Sphere>::type>::get_center( s );
}

template <typename Sphere>
inline typename sphere_access_traits<typename remove_const_ref<Sphere>::type>::radius_type get_radius( const Sphere& s )
{
	return sphere_access_traits<typename remove_const_ref<Sphere>::type>::get_radius( s );
}

template <typename Sequence, typename ReferenceFrame>
struct is_sphere< reference_frame_adaptor< Sequence, ReferenceFrame > > : is_sphere< Sequence >{};

}//namespace geometrix;

#endif //GEOMETRIX_SPHERE_TRAITS_HPP
