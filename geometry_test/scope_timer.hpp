// scope_timer.hpp
// Author: Brandon Kohn

#ifndef _UTILITY_SCOPETIMER_HPP
#define _UTILITY_SCOPETIMER_HPP


#include <boost/thread\xtime.hpp>
#include <boost/foreach.hpp>
#include <boost/numeric/conversion\cast.hpp>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <fstream>

#define SCOPE_TIMERS_ENABLED 1

namespace utility
{
   
    //! Function to perform a timing (lowish? resolution timing) via boost.    
    inline double seconds_elapsed( const boost::xtime& t1, const boost::xtime& t2 )
    {
        const double nsec_sec = 1e9;
        double d = (double)(t2.sec + t2.nsec/nsec_sec) - (double)(t1.sec + t1.nsec /nsec_sec);	
        return d;
    }

    namespace detail
    {
    struct call_data
    {
        call_data()
            : counts( 0 ),
            time( 0 ),
            selfTime( 0 )
        {}

        std::size_t counts;
        double time;
        double selfTime;
        std::vector< double > data;
    };

    class call_map : public std::map< std::string, call_data >
    {
    public:

        typedef std::map< std::string, call_data > map_type;

        static call_map& instance()
        {
            static call_map theMap;
            return theMap;
        }

        call_map();
        ~call_map();

    };
}// detail

class scoped_timer
{
public:

    scoped_timer( const std::string& functionName );
    ~scoped_timer();
    
private:

    class call_stack : public std::stack< std::string >
    {
    public:

        typedef std::stack< std::string > stack_type;

        static call_stack& instance()
        {
            static call_stack theStack;
            return theStack;
        }
    };
    
    std::string m_function;
    std::string m_ancestorFunction;
    boost::xtime t1, t2;

};

}//namespace utility

#if( SCOPE_TIMERS_ENABLED )
#define MEASURE_SCOPE_TIME( function_name ) \
    utility::scoped_timer ___scopeTimerInstance( ( function_name ) );
#else
#define MEASURE_SCOPE_TIME( function_name ) 
#endif

#endif // _UTILITY_SCOPETIMER_HPP
