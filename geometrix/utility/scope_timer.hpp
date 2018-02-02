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

#if !defined(GEOMETRIX_DISABLE_SCOPE_TIMERS)
#define GEOMETRIX_SCOPE_TIMERS_ENABLED 1
#endif

namespace geometrix {
	namespace scope_timer_detail
	{
		struct call_data
		{
			std::size_t counts{ 0 };
			std::int64_t time{ 0 };
		};

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
			std::int64_t seconds = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
			call_map& callMap = call_map::instance();
			call_data& data = callMap[m_function];
			data.time += seconds;
			data.counts += 1;
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
