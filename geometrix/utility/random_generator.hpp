//
//! Copyright � 2008-2011
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_RANDOM_GENERATOR_HPP
#define GEOMETRIX_RANDOM_GENERATOR_HPP

#include <boost/random.hpp>

namespace geometrix {
    template <typename RandomNumberGenerator>
    class random_index_generator
    {
    public:

        random_index_generator( std::size_t maxIndex )
            : m_range( std::size_t(0), maxIndex )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate an integer on the interval [0,maxIndex).
        std::size_t operator()() const { return m_generator(); }

    private:

        //! order of these is important so initializer list gets the order correct.
        mutable RandomNumberGenerator                                                              m_randomNumberGenerator;
        boost::uniform_int<std::size_t>                                                            m_range;
        mutable boost::variate_generator<RandomNumberGenerator&, boost::uniform_int<std::size_t> > m_generator;
    };

    template <typename RandomNumberGenerator = boost::mt19937>
    class random_real_generator
    {
    public:

        random_real_generator( double maxReal )
            : m_range( 0.0, maxReal )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate a real (double) on the interval [0.0,maxReal).
        double operator()() const { return m_generator(); }

    private:

        //! order of these is important so initializer list gets the order correct.
        mutable RandomNumberGenerator                                                    m_randomNumberGenerator;
        boost::uniform_real<>                                                            m_range;
        mutable boost::variate_generator<RandomNumberGenerator&, boost::uniform_real<> > m_generator;
    };

    template <typename RandomNumberGenerator = boost::mt19937>
    class random_integer_generator
    {
    public:

        random_integer_generator()
            : m_range( std::size_t(0), (std::numeric_limits<std::size_t>::max)() )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate a number on the interval [0,N)
        std::size_t operator()( std::size_t N = (std::numeric_limits<std::size_t>::max)() ) const { return (m_generator() % N); }

    private:

        //! order of these is important so initializer list gets the order correct.
        mutable RandomNumberGenerator                                                              m_randomNumberGenerator;
        boost::uniform_int<std::size_t>                                                            m_range;
        mutable boost::variate_generator<RandomNumberGenerator&, boost::uniform_int<std::size_t> > m_generator;
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

}//namespace geometrix;

#endif //GEOMETRIX_RANDOM_GENERATOR_HPP
