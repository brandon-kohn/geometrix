//
//! Copyright © 2013
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

//! Note: These are outside include guards on purpose.

#undef GEOMETRIX_ASSERT
#undef GEOMETRIX_ASSERT_MSG
#undef GEOMETRIX_VERIFY
#undef GEOMETRIX_VERIFY_MSG

#define BOOST_ENABLE_ASSERT_DEBUG_HANDLER 
#include <boost/assert.hpp>				
#define GEOMETRIX_ASSERT(Test) BOOST_ASSERT(Test)
#define GEOMETRIX_VERIFY(Test) BOOST_VERIFY(Test)	
#define GEOMETRIX_ASSERT_MSG(Test, Msg) BOOST_ASSERT_MSG(Test, Msg)
#define GEOMETRIX_VERIFY_MSG(Test, Msg) BOOST_VERIFY_MSG(Test, Msg)	

#ifndef GEOMETRIX_DEFINE_ASSERT_HANDLERS
#define GEOMETRIX_DEFINE_ASSERT_HANDLERS

#include <geometrix/utility/ignore_unused_warnings.hpp>

#ifndef NDEBUG
	#include <iostream>
    #if defined(_WIN32) && defined(_MSC_VER)
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
		#include <csignal>
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

#endif//! GEOMETRIX_DEFINE_ASSERT_HANDLERS
