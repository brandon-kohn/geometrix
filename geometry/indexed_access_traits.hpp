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

//! \brief Default traits for an indexed access sequence.
template < typename Sequence, typename Enable = void >
struct indexed_access_traits
{
    typedef Sequence                                          sequence_type;
    typedef typename sequence_type::value_type                indexed_type;
    typedef typename sequence_type::dimension_type            dimension_type;
    typedef typename has_compile_time_access< sequence_type > has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >     has_run_time_access_type;

    BOOST_MPL_ASSERT_MSG( 
		  ( false )
		, INDEXED_ACCESS_TRAITS_NOT_DEFINED
		, (indexed_access_traits<Sequence>) );
};

//! \brief Specialization of indexed access traits for type which have both run-time and compile-time access.
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< has_compile_time_access< Sequence >::value && has_run_time_access< Sequence >::value >::type >
{
    typedef Sequence                                          sequence_type;
    typedef typename sequence_type::value_type                indexed_type;
    typedef typename sequence_type::dimension_type            dimension_type;
    typedef typename has_compile_time_access< sequence_type > has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >     has_run_time_access_type;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline typename indexed_type get( const Sequence& sequence ) 
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
    static inline indexed_type get( const Sequence& sequence, size_t index  ) 
    {        
        assert( index >= 0 && index < dimension_type::value );		   
        return sequence[ index ];
    }
};

//! \brief Specialization of indexed access traits for type which have run-time access but no compile-time access.
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< !has_compile_time_access< Sequence >::value && has_run_time_access< Sequence >::value >::type >
{
    typedef Sequence                                          sequence_type;
    typedef typename sequence_type::value_type                indexed_type;
    typedef typename sequence_type::dimension_type            dimension_type;
    typedef typename has_compile_time_access< sequence_type > has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >     has_run_time_access_type;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline typename indexed_type get( const Sequence& sequence ) 
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
    static inline indexed_type get( const Sequence& sequence, size_t index ) 
    {        
        assert( index >= 0 && index < dimension_type::value );		   
        return sequence[ index ];
    }
};

//! \brief Specialization of indexed access traits for type which have compile-time access, but no run-time access.
template < typename Sequence >
struct indexed_access_traits< Sequence, typename boost::enable_if_c< has_compile_time_access< Sequence >::value && !has_run_time_access< Sequence >::value >::type >
{
    typedef Sequence                                          sequence_type;
    typedef typename sequence_type::value_type                indexed_type;
    typedef typename sequence_type::dimension_type            dimension_type;
    typedef typename has_compile_time_access< sequence_type > has_compile_time_access_type;
    typedef typename has_run_time_access< sequence_type >     has_run_time_access_type;

    //! \brief compile time access if available for the sequence.
    template <unsigned int Index>
    static inline typename indexed_type get( const Sequence& sequence ) 
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

}}}//namespace boost::numeric::geometry;

#endif //_BOOST_GEOMETRY_INDEXED_ACCESS_TRAITS_HPP
