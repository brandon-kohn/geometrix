//
//! Copyright © 2008-2011
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

        random_index_generator( std::size_t maxIndex, unsigned long seed = 42 )
            : m_randomNumberGenerator(seed)
            , m_range( std::size_t(0), maxIndex )
            , m_generator( m_randomNumberGenerator, m_range )
        {}
        
        random_index_generator( std::size_t maxIndex, const RandomNumberGenerator& rng )
            : m_randomNumberGenerator(rng)
            , m_range( std::size_t(0), maxIndex )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate an integer on the interval [0,maxIndex).
        std::size_t operator()() const { return m_generator(); }
        void seed( unsigned long seed ) { m_generator.engine().seed( seed ); m_generator.distribution().reset(); }

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

        random_real_generator( double maxReal = 1.0, unsigned long seed = 42 )
            : m_randomNumberGenerator(seed)
            , m_range( 0.0, maxReal )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate a real (double) on the interval [0.0,maxReal).
        double operator()() const { return m_generator(); }
        void seed( unsigned long seed ) { m_generator.engine().seed( seed ); m_generator.distribution().reset(); }

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

        random_integer_generator( unsigned long seed = 42 )
            : m_randomNumberGenerator(seed)
            , m_range( std::size_t(0), (std::numeric_limits<std::size_t>::max)() )
            , m_generator( m_randomNumberGenerator, m_range )
        {}

        //! Generate a number on the interval [0,N)
        std::size_t operator()( std::size_t N = (std::numeric_limits<std::size_t>::max)() ) const { return (m_generator() % N); }
        void seed( unsigned long seed ) { m_generator.engine().seed( seed ); m_generator.distribution().reset(); }

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
    
    template <typename Range, typename RNG>
    inline typename boost::range_iterator<Range>::type random_element(const Range& r, const random_index_generator<RNG>& rng)
    {
        auto it = boost::begin(r);
        std::advance(it, rng());
        return it;
    }

}//namespace geometrix;

#endif //GEOMETRIX_RANDOM_GENERATOR_HPP
