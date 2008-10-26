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
#include "detail/indexed_sequence_access_defines.hpp"

namespace boost
{
namespace numeric
{
namespace geometry
{

//! \brief Default compile-time access tag.
template <typename Sequence>
struct use_indexed_access_type
    : boost::integral_constant< indexed_sequence_access_type,
                                BOOST_GEOMETRY_INDEXED_SEQUENCE_ACCESS_TYPE >{};

//! \brief types to clarify definition of access type.
struct use_compile_time_access{ const static indexed_sequence_access_type value = compile_time_access; };
struct use_run_time_access{ const static indexed_sequence_access_type value = run_time_access; };
struct use_both_prefer_compile_time_access{ const static indexed_sequence_access_type value = both_prefer_compile_time_access; };
struct use_both_prefer_run_time_access{ const static indexed_sequence_access_type value = both_prefer_run_time_access; };

template <typename T1>
struct should_use_compile_time_access1
{    
    typedef typename boost::mpl::
        if_c< use_indexed_access_type<T1>::value >= compile_time_access,
            //Then
            boost::true_type,
            //Else
            boost::false_type                      
    >::type type;
};

template <typename T1, typename T2> 
struct should_use_compile_time_access2
{    
    typedef typename boost::mpl::
        if_c< should_use_compile_time_access1<T1>::type::value ||
              should_use_compile_time_access1<T2>::type::value,
            //Then
            boost::true_type,
            //Else
            boost::false_type                      
    >::type type;
};

template <typename T1>
struct should_use_run_time_access1
{    
    typedef typename boost::mpl::
        if_c< use_indexed_access_type<T1>::value < compile_time_access,
            //Then
            boost::true_type,
            //Else
            boost::false_type                      
    >::type type;
};

template <typename T1, typename T2>
struct should_use_run_time_access2
{    
    typedef typename boost::mpl::
        if_c< should_use_run_time_access1<T1>::type::value &&
              should_use_run_time_access1<T2>::type::value,
            //Then
            boost::true_type,
            //Else
            boost::false_type                      
    >::type type;
};

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
    typedef typename use_indexed_access_type< sequence_type >        access_type;

    //! Typedefs common for containers
    typedef typename sequence_traits<sequence_type>::value_type      value_type;
    typedef typename sequence_traits<sequence_type>::reference       reference;
    typedef typename sequence_traits<sequence_type>::const_reference const_reference;

};

//! \brief Specialization of indexed access traits for type which have run-time access
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< use_indexed_access_type< Sequence >::value == use_run_time_access::value >::type >
{
    typedef Sequence                                                 sequence_type;
    typedef typename sequence_traits<sequence_type>::value_type      indexed_type;
    typedef typename sequence_traits<sequence_type>::dimension_type  dimension_type;
    typedef typename use_indexed_access_type< sequence_type >        access_type;

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

//! \brief Specialization of indexed access traits for type which have compile-time access
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< use_indexed_access_type< Sequence >::value == use_compile_time_access::value >::type >
{
    typedef Sequence                                                 sequence_type;
    typedef typename sequence_traits<sequence_type>::value_type      indexed_type;
    typedef typename sequence_traits<sequence_type>::dimension_type  dimension_type;
    typedef typename use_indexed_access_type< sequence_type >        access_type;

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

//! \brief Specialization of indexed access traits for type which have both run-time and compile-time access.
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< use_indexed_access_type< Sequence >::value == use_both_prefer_compile_time_access::value ||
                                                                     use_indexed_access_type< Sequence >::value == use_both_prefer_run_time_access::value>::type >
{
    typedef Sequence                                                 sequence_type;
    typedef typename sequence_traits<sequence_type>::value_type      indexed_type;
    typedef typename sequence_traits<sequence_type>::dimension_type  dimension_type;
    typedef typename use_indexed_access_type< sequence_type >        access_type;

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

//! \brief Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_INDEXED_ACCESS_TRAITS( Sequence )               \
template<> struct is_indexed_sequence<Sequence> : boost::true_type{};

//! \brief Macro for point type which does not have embedded traits - User Defined Points
#define BOOST_DEFINE_USER_INDEXED_ACCESS_TRAITS( Sequence, IndexedSequenceAccess )                   \
template<> struct use_indexed_access_type< Sequence > : boost::integral_constant<indexed_sequence_access_type, IndexedSequenceAccess::value>{};\
template<> struct is_indexed_sequence< Sequence > : boost::true_type{};

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
