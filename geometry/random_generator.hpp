//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef _GENERATIVE_GEOMETRY_RANDOM_GENERATOR_HPP
#define _GENERATIVE_GEOMETRY_RANDOM_GENERATOR_HPP
#pragma once

#include <boost/random.hpp>

namespace generative
{
namespace numeric
{
namespace geometry
{
    template <typename RandomNumberGenerator>
    class random_index_generator
    {
    public:

        random_index_generator( size_t maxIndex )
            : m_range( size_t(0), maxIndex )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate an integer on the interval [0,maxIndex).
        size_t operator()(){ return m_generator(); }

    private:

        //! order of these is important so initializer list gets the order correct.
        RandomNumberGenerator                                                         m_randomNumberGenerator;
        boost::uniform_int<size_t>                                                    m_range;
        boost::variate_generator<RandomNumberGenerator&, boost::uniform_int<size_t> > m_generator;
    };

    template <typename RandomNumberGenerator>
    class random_real_generator
    {
    public:

        random_real_generator( double maxReal )
            : m_range( 0.0, maxReal )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate a real (double) on the interval [0.0,maxReal).
        double operator()(){ return m_generator(); }

    private:

        //! order of these is important so initializer list gets the order correct.
        RandomNumberGenerator                                                    m_randomNumberGenerator;
        boost::uniform_real<>                                                    m_range;
        boost::variate_generator<RandomNumberGenerator&, boost::uniform_real<> > m_generator;
    };

    template <typename RandomNumberGenerator>
    class random_integer_generator
    {
    public:

        random_integer_generator()
            : m_range( size_t(0), (std::numeric_limits<size_t>::max)() )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate a number on the interval [0,N)
        size_t operator()( size_t N ){ return (m_generator() % N); }

    private:

        //! order of these is important so initializer list gets the order correct.
        RandomNumberGenerator                                                         m_randomNumberGenerator;
        boost::uniform_int<size_t>                                                    m_range;
        boost::variate_generator<RandomNumberGenerator&, boost::uniform_int<size_t> > m_generator;
    };

    //! generator to generate an ordered sequence (for synthesizing std::iota.. which isn't on all platforms).
    //! TODO: Just use counting_iterator...
    template <typename T>
    struct sequence_generator
    {
        sequence_generator( const T& init_value )
            : m_value( init_value )
        {}
        
        T operator() () const { return m_value++; }
        
        mutable T m_value;
    };

}}}//namespace generative::numeric::geometry;

#endif //_GENERATIVE_GEOMETRY_RANDOM_GENERATOR_HPP
