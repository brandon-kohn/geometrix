//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_MATH_HPP
#define GEOMETRIX_VECTOR_MATH_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/algebra/dot_product.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {
        template <typename LHS, typename RHS>
        struct multiplies<LHS,RHS, typename numeric_traits<LHS>::is_numeric, typename geometric_traits<RHS>::is_vector>
            : boost::mpl::transform_view
                <
                    RHS
                  , multiplies<LHS, boost::mpl::_1>
                >
        {};

        //! Addition                
        template <typename LHS, typename RHS>
        struct plus<LHS,RHS, typename geometric_traits<LHS>::is_vector, typename geometric_traits<RHS>::is_vector>
            : boost::mpl::transform_view
                <
                    boost::mpl::zip_view< boost::mpl::vector<LHS,RHS> >
                  , boost::mpl::unpack_args< plus<boost::mpl::_1,boost::mpl::_2> >
                >
        {};

        template <typename LHS, typename RHS>
        struct plus<LHS,RHS, typename geometric_traits<LHS>::is_point, typename geometric_traits<RHS>::is_vector>
            : boost::mpl::transform_view
                <
                    boost::mpl::zip_view< boost::mpl::vector<LHS,RHS> >
                  , boost::mpl::unpack_args< plus<boost::mpl::_1,boost::mpl::_2> >
                >
        {};
        
        //! Subtraction
        template <typename LHS, typename RHS>
        struct minus<LHS,RHS, typename geometric_traits<LHS>::is_vector, typename geometric_traits<RHS>::is_vector>
            : boost::mpl::transform_view
                <
                    boost::mpl::zip_view< boost::mpl::vector<LHS,RHS> >
                  , boost::mpl::unpack_args< minus<boost::mpl::_1,boost::mpl::_2> >
                >
        {};
        
        template <typename LHS, typename RHS>
        struct minus<LHS,RHS, typename geometric_traits<LHS>::is_point, typename geometric_traits<RHS>::is_vector>
            : boost::mpl::transform_view
                <
                    boost::mpl::zip_view< boost::mpl::vector<LHS,RHS> >
                  , boost::mpl::unpack_args< minus<boost::mpl::_1,boost::mpl::_2> >
                >
        {};

        template <typename LHS, typename RHS>
        struct minus<LHS,RHS, typename geometric_traits<LHS>::is_point, typename geometric_traits<RHS>::is_point>
            : boost::mpl::transform_view
                <
                    boost::mpl::zip_view< boost::mpl::vector<LHS,RHS> >
                  , boost::mpl::unpack_args< minus<boost::mpl::_1,boost::mpl::_2> >
                >
        {};

        //! Division
        template <typename LHS, typename RHS>
        struct divides<LHS,RHS, typename geometric_traits<LHS>::is_vector, typename geometric_traits<RHS>::is_scalar>
            : boost::mpl::transform_view
                <
                    LHS
                  , divides<boost::mpl::_1, typename numeric_traits<RHS>::numeric_type >
                >
        {};

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

        template <unsigned int Index, typename Vector>
        struct normal_at_index
            : result_of::divides
                <
                    typename arithmetic_promotion_policy
                    <
                        typename type_at<Vector,Index>::type
                    >::type
                  , typename result_of::multiplies<Vector,Vector>::type 
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
    typename result_of::magnitude_sqrd<Vector>::type magnitude_sqrd( const Vector& v )
    {
        BOOST_STATIC_ASSERT(( is_vector<Vector>::value ));
        return detail::magnitude_sqrd<Vector,dimension_of<Vector>::value-1>::eval(v);
    }

    //! \brief Return the magnitude of a vector.
    template <typename Vector>
    typename result_of::magnitude<Vector>::type magnitude( const Vector& v )
    {
        BOOST_STATIC_ASSERT(( is_vector<Vector>::value ));
        return math::sqrt( arithmetic_promote( magnitude_sqrd( v ) ) );
    }

    //! \brief Normalize a vector (returns a new unit vector with the same orientation as the original).
    template <typename Vector>
    Vector normalize( const Vector& v )
    {
        BOOST_STATIC_ASSERT(( is_vector<Vector>::value ));
        return construct<Vector>( algebra::norm(v) );
    }

    //! \brief Access the normalized value at the ith index.
    template <unsigned int Index, typename Vector>
    typename result_of::normal_at_index<Index,Vector>::type normal_at_index( const Vector& v )
    {
        BOOST_STATIC_ASSERT(( is_vector<Vector>::value ));
        return arithmetic_promote( get<Index>(v) ) / magnitude( v );
    }
        
    //! Get the perpendicular of a 2d vector.
    template <typename Vector>
    Vector perp( const Vector& u )
    {
        BOOST_CONCEPT_ASSERT(( Vector2DConcept<Vector> ));
        return construct<Vector>( - get<1>(u), get<0>(u) );
    }

}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_MATH_HPP
