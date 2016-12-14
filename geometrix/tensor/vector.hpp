//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_HPP
#define GEOMETRIX_VECTOR_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/tensor/numeric_sequence.hpp>
#include <geometrix/tensor/detail/vector_generator.hpp>
#include <geometrix/space/neutral_reference_frame.hpp>
#include <geometrix/tensor/index_operator_vector_access_policy.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

namespace geometrix {
    
//! Concrete geometrix::vector<T,D> Types for some of the more common coordinate types.
typedef vector<float, 2>       vector_float_2d;
typedef vector<float, 3>       vector_float_3d;
typedef vector<float, 4>       vector_float_4d;
typedef vector<double, 2>      vector_double_2d;
typedef vector<double, 3>      vector_double_3d;
typedef vector<double, 4>      vector_double_4d;

typedef vector<int, 2>         vector_int_2d;
typedef vector<int, 3>         vector_int_3d;

}//namespace geometrix;

namespace boost {
	namespace mpl {

		template<typename>
		struct sequence_tag;

		template<typename T, std::size_t D>
		struct sequence_tag<geometrix::vector<T, D>>
		{
			typedef fusion::fusion_sequence_tag type;
		};

		template<typename T, std::size_t D>
		struct sequence_tag<const geometrix::vector<T, D>>
		{
			typedef fusion::fusion_sequence_tag type;
		};
	}
}//! namespace boost::mpl;
/***/

namespace geometrix {
	template<typename T, std::size_t D>
	struct tensor_traits< geometrix::vector<T, D> >
		: index_operator_vector_access_policy<geometrix::vector<T, D>>
	{
		typedef index_operator_vector_access_policy<geometrix::vector<T, D>> access_policy;
		typedef boost::mpl::int_<1> tensor_order;
		typedef void GEOMETRIX_TENSOR_ORDER(1);
		typedef void is_tensor;
		typedef void make_fusion_sequence;
	};

	template<typename T, std::size_t D>
	struct geometric_traits< geometrix::vector<T, D> >
	{
		typedef geometrix::vector<T, D>               vector_type;
		typedef void                                  is_vector;
		typedef neutral_reference_frame<D>            reference_frame;
		typedef void                                  is_coordinate_sequence;
		typedef typename arithmetic_promotion_policy<T>::type arithmetic_type;
		typedef decltype(std::declval<arithmetic_type>() / std::declval<arithmetic_type>()) dimensionless_type;
		typedef void                                  is_numeric_sequence;
		typedef geometrix::vector<T, D>               sequence_type;
		typedef GEOMETRIX_AS_MPL_VECTOR((T))          storage_types;
		typedef dimension<D>                          dimension_type;
		typedef void                                  is_sequence;
		typedef void GEOMETRIX_TYPE_DIVERSITY_TAG((T));
	};
}//! namespace geometrix;
/***/

#endif //GEOMETRIX_VECTOR_HPP
