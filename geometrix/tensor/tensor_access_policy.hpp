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
#include <boost/call_traits.hpp>

namespace geometrix {

//! Meta function to resolve the type at a given index for a collection.
template 
    <
        typename T
      , unsigned int Index0=0
      , unsigned int Index1=0
      , typename EnableIf = void
    >
struct type_at 
{};

template <typename T>
struct type_at<T, 0, 0, typename tensor_traits<typename remove_const_ref<T>::type>::rank_0 >
{
    typedef typename access_policy_of<T>::type::type_at::type type;
};

template <typename T, unsigned int Index>
struct type_at<T, Index, 0, typename tensor_traits<typename remove_const_ref<T>::type>::rank_1 >
{
    typedef typename access_policy_of<T>::type::template type_at<Index>::type type;
};

template <typename T, unsigned int Row, unsigned int Column>
struct type_at<T, Row, Column, typename tensor_traits<typename remove_const_ref<T>::type>::rank_2 >
{
    typedef typename access_policy_of<T>::type::template type_at<Row,Column>::type type;
};

//! Compile time access with 1 index.
template <unsigned int Index, typename T>
inline typename type_at<T, Index>::type get( const T& s )
{
    return access_policy_of<T>::type::template get<Index>( s );
}

//! Compile time access with 2 indices
template <unsigned int Index0, unsigned int Index1, typename T>
inline typename type_at<T, Index0, Index1>::type get( const T& s )
{
    return access_policy_of<T>::type::template get<Index0, Index1>( s );
}
// 
// //! Runtime access with 1 index.
// template <typename T>
// inline typename type_at<T,0>::type get( const T& c, std::size_t i )
// {
//     return access_policy_of<T>::type::get( c, i );
// }
// 
// //! Runtime access with 2 indices.
// template <typename T>
// inline typename type_at<T,0,0>::type get( const T& c, std::size_t i, std::size_t j )
// {
//     typedef typename remove_const_ref< T >::type tensor_type;
//     return access_policy_of<tensor_type>::type::get( c, i, j );
// }

template <typename Scalar>
inline typename type_at<Scalar,0,0>::type get( const Scalar& s )
{
    typedef typename remove_const_ref< Scalar >::type scalar_type;
    return access_policy_of<scalar_type>::type::get(s);
}

//! Mutators
//! Compile time access with 1 index.
template <unsigned int Index, typename T>
inline void set( T& s, typename boost::call_traits<typename type_at<T, Index>::type>::param_type v )
{
    access_policy_of<T>::type::template set<Index>( s, v );
}

//! Compile time access with 2 indices
template <unsigned int Index0, unsigned int Index1, typename T>
inline void set( T& s, typename boost::call_traits<typename type_at<T, Index0, Index1>::type>::param_type v )
{
    access_policy_of<T>::type::template set<Index0, Index1>( s, v );
}

// 
// //! Runtime access with 1 index.
// template <typename T>
// inline void set( T& c, std::size_t i, typename boost::call_traits<typename type_at<T,0>::type>::param_type v )
// {
//     access_policy_of<T>::type::set( c, i, v );
// }
// 
// //! Runtime access with 1 index.
// template <typename T>
// inline void set( T& c, std::size_t i, std::size_t j, typename boost::call_traits<typename type_at<T,0>::type>::param_type v )
// {
//     access_policy_of<T>::type::set( c, i, j, v );
// }

template <typename Scalar>
inline void set( Scalar& s, const typename type_at<Scalar,0,0>::type& v )
{
    return access_policy_of<Scalar>::type::set(s,v);
}

}//namespace geometrix;

#endif //GEOMETRIX_ACCESS_POLICY_HPP
