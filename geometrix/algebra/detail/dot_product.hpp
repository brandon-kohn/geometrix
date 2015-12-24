//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/concept_check.hpp>

#if !defined(GEOMETRIX_MAX_DOT_PRODUCT_LENGTH)
#define GEOMETRIX_MAX_DOT_PRODUCT_LENGTH 10
#endif

#if !defined(GEOMETRIX_DONT_USE_PREPROCESSED_FILES)
#if !defined(GEOMETRIX_DETAIL_DOT_PRODUCT_HPP)
#define GEOMETRIX_DETAIL_DOT_PRODUCT_HPP 
        #include <geometrix/algebra/detail/preprocessed/dot_product.hpp>
#endif
#elif !defined(BOOST_PP_IS_ITERATING)
#if !defined(GEOMETRIX_DETAIL_DOT_PRODUCT_HPP )
#define GEOMETRIX_DETAIL_DOT_PRODUCT_HPP 

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2, line: 0, output: "preprocessed/dot_product.hpp")
    #endif
//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
namespace geometrix {

    namespace result_of 
    {
        namespace detail
        {
            template 
            <
                typename LHS
              , typename RHS
              , typename Dimension = typename dimension_of<LHS>::type
            >
            struct dot_
                : result_of::plus
                  <
                      typename dot_< LHS, RHS, dimension<Dimension::value-1> >::type
                    , typename result_of::multiplies
                      <
                          typename type_at<LHS,Dimension::value-1>::type 
                        , typename type_at<RHS,Dimension::value-1>::type 
                      >::type
                  >
            {};

        }//namespace detail;

        template 
            <
                typename LHS
              , typename RHS
              , typename Dimension = typename dimension_of<LHS>::type
              , typename LeftIsHomogeneous=void
              , typename RightIsHomogeneous=void
            >
        struct dot_product
            : detail::dot_<LHS,RHS,Dimension>
        {};
         
        template <typename LHS, typename RHS, std::size_t D>
        struct dot_product
            <
                LHS
              , RHS
              , dimension<D>
              , typename geometric_traits<typename remove_const_ref<LHS>::type>::is_homogeneous
              , typename geometric_traits<typename remove_const_ref<RHS>::type>::is_homogeneous
            >
            : select_arithmetic_type_from_sequences
                <
                    LHS
                  , RHS
                >
        {};   

        template <typename LHS, typename RHS>
        struct multiplies
            <
                LHS
              , RHS
              , typename geometric_traits<typename remove_const_ref<LHS>::type>::is_vector
              , typename geometric_traits<typename remove_const_ref<RHS>::type>::is_vector
            >
            : dot_product
              <
                  LHS
                , RHS
              >
        {};
    }//namespace result_of;

    namespace detail
    {
        template <typename NumericSequence1, typename NumericSequence2, typename Dimension = typename dimension_of<NumericSequence1>::type >
        struct dot_product
        {
            typedef typename result_of::dot_product
                    <
                        NumericSequence1
                      , NumericSequence2
                    >::type result_type; 

                result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
                {
                    return dot_product
                        < 
                            NumericSequence1
                          , NumericSequence2
                          , dimension<Dimension::value-1>
                        >()(A,B) + get<Dimension::value-1>(A) * get<Dimension::value-1>(B);           
                }
        };
    }//namespace detail;

    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 1)
    #endif
    #define BOOST_PP_ITERATION_PARAMS_1                                                            \
        (3, (2, GEOMETRIX_MAX_DOT_PRODUCT_LENGTH, <geometrix/algebra/detail/dot_product.hpp>))
    #include BOOST_PP_ITERATE()
        
    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(preserve: 2)
    #endif

    }//namespace geometrix
    
    #if defined(__WAVE__) && defined(GEOMETRIX_CREATE_PREPROCESSED_FILES)
        #pragma wave option(output: null)
    #endif      

    #endif// GEOMETRIX_DETAIL_DOT_PRODUCT_HPP 
#else

#define N BOOST_PP_ITERATION()

#define GEOMETRIX_DOT_PRODUCT_RESULT_2()                 \
    result_of::plus                                      \
    <                                                    \
        typename result_of::multiplies                   \
        <                                                \
            typename type_at<LHS,0>::type                \
          , typename type_at<RHS,0>::type                \
        >::type                                          \
      , typename result_of::multiplies                   \
        <                                                \
            typename type_at<LHS,1>::type                \
          , typename type_at<RHS,1>::type                \
        >::type                                          \
    >                                                    \
/***/

#define GEOMETRIX_DOT_PRODUCT_RESULT_3()                 \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_2()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 2>::type             \
             , typename type_at<RHS , 2>::type           \
         >::type                                         \
     >                                                   \
/***/
        
#define GEOMETRIX_DOT_PRODUCT_RESULT_4()                 \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_3()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 3>::type             \
             , typename type_at<RHS , 3>::type           \
         >::type                                         \
     >                                                   \
/***/

#define GEOMETRIX_DOT_PRODUCT_RESULT_5()                 \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_4()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 4>::type             \
             , typename type_at<RHS , 4>::type           \
         >::type                                         \
     >                                                   \
/***/

#define GEOMETRIX_DOT_PRODUCT_RESULT_6()                 \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_5()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 5>::type             \
             , typename type_at<RHS , 5>::type           \
         >::type                                         \
     >                                                   \
/***/

#define GEOMETRIX_DOT_PRODUCT_RESULT_7()                 \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_6()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 6>::type             \
             , typename type_at<RHS , 6>::type           \
         >::type                                         \
     >                                                   \
/***/

#define GEOMETRIX_DOT_PRODUCT_RESULT_8()                 \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_7()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 7>::type             \
             , typename type_at<RHS , 7>::type           \
         >::type                                         \
     >                                                   \
/***/

#define GEOMETRIX_DOT_PRODUCT_RESULT_9()                 \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_8()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 8>::type             \
             , typename type_at<RHS , 8>::type           \
         >::type                                         \
     >                                                   \
/***/

#define GEOMETRIX_DOT_PRODUCT_RESULT_10()                \
     result_of::plus                                     \
     <                                                   \
         typename GEOMETRIX_DOT_PRODUCT_RESULT_9()::type \
       , typename result_of::multiplies                  \
         <                                               \
             typename type_at<LHS , 9>::type             \
             , typename type_at<RHS , 9>::type           \
         >::type                                         \
     >                                                   \
/***/

    namespace result_of                                   
    {                                                     
        namespace detail                                  
        {                                                 
            template <typename LHS, typename RHS>         
            struct dot_                     
                   <                                      
                       LHS                                
                     , RHS                                
                     , dimension<N>                       
                   >                                      
                : BOOST_PP_CAT(GEOMETRIX_DOT_PRODUCT_RESULT_,N)() 
            {};                                           
        }                                                 
    }         

    namespace detail 
    {

#define GEOMETRIX_DOT_PRODUCT_ELEMENT_(z, n, data) \
    BOOST_PP_EXPR_IF(n,+) get<n>(A) * get<n>(B)    \
/***/

#define GEOMETRIX_DOT_PRODUCT_(n)                         \
    BOOST_PP_REPEAT(n, GEOMETRIX_DOT_PRODUCT_ELEMENT_, _) \
/***/

        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<N> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 

            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {                
                return GEOMETRIX_DOT_PRODUCT_(N);
            }
        };

    }//namespace detail;

#undef N
#undef GEOMETRIX_DOT_PRODUCT_ELEMENT_
#undef GEOMETRIX_DOT_PRODUCT_
#undef GEOMETRIX_DOT_PRODUCT_RESULT_2
#undef GEOMETRIX_DOT_PRODUCT_RESULT_3
#undef GEOMETRIX_DOT_PRODUCT_RESULT_4
#undef GEOMETRIX_DOT_PRODUCT_RESULT_5
#undef GEOMETRIX_DOT_PRODUCT_RESULT_6
#undef GEOMETRIX_DOT_PRODUCT_RESULT_7
#undef GEOMETRIX_DOT_PRODUCT_RESULT_8
#undef GEOMETRIX_DOT_PRODUCT_RESULT_9
#undef GEOMETRIX_DOT_PRODUCT_RESULT_10

#endif

