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
#include <geometrix/arithmetic/arithmetic_promotion_policy.hpp>

#include <boost/typeof/typeof.hpp>

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
            typedef typename remove_const_ref<LHS>::type lhs;
            typedef typename remove_const_ref<RHS>::type rhs;
        public:
            typedef BOOST_TYPEOF_TPL( lhs() + rhs() ) type;
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
            typedef typename remove_const_ref<LHS>::type lhs;
            typedef typename remove_const_ref<RHS>::type rhs;
        public:
            typedef BOOST_TYPEOF_TPL( lhs() - rhs() ) type;
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
            typedef typename remove_const_ref<LHS>::type lhs;
            typedef typename remove_const_ref<RHS>::type rhs;
        public:
            typedef BOOST_TYPEOF_TPL( lhs() * rhs() ) type;            
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
            typedef typename arithmetic_promotion_policy<typename remove_const_ref<LHS>::type>::type PLHS;
            typedef typename remove_const_ref<RHS>::type rhs;
        public:
            typedef BOOST_TYPEOF_TPL( PLHS() / rhs() ) type;            
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
