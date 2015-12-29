//
//! Copyright © 2008-2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_EXTERIOR_PRODUCT_HPP
#define GEOMETRIX_EXTERIOR_PRODUCT_HPP

#include <geometrix/algebra/cross_product.hpp>
#include <geometrix/algebra/dot_product.hpp>

namespace geometrix {

    namespace result_of 
    {
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
        //! Function to find the exterior product between two vectors
        template <typename Vector1, typename Vector2>
        inline typename result_of::exterior_product_area<Vector1, Vector2>::type
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

        //! Function to find the exterior product between two vectors
        template <typename Vector1, typename Vector2>
        inline typename result_of::exterior_product_area<Vector1, Vector2>::type
            exterior_product_area( const Vector1& A, const Vector2& B, dimension<2> )
        {  
            BOOST_CONCEPT_ASSERT(( Vector2DConcept<Vector1> ));
            BOOST_CONCEPT_ASSERT(( Vector2DConcept<Vector2> ));    
            return ( get<0>(A) * get<1>(B) - get<1>(A) * get<0>(B) );
        }

        //! Function to find the exterior product between three vectors
        template <typename Vector1, typename Vector2, typename Vector3>
        inline typename result_of::exterior_product_volume<Vector1, Vector2, Vector3>::type
            exterior_product_volume( const Vector1& A, const Vector2& B, const Vector3& C, dimension<3> )
        {
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector1> ));
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector2> ));
            BOOST_CONCEPT_ASSERT(( Vector3DConcept<Vector3> ));
            return get((A^B)*C);
        }

    }//namespace detail

    //! Calculate the magnitude of the cross product between two vectors.
    template <typename Vector1, typename Vector2>
    inline typename result_of::exterior_product_area<Vector1, Vector2>::type exterior_product_area( const Vector1& v1, const Vector2& v2 )
    {
        return detail::exterior_product_area( v1, v2, typename dimension_of< Vector1 >::type() );
    }

    //! Function to find the cross product between two vectors
    template <typename Vector1, typename Vector2, typename Vector3>
    inline typename result_of::exterior_product_volume<Vector1, Vector2, Vector3>::type
        exterior_product_volume( const Vector1& A, const Vector2& B, const Vector3& C )
    {
        return detail::exterior_product_volume( A, B, C, typename dimension_of< Vector1 >::type() );
    }

}//namespace geometrix;

#endif //GEOMETRIX_EXTERIOR_PRODUCT_HPP
