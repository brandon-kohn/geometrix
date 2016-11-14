//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_PERP_HPP
#define GEOMETRIX_VECTOR_MATH_PERP_HPP

#include <geometrix/tensor/vector.hpp>
#include <geometrix/algorithm/orientation_enum.hpp>
#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {      
		template <typename Vector, typename EnableIf = void>
		struct perp
		{
			using type = Vector;// vector<typename geometric_traits<Vector>::dimensionless_type, dimension_of<Vector>::value>;
		};

		template <typename Vector, typename EnableIf = void>
		struct left_normal
		{
			using type = Vector;// vector<typename geometric_traits<Vector>::dimensionless_type, dimension_of<Vector>::value>;
		};

		template <typename Vector, typename EnableIf = void>
		struct right_normal
		{
			using type = Vector;// vector<typename geometric_traits<Vector>::dimensionless_type, dimension_of<Vector>::value>;
		};
	}//namespace result_of;

    //! Get the left-normal perpendicular of a 2d vector.
    template <typename Vector>
    inline typename result_of::perp<Vector>::type perp( const Vector& u )
    {
        BOOST_CONCEPT_ASSERT(( Vector2DConcept<Vector> ));
        return construct<typename result_of::perp<Vector>::type>( get<1>( -u ), get<0>( u ) );
    }

	//! Get the left normal perpendicular of a 2d vector.
	template <typename Vector>
	inline typename result_of::left_normal<Vector>::type left_normal( const Vector& u )
	{
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector>) );
		return construct<typename result_of::left_normal<Vector>::type>( get<1>( -u ), get<0>( u ) );
	}

	//! Get the right normal perpendicular of a 2d vector.
	template <typename Vector>
	inline typename result_of::right_normal<Vector>::type right_normal( const Vector& u )
	{
		BOOST_CONCEPT_ASSERT( (Vector2DConcept<Vector>) );
		return construct<typename result_of::right_normal<Vector>::type>( get<1>( u ), get<0>( -u ) );
	}
	
	namespace detail {
		template <orientation_type Orientation, typename Vector>
		struct oriented_normal_helper {};

		template<typename Vector>
		struct oriented_normal_helper<oriented_right, Vector>
		{
			using result_type = typename result_of::right_normal<Vector>::type;

			static result_type apply(const Vector& u)
			{
				return right_normal(u);
			}
		};
		
		template<typename Vector>
		struct oriented_normal_helper<oriented_left, Vector>
		{
			using result_type = typename result_of::left_normal<Vector>::type;

			static result_type apply(const Vector& u)
			{
				return left_normal(u);
			}
		};
	}//! namespace detail;

	namespace result_of {
		template <orientation_type Orientation, typename Vector>
		struct oriented_normal
		{
			using type = typename geometrix::detail::oriented_normal_helper<Orientation, Vector>::result_type;
		};
	}//! namespace result_of

	template <orientation_type Orientation, typename Vector>
	inline typename result_of::oriented_normal<Orientation, Vector>::type oriented_normal(const Vector& u)
	{
		return detail::oriented_normal_helper<Orientation, Vector>::apply(u);
	}

}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_PERP_HPP
