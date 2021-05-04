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

#include <geometrix/tensor/vector.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>
#include <geometrix/arithmetic/vector.hpp>

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
            using area_type = typename multiplies<arithmetic_type, arithmetic_type>::type;
        public:

            typedef vector<area_type, 3> type;
        };
    }//namespace result_of;

}//namespace geometrix;

#endif //GEOMETRIX_CROSS_PRODUCT_HPP
