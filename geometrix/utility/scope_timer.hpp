//
//! Copyright © 2008-2015
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_SCOPETIMER_HPP
#define GEOMETRIX_UTILITY_SCOPETIMER_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/container/flat_map.hpp>

#include <chrono>
#include <cstdint>
#include <cmath>

#if !defined(GEOMETRIX_DISABLE_SCOPE_TIMERS)
#define GEOMETRIX_SCOPE_TIMERS_ENABLED 1
#endif

namespace geometrix {
	namespace scope_timer_detail
	{
        //! From John D. Cook: https://www.johndcook.com/blog/standard_deviation
        template <typename T> 
        class running_stat 
        {
        public:
            running_stat()
                : m_n{}
            {}

            void clear()
            {
                m_n = 0;
            }

            template <typename T> 
            BOOST_FORCEINLINE void push(T x)
            {
                // See Knuth TAOCP vol 2, 3rd edition, page 232
                if (BOOST_LIKELY(++m_n != 1))
                {
                    m_sum += x;
                    m_newM = m_oldM + (x - m_oldM) / m_n;
                    m_newS = m_oldS + (x - m_oldM)*(x - m_newM);
        
                    // set up for next iteration
                    m_oldM = m_newM; 
                    m_oldS = m_newS;
                }
                else
                {
                    m_sum = x;
                    m_oldM = m_newM = x;
                    m_oldS = 0.0;
                }
            }

            std::uint64_t count() const
            {
                return m_n;
            }

            double mean() const
            {
                return (m_n > 0) ? m_newM : 0.0;
            }

            double variance() const
            {
                return ( (m_n > 1) ? m_newS/(m_n - 1) : 0.0 );
            }

            double standard_deviation() const
            {
                return std::sqrt( variance() );
            }

            T sum() const
            {
                return m_sum;
            }

        private:
            std::uint64_t m_n;
            double m_oldM, m_newM, m_oldS, m_newS;
            T m_sum;
        };

		using call_data = running_stat<std::uint64_t>;
		class call_map : public boost::container::flat_map<std::string, call_data>
		{
		public:

			typedef boost::container::flat_map<std::string, call_data> map_type;

			static call_map& instance();
			void write() const;

			call_map() {}
			~call_map();

		};
	}//! namespace scope_timer_detail

	class scope_timer
	{
	public:
		
		scope_timer(const std::string& functionName)
			: m_function(functionName)
			, t1(std::chrono::high_resolution_clock::now())
		{}

		~scope_timer()
		{
			using namespace scope_timer_detail;
			t2 = std::chrono::high_resolution_clock::now();
			std::uint64_t seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
			call_map& callMap = call_map::instance();
			callMap[m_function].push(seconds);
		}

	private:

		std::string m_function;
		std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

	};
}//! namespace geometrix;

#if( GEOMETRIX_SCOPE_TIMERS_ENABLED )
#define GEOMETRIX_MEASURE_SCOPE_TIME( function_name ) \
    geometrix::scope_timer BOOST_PP_CAT(scope_timer_instance, __LINE__)( ( function_name ) );
#else
#define GEOMETRIX_MEASURE_SCOPE_TIME( function_name ) 
#endif

#endif // GEOMETRIX_UTILITY_SCOPETIMER_HPP
