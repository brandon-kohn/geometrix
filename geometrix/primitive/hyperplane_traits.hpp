//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_HYPERPLANE_TRAITS_HPP
#define GEOMETRIX_HYPERPLANE_TRAITS_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/primitive/line.hpp>
#include <geometrix/primitive/plane.hpp>

namespace geometrix {
//! \brief Tag to check if a type is a plane
template <typename Plane, typename EnableIf=void>
struct is_hyperplane : boost::false_type{};

template <typename Plane>
struct is_hyperplane<Plane, typename geometric_traits<typename std::decay<Plane>::type>::hyperplane_dimension> : boost::true_type{};

template <typename T>
constexpr bool is_hyperplane_v = is_hyperplane<T>::value;

//! \brief plane access traits struct
//! NOTE: must be specialized for user types.
template <typename Plane, typename EnableIf=void>
struct hyperplane_access_traits
{
    using plane_type = typename std::decay<Plane>::type;
    using vector_type = typename geometric_traits< plane_type >::vector_type;
    using point_type = typename geometric_traits< plane_type >::point_type;
    using dimension_type = typename geometric_traits< plane_type >::dimension_type;
    using arithmetic_type = typename arithmetic_type_of<point_type>::type;
	using dimensionless_type = typename dimensionless_type_of<vector_type>::type;
	using unit_vector_type = vector<dimensionless_type, dimension_type::value>;

    static vector_type      get_normal_vector( const plane_type& p ){ return p.get_normal_vector(); }
    static unit_vector_type get_unit_normal_vector( const plane_type& p ){ return p.get_unit_normal_vector(); }
    static arithmetic_type  get_distance_to_origin( const plane_type& p ) { return p.get_distance_to_origin(); }
};

#ifdef __cpp_concepts

namespace concepts {
	template <typename T>
	concept Hyperplane = is_hyperplane_v<T> && requires( T p ) {
		{ hyperplane_access_traits<T>::get_normal_vector( p ) } -> std::convertible_to<typename geometric_traits<T>::vector_type>;
		{ hyperplane_access_traits<T>::get_unit_normal_vector( p ) } -> std::convertible_to<typename geometric_traits<T>::unit_vector_type>;
		{ hyperplane_access_traits<T>::get_distance_to_origin( p ) } -> std::convertible_to<typename geometric_traits<T>::arithmetic_type>;
	};
} // namespace concepts
#endif

namespace result_of{
    
    template <typename T, typename EnableIf=void>
    struct make_hyperplane{};
    
    template <typename T>
    struct make_hyperplane<T, typename std::enable_if<dimension_of<T>::value == 2>::type>
    {
    private:
        using point_t = typename point_type_of<T>::type;
    public:
    
        using type = line<point_t>;
        
    };
    
    template <typename T>
    struct make_hyperplane<T, typename std::enable_if<dimension_of<T>::value == 3>::type>
    {
    private:
        using point_t = typename point_type_of<T>::type;
    public:
    
        using type = plane<point_t>;
        
    };
    
}//! namespace result_of;

template <typename T, typename std::enable_if<dimension_of<T>::value == 2, int>::type = 0>
inline typename result_of::make_hyperplane<T>::type make_hyperplane(const T& simplex)
{
    static_assert(is_segment<T>::value, "segment type expected");
    return make_line(simplex);
}

template <typename T, typename std::enable_if<dimension_of<T>::value == 3, int>::type = 0>
inline typename result_of::make_hyperplane<T>::type make_hyperplane(const T& simplex)
{
    //static_assert(is_segment<T>::value, "triangle type expected");
    return make_plane(simplex);
}
}//namespace geometrix;

#endif //GEOMETRIX_HYPERPLANE_TRAITS_HPP
