//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_VECTOR_ARITHMETIC_HPP
#define GEOMETRIX_VECTOR_ARITHMETIC_HPP

#include <geometrix/primitive/point_traits.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/arithmetic/arithmetic.hpp>

#include <boost/fusion/include/mpl.hpp>

namespace geometrix {

    namespace result_of 
    {
        //! Multiplication
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
        
    }//namespace result_of;
       
}//namespace geometrix;

#endif //GEOMETRIX_VECTOR_ARITHMETIC_HPP
