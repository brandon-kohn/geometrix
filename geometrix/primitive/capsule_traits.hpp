//
//! Copyright © 2018
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CAPSULE_TRAITS_HPP
#define GEOMETRIX_CAPSULE_TRAITS_HPP

#include <geometrix/primitive/segment_traits.hpp>

namespace geometrix {
//! \brief Tag to check if a type is a capsule
template <typename Capsule, typename EnableIf=void>
struct is_capsule : boost::false_type{};

//! \brief capsule access traits struct
//! NOTE: must be specialized for user types.
template <typename Capsule, typename EnableIf=void>
struct capsule_access_traits
{
    BOOST_MPL_ASSERT_MSG(
          ( false )
        , CAPSULE_ACCESS_TRAITS_NOT_DEFINED
        , (Capsule) );

    typedef Capsule                                                  capsule_type;
    typedef typename geometric_traits<capsule_type>::segment_type    segment_type;
    typedef typename geometric_traits<capsule_type>::point_type      point_type;
    typedef typename geometric_traits<capsule_type>::length_type     length_type;
    typedef typename geometric_traits<capsule_type>::dimension_type  dimension_type;

    static const segment_type&     get_segment( const capsule_type& c ){ return c.get_segment(); }
    static const length_type&      get_radius( const capsule_type& c ){ return c.get_radius(); }
};

template <typename T>
constexpr bool is_capsule_v = is_capsule<T>::value;

#ifdef __cpp_concepts
namespace concepts {
	template <typename T>
	concept Capsule = is_capsule_v<T> && requires( T c ) 
    {
		{ capsule_access_traits<T>::get_segment( c ) } -> std::convertible_to<typename geometric_traits<T>::segment_type>;
		{ capsule_access_traits<T>::get_radius( c ) } -> std::convertible_to<typename geometric_traits<T>::length_type>;
	};
} // namespace concepts
#endif

}//namespace geometrix;

//! \brief Macro for defining a capsule type traits.
#define GEOMETRIX_DEFINE_CAPSULE_TRAITS( Point, Capsule )                 \
namespace geometrix {                                                     \
template <> is_capsule< Capsule > : boost::true_type{};                   \
template <>                                                               \
struct geometric_traits< Capsule >                                        \
{                                                                         \
    typedef segment<Point>                               segment_type;    \
    typedef typename arithmetic_type_of<Point>::type     length_type;     \
    typedef Point                                        point_type;      \
    typedef Capsule                                      capsule_type;    \
    typedef geometric_traits<point_type>::dimension_type dimension_type;  \
};                                                                        \
}                                                                         \
/***/

#endif //GEOMETRIX_CAPSULE_TRAITS_HPP
