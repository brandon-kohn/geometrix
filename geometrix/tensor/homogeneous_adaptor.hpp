//
//! Copyright © 2017
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_HOMOGENEOUS_ADAPTOR_HPP
#define GEOMETRIX_HOMOGENEOUS_ADAPTOR_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/numeric/constants.hpp>
#include <boost/mpl/if.hpp>

namespace geometrix {

	template <typename Sequence, typename HomogeneousCoordPolicy>
	struct homogeneous_adaptor;

	namespace detail {

		template <typename Sequence, typename Policy, int Index, int HomoIndex>
		struct homogeneous_adaptor_get_helper
		{
			using result_type = typename type_at<Sequence, Index>::type;

			static inline result_type get(const Sequence& seq)
			{
				return geometrix::get<Index>(seq);
			}
		};

		template <typename Sequence, typename Policy, int SameIndex>
		struct homogeneous_adaptor_get_helper<Sequence, Policy, SameIndex, SameIndex>
		{
			using result_type = typename Policy::type;

			static inline result_type get(const Sequence&)
			{
				return Policy().get();
			}
		};
	}//! namespace detail;

	 //! Treat a point as a point.
	template <typename Sequence, typename HomogeneousCoordPolicy>
	struct homogeneous_adaptor
	{
		GEOMETRIX_STATIC_ASSERT((is_numeric_sequence<Sequence>::value));
		BOOST_STATIC_CONSTANT(int, homogeneous_index = dimension_of<Sequence>::value);
		using sequence_type = Sequence;
		using homogeneous_policy_type = HomogeneousCoordPolicy;

		homogeneous_adaptor(const Sequence& p)
			: m_seq(p)
		{
		}

		template <int Index>
		typename detail::homogeneous_adaptor_get_helper<Sequence, HomogeneousCoordPolicy, Index, homogeneous_index>::result_type get() const
		{
			return detail::homogeneous_adaptor_get_helper<Sequence, HomogeneousCoordPolicy, Index, homogeneous_index>::get(m_seq);
		}

	private:

		const Sequence& m_seq;

	};


	template <typename Coord>
	struct positional_homogeneous_coord_policy
	{
		using type = Coord;

		type get() const
		{
			return constants::one<type>();
		}
	};

	template <typename Coord>
	struct normalized_homogeneous_coord_policy
	{
		using type = Coord;

		type get() const
		{
			return constants::zero<type>();
		}
	};

	template <typename Sequence, typename HomogeneousCoordPolicy>
	struct is_point< homogeneous_adaptor<Sequence, HomogeneousCoordPolicy> > : is_point<Sequence> {};

	template <typename Sequence, typename HomogeneousCoordPolicy>
	struct is_vector< homogeneous_adaptor<Sequence, HomogeneousCoordPolicy> > : is_vector<Sequence> {};

	template <typename Sequence, typename Policy>
	struct geometric_traits< homogeneous_adaptor< Sequence, Policy >, typename geometric_traits<Sequence>::is_point >
		: diversity_base< Sequence >
	{
		typedef homogeneous_adaptor<Sequence, Policy>                point_type;
		typedef void                                                 is_point;
		typedef typename geometric_traits<Sequence>::reference_frame reference_frame;
		typedef void                                                 is_coordinate_sequence;
		typedef typename geometric_traits<Sequence>::arithmetic_type arithmetic_type;
		using dimensionless_type = typename geometric_traits<Sequence>::dimensionless_type;
		typedef void                                                 is_numeric_sequence;
		typedef homogeneous_adaptor<Sequence, Policy>                sequence_type;
		typedef dimension<dimension_of<Sequence>::value + 1>         dimension_type;
		typedef void                                                 is_sequence;
	};

	template <typename Sequence, typename Policy>
	struct geometric_traits< homogeneous_adaptor< Sequence, Policy >, typename geometric_traits<Sequence>::is_vector >
		: diversity_base< Sequence >
	{
		typedef homogeneous_adaptor<Sequence, Policy>                vector_type;
		typedef void                                                 is_vector;
		typedef typename geometric_traits<Sequence>::reference_frame reference_frame;
		typedef void                                                 is_coordinate_sequence;
		typedef typename geometric_traits<Sequence>::arithmetic_type arithmetic_type;
		using dimensionless_type = typename geometric_traits<Sequence>::dimensionless_type;
		typedef void                                                 is_numeric_sequence;
		typedef homogeneous_adaptor<Sequence, Policy>                sequence_type;
		typedef dimension<dimension_of<Sequence>::value + 1>         dimension_type;
		typedef void                                                 is_sequence;
	};


	template <typename T>
	struct homogeneous_adaptor_access_policy
	{
		typedef void compile_time_access;
		BOOST_STATIC_CONSTANT(int, homogeneous_index = T::homogeneous_index);

		template <std::size_t Index>
		struct type_at
			: boost::mpl::if_c
			<
			  Index != homogeneous_index
			, ::geometrix::type_at<typename T::sequence_type, Index>
			, typename T::homogeneous_policy_type
			>::type
		{};

		template <>
		struct type_at<homogeneous_index>
			: T::homogeneous_policy_type
		{};

		//! \brief compile time access if available for the collection.
		template <std::size_t Index>
		static typename type_at<Index>::type get(const T& collection)
		{
			return collection.template get<Index>();
		}

		//! \brief compile time access if available for the collection.
		//template <std::size_t Index>
		//static void set( T& collection, typename boost::call_traits<typename type_at<Index>::type>::param_type v )
		//{
		//    collection.template set<Index>(v);
		//}
	};

	template <typename Coord, typename Sequence>
	inline homogeneous_adaptor<Sequence, positional_homogeneous_coord_policy<Coord>> as_positional_homogeneous(const Sequence& p)
	{
		return homogeneous_adaptor<Sequence, positional_homogeneous_coord_policy<Coord>>(p);
	}

	template <typename Coord, typename Vector>
	inline homogeneous_adaptor<Vector, normalized_homogeneous_coord_policy<Coord>> as_normalized_homogeneous(const Vector& p)
	{
		return homogeneous_adaptor<Vector, normalized_homogeneous_coord_policy<Coord>>(p);
	}

	template <typename Sequence, typename Policy>
	struct tensor_traits< homogeneous_adaptor<Sequence, Policy> >
		: tensor_traits< typename remove_const_ref<Sequence>::type >
	{
		typedef homogeneous_adaptor_access_policy<homogeneous_adaptor<Sequence, Policy>> access_policy;
		typedef boost::mpl::int_<1> tensor_order;
		typedef void                GEOMETRIX_TENSOR_ORDER(1);
		typedef void                is_tensor;
	};

}//namespace geometrix;

#endif //GEOMETRIX_HOMOGENEOUS_ADAPTOR_HPP
