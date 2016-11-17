//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_FUSION_VECTOR_HPP
#define GEOMETRIX_FUSION_VECTOR_HPP

#include <geometrix/utility/construction_policy.hpp>
#include <geometrix/tensor/vector_traits.hpp>
#include <geometrix/utility/preprocessor.hpp>
#include <geometrix/tensor/fusion_vector_access_policy.hpp>

#include <boost/fusion/adapted/struct.hpp>

namespace geometrix {

#define GEOMETRIX_FUSION_VECTOR( Name, Types, ReferenceFrame )                         \
struct Name                                                                                     \
{                                                                                               \
    template <BOOST_PP_ENUM(BOOST_PP_SEQ_SIZE(Types), GEOMETRIX_TEMPLATE_PARAM_, _ )>           \
    Name( BOOST_PP_ENUM(BOOST_PP_SEQ_SIZE(Types), GEOMETRIX_PARAM_, _ ) )                       \
    : m( BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(Types), a) ){}                                  \
    typedef boost::fusion::vector                                                               \
    <                                                                                           \
        BOOST_PP_ENUM(Rows, GEOMETRIX_TEXT, Types)                                              \
    > vector_type;                                                                              \
    vector_type m;                                                                              \
};                                                                                              \
GEOMETRIX_DEFINE_FUSION_VECTOR_TRAITS                                                           \
(                                                                                               \
    Name                                                                                        \
  , Types                                                                                       \
  , BOOST_PP_SEQ_SIZE(Types)                                                                    \
  , ReferenceFrame                                                                              \
  , fusion_vector_access_policy<Name>)                                                          \
/***/

#define GEOMETRIX_FUSION_POD_VECTOR_MEMBER_( z, n, data) \
    BOOST_PP_SEQ_ELEM(n, data) BOOST_PP_CAT(a, n);       \
/***/
    
#define GEOMETRIX_FUSION_POD_VECTOR_ADAPT_MEMBERS_(z,n,data) \
   BOOST_PP_LPAREN() BOOST_PP_SEQ_ELEM(n,data)               \
       BOOST_PP_COMMA() BOOST_PP_CAT(a,n);                   \
   BOOST_PP_RPAREN()                                         \
/***/

#define GEOMETRIX_FUSION_POD_VECTOR( Name, Types, DimensionlessType, ReferenceFrame )          \
struct Name                                                                                    \
{                                                                                              \
      BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(Types), GEOMETRIX_FUSION_POD_VECTOR_MEMBER_, Types)    \
};                                                                                             \
BOOST_FUSION_ADAPT_STRUCT                                                                      \
(                                                                                              \
    Name                                                                                       \
  , BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(Types), GEOMETRIX_FUSION_POD_VECTOR_ADAPT_MEMBERS_,Types)\
)                                                                                              \
GEOMETRIX_DEFINE_FUSION_VECTOR_TRAITS                                                          \
(                                                                                              \
    Name                                                                                       \
  , Types                                                                                      \
  , BOOST_PP_SEQ_SIZE(Types)                                                                   \
  , DimensionlessType                                                                          \
  , ReferenceFrame                                                                             \
  , fusion_vector_access_policy<Name>)                                                         \
GEOMETRIX_DEFINE_POD_NUMERIC_SEQUENCE_CONSTRUCTION_POLICY(Name, BOOST_PP_SEQ_SIZE(Types))      \
/***/

//! Create traits for 2d, 3d and 4d fusion vector types.

template <typename A, typename B>
struct tensor_traits< boost::fusion::vector<A,B> >
{
    typedef fusion_vector_access_policy< boost::fusion::vector<A,B> > access_policy;
    typedef void                                                      is_tensor;
    typedef void                                                      rank_1;
};

template <typename A, typename B>
struct geometric_traits< boost::fusion::vector<A,B> >
{                                                                                                                      
    typedef void                       is_vector;                                                           
    typedef neutral_reference_frame<2> reference_frame;
    typedef void                       is_coordinate_sequence;                                              
    typedef void                       is_numeric_sequence;                                                 
    typedef boost::mpl::vector<A,B>    storage_types;                                                       
    typedef dimension<2>               dimension_type;                                                      
    typedef void                       is_sequence;                                                         
    typedef void                       is_heterogeneous;
};

template <typename A, typename B, typename C>
struct tensor_traits< boost::fusion::vector<A,B,C> >
{
    typedef fusion_vector_access_policy< boost::fusion::vector<A,B,C> > access_policy;
    typedef void                                                        is_tensor;
    typedef void                                                        rank_1;
};

template <typename A, typename B, typename C>
struct geometric_traits< boost::fusion::vector<A,B,C> >
{                                                                                                                      
    typedef void                       is_vector;                                                           
    typedef neutral_reference_frame<3> reference_frame;
    typedef void                       is_coordinate_sequence;                                              
    typedef void                       is_numeric_sequence;                                                 
    typedef boost::mpl::vector<A,B,C>  storage_types;                                                       
    typedef dimension<3>               dimension_type;                                                      
    typedef void                       is_sequence;                                                         
    typedef void                       is_heterogeneous;
};

template <typename A, typename B, typename C, typename D>
struct tensor_traits< boost::fusion::vector<A,B,C,D> >
{
    typedef fusion_vector_access_policy< boost::fusion::vector<A,B,C,D> > access_policy;
    typedef void                                                          is_tensor;
    typedef void                                                          rank_1;
};

template <typename A, typename B, typename C, typename D>
struct geometric_traits< boost::fusion::vector<A,B,C,D> >
{                                                                                                                      
    typedef void                        is_vector;                                                           
    typedef neutral_reference_frame<4>  reference_frame;
    typedef void                        is_coordinate_sequence;                                              
    typedef void                        is_numeric_sequence;                                                 
    typedef boost::mpl::vector<A,B,C,D> storage_types;                                                       
    typedef dimension<4>                dimension_type;                                                      
    typedef void                        is_sequence;                                                         
    typedef void                        is_heterogeneous;
};

}//namespace geometrix;

#endif//GEOMETRIX_FUSION_VECTOR_HPP
