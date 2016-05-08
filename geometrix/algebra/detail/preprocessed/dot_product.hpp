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
            struct dot_product_helper
                : result_of::plus
                  <
                      typename dot_product_helper< LHS, RHS, dimension<Dimension::value-1> >::type
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
            : detail::dot_product_helper<LHS,RHS,Dimension>
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
        {
			typedef typename select_arithmetic_type_from_sequences<LHS, RHS>::type atype;
			typedef decltype(atype()*atype()) type;
		}; 

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
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<2> 
                   > 
                : result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<2> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<3> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<3> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<4> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 3>::type , typename type_at<RHS , 3>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<4> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B) + get<3>(A) * get<3>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<5> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 3>::type , typename type_at<RHS , 3>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 4>::type , typename type_at<RHS , 4>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<5> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B) + get<3>(A) * get<3>(B) + get<4>(A) * get<4>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<6> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 3>::type , typename type_at<RHS , 3>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 4>::type , typename type_at<RHS , 4>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 5>::type , typename type_at<RHS , 5>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<6> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B) + get<3>(A) * get<3>(B) + get<4>(A) * get<4>(B) + get<5>(A) * get<5>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<7> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 3>::type , typename type_at<RHS , 3>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 4>::type , typename type_at<RHS , 4>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 5>::type , typename type_at<RHS , 5>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 6>::type , typename type_at<RHS , 6>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<7> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B) + get<3>(A) * get<3>(B) + get<4>(A) * get<4>(B) + get<5>(A) * get<5>(B) + get<6>(A) * get<6>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<8> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 3>::type , typename type_at<RHS , 3>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 4>::type , typename type_at<RHS , 4>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 5>::type , typename type_at<RHS , 5>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 6>::type , typename type_at<RHS , 6>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 7>::type , typename type_at<RHS , 7>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<8> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B) + get<3>(A) * get<3>(B) + get<4>(A) * get<4>(B) + get<5>(A) * get<5>(B) + get<6>(A) * get<6>(B) + get<7>(A) * get<7>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<9> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 3>::type , typename type_at<RHS , 3>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 4>::type , typename type_at<RHS , 4>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 5>::type , typename type_at<RHS , 5>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 6>::type , typename type_at<RHS , 6>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 7>::type , typename type_at<RHS , 7>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 8>::type , typename type_at<RHS , 8>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<9> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B) + get<3>(A) * get<3>(B) + get<4>(A) * get<4>(B) + get<5>(A) * get<5>(B) + get<6>(A) * get<6>(B) + get<7>(A) * get<7>(B) + get<8>(A) * get<8>(B);
            }
        };
    }
        
    namespace result_of 
    { 
        namespace detail 
        { 
            template <typename LHS, typename RHS> 
            struct dot_product_helper 
                   < 
                       LHS 
                     , RHS 
                     , dimension<10> 
                   > 
                : result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::plus < typename result_of::multiplies < typename type_at<LHS,0>::type , typename type_at<RHS,0>::type >::type , typename result_of::multiplies < typename type_at<LHS,1>::type , typename type_at<RHS,1>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 2>::type , typename type_at<RHS , 2>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 3>::type , typename type_at<RHS , 3>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 4>::type , typename type_at<RHS , 4>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 5>::type , typename type_at<RHS , 5>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 6>::type , typename type_at<RHS , 6>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 7>::type , typename type_at<RHS , 7>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 8>::type , typename type_at<RHS , 8>::type >::type >::type , typename result_of::multiplies < typename type_at<LHS , 9>::type , typename type_at<RHS , 9>::type >::type > 
            {}; 
        } 
    } 
    namespace detail 
    {
        template <typename NumericSequence1, typename NumericSequence2>
        struct dot_product< NumericSequence1, NumericSequence2, dimension<10> >
        {
            typedef typename result_of::dot_product
                <
                    NumericSequence1
                  , NumericSequence2
                >::type result_type; 
            result_type operator() ( const NumericSequence1& A, const NumericSequence2& B ) const
            {
                return get<0>(A) * get<0>(B) + get<1>(A) * get<1>(B) + get<2>(A) * get<2>(B) + get<3>(A) * get<3>(B) + get<4>(A) * get<4>(B) + get<5>(A) * get<5>(B) + get<6>(A) * get<6>(B) + get<7>(A) * get<7>(B) + get<8>(A) * get<8>(B) + get<9>(A) * get<9>(B);
            }
        };
    }
        
    }//namespace geometrix
    
