//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_CROSS_PRODUCT_HPP
#define GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_CROSS_PRODUCT_HPP

#include <geometrix/algebra/functions/binary_function.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

namespace geometrix {

	namespace result_of
	{
		template <typename LHS, typename RHS, std::size_t Index>
		struct cross_product_at_index{};

		template <typename LHS, typename RHS>
		struct cross_product_at_index< LHS, RHS, 0 >
			: minus
			<
			typename multiplies< typename type_at<LHS, 1>::type, typename type_at<RHS, 2>::type >::type
			, typename multiplies< typename type_at<LHS, 2>::type, typename type_at<RHS, 1>::type >::type
			>
		{};

		template <typename LHS, typename RHS>
		struct cross_product_at_index< LHS, RHS, 1 >
			: minus
			<
			typename multiplies< typename type_at<LHS, 2>::type, typename type_at<RHS, 0>::type >::type
			, typename multiplies< typename type_at<LHS, 0>::type, typename type_at<RHS, 2>::type >::type
			>
		{};

		template <typename LHS, typename RHS>
		struct cross_product_at_index< LHS, RHS, 2 >
			: minus
			<
			typename multiplies< typename type_at<LHS, 0>::type, typename type_at<RHS, 1>::type >::type
			, typename multiplies< typename type_at<LHS, 1>::type, typename type_at<RHS, 0>::type >::type
			>
		{};

	}//namespace result_of;

	namespace detail
	{
		template <std::size_t Index>
		struct cross_product_at_index
		{
			typedef boost::mpl::int_<Index> index_type;
			BOOST_MPL_ASSERT_MSG
				(
				(false)
				, CROSS_PRODUCT_NOT_DEFINED_AT_SPECIFIED_INDEX
				, (index_type)
				);
		};

		template <>
		struct cross_product_at_index < 0 >
		{
			//! Calculate the cross product between two vectors at the specified index.
			template <typename Vector1, typename Vector2>
			typename result_of::cross_product_at_index<Vector1, Vector2, 0>::type operator() ( const Vector1& A, const Vector2& B )
			{
				BOOST_CONCEPT_ASSERT( (TensorConcept< Vector1 >) );
				BOOST_CONCEPT_ASSERT( (TensorConcept< Vector2 >) );
				return (get<1>( A ) * get<2>( B ) -get<2>( A ) * get<1>( B ));
			}
		};

		template <>
		struct cross_product_at_index < 1 >
		{
			//! Calculate the cross product between two vectors at the specified index.
			template <typename Vector1, typename Vector2>
			typename result_of::cross_product_at_index<Vector1, Vector2, 1>::type operator() ( const Vector1& A, const Vector2& B )
			{
				BOOST_CONCEPT_ASSERT( (TensorConcept< Vector1 >) );
				BOOST_CONCEPT_ASSERT( (TensorConcept< Vector2 >) );
				return (get<2>( A ) * get<0>( B ) -get<0>( A ) * get<2>( B ));
			}
		};

		template <>
		struct cross_product_at_index < 2 >
		{
			//! Calculate the cross product between two vectors at the specified index.
			template <typename Vector1, typename Vector2>
			typename result_of::cross_product_at_index<Vector1, Vector2, 1>::type operator() ( const Vector1& A, const Vector2& B )
			{
				BOOST_CONCEPT_ASSERT( (TensorConcept< Vector1 >) );
				BOOST_CONCEPT_ASSERT( (TensorConcept< Vector2 >) );
				return (get<0>( A ) * get<1>( B ) -get<1>( A ) * get<0>( B ));
			}
		};

	}//namespace detail

	//! Calculate the cross product between two vectors at the specified index.
	template <std::size_t Index, typename Vector1, typename Vector2>
	inline typename result_of::cross_product_at_index<Vector1, Vector2, Index>::type cross_product_at_index( const Vector1& v1, const Vector2& v2 )
	{
		return detail::cross_product_at_index<Index>()(v1, v2);
	}

    namespace tag
    {
        typedef boost::proto::tag::bitwise_xor cross_product;        
    }//namespace tag;

    GEOMETRIX_LINEAR_ALGEBRA_BINARY_OP( tag::cross_product, is_vector, is_vector );
                
    //! Cross Product of Vectors
    template <typename Left, typename Right>
    struct bin_fun
        <
            tag::cross_product
          , Left
          , Right
          , typename geometric_traits<typename remove_const_ref<Left>::type>::is_vector
          , typename geometric_traits<typename remove_const_ref<Right>::type>::is_vector 
        >
        : binary_diversity_base<Left,Right>
    {
        typedef void                                     is_vector;
        typedef void                                     rank_1;
        typedef typename dimension_of<Right>::type       dimension_type;
        typedef typename reference_frame_of<Right>::type reference_frame;//! Todo: This isn't properly calculated under transforms.
        typedef void                                     is_sequence;
        typedef void                                     is_numeric_sequence;
        typedef void                                     is_coordinate_sequence;

        template <std::size_t Index, typename Callable = boost::proto::callable >
        struct context : boost::proto::callable_context< const context<Index, Callable> >
        {
            typedef typename result_of::cross_product_at_index<Left, Right, Index>::type result_type;

            result_type operator()(tag::cross_product, const Left& l, const Right& r ) const
            {
                return cross_product_at_index<Index>( l, r );
            }
        };
    };

}//namespace geometrix;

#endif//GEOMETRIX_LINEAR_ALGEBRA_BINARY_FUNCTIONS_CROSS_PRODUCT_HPP
