//
//! Copyright © 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_ACCESS_POLICY_HPP
#define GEOMETRIX_ACCESS_POLICY_HPP

#include <geometrix/tensor/tensor_traits.hpp>
#include <geometrix/utility/type_traits.hpp>

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>
#include <boost/call_traits.hpp>

namespace geometrix {

//! Meta function to resolve the type at a given index for a collection.
template 
    <
        typename T
      , std::size_t Index0=0
      , std::size_t Index1=0
      , typename EnableIf = void
    >
struct type_at 
{};

template <typename T>
struct type_at<T, 0, 0, typename tensor_traits<typename remove_const_ref<T>::type>::rank_0 >
{
    typedef typename access_policy_of<T>::type::type_at::type type;
};

template <typename T, std::size_t Index>
struct type_at<T, Index, 0, typename tensor_traits<typename remove_const_ref<T>::type>::rank_1 >
{
    typedef typename access_policy_of<T>::type::template type_at<Index>::type type;
};

template <typename T, std::size_t Row, std::size_t Column>
struct type_at<T, Row, Column, typename tensor_traits<typename remove_const_ref<T>::type>::rank_2 >
{
    typedef typename access_policy_of<T>::type::template type_at<Row,Column>::type type;
};

//! Compile time access with 1 index.
template <std::size_t Index, typename T>
inline BOOST_CONCEPT_REQUIRES( ((TensorConcept<T>)), (typename type_at<T, Index>::type) ) get( const T& s )
{
	static_assert(tensor_order_of<T>::value == 1, "s is not a tensor of order 1.");
    return access_policy_of<T>::type::template get<Index>( s );
}

//! Compile time access with 2 indices
template <std::size_t Index0, std::size_t Index1, typename T>
inline BOOST_CONCEPT_REQUIRES( ((TensorConcept<T>)), (typename type_at<T, Index0, Index1>::type) ) get( const T& s )
{
	static_assert(tensor_order_of<T>::value == 2, "s is not a tensor of order 2.");
    return access_policy_of<T>::type::template get<Index0, Index1>( s );
}

template <typename Scalar>
inline BOOST_CONCEPT_REQUIRES( ((TensorConcept<Scalar>)), (typename type_at<Scalar, 0, 0>::type) ) get( const Scalar& s )
{    
	static_assert(tensor_order_of<Scalar>::value == 0, "s is not a tensor of order 0.");
    return access_policy_of<Scalar>::type::get(s);
}

//! Mutators
//! Compile time access with 1 index.
template <std::size_t Index, typename T, typename Value>
inline BOOST_CONCEPT_REQUIRES( ((TensorConcept<T>)), (void) ) set( T& s, Value v )
{
	static_assert(tensor_order_of<T>::value == 1, "s is not a tensor of order 1.");
    access_policy_of<T>::type::template set<Index>( s, v );
}

//! Compile time access with 2 indices
template <std::size_t Index0, std::size_t Index1, typename T, typename Value>
inline BOOST_CONCEPT_REQUIRES( ((TensorConcept<T>)), (void) ) set( T& s, Value v )
{
	static_assert(tensor_order_of<T>::value == 2, "s is not a tensor of order 2.");
    access_policy_of<T>::type::template set<Index0, Index1>( s, v );
}

template <typename Scalar, typename Value>
inline BOOST_CONCEPT_REQUIRES( ((TensorConcept<Scalar>)), (void) ) set( Scalar& s, Value v )
{
	static_assert(tensor_order_of<Scalar>::value == 0, "s is not a tensor of order 0.");
    return access_policy_of<Scalar>::type::set(s,v);
}

}//namespace geometrix;

#endif //GEOMETRIX_ACCESS_POLICY_HPP
