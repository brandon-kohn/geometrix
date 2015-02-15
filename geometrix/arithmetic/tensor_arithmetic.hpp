//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_TENSOR_ARITHMETIC_HPP
#define GEOMETRIX_TENSOR_ARITHMETIC_HPP

#include <geometrix/numeric/numeric_traits.hpp>
#include <geometrix/numeric/function_traits.hpp>
#include <geometrix/arithmetic/scalar_arithmetic.hpp>
#include <geometrix/arithmetic/tensor_function.hpp>

#include <boost/typeof/typeof.hpp>

namespace geometrix { 
    
    namespace result_of 
    {

        template <typename F, typename RHS>
        struct multiplies
            <
                tensor_function<F>
              , RHS
              , void
              , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
            >
        {
            typedef typename remove_const_ref<RHS>::type rhs;            
            typedef BOOST_TYPEOF_TPL( F()(rhs()) ) type;
            
            template <typename Sig>
            struct result;
            
            template <typename B>
            struct result<multiplies(const tensor_function<F>&,const B&)>
            {
                typedef BOOST_TYPEOF_TPL( F()(B()) ) type;
            };
            
            template<typename B>
            typename result<multiplies(const tensor_function<F>&, const B&)>::type operator()(const tensor_function<F>&, const B& b) const
            {
                return F()(b);
            }
        };

        template <typename LHS, typename F>
        struct multiplies
            <
                LHS
              , tensor_function<F>
              , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
              , void
            >
        {
            typedef typename remove_const_ref<LHS>::type lhs;        
            typedef BOOST_TYPEOF_TPL( F()( lhs() ) ) type;
            
            template <typename Sig>
            struct result;
            
            template <typename A>
            struct result<multiplies(const A&,const tensor_function<F>&)>
            {
                typedef BOOST_TYPEOF_TPL( F()( A() ) ) type;
            };
            
            template<typename A>
            typename result<multiplies(const A&, const tensor_function<F>&)>::type operator()(const A& a, const tensor_function<F>&) const
            {
                return F()(a);
            }
        };
    }//namespace result_of;

    template <typename F, typename RHS>
    struct multiplies
        <
            tensor_function<F>
          , RHS
          , void
          , typename numeric_traits<typename remove_const_ref<RHS>::type>::is_numeric
        >
    {
        typedef typename result_of::multiplies<F,RHS>::type result_type;
        
        result_type operator()(const tensor_function<F>&, const RHS& b) const
        {
            return F()(b);
        }
    };
    
    template <typename LHS, typename F>
    struct multiplies
        <
            LHS
          , tensor_function<F>
          , typename numeric_traits<typename remove_const_ref<LHS>::type>::is_numeric
          , void
        >
    {
        typedef typename result_of::multiplies<LHS,F>::type result_type;

        result_type operator()(const LHS& a, const tensor_function<F>&) const
        {
            return F()(a);
        }
    };

    namespace detail
    {
        template <typename Matrix1, typename Matrix2, unsigned int Row, unsigned int Column>
        struct tensor_product_index
        {
        public:

            struct A : 
                matrix_index
                <
                    boost::mpl::divides< boost::mpl::int_<Row>, typename row_dimension_of<Matrix2>::type >::value
                  , boost::mpl::divides< boost::mpl::int_<Column>, typename column_dimension_of<Matrix2>::type >::value
                >
            {};
            
            struct B :
                matrix_index
                <
                    boost::mpl::modulus< boost::mpl::int_<Row>, typename row_dimension_of<Matrix2>::type >::value
                  , boost::mpl::modulus< boost::mpl::int_<Column>, typename column_dimension_of<Matrix2>::type >::value
                >      
            {};
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column, typename Type1 = void, typename Type2 = void>
        struct tensor_prod_elem
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1, tensor_product_index<Tensor1, Tensor2,Row,Column>::A::row::value, tensor_product_index<Tensor1, Tensor2,Row,Column>::A::col::value>::type
                , typename type_at<Tensor2, tensor_product_index<Tensor1, Tensor2,Row,Column>::B::row::value, tensor_product_index<Tensor1, Tensor2,Row,Column>::B::col::value>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get
                       <
                           tensor_product_index<Tensor1, Tensor2, Row, Column>::A::row::value
                         , tensor_product_index<Tensor1, Tensor2, Row, Column>::A::col::value
                       >(t1) 
                     * get
                       <
                           tensor_product_index<Tensor1, Tensor2, Row, Column>::B::row::value
                         , tensor_product_index<Tensor1, Tensor2, Row, Column>::B::col::value
                       >(t2);                     
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_0, typename tensor_traits<Tensor2>::rank_0>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1>::type
                , typename type_at<Tensor2>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get(t1) * get(t2);
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_0, typename tensor_traits<Tensor2>::rank_1>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1>::type
                , typename type_at<Tensor2, Row>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get(t1) * get<Column>(t2);
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_0, typename tensor_traits<Tensor2>::rank_2>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1>::type
                , typename type_at<Tensor2, Row, Column>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get(t1) * get<Row, Column>(t2);                     
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_1, typename tensor_traits<Tensor2>::rank_0>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1, Row>::type
                , typename type_at<Tensor2>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get<Row>(t1) * get(t2);    
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_1, typename tensor_traits<Tensor2>::rank_1>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1, Row>::type
                , typename type_at<Tensor2, Column>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get<Row>(t1) * get<Column>(t2);                     
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_1, typename tensor_traits<Tensor2>::rank_2>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1, tensor_product_index<column_vector<Tensor1>, Tensor2, Row, Column>::A::row::value>::type
                , typename type_at<Tensor2, tensor_product_index<column_vector<Tensor1>, Tensor2, Row, Column>::B::col::value>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get
                       <
                           tensor_product_index<column_vector<Tensor1>, Tensor2, Row, Column>::A::row::value                         
                       >(t1) 
                     * get
                       <
                           tensor_product_index<column_vector<Tensor1>, Tensor2, Row, Column>::B::row::value
                         , tensor_product_index<column_vector<Tensor1>, Tensor2, Row, Column>::B::col::value
                       >(t2);                     
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_2, typename tensor_traits<Tensor2>::rank_0>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at<Tensor1, Row, Column>::type
                , typename type_at<Tensor2>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get<Row, Column>(t1) * get(t2);
            }
        };

        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_prod_elem<Tensor1, Tensor2, Row, Column, typename tensor_traits<Tensor1>::rank_2, typename tensor_traits<Tensor2>::rank_1>
        {
            typedef typename result_of::multiplies
              <
                  typename type_at
                  <
                      Tensor1
                    , tensor_product_index<Tensor1, row_vector<Tensor2>, Row, Column>::A::row::value
                    , tensor_product_index<Tensor1, row_vector<Tensor2>,Row,Column>::A::col::value
                  >::type
                , typename type_at<Tensor2, tensor_product_index<Tensor1, row_vector<Tensor2>, Row, Column>::B::col::value>::type
              >::type result_type;

            result_type operator()( const Tensor1& t1, const Tensor2& t2 ) const
            {
                return get
                       <
                           tensor_product_index<Tensor1, row_vector<Tensor2>, Row, Column>::A::row::value
                         , tensor_product_index<Tensor1, row_vector<Tensor2>, Row, Column>::A::col::value
                       >(t1) 
                     * get
                       <
                           tensor_product_index<Tensor1, row_vector<Tensor2>, Row, Column>::B::col::value
                       >(t2);                     
            }
        };
    }//namespace detail;

    namespace result_of
    {
        template <typename Tensor1, typename Tensor2, unsigned int Row, unsigned int Column>
        struct tensor_product_element
        {
            //typedef double type;
            typedef typename geometrix::detail::tensor_prod_elem<Tensor1, Tensor2, Row, Column>::result_type type;
        };

        template <typename Matrix1, typename Matrix2>
        struct tensor_product
        {
            typedef dimension< row_dimension_of<Matrix1>::value * row_dimension_of<Matrix2>::value >       row_dimension;
            typedef dimension< column_dimension_of<Matrix1>::value * column_dimension_of<Matrix2>::value > col_dimension;                
        };       

    }//namespace result_of;

    //! Calculate the tensor product at the specified index.
    template <unsigned int Row, unsigned int Column, typename Tensor1, typename Tensor2>
    inline typename result_of::tensor_product_element
        <
            Tensor1
          , Tensor2 
          , Row
          , Column
        >::type tensor_product_element( const Tensor1& m1, const Tensor2& m2 )
    {
        BOOST_STATIC_ASSERT( is_tensor<Tensor1>::value );
        BOOST_STATIC_ASSERT( is_tensor<Tensor2>::value );
        return detail::tensor_prod_elem<Tensor1, Tensor2, Row,Column>()( m1, m2 );
    }

}//namespace geometrix;

#endif//GEOMETRIX_TENSOR_ARITHMETIC_HPP
