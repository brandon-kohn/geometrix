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

#include <geometrix/tensor/geometric_traits.hpp>

namespace geometrix {
//! \brief Tag to check if a type is a sphere
template <typename Sphere, typename EnableIf=void>
struct is_sphere : boost::false_type{};

//! \brief sphere traits struct. 
template <typename Sphere, typename EnableIf=void>
struct sphere_traits
{
    BOOST_MPL_ASSERT_MSG( 
          ( false )
        , SPHERE_TRAITS_NOT_DEFINED
        , (Sphere) );	
};

//! \brief Macro for defining a sphere type traits.
#define GEOMETRIX_DEFINE_SPHERE_TRAITS( Vector, Sphere )                  \
namespace geometrix {                                                     \
template <> is_sphere< Sphere > : boost::true_type{};                     \
template <>                                                               \
struct sphere_traits< Sphere >                                            \
{                                                                         \
    typedef Vector                                        vector_type;    \
    typedef Sphere                                        sphere_type;    \
    typedef geometric_traits< vector_type >::dimension_type  dimension_type; \
    typedef geometric_traits< vector_type >::coordinate_type coordinate_type;\
};                                                                        \
}                                                                         \
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
 
    typedef Sphere                                                 sphere_type;                                           
    typedef typename sphere_traits< sphere_type >::vector_type     vector_type;   
    typedef typename sphere_traits< sphere_type >::coordinate_type coordinate_type;
    typedef typename sphere_traits< sphere_type >::dimension_type  dimension_type; 
                         
    static const vector_type&      get_center( const sphere_type& s ){ return s.get_center(); }        	
    static const coordinate_type&  get_radius( const sphere_type& s ){ return s.get_t(); }

};

template <typename Sequence, typename ReferenceFrame>
struct sphere_traits< reference_frame_adaptor< Sequence, ReferenceFrame > > : public sphere_traits< Sequence >{};

template <typename Sequence, typename ReferenceFrame>
struct is_sphere< reference_frame_adaptor< Sequence, ReferenceFrame > > : is_sphere< Sequence >{};

}//namespace geometrix;

#endif //GEOMETRIX_SPHERE_TRAITS_HPP
