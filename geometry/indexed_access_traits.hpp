//
//!  Copyright (c) 2008
//!  Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
#define _BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
#pragma once

#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include "sequence_traits.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief Default compile-time access tag.
template <typename Sequence>
struct has_compile_time_access : boost::false_type{};

//! \brief Default run-time access tag.
template <typename Sequence>
struct has_run_time_access : boost::false_type{};

//! \brief Tag to check if a type is an indexed_sequence
template <typename Sequence>
struct is_indexed_sequence : boost::false_type{};

//! \brief Default traits for an indexed access sequence.
template < typename Sequence, typename Enable = void >
struct indexed_access_traits
{
    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, INDEXED_ACCESS_TRAITS_NOT_DEFINED
		, (indexed_access_traits<Sequence>) );

    typedef Sequence                                                 sequence_type;
    typedef typename sequence_traits<sequence_type>::value_type      indexed_type;
    typedef typename sequence_traits<sequence_type>::dimension_type  dimension_type;
    typedef typename has_compile_time_access< sequence_type >        has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >            has_run_time_access_type;

    //! Typedefs common for containers
    typedef typename sequence_traits<sequence_type>::value_type      value_type;
    typedef typename sequence_traits<sequence_type>::reference       reference;
    typedef typename sequence_traits<sequence_type>::const_reference const_reference;

};

//! \brief Specialization of indexed access traits for type which have both run-time and compile-time access.
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< has_compile_time_access< Sequence >::value && has_run_time_access< Sequence >::value >::type >
{
    typedef Sequence                                                 sequence_type;
    typedef typename sequence_traits<sequence_type>::value_type      indexed_type;
    typedef typename sequence_traits<sequence_type>::dimension_type  dimension_type;
    typedef typename has_compile_time_access< sequence_type >        has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >            has_run_time_access_type;

    //! Typedefs common for containers
    typedef typename sequence_traits<sequence_type>::value_type      value_type;
    typedef typename sequence_traits<sequence_type>::reference       reference;
    typedef typename sequence_traits<sequence_type>::const_reference const_reference;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline const_reference get( const Sequence& sequence ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Sequence)
        );

        return sequence.get<dimension_traits<Index>::value>();
    }

    //! \brief run-time access method if the sequence supports it.
    static inline const_reference get( const Sequence& sequence, size_t index  ) 
    {        
        assert( index >= 0 && index < dimension_type::value );		   
        return sequence[ index ];
    }

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline reference get( Sequence& sequence ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Sequence)
        );

        return sequence.get<dimension_traits<Index>::value>();
    }

    //! \brief run-time access method if the sequence supports it.
    static inline reference get( Sequence& sequence, size_t index  ) 
    {        
        assert( index >= 0 && index < dimension_type::value );		   
        return sequence[ index ];
    }
};

//! \brief Specialization of indexed access traits for type which have run-time access but no compile-time access.
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< !has_compile_time_access< Sequence >::value && has_run_time_access< Sequence >::value >::type >
{
    typedef Sequence                                                 sequence_type;
    typedef typename sequence_traits<sequence_type>::value_type      indexed_type;
    typedef typename sequence_traits<sequence_type>::dimension_type  dimension_type;
    typedef typename has_compile_time_access< sequence_type >        has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >            has_run_time_access_type;

    //! Typedefs common for containers
    typedef typename sequence_traits<sequence_type>::value_type      value_type;
    typedef typename sequence_traits<sequence_type>::reference       reference;
    typedef typename sequence_traits<sequence_type>::const_reference const_reference;
    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline const_reference get( const Sequence& sequence ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Sequence)
        );

        return sequence[ Index ];
    }

    //! \brief run-time access method if the sequence supports it.
    static inline const_reference get( const Sequence& sequence, size_t index ) 
    {        
        assert( index >= 0 && index < dimension_type::value );		   
        return sequence[ index ];
    }

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline reference get( Sequence& sequence ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Sequence)
        );

        return sequence[ Index ];
    }

    //! \brief run-time access method if the sequence supports it.
    static inline reference get( Sequence& sequence, size_t index ) 
    {        
        assert( index >= 0 && index < dimension_type::value );		   
        return sequence[ index ];
    }
};

//! \brief Specialization of indexed access traits for type which have compile-time access, but no run-time access.
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< has_compile_time_access< Sequence >::value && !has_run_time_access< Sequence >::value >::type >
{
    typedef Sequence                                                 sequence_type;
    typedef typename sequence_traits<sequence_type>::value_type      indexed_type;
    typedef typename sequence_traits<sequence_type>::dimension_type  dimension_type;
    typedef typename has_compile_time_access< sequence_type >        has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >            has_run_time_access_type;

    //! Typedefs common for containers
    typedef typename sequence_traits<sequence_type>::value_type      value_type;
    typedef typename sequence_traits<sequence_type>::reference       reference;
    typedef typename sequence_traits<sequence_type>::const_reference const_reference;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline const_reference get( const Sequence& sequence ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Sequence)
        );

        return sequence.get<dimension_traits<Index>::value>();
    }

    template <unsigned int Index>
    static inline reference get( Sequence& sequence ) 
    {
        BOOST_MPL_ASSERT_MSG
        (
           ( dimension_traits< Index >::value >= 0 && dimension_traits< Index >::value < dimension_type::value )
		   , INDEX_OUT_OF_BOUNDS
		   , (Sequence)
        );

        return sequence.get<dimension_traits<Index>::value>();
    }

    //! TODO: fake run-time with switch?
};

//! \brief types to clarify definition of access type.
template <bool T> struct use_compile_time_access{ const static bool value = T; };
template <bool T> struct use_run_time_access{ const static bool value = T; };

//! \brief Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_INDEXED_ACCESS_TRAITS( Sequence )               \
template<> struct is_indexed_sequence<Sequence> : boost::true_type{};

//! \brief Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_USER_INDEXED_ACCESS_TRAITS( Sequence, HasRunTimeAccess, HasCompileTimeAccess )                   \
template<> struct has_compile_time_access< Sequence > : boost::integral_constant<bool, HasCompileTimeAccess::value>{};\
template<> struct has_run_time_access< Sequence > : boost::integral_constant<bool, HasRunTimeAccess::value>{};        \
template<> struct is_indexed_sequence< Sequence > : boost::true_type{};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
