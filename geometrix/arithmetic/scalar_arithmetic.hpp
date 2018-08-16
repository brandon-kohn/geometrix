//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_SCALAR_MATH_HPP
#define GEOMETRIX_SCALAR_MATH_HPP

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/function_traits.hpp>
#include <geometrix/tensor/tensor_access_policy.hpp>
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>
#include <geometrix/tensor/scalar.hpp>

#include <boost/typeof/typeof.hpp>
#include <utility>

namespace geometrix {

    namespace result_of
    {
        template <typename LHS, typename RHS>
        struct plus
            <
                LHS
              , RHS
              , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
              , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
            >
        {
        private:
            using lhs = typename type_at<typename remove_const_ref<LHS>::type>::type;
            using rhs = typename type_at<typename remove_const_ref<RHS>::type>::type;
        public:
            typedef decltype( std::declval<lhs>() + std::declval<rhs>() ) type;
        };

        template <typename LHS, typename RHS>
        struct minus
            <
                LHS
              , RHS
              , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
              , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
            >
        {
        private:
            using lhs = typename type_at<typename remove_const_ref<LHS>::type>::type;
            using rhs = typename type_at<typename remove_const_ref<RHS>::type>::type;
        public:
            typedef decltype( std::declval<lhs>() - std::declval<rhs>() ) type;
        };

        template <typename LHS, typename RHS>
        struct multiplies
            <
                LHS
              , RHS
              , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
              , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
            >
        {
        private:
            using lhs = typename type_at<typename remove_const_ref<LHS>::type>::type;
            using rhs = typename type_at<typename remove_const_ref<RHS>::type>::type;
        public:
            typedef decltype( std::declval<lhs>() * std::declval<rhs>() ) type;
        };

        template <typename LHS, typename RHS>
        struct divides
            <
                LHS
              , RHS
              , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
              , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
            >
        {
        private:
            using plhs = typename arithmetic_promotion_policy<typename type_at<typename remove_const_ref<LHS>::type>::type>::type;
            using rhs = typename type_at<typename remove_const_ref<RHS>::type>::type;
        public:
            typedef decltype( std::declval<plhs>() / std::declval<rhs>() ) type;
        };

    }//namespace result_of;

    template <typename LHS, typename RHS>
    struct plus
        <
            LHS
          , RHS
          , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
          , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
        >
    {
        typedef typename result_of::plus<LHS,RHS>::type result_type;

        result_type operator() ( const LHS& l, const RHS& r ) const
        {
            return l + r;
        }
    };

    template <typename LHS, typename RHS>
    struct minus
        <
            LHS
          , RHS
          , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
          , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
        >
    {
        typedef typename result_of::minus<LHS,RHS>::type result_type;

        result_type operator() ( const LHS& l, const RHS& r ) const
        {
            return l - r;
        }
    };

    template <typename LHS, typename RHS>
    struct multiplies
        <
            LHS
          , RHS
          , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
          , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
        >
    {
        typedef typename result_of::multiplies<LHS,RHS>::type result_type;

        result_type operator() ( const LHS& l, const RHS& r ) const
        {
            return l * r;
        }
    };

    template <typename LHS, typename RHS>
    struct divides
        <
            LHS
          , RHS
          , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
          , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
        >
    {
        typedef typename result_of::divides<LHS,RHS>::type result_type;

        result_type operator() ( const LHS& l, const RHS& r ) const
        {
            return arithmetic_promotion_policy<LHS>::promote(l) / r;
        }
    };
}//namespace geometrix;

#endif//GEOMETRIX_SCALAR_MATH_HPP
