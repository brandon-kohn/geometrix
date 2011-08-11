//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_CROSS_PRODUCT_HPP
#define GEOMETRIX_CROSS_PRODUCT_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>
#include <geometrix/arithmetic/vector_arithmetic.hpp>

#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/iter_fold.hpp>
#include <boost/mpl/range_c.hpp>

namespace geometrix {

    namespace result_of 
    {
        //! Cross Product:
        template <typename LHS, typename RHS, typename LeftIsHomogeneous=void, typename RightIsHomogeneous=void>
        struct cross_product
        {
        private:
            typedef typename type_at<LHS,0>::type lhs_0;
            typedef typename type_at<LHS,1>::type lhs_1;
            typedef typename type_at<LHS,2>::type lhs_2;
            typedef typename type_at<RHS,0>::type rhs_0;
            typedef typename type_at<RHS,1>::type rhs_1;
            typedef typename type_at<RHS,2>::type rhs_2;
        public:
            typedef boost::fusion::vector
                <
                    typename minus
                    <
                        typename multiplies<lhs_1, rhs_2>::type
                      , typename multiplies<lhs_2, rhs_1>::type
                    >::type
                  , typename minus
                    <
                        typename multiplies<lhs_2, rhs_0>::type
                      , typename multiplies<lhs_0, rhs_2>::type
                    >::type
                  , typename minus
                    <
                        typename multiplies<lhs_0, rhs_1>::type
                      , typename multiplies<lhs_1, rhs_0>::type
                    >::type
                > type;
        };

        template <typename Vector1, typename Vector2>
        struct cross_product
            <
                Vector1
              , Vector2
              , typename geometric_traits<typename remove_const_ref<Vector1>::type>::is_homogeneous
              , typename geometric_traits<typename remove_const_ref<Vector2>::type>::is_homogeneous
            >
        {
        private:
            typedef typename dimension_of<Vector1>::type v1;
            typedef typename dimension_of<Vector2>::type v2;

            BOOST_MPL_ASSERT_MSG
            (
                ( v1::value == 3 && v2::value == 3 )
              , VECTOR_DIMENSIONS_SUPPLIED_TO_CROSS_PRODUCT_ARE_NOT_COMPATIBLE
              , (cross_product<Vector1, Vector2>)
            );

            typedef typename select_arithmetic_type_from_sequences
            <  
                Vector1
              , Vector2
            >::type arithmetic_type;
        public:

            typedef vector<arithmetic_type, 3> type;
        };
    
        template <typename LHS, typename RHS, unsigned int Index>
        struct cross_product_at_index{};

        template <typename LHS, typename RHS>
        struct cross_product_at_index< LHS, RHS, 0 >
            : minus
              <
                  typename multiplies< typename type_at<LHS,1>::type, typename type_at<RHS,2>::type >::type
                , typename multiplies< typename type_at<LHS,2>::type, typename type_at<RHS,1>::type >::type
              >
        {};

        template <typename LHS, typename RHS>
        struct cross_product_at_index< LHS, RHS, 1 >
            : minus
              <
                  typename multiplies< typename type_at<LHS,2>::type, typename type_at<RHS,0>::type >::type
                , typename multiplies< typename type_at<LHS,0>::type, typename type_at<RHS,2>::type >::type
              >
        {};

        template <typename LHS, typename RHS>
        struct cross_product_at_index< LHS, RHS, 2 >
            : minus
              <
                  typename multiplies< typename type_at<LHS,0>::type, typename type_at<RHS,1>::type >::type
                , typename multiplies< typename type_at<LHS,1>::type, typename type_at<RHS,0>::type >::type
              >
        {};

        template <typename Vector1, typename Vector2, typename Dimension = typename dimension_of<Vector1>::type>
        struct exterior_product_area
        {};
        
        //! Function to find the magnitude of the wedge product between two vectors.
        template <typename Vector1, typename Vector2>
        struct exterior_product_area< Vector1, Vector2, dimension<2> >
            : minus
              <
                  typename multiplies
                  <
                      typename type_at<Vector1,0>::type
                    , typename type_at<Vector2,1>::type 
                  >::type
                , typename multiplies
                  <
                      typename type_at<Vector1,1>::type
                    , typename type_at<Vector2,0>::type 
                  >::type
              >
        {};

        template <typename Vector1, typename Vector2>
        struct exterior_product_area< Vector1, Vector2, dimension<3> >
            : magnitude
              < 
                  typename cross_product
                  <
                      Vector1
                    , Vector2
                  >::type
              >
        {};

        template <typename Vector1, typename Vector2, typename Vector3, typename Dimension = typename dimension_of<Vector1>::type >
        struct exterior_product_volume
        {};

        template <typename Vector1, typename Vector2, typename Vector3>
        struct exterior_product_volume< Vector1, Vector2, Vector3, dimension<3> >
            : dot_product
              <
                  typename cross_product
                  <
                      Vector1
                    , Vector2
                  >::type
                , Vector3
              >
        {};
        
    }//namespace result_of;

    namespace detail
    {
        template <unsigned int Index>
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
        struct cross_product_at_index<0>
        {
            //! Calculate the cross product between two vectors at the specified index.
            template <typename Vector1, typename Vector2>
            typename result_of::cross_product_at_index<Vector1, Vector2, 0>::type operator() ( const Vector1& A, const Vector2& B )
            {
                BOOST_CONCEPT_ASSERT(( TensorConcept< Vector1 > ));
                BOOST_CONCEPT_ASSERT(( TensorConcept< Vector2 > ));

                return ( get<1>( A ) * get<2>(B) - get<2>(A) * get<1>(B) );
            }
        };
        
        template <>
        struct cross_product_at_index<1>
        {
            //! Calculate the cross product between two vectors at the specified index.
            template <typename Vector1, typename Vector2>
            typename result_of::cross_product_at_index<Vector1, Vector2, 1>::type operator() ( const Vector1& A, const Vector2& B )
            {
                BOOST_CONCEPT_ASSERT(( TensorConcept< Vector1 > ));
                BOOST_CONCEPT_ASSERT(( TensorConcept< Vector2 > ));
                return ( get<2>( A ) * get<0>( B ) - get<0>( A ) * get<2>( B ) );
            }
        };

        template <>
        struct cross_product_at_index<2>
        {
            //! Calculate the cross product between two vectors at the specified index.
            template <typename Vector1, typename Vector2>
            typename result_of::cross_product_at_index<Vector1, Vector2, 1>::type operator() ( const Vector1& A, const Vector2& B )
            {
                BOOST_CONCEPT_ASSERT(( TensorConcept< Vector1 > ));
                BOOST_CONCEPT_ASSERT(( TensorConcept< Vector2 > ));

                return ( get<0>(A) * get<1>(B) - get<1>(A) * get<0>(B) );  
            }
        };

        //! Function to find the cross product between two vectors
        template <typename Vector1, typename Vector2>
        typename result_of::exterior_product_area<Vector1, Vector2>::type
            exterior_product_area( const Vector1& A, const Vector2& B, dimension<3> )
        {
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector1> ));
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector2> ));            
            return
                (
                      ( get<1>(A) * get<2>(B) - get<2>(A) * get<1>(B) ) 
                    - ( get<0>(A) * get<2>(B) - get<2>(A) * get<0>(B) )
                    + ( get<0>(A) * get<1>(B) - get<1>(A) * get<0>(B) )
                );
        }

        //! Function to find the cross product between two vectors
        template <typename Vector1, typename Vector2>
        typename result_of::exterior_product_area<Vector1, Vector2>::type
            exterior_product_area( const Vector1& A, const Vector2& B, dimension<2> )
        {  
            boost::function_requires< Vector2DConcept< Vector1 > >();
            boost::function_requires< Vector2DConcept<Vector2> >();
            return ( get<0>(A) * get<1>(B) - get<1>(A) * get<0>(B) );
        }

        //! Function to find the cross product between two vectors
        template <typename Vector1, typename Vector2, typename Vector3>
        typename result_of::exterior_product_volume<Vector1, Vector2, Vector3>::type
            exterior_product_volume( const Vector1& A, const Vector2& B, const Vector3& C, dimension<3> )
        {
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector1> ));
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector2> ));
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector3> ));
            using namespace algebra;
            return get((A^B)*C);
        }

    }//namespace detail

    //! Calculate the cross product between two vectors at the specified index.
    template <unsigned int Index, typename Vector1, typename Vector2>
    typename result_of::cross_product_at_index<Vector1, Vector2, Index>::type cross_product_at_index( const Vector1& v1, const Vector2& v2 )
    {
        return detail::cross_product_at_index<Index>()( v1, v2 );
    }
        
    //! Calculate the magnitude of the cross product between two vectors.
    template <typename Vector1, typename Vector2>
    typename result_of::exterior_product_area<Vector1, Vector2>::type exterior_product_area( const Vector1& v1, const Vector2& v2 )
    {
        return detail::exterior_product_area( v1, v2, dimension_of< Vector1 >::type() );
    }
    
    //! Function to find the cross product between two vectors
    template <typename Vector1, typename Vector2, typename Vector3>
    typename result_of::exterior_product_volume<Vector1, Vector2, Vector3>::type
    exterior_product_volume( const Vector1& A, const Vector2& B, const Vector3& C )
    {
        return detail::exterior_product_volume( A, B, C, dimension_of< Vector1 >::type() );
    }

}//namespace geometrix;

#endif //GEOMETRIX_CROSS_PRODUCT_HPP
