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
#include <geometrix/tensor/sequence.hpp>
#include <geometrix/utility/generative_category.hpp>
#include <geometrix/utility/type_traits.hpp>

#include <boost/concept_check.hpp>

namespace geometrix {

// Access types whose index is specified as a template parameter.
template <typename T, typename EnableIf=void>
struct is_compile_time_access : boost::false_type
{};

// Access types whose index is a run time argument to a function.
template <typename T, typename EnableIf=void>
struct is_run_time_access : boost::false_type
{};

//! \brief types to clarify definition of access type.
struct require_compile_time_access_policy{ const static access_type value = e_compile_time_access; };
struct require_run_time_access_policy{ const static access_type value = e_run_time_access; };
struct prefer_compile_time_access_policy{ const static access_type value = e_prefer_compile_time_access; };
struct prefer_run_time_access_policy{ const static access_type value = e_prefer_run_time_access; };

//! \brief convenience functor to determine if a collection should be accessed using compile-time semantics.
template <typename T>
struct is_compile_time_access
    <
        T
      , typename access_policy_of<T>::compile_time_access
    > 
    : boost::true_type
{};

template <typename T>
struct is_run_time_access
    <
        T
      , typename access_policy_of<T>::run_time_access
    > 
    : boost::true_type
{};

//! \brief A concept check to enforce the access_policy run-time access interface.

//! A run-time access Tensor must define an access method of the following form:
//! \code
//! const_reference access_policy<T>::get( const T& collection, std::size_t index );
//! reference access_policy<T>::get( T& collection, std::size_t index );
//! \endcode
template <typename Tensor>
struct RunTimeAccessConcept
{
    void constraints() const
    {
        BOOST_STATIC_ASSERT(( is_run_time_access<Tensor>::value ));
    }
};

//! A compile-time access Tensor must define an access method of the following form:
//! \code
//! template <unsigned int Index> const_reference access_policy<T>::get( const T& collection );
//! template <unsigned int Index> reference access_policy<T>::get( T& collection );
//! \endcode
template <typename Tensor>
struct CompileTimeAccessConcept
{
    void constraints() const
    {
        BOOST_CONCEPT_ASSERT(( TensorConcept<Tensor> ));
        BOOST_STATIC_ASSERT(( is_compile_time_access<Tensor>::value ));
    }
};

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
    typedef typename access_policy_of<T>::type::type_at<Index>::type type;
};

template <typename T, unsigned int Row, unsigned int Column>
struct type_at<T, Row, Column, typename tensor_traits<typename remove_const_ref<T>::type>::rank_2 >
{
    typedef typename access_policy_of<T>::type::type_at<Row,Column>::type type;
};

//! Compile time access with 1 index.
template <unsigned int Index, typename T>
inline typename type_at<T, Index>::type get( const T& s )
{
    return access_policy_of<T>::type::get<Index>( s );
}

//! Compile time access with 2 indices
template <unsigned int Index0, unsigned int Index1, typename T>
inline typename type_at<T, Index0, Index1>::type get( const T& s )
{
    return access_policy_of<T>::type::get<Index0, Index1>( s );
}

//! Runtime access with 1 index.
template <typename T>
inline typename type_at<T,0>::type get( const T& c, std::size_t i )
{
    return access_policy_of<T>::type::get( c, i );
}

//! Runtime access with 2 indices.
template <typename T>
inline typename type_at<T,0,0>::type get( const T& c, std::size_t i, std::size_t j )
{
    typedef typename remove_const_ref< T >::type tensor_type;
    return access_policy_of<tensor_type>::type::get( c, i, j );
}

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
    access_policy_of<T>::type::set<Index>( s, v );
}

//! Compile time access with 2 indices
template <unsigned int Index0, unsigned int Index1, typename T>
inline void set( T& s, typename boost::call_traits<typename type_at<T, Index0, Index1>::type>::param_type v )
{
    access_policy_of<T>::type::set<Index0, Index1>( s, v );
}
//! Runtime access with 1 index.
template <typename T>
inline void set( T& c, std::size_t i, typename boost::call_traits<typename type_at<T,0>::type>::param_type v )
{
    access_policy_of<T>::type::set( c, i, v );
}

//! Runtime access with 1 index.
template <typename T>
inline void set( T& c, std::size_t i, std::size_t j, typename boost::call_traits<typename type_at<T,0>::type>::param_type v )
{
    access_policy_of<T>::type::set( c, i, j, v );
}

template <typename Scalar>
inline void set( Scalar& s, const typename type_at<Scalar,0,0>::type& v )
{
    return access_policy_of<scalar_type>::type::set(s,v);
}

}//namespace geometrix;

#endif //GEOMETRIX_ACCESS_POLICY_HPP
