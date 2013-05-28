//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_MAGNITUDE_HPP
#define GEOMETRIX_VECTOR_MATH_MAGNITUDE_HPP

#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/arithmetic/vector/vector_arithmetic.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {
        template <typename Vector, typename EnableIf=void>
        struct magnitude_sqrd{};

        template <typename Vector>
        struct magnitude_sqrd<Vector, typename geometric_traits<Vector>::is_vector>
            : dot_product<Vector,Vector>
        {};
        
        template <typename Vector, typename EnableIf=void>
        struct magnitude{};

        template <typename Vector>
        struct magnitude<Vector, typename geometric_traits<Vector>::is_vector>
            : boost::result_of
                <
                    math::square_root
                    (
                        typename arithmetic_promotion_policy
                        <
                            typename dot_product<Vector,Vector>::type
                        >::type
                    )
                >
        {};

    }//namespace result_of;

    namespace detail
    {
        template <typename Vector, unsigned int Index>
        struct magnitude_sqrd
        {
            typedef typename result_of::magnitude_sqrd<Vector>::type result_type;

            static result_type eval( const Vector& v )
            {
                BOOST_AUTO( a, get<Index>(v) );
                return a * a + magnitude_sqrd<Vector, Index-1>::eval(v);
            }
        };

        template <typename Vector>
        struct magnitude_sqrd<Vector,0>
        {
            typedef typename result_of::magnitude_sqrd<Vector>::type result_type;

            static result_type eval( const Vector& v )
            {
                BOOST_AUTO( a, get<0>(v) );
                return a * a;
            }
        };

        template <typename Vector>
        struct magnitude_sqrd<Vector,1>
        {
            typedef typename result_of::magnitude_sqrd<Vector>::type result_type;

            static result_type eval( const Vector& v ) 
            {
                BOOST_AUTO( a, get<0>(v) );
                BOOST_AUTO( b, get<1>(v) );
                return a * a + b * b;
            }
        };

        template <typename Vector>
        struct magnitude_sqrd<Vector,2>
        {
            typedef typename result_of::magnitude_sqrd<Vector>::type result_type;

            static result_type eval( const Vector& v ) 
            {
                typename type_at<Vector,0>::type a = get<0>(v);
                typename type_at<Vector,1>::type b = get<1>(v);
                typename type_at<Vector,2>::type c = get<2>(v);
                return a * a + b * b + c * c;
            }
        };

        template <typename Vector>
        struct magnitude_sqrd<Vector,3>
        {
            typedef typename result_of::magnitude_sqrd<Vector>::type result_type;

            static result_type eval( const Vector& v )
            {
                BOOST_AUTO( a, get<0>(v) );
                BOOST_AUTO( b, get<1>(v) );
                BOOST_AUTO( c, get<2>(v) );
                BOOST_AUTO( d, get<3>(v) );
                return a * a + b * b + c * c + d * d;
            }
        };        

    }//namespace detail;

    //! \brief Return the sqrd magnitude of a vector.
    template <typename Vector>
    inline typename result_of::magnitude_sqrd<Vector>::type magnitude_sqrd( const Vector& v )
    {
        BOOST_CONCEPT_ASSERT(( VectorConcept<Vector> ));
        return detail::magnitude_sqrd<Vector,dimension_of<Vector>::value-1>::eval(v);
    }

    //! \brief Return the magnitude of a vector.
    template <typename Vector>
    inline typename result_of::magnitude<Vector>::type magnitude( const Vector& v )
    {
        BOOST_CONCEPT_ASSERT(( VectorConcept<Vector> ));
        return math::sqrt( arithmetic_promote( magnitude_sqrd( v ) ) );
    }

}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_MAGNITUDE_HPP
