//
//! Copyright © 2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GEOMETRIX_UTILITY_ASSERT_HPP
#define GEOMETRIX_UTILITY_ASSERT_HPP

#include <geometrix/utility/ignore_unused_warnings.hpp>

#ifdef NDEBUG
    #define GEOMETRIX_ASSERT(Test)
#else
	#if defined(_WIN32) && defined(_MSC_VER)
		#define BOOST_ENABLE_ASSERT_HANDLER 
		#include <boost/assert.hpp>		
		#include <iostream>
		#define GEOMETRIX_ASSERT(Test) BOOST_ASSERT(Test)
		namespace boost
		{
			inline void assertion_failed( char const * expr, char const * function, char const * file, long line )
			{
				std::cerr << file << "(" << line << "): " << "Assertion failed: |" << expr << "| in " << function;
				__debugbreak();
			}

			inline void assertion_failed_msg( char const * expr, char const * msg, char const * function, char const * file, long line )
			{
				std::cerr << file << "(" << line << "): " << "Assertion failed: |" << expr << "| with message " << msg << " at " << function;
				__debugbreak();
			}
		}		
	#elif defined(__APPLE__) || defined(__linux__)
		#define BOOST_ENABLE_ASSERT_HANDLER 
		#include <csignal>
		#include <boost/assert.hpp>		
		#include <iostream>
		#define GEOMETRIX_ASSERT(Test) BOOST_ASSERT(Test)
		namespace boost
		{
			inline void assertion_failed( char const * expr, char const * function, char const * file, long line )
			{
				std::cerr << "Assertion failed: |" << expr << "| at " << function << " in " << file << " (" << line << ")";
				raise(SIGTRAP);
			}

			inline void assertion_failed_msg( char const * expr, char const * msg, char const * function, char const * file, long line )
			{
				std::cerr << "Assertion failed: |" << expr << "| with message " << msg << " at " << function << " in " << file << " (" << line << ")";
				raise(SIGTRAP);
			}
		}
	#else 
		#define BOOST_ENABLE_ASSERT_HANDLER 
		#include <boost/assert.hpp>		
		#include <iostream>
		#define GEOMETRIX_ASSERT(Test) BOOST_ASSERT(Test)
		namespace boost
		{
			inline void assertion_failed( char const * expr, char const * function, char const * file, long line )
			{
				std::cerr << "Assertion failed: |" << expr << "| at " << function << " in " << file << " (" << line << ")";
				//! Don't know which wait to break.. so put a breakpoint on the line above.
			}

			inline void assertion_failed_msg( char const * expr, char const * msg, char const * function, char const * file, long line )
			{
				std::cerr << "Assertion failed: |" << expr << "| with message " << msg << " at " << function << " in " << file << " (" << line << ")";
				//! Don't know which wait to break.. so put a breakpoint on the line above.
			}
		}
	#endif
#endif

#endif//! GEOMETRIX_UTILITY_ASSERT_HPP
